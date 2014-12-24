#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat3 Normal;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 vertexPosition;
out vec3 vertexNormal;

void main()
{
    vertexNormal   = Normal * normal;
    vertexPosition = vec3(Model * vec4(position, 1.0f));

    gl_Position = Projection * View * Model * vec4(position, 1.0f);
}