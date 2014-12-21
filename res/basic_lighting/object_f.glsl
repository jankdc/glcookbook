#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;

layout (location = 0) out vec4 color;

uniform vec3 modelColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 ambientColor = vec3(0.2f);
    vec3 diffuseColor = vec3(0.5f);
    vec3 specularColor = vec3(0.5f);
    vec3 norm = normalize(vertexNormal);
    vec3 lightDir = normalize(lightPos - vertexPos);
    vec3 viewDir = normalize(viewPos - vertexPos);
    vec3 reflectDir = reflect(-lightDir, vertexNormal);
    float diffuse = max(dot(norm, lightDir), 0.0f);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    diffuseColor = diffuse * diffuseColor;
    specularColor = specular * specularColor;
    vec3 result = (ambientColor + diffuseColor + specularColor) * modelColor;

    color = vec4(result, 1.0f);
}