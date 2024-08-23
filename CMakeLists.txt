cmake_minimum_required(VERSION 3.11)
project(learn_opengl VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

# GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.10
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)


# GLAD
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG c
)

# TODO: FetchContent_MakeAvailable 사용하기
# https://cmake.org/cmake/help/latest/module/FetchContent.html#overriding-where-to-find-cmakelists-txt
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
  FetchContent_Populate(glad)
endif()

add_library(glad ${glad_SOURCE_DIR}/src/glad.c)
set_target_properties(glad PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${glad_BINARY_DIR})
target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include/)


# STB
FetchContent_Declare(
	stb_image
	GIT_REPOSITORY https://github.com/nothings/stb.git
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
)

FetchContent_GetProperties(stb_image)
if(NOT stb_image_POPULATED)
  FetchContent_Populate(stb_image)
endif()

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${stb_image_SOURCE_DIR}/)


# GLM
FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)


set(LIBS
  ${OPENGL_LIBRARY}
  glfw
  glad
  glm::glm
  stb_image
)


include_directories(src/)

file(GLOB COMMON
  "src/common/*.h"
  "src/common/*.cpp"
)


function(create_project_from_sources TUTORIAL_DIR)
  file(GLOB SOURCE
    "${TUTORIAL_DIR}/*.h"
    "${TUTORIAL_DIR}/*.cpp"
  )
  file(GLOB SHADERS
    "${TUTORIAL_DIR}/*.vs"
    "${TUTORIAL_DIR}/*.fs"
    "${TUTORIAL_DIR}/*.tcs"
    "${TUTORIAL_DIR}/*.tes"
    "${TUTORIAL_DIR}/*.gs"
    "${TUTORIAL_DIR}/*.cs"
  )
	file(GLOB DLLS)
  
  get_filename_component(TUTORIAL_NAME ${TUTORIAL_DIR} NAME)

  add_executable(${TUTORIAL_NAME} ${SOURCE} ${SHADERS} ${COMMON})
  target_link_libraries(${TUTORIAL_NAME} ${LIBS})

  set_target_properties(${TUTORIAL_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/src/tutorials/${TUTORIAL_NAME})

  foreach(SHADER ${SHADERS})
    add_custom_command(TARGET ${TUTORIAL_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SHADER} $<TARGET_FILE_DIR:${TUTORIAL_NAME}>)
  endforeach(SHADER)

  foreach(DLL ${DLLS})
    add_custom_command(TARGET ${TUTORIAL_NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:${TUTORIAL_NAME}>)
  endforeach(DLL)
endfunction()


file(GLOB TUTORIAL_DIRS LIST_DIRECTORIES true "src/tutorials/*")

foreach(TUTORIAL_DIR ${TUTORIAL_DIRS})
  IF(IS_DIRECTORY ${TUTORIAL_DIR})
    create_project_from_sources(${TUTORIAL_DIR})
  ENDIF()
endforeach(TUTORIAL_DIR)
