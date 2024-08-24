#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "common/init_gl.h"
#include "common/shader.h"
#include "common/texture.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

float texture2_alpha = 0.2;

int main() {
  // ==== initialize glfw & opengl ====
  auto window = init_gl("Triangle with texture", 800, 600);
  glfwSetKeyCallback(window, key_callback);
  // ==== end initializing ====

  // compile shader program
  Shader shader_program("./shader.vs", "./shader.fs");

  // clang-format off
  float vertices[] = {
    // position          // color         // texture coords
     0.5,  0.5,  0.0,    1.0, 0.0, 0.0,    1.0,  1.0,    // top right
    -0.5,  0.5,  0.0,    0.0, 1.0, 0.0,    0.0,  1.0,    // top left
     0.5, -0.5,  0.0,    0.0, 0.0, 1.0,    1.0,  0.0,    // bottom right
    -0.5, -0.5,  0.0,    1.0, 1.0, 0.0,    0.0,  0.0     // bottom left
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

  GLsizei stride = 8 * sizeof(float);
  void *pos_offset = (void *)0, *color_offset = (void *)(3 * sizeof(float)),
       *texture_coord_offset = (void *)(6 * sizeof(float));
  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, pos_offset);
  glEnableVertexAttribArray(0);
  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, color_offset);
  glEnableVertexAttribArray(1);
  // texture coords
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, texture_coord_offset);
  glEnableVertexAttribArray(2);

  // ==== load texture ====
  stbi_set_flip_vertically_on_load(true);
  Texture container("./container.jpg", GL_RGB, GL_RGB);
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

  Texture face("./awesomeface.png", GL_RGBA, GL_RGB);
  glBindTexture(GL_TEXTURE_2D, face.id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // ==== load texture end ====

  shader_program.use();
  shader_program.set_int("texture1", 0);
  shader_program.set_int("texture2", 1);

  // main loop
  while (!glfwWindowShouldClose(window)) {
    // ==== render section ====
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, face.id);

    // use complied shader program
    shader_program.use();
    shader_program.set_float("texture2_alpha", texture2_alpha);
    glBindVertexArray(VAO);
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
  } else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
    texture2_alpha += 0.01;
    if (texture2_alpha > 1)
      texture2_alpha = 1;
  } else if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
    texture2_alpha -= 0.01;
    if (texture2_alpha < 0)
      texture2_alpha = 0;
  }
}
