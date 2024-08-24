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

void Shader::set_bool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_float(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_vec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::set_vec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::set_vec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::set_vec3(const std::string &name, float x, float y,
                      float z) const {
  glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::set_vec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::set_vec4(const std::string &name, float x, float y, float z,
                      float w) const {
  glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::set_mat4(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}
