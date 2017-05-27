#version 420 core

layout (location = 0) in vec3 inColor;

out vec4 outFragColor;

void main()
{
    outFragColor = vec4(inColor, 1.0);
}
