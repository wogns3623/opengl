#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <stb_image.h>

#include <iostream>
#include <string>

class Texture {
private:
  void load_data(const std::string &path, GLenum input_data_format,
                 GLenum generated_texture_format);

public:
  GLuint id;
  int width;
  int height;
  int nr_channels;

  Texture(const std::string &path, GLenum input_data_format,
          GLenum generated_texture_format);
};

#endif
