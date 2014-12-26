#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexture;

struct material {
    sampler2D kd;
    sampler2D ks;
    float a;
};

struct dlight {
    vec3 direction;
    vec3 ka;
    vec3 kd;
    vec3 ks;
};

struct plight {
    vec3 position;
    vec3 ka;
    vec3 kd;
    vec3 ks;

    float kc;
    float kl;
    float kq;
};

vec3 getDirLight(dlight light, vec3 normal, vec3 viewDir);
vec3 getPointLight(plight light, vec3 normal, vec3 viewDir, vec3 fragPos);

uniform plight PLight;
uniform dlight DLight;
uniform material Material;
uniform vec3 CameraPosition;

out vec4 finalColor;

void main()
{
    vec3 viewDir = normalize(CameraPosition - vertexPosition);
    vec3 normal  = normalize(vertexNormal);

    vec3 res = vec3(0.0f);
    res += getDirLight(DLight, normal, viewDir);
    res += getPointLight(PLight, normal, viewDir, vertexPosition);

    finalColor = vec4(res, 1.0f);
}

vec3 getDirLight(dlight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 diffuseMap = vec3(texture(Material.kd, vertexTexture));
    vec3 specularMap = vec3(texture(Material.ks, vertexTexture));

    float color = max(dot(normal, lightDir), 0.0f);
    float intensity = pow(max(dot(viewDir, reflectDir), 0.0), Material.a);

    vec3 ka = light.ka * diffuseMap;
    vec3 kd = light.kd * diffuseMap * color;
    vec3 ks = light.ks * specularMap * intensity;

    return (ka + kd + ks);
}

vec3 getPointLight(plight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 diffuseMap = vec3(texture(Material.kd, vertexTexture));
    vec3 specularMap = vec3(texture(Material.ks, vertexTexture));

    float color = max(dot(normal, lightDir), 0.0f);
    float intensity = pow(max(dot(viewDir, reflectDir), 0.0), Material.a);

    float dist = length(light.position - fragPos);
    float atten = 1.0f / (PLight.kc + PLight.kl * dist + PLight.kq * (dist * dist));

    vec3 ka = light.ka * diffuseMap * atten;
    vec3 kd = light.kd * diffuseMap * color * atten;
    vec3 ks = light.ks * specularMap * intensity * atten;

    return (ka + kd + ks);
}