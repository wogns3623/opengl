#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertex_shader_source = "\
#version 330 core\n\
layout (location = 0) in vec3 aPos;\n\
\n\
void main() {\n\
  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n\
}\0\
";
const GLsizei vertex_shader_string_count = 1;

const char *fragment_shader_source = "\
#version 330 core\n\
out vec4 FragColor;\n\
\n\
void main() {\n\
  FragColor = vec4(1.0f, 0.2f, 0.4f, 1.0f);\n\
}\0\
";
const GLsizei fragment_shader_string_count = 1;

int success;
char infoLog[512];

int main() {
  // ==== initialize glfw & opengl ====
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window = glfwCreateWindow(800, 600, "First Triangle", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);
  // ==== end initializing ====

  // ==== compile shader program ====
  // vertex shader
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, vertex_shader_string_count,
                 &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);

  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, sizeof(infoLog), NULL, infoLog);
    std::cout << "Error: Cannot compile vertex shader\n"
              << infoLog << std::endl;
  }

  // fragment shader
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, fragment_shader_string_count,
                 &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, sizeof(infoLog), NULL, infoLog);
    std::cout << "Error: Cannot compile fragment shader\n"
              << infoLog << std::endl;
  }

  // create shader program
  GLuint shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  glGetProgramiv(shader_program, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_program, sizeof(infoLog), NULL, infoLog);
    std::cout << "Error: Cannot link shader program\n" << infoLog << std::endl;
  }

  // cleanup shaders
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  // ==== end shader compile ====

  // triangle
  // clang-format off
  float vertices[] = {
    // first triangle
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    // -0.5f,  0.5f, 0.0f,  // top left 

    // second triangle
    //  0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
  };
  // clang-format on

  // can eliminate duplicate vertices by specifying the index of the vertices
  // clang-format off
  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };
  // clang-format on

  // create vertex buffer object, vertex array object, element buffer object and
  // get its id
  GLuint VBO, EBO, VAO;
  // create n buffers, currently n = 1
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  // create vertex array object
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  // bind VBO into GL_ARRAY_BUFFER
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // after this, all GL_ARRAY_BUFFER targeted buffer refer VBO

  // copy vertices into GL_ARRAY_BUFFER(VBO)
  // GL_STATIC_DRAW is some kind of optimization option?
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // bind EBO into GL_ELEMENT_ARRAY_BUFFER
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // let them know how to analysis given data
  // location in vertex shader
  GLuint location = 0;
  GLint vector_size = 3, vertex_size = 3;
  GLenum data_type = GL_FLOAT;
  GLboolean should_normalize = GL_FALSE;
  // if 0, opengl decide it
  GLsizei stride = vertex_size * sizeof(float);
  // where data starts
  void *offset = (void *)0;
  glVertexAttribPointer(location, vector_size, data_type, should_normalize,
                        stride, offset);
  glEnableVertexAttribArray(location);

  // main loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // ==== render section ====
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // use complied shader program
    glUseProgram(shader_program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // ==== end render section ====

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shader_program);

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
