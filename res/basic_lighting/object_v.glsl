#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat3 modelNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexPos;
out vec3 vertexNormal;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0f);
    vertexPos = vec3(model * vec4(pos, 1.0f));
    vertexNormal = modelNormal * normal;
}