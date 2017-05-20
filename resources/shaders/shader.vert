#version 420 core

layout (location = 0) in vec3 inPosition;

layout (location = 0) out vec3 fragPos;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    gl_Position = proj * view * vec4(inPosition, 1.0);
    fragPos = inPosition;
}
