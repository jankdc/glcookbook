#version 330 core

// This is the final color output for each fragment.
out vec4 color;

// We set this variable in the OpenGL code.
uniform vec4 ourColor;

void main() {
  color = ourColor;
}
