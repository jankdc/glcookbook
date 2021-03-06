#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;

struct material {
    vec3 ka;
    vec3 kd;
    vec3 ks;
    float a;
};

struct light {
    vec3 position;
    vec3 ka;
    vec3 kd;
    vec3 ks;
};

uniform light Light;
uniform material Material;
uniform vec3 CameraPosition;

out vec4 finalColor;

void main()
{
    vec3 ld = normalize(Light.position - vertexPosition);
    vec3 v  = normalize(CameraPosition - vertexPosition);
    vec3 rd = reflect(-ld, vertexNormal);
    vec3 n  = normalize(vertexNormal);

    vec3 ka = Light.ka * Material.ka;
    vec3 kd = Light.kd * Material.kd * max(dot(n, ld), 0.0f);
    vec3 ks = Light.ks * Material.ks * pow(max(dot(v, rd), 0.0), Material.a);

    finalColor = vec4(ka + kd + ks, 1.0f);
}