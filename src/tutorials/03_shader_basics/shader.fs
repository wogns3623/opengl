#version 330 core

// input from vertex shader
in vec3 vertex_color;

out vec4 frag_color;

void main() {
  frag_color = vec4(vertex_color, 1.0);
}
