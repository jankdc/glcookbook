#version 330 core

layout (location = 0) in vec3 position;

// We set this variable in the OpenGL code.
uniform vec4 ourColor;

void main() {
  // See how we directly give a vec3 to vec4's contructor.
  gl_Position = vec4(position, 1.0);
}
