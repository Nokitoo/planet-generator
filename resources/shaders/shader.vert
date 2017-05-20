#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragColor;
layout (location = 2) out vec3 fragNormal;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * vec4(inPosition, 1.0);
    fragPos = inPosition;
    fragColor = inColor;
    fragNormal = inNormal;
}
