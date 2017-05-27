#version 420 core

layout(triangles) in;
layout(line_strip, max_vertices = 9) out;

layout (location = 0) in vec3 inPos[];
layout (location = 1) in flat float inQuadTreelevel[];
layout (location = 2) in vec3 inNormal[];
layout (location = 3) in vec3 inCubeMapCoord[];

layout (location = 0) out vec3 outColor;

uniform mat4 view;
uniform mat4 proj;
uniform int wireframeDisplayed;
uniform int normalsDisplayed;

void emitWireframe() {
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        outColor = vec3(1.0);
        EmitVertex();
    }
    EndPrimitive();
}

void emitNormals() {
    mat4 projViewMatrix = proj * view;
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = projViewMatrix * vec4(inPos[i], 1.0);
        outColor = vec3(0.0, 0.0, 1.0);
        EmitVertex();

        gl_Position = projViewMatrix * vec4(inPos[i] + (inNormal[i] * 5.0), 1.0);
        outColor = vec3(1.0, 0.0, 0.0);
        EmitVertex();

        EndPrimitive();
    }
}

void main()
{
    if (wireframeDisplayed == 1) {
        emitWireframe();
    }

    if (normalsDisplayed == 1) {
        emitNormals();
    }
}
