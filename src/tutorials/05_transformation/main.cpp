#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/init_gl.h"
#include "common/shader.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

typedef struct s_texture {
  GLuint id;
  int width;
  int height;
  int nr_channels;
} Texture;

int main() {
  // ==== initialize glfw & opengl ====
  auto window = init_gl("transformation", 800, 600);
  glfwSetKeyCallback(window, key_callback);
  // ==== end initializing ====

  // compile shader program
  Shader shader_program("./shader.vs", "./shader.fs");

  // clang-format off
  float vertices[] = {
    // position          // texture coords
     0.5f,  0.5f,  0.0f,    1.0f,  1.0f,    // top right
    -0.5f,  0.5f,  0.0f,    0.0f,  1.0f,    // top left
     0.5f, -0.5f,  0.0f,    1.0f,  0.0f,    // bottom right
    -0.5f, -0.5f,  0.0f,    0.0f,  0.0f     // bottom left
  };

  unsigned int indices[] = {
    0, 1, 2,
    1, 2, 3
  };
  // clang-format on

  GLuint VBO, EBO, VAO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  GLsizei stride = 5 * sizeof(float);
  void *pos_offset = (void *)0,
       *texture_coord_offset = (void *)(3 * sizeof(float));
  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, pos_offset);
  glEnableVertexAttribArray(0);
  // texture coords
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, texture_coord_offset);
  glEnableVertexAttribArray(1);

  // ==== load texture ====
  Texture container;
  glGenTextures(1, &container.id);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, container.id);

  // str is equivalent to xyz
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // if you choose GL_CLAMP_TO_BORDER, you should specify a border color
  // float border_color[] = {1.0, 1.0, 0.0, 1.0};
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // tell stb_image.h to flip loaded texture's on the y-axis.
  stbi_set_flip_vertically_on_load(true);
  unsigned char *container_data =
      stbi_load("./container.jpg", &container.width, &container.height,
                &container.nr_channels, 0);

  if (container_data) {
    GLint generate_mipmap_level = 0; // base level
    GLenum generated_texture_format = GL_RGB, input_data_format = GL_RGB,
           input_data_type = GL_UNSIGNED_BYTE;
    glTexImage2D(GL_TEXTURE_2D, generate_mipmap_level, generated_texture_format,
                 container.width, container.height, 0, input_data_format,
                 input_data_type, container_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(container_data);

  // use complied shader program
  shader_program.use();
  shader_program.setInt("texture1", 0);

  // main loop
  while (!glfwWindowShouldClose(window)) {
    // ==== render section ====
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container.id);

    glBindVertexArray(VAO);
    GLint transform_loc = glGetUniformLocation(shader_program.id, "transform");
    float time = glfwGetTime();

    // transform matrix
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // reset to identity matrix
    trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    float scale_factor = sin(time);
    trans = glm::scale(trans, glm::vec3(scale_factor, scale_factor, 1.0f));

    glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // ==== end render section ====

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader_program.id);

  glfwTerminate();
  return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
