#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;

uniform mat3 Normal;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 vertexPosition;
out vec3 vertexNormal;
out vec2 vertexTexture;

void main()
{
    vertexNormal   = Normal * normal;
    vertexPosition = vec3(Model * vec4(position, 1.0f));
    vertexTexture  = texture;

    gl_Position = Projection * View * Model * vec4(position, 1.0f);
}