#version 420 core

layout(triangles) in;
layout(line_strip, max_vertices = 11) out;

layout (location = 0) in vec3 inPos[];
layout (location = 1) in vec3 inNormal[];
layout (location = 2) in vec3 inCubeMapCoord[];

layout (location = 0) out vec3 outColor;

uniform mat4 view;
uniform mat4 proj;
uniform int wireframeDisplayed;
uniform int verticesNormalsDisplayed;
uniform int facesNormalsDisplayed;

void emitWireframe() {
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = gl_in[i].gl_Position;
        outColor = vec3(1.0);
        EmitVertex();
    }
    EndPrimitive();
}

void emitVerticesNormals() {
    mat4 projViewMatrix = proj * view;
    for (int i = 0; i < 3; ++i)
    {
        gl_Position = projViewMatrix * vec4(inPos[i], 1.0);
        outColor = vec3(0.0, 1.0, 0.0);
        EmitVertex();

        gl_Position = projViewMatrix * vec4(inPos[i] + (inNormal[i] * 5.0), 1.0);
        outColor = vec3(1.0);
        EmitVertex();

        EndPrimitive();
    }
}

void emitFacesNormals() {
    mat4 projViewMatrix = proj * view;
    vec3 p0 = inPos[0];
    vec3 p1 = inPos[1];
    vec3 p2 = inPos[2];

    vec3 v0 = p0 - p1;
    vec3 v1 = p2 - p1;
    vec3 normal = normalize(cross(v1, v0));

    vec3 center = (p0 + p1 + p2) / 3.0;

    gl_Position = projViewMatrix * vec4(center, 1.0);
    outColor = vec3(0.0, 0.0, 1.0);
    EmitVertex();

    gl_Position = projViewMatrix * vec4(center + (normal * 5.0), 1.0);
    outColor = vec3(1.0, 0.0, 0.0);
    EmitVertex();
    EndPrimitive();
}

void main()
{
    if (wireframeDisplayed == 1) {
        emitWireframe();
    }

    if (verticesNormalsDisplayed == 1) {
        emitVerticesNormals();
    }

    if (facesNormalsDisplayed == 1) {
        emitFacesNormals();
    }
}
