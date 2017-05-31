#version 420 core

layout (location = 0) in vec3 inPosition;

layout (location = 0) out vec4 outColor;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    outColor = vec4(1.0, 0.2, 0.2, 0.5);

    gl_Position = proj * view * vec4(inPosition, 1.0);
}
