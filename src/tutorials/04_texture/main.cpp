#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "common/shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);
void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam);

typedef struct s_texture {
  GLuint id;
  int width;
  int height;
  int nr_channels;
} Texture;

float texture2_alpha = 0.2;

int main() {
  // ==== initialize glfw & opengl ====
  glfwInit();

  // During init, enable debug output

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window = glfwCreateWindow(800, 600, "Triangle with texture", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(MessageCallback, 0);
  glViewport(0, 0, 800, 600);
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

  Texture face;
  glGenTextures(1, &face.id);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, face.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unsigned char *face_data = stbi_load("./awesomeface.png", &face.width,
                                       &face.height, &face.nr_channels, 0);

  if (face_data) {
    GLint generate_mipmap_level = 0; // base level
    GLenum generated_texture_format = GL_RGB, input_data_format = GL_RGBA,
           input_data_type = GL_UNSIGNED_BYTE;
    glTexImage2D(GL_TEXTURE_2D, generate_mipmap_level, generated_texture_format,
                 face.width, face.height, 0, input_data_format, input_data_type,
                 face_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(face_data);

  // ==== load texture end ====

  shader_program.use();
  shader_program.setInt("texture1", 0);
  shader_program.setInt("texture2", 1);

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
    shader_program.setFloat("texture2_alpha", texture2_alpha);
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

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
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

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam) {
  fprintf(stderr,
          "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
          message);
}
