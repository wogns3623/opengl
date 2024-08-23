#version 330 core

in vec2 texture_coord;

out vec4 frag_color;

uniform sampler2D texture1;

void main() {
  frag_color = texture(texture1, texture_coord);
}
