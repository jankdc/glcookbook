#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex;

out vec3 fragColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(pos, 0.0, 1.0);

    fragColor = color;
    texCoord = tex;
}