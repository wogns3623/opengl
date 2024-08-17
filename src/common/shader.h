#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
private:
  char info_log[512];
  GLint success;

  std::string read_shader_code(const std::string &shader_path);
  GLuint compile_shader(const char *shader_code, GLenum shader_type);

public:
  GLuint ID;

  Shader(const std::string &vertex_path, const std::string &fragment_path);

  void use();

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif
