cmake_minimum_required(VERSION 3.5.0)
project(learn_opengl VERSION 0.1.0 LANGUAGES C CXX)


# Compile external dependencies
add_subdirectory(dependencies)


include_directories(
  src/
  dependencies/glad/include/
  dependencies/glfw/include/
  dependencies/stb/
)

set(LIBS
  ${OPENGL_LIBRARY}
  glfw
  glad
  STB_IMAGE
)

add_subdirectory("src")
