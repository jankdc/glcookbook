#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;

uniform float offset;

out vec3 ourColor;
out vec2 otherPos;

void main()
{
    gl_Position = vec4(pos.x + offset, pos.y, 0.0, 1.0);
    otherPos = pos;
    ourColor = color;
}
