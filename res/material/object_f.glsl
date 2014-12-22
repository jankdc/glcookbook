#version 330 core

in vec3 vertexPos;
in vec3 vertexNormal;

layout (location = 0) out vec4 color;

uniform vec3 modelColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 ld = normalize(lightPos - vertexPos);
    vec3 v  = normalize(viewPos - vertexPos);
    vec3 rd = reflect(-ld, vertexNormal);
    vec3 n  = normalize(vertexNormal);
    int  a  = 64;

    vec3 ka = vec3(0.2f);
    vec3 kd = vec3(0.5f) * max(dot(n, ld), 0.0f);
    vec3 ks = vec3(0.5f) * pow(max(dot(v, rd), 0.0), a);

    color = vec4((ka + kd + ks) * modelColor, 1.0f);
}