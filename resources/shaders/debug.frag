#version 420 core

layout (location = 0) in vec3 fragPos;
layout (location = 1) in flat float fragQuadTreelevel;
layout (location = 2) in vec3 fragNormal;
layout (location = 3) in vec3 cubeMapCoord;

out vec4 outFragColor;

void main()
{
    outFragColor = vec4(1.0);
}
