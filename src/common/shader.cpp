#include "shader.h"

std::string Shader::read_shader_code(const std::string &shader_path) {
  std::string code;
  std::ifstream shader_file;

  shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    shader_file.open(shader_path);
    std::stringstream shader_stream;

    shader_stream << shader_file.rdbuf();
    shader_file.close();

    code = shader_stream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "Error: Cannot read shader file " << shader_path << std::endl;
  }

  return code;
}

GLuint Shader::compile_shader(const char *shader_code, GLenum shader_type) {
  GLuint shader_id;

  shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &shader_code, NULL);
  glCompileShader(shader_id);

  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
    std::cout << "Error: Cannot compile shader type " << shader_type << "\n"
              << info_log << std::endl;
  }

  return shader_id;
}

Shader::Shader(const std::string &vertex_path,
               const std::string &fragment_path) {
  auto vertex_code = read_shader_code(vertex_path);
  auto vertex = compile_shader(vertex_code.c_str(), GL_VERTEX_SHADER);

  auto fragment_code = read_shader_code(fragment_path);
  auto fragment = compile_shader(fragment_code.c_str(), GL_FRAGMENT_SHADER);

  id = glCreateProgram();

  glAttachShader(id, vertex);
  glAttachShader(id, fragment);
  glLinkProgram(id);

  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(id, sizeof(info_log), NULL, info_log);
    std::cout << "Error: Cannot link shader program\n" << info_log << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
};

void Shader::use() { glUseProgram(id); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
