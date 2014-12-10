#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    vec4 texSample1 = texture(ourTexture1, texCoord);
    vec4 texSample2 = texture(ourTexture2, vec2(-texCoord.x, texCoord.y));
    color = mix(texSample1, texSample2, 0.2);
}