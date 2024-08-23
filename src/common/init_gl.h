#pragma once
#ifndef INIT_GL_H
#define INIT_GL_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>

GLFWwindow *init_gl(const char *title, int width, int height);

#endif
