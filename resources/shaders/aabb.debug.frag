#version 420 core

layout (location = 0) in vec4 inColor;

out vec4 outFragColor;

void main()
{
    outFragColor = inColor;
}
