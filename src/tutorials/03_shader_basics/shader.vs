#version 330 core

// layout (location = 0) mean the a_pos variable has attribute position 0
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_color;

// output to fragment shader
out vec3 vertex_color;

void main() {
  gl_Position = vec4(a_pos, 1.0);
  vertex_color = a_color;
}
