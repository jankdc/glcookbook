#version 330 core

layout (location = 0) out vec4 finalColor;

uniform vec3 MainColor;

void main()
{
    finalColor = vec4(MainColor, 1.0f);
}