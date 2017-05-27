#version 420 core

layout(triangles) in;
layout(line_strip, max_vertices = 3) out;

layout (location = 0) in vec3 inPos[];
layout (location = 1) in flat float inQuadTreelevel[];
layout (location = 2) in vec3 inNormal[];
layout (location = 3) in vec3 inCubeMapCoord[];

layout (location = 0) out vec3 outPos;
layout (location = 1) out flat float outQuadTreelevel;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outCubeMapCoord;

void main()
{
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;

        outPos = inPos[i];
        outQuadTreelevel = inQuadTreelevel[i];
        outPos = inPos[i];
        outNormal = inNormal[i];
        outQuadTreelevel = inQuadTreelevel[i];

        EmitVertex();
    }
    EndPrimitive();
}
