#version 330 core

layout (location = 0) out vec4 color;

uniform vec3 modelColor;

void main()
{
    color = vec4(modelColor, 1.0f);
}