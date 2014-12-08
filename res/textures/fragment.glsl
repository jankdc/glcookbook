#version 330 core

in vec3 fragColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D outTexture;

void main()
{
    color = texture(outTexture, texCoord);
}