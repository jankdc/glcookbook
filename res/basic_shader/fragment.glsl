#version 330 core

in vec3 ourColor;
in vec2 otherPos;
out vec4 finalColor;

void main()
{
    finalColor = vec4(otherPos, 0.0f, 1.0f);
}
