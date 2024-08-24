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
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

typedef struct s_Camera {
  glm::vec3 pos;
  glm::vec3 front;
  glm::vec3 up;
} Camera;

int width = 800, height = 600;
float fov = 45.0f, aspect_ratio = width / height, near_plain = 0.1f,
      far_plain = 100.0f;

int main() {
  // ==== initialize glfw & opengl ====
  auto window = init_gl("coordinate system", width, height);
  glfwSetKeyCallback(window, key_callback);
  glfwSetScrollCallback(window, scroll_callback);
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

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  Camera cam;
  cam.pos = glm::vec3(0.0f);
  cam.front = glm::vec3(0.0f);
  cam.up = glm::vec3(0.0f, 1.0f, 0.0f);
  float cam_speed = 3.0f;

  float mouse_yaw = 0.0f;
  float mouse_pitch = 0.0f;
  float mouse_sensitivity = 0.05f;

  shader_program.use();
  shader_program.set_int("texture1", 0);
  shader_program.set_int("texture2", 1);

  float delta_time = 0.0f;
  float last_frame = 0.0f;

  glfwSetCursorPos(window, 0, 0);
  // main loop
  while (!glfwWindowShouldClose(window)) {
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    // ==== handle input ====
    // mouse movement input
    glm::dvec2 mouse_offset(0.0);
    glfwGetCursorPos(window, &mouse_offset.x, &mouse_offset.y);
    glfwSetCursorPos(window, 0, 0);

    mouse_yaw += mouse_offset.x * mouse_sensitivity;
    mouse_pitch += -mouse_offset.y * mouse_sensitivity; // reverse y front

    if (mouse_pitch > 89.0f)
      mouse_pitch = 89.0f;
    else if (mouse_pitch < -89.0f)
      mouse_pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(mouse_pitch)) * cos(glm::radians(mouse_yaw));
    front.y = sin(glm::radians(mouse_pitch));
    front.z = cos(glm::radians(mouse_pitch)) * sin(glm::radians(mouse_yaw));
    cam.front = glm::normalize(front);

    // keyboard input
    float cam_speed_delta = cam_speed * delta_time;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cam.pos += cam.front * cam_speed_delta;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cam.pos -= cam.front * cam_speed_delta;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cam.pos +=
          glm::normalize(glm::cross(cam.front, cam.up)) * cam_speed_delta;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cam.pos -=
          glm::normalize(glm::cross(cam.front, cam.up)) * cam_speed_delta;
    // ==== handle input end ====

    // ==== render section ====
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, container.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, face.id);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(fov), aspect_ratio, near_plain,
                                  far_plain);
    shader_program.set_mat4("projection", projection);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cam.pos, cam.pos + cam.front, cam.up);
    shader_program.set_mat4("view", view);

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, current_frame * glm::radians(20.0f * (i + 1)),
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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  if (fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if (fov <= 1.0f)
    fov = 1.0f;
  if (fov >= 45.0f)
    fov = 45.0f;
}
