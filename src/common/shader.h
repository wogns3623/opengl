#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>

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
  GLuint id;

  Shader(const std::string &vertex_path, const std::string &fragment_path);

  void use();

  void set_bool(const std::string &name, bool value) const;

  void set_int(const std::string &name, int value) const;

  void set_float(const std::string &name, float value) const;

  void set_vec2(const std::string &name, const glm::vec2 &value) const;
  void set_vec2(const std::string &name, float x, float y) const;

  void set_vec3(const std::string &name, const glm::vec3 &value) const;
  void set_vec3(const std::string &name, float x, float y, float z) const;

  void set_vec4(const std::string &name, const glm::vec4 &value) const;
  void set_vec4(const std::string &name, float x, float y, float z,
                float w) const;

  void set_mat4(const std::string &name, const glm::mat4 &value) const;
};

#endif
