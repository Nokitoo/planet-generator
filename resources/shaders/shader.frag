#version 420 core

layout (location = 0) in vec3 fragPos;
layout (location = 1) in vec3 fragColor;
layout (location = 2) in vec3 fragNormal;

out vec4 outFragColor;

uniform vec3 lightDir = vec3(0.0, -0.5, 0.5);

vec3 getAmbient() {
    return fragColor * 0.2;
}

vec3 getDiffuse() {
    vec3 normal = normalize(fragNormal);
    float diff = max(dot(normalize(lightDir), normal), 0.0);

    return fragColor * diff;
}

void main()
{
    vec3 ambient = getAmbient();
    vec3 diffuse = getDiffuse();

    outFragColor = vec4(ambient + diffuse, 1.0);
}
