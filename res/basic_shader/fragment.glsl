#version 330 core

in vec3 ourColor;
out vec4 finalColor;

void main()
{
    finalColor = vec4(ourColor, 1.0f);
}
