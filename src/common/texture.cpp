#include "texture.h"

void Texture::load_data(const std::string &path,
                        GLenum input_data_format = GL_RGB,
                        GLenum generated_texture_format = GL_RGB) {
  unsigned char *file_data =
      stbi_load(path.c_str(), &width, &height, &nr_channels, 0);

  if (file_data) {
    GLint generate_mipmap_level = 0; // base level
    GLenum input_data_type = GL_UNSIGNED_BYTE;
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, generate_mipmap_level, generated_texture_format,
                 width, height, 0, input_data_format, input_data_type,
                 file_data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture: " << path << std::endl;
  }

  stbi_image_free(file_data);
}

Texture::Texture(const std::string &path, GLenum input_data_format,
                 GLenum generated_texture_format) {
  glGenTextures(1, &id);
  load_data(path, input_data_format, generated_texture_format);
}
