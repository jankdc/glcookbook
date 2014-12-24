#version 330 core

struct Material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float a;
};

struct Light {
    vec3 pos;
    vec3 ka;
    vec3 kd;
    vec3 ks;
};

in vec3 vertexPos;
in vec3 vertexNormal;

layout (location = 0) out vec4 color;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

void main()
{
    vec3 ld = normalize(light.pos - vertexPos);
    vec3 v  = normalize(viewPos - vertexPos);
    vec3 rd = reflect(-ld, vertexNormal);
    vec3 n  = normalize(vertexNormal);

    vec3 ka = light.ka * material.ka;
    vec3 kd = light.kd * material.kd * max(dot(n, ld), 0.0f);
    vec3 ks = light.ks * material.ks * pow(max(dot(v, rd), 0.0), material.a);

    color = vec4(ka + kd + ks, 1.0f);
}