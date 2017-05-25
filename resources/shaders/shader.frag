#version 420 core

layout (location = 0) in vec3 fragPos;
layout (location = 1) in flat float fragQuadTreelevel;
layout (location = 2) in vec3 fragNormal;
layout (location = 3) in vec3 cubeMapCoord;

out vec4 outFragColor;

uniform vec3 lightDir = vec3(0.0, -0.5, -0.5);
uniform samplerCube heightMap;

uniform int wireframe;

vec3 getAmbient(vec3 color) {
    return color * 0.2;
}

vec3 getDiffuse(vec3 color) {
    vec3 normal = normalize(fragNormal);
    float diff = max(dot(normalize(-lightDir), normal), 0.0);

    return color * diff;
}

void main()
{
    vec3 color = vec3(fragQuadTreelevel / 10.0, fragQuadTreelevel / 10.0, 0.0);
    vec3 ambient = getAmbient(color);
    vec3 diffuse = getDiffuse(color);

    if (wireframe == 1) {
        outFragColor = vec4(0.0);
    }
    else {
        outFragColor = vec4(ambient + diffuse, 1.0);
    }
}
