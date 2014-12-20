#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;

layout (location = 0) out vec4 color;

uniform vec3 modelColor;
uniform vec3 lightPos;

void main()
{
    vec3 ambientColor = vec3(0.2f);
    vec3 diffuseColor = vec3(0.5f);
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(lightPos - vertexPos);
    float diffuse = max(dot(norm, lightDir), 0.0f);
    diffuseColor = diffuse * diffuseColor;
    vec3 result = (ambientColor + diffuseColor) * modelColor;

    color = vec4(result, 1.0f);
}