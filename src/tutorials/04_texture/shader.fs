#version 330 core

in vec3 our_color;
in vec2 texture_coord;

out vec4 frag_color;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float texture2_alpha;

void main() {
  frag_color = mix(texture(texture1, texture_coord), texture(texture2, texture_coord), texture2_alpha);
}
