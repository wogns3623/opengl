#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common/init_gl.h"
#include "common/shader.h"
#include "common/texture.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods);

int main() {
  // ==== initialize glfw & opengl ====
  auto window = init_gl("coordinate system", 800, 600);
  glfwSetKeyCallback(window, key_callback);

  glEnable(GL_DEPTH_TEST);
  // ==== end initializing ====

  // compile shader program
  Shader shader_program("./shader.vs", "./shader.fs");

  // clang-format off
  float vertices[] = {
    // x, y, z, texture_coord_x, texture_coord_y
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
    //  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3
    // -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
    //  0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 7
    // -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 8
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 9
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 10
    // -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 10
    // -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4
    // -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 8

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 11
    //  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 12
    //  0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 12
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 13
    //  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 11

    // -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 10
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 14
    //  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
    //  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
    // -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4
    // -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 10

    // -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3
    //  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
    //  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 11
    //  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 11
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f // 15
    // -0.5f,  0.5f, -0.5f,  0.0f, 1.0f // 3
  };

  unsigned int indices[] = {
    0, 1, 2, 0, 2, 3,
    4, 5, 6, 4, 6, 7,
    8, 9, 10, 4, 8, 10,
    2, 11, 12, 11, 12, 13,
    5, 10, 14, 4, 5, 10,
    2, 3, 11, 3, 11, 15
  };

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
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
  stbi_set_flip_vertically_on_load(true);
  Texture container("./container.jpg", GL_RGB, GL_RGB);
  glBindTexture(GL_TEXTURE_2D, container.id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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
  // ==== load texture done ====

  shader_program.use();
  shader_program.set_int("texture1", 0);
  shader_program.set_int("texture2", 1);

  // main loop
  while (!glfwWindowShouldClose(window)) {
    float time = glfwGetTime();

    // ==== render section ====
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, face.id);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::mat4(1.0f);

    float fov = glm::radians(45.0f), aspect_ratio = 800.0f / 600.0f,
          near_plain = 0.1f, far_plain = 100.0f;
    projection = glm::perspective(fov, aspect_ratio, near_plain, far_plain);

    shader_program.set_mat4("view", view);
    shader_program.set_mat4("projection", projection);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, time * glm::radians(20.0f * (i + 1)),
                          glm::vec3(0.5f, 1.0f, 0.0f));
      shader_program.set_mat4("model", model);

      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
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
