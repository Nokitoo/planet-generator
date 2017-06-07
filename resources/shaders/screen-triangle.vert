#version 420 core

layout(location = 0) out vec2 texCoord;

uniform float imageSize;

void main()
{
    float x = -1.0 + float((gl_VertexID & 1) << 2);
    float y = -1.0 + float((gl_VertexID & 2) << 1);

    // The width and the height are the same
    // because it's a cube map
    texCoord.x = (x + 1.0) * 0.5 * imageSize;
    texCoord.y = (y + 1.0) * 0.5 * imageSize;

    gl_Position = vec4(x, y, 0, 1);
}
