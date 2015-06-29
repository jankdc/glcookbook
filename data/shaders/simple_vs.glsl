#version 330 core

layout (location = 0) in vec3 position;

// Specify a color output to the fragment shader.
out vec4 vertexColor;

void main() {
  // See how we directly give a vec3 to vec4's contructor.
  gl_Position = vec4(position, 1.0);
  // Dark-reddish color.
  vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
}
