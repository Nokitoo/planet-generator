#version 420 core

layout(location = 0) in vec2 texCoord;

layout(rgba32f, binding = 0) readonly uniform imageCube heightMap;

out vec4 outFragColor1;
out vec4 outFragColor2;
out vec4 outFragColor3;
out vec4 outFragColor4;
out vec4 outFragColor5;
out vec4 outFragColor6;

uniform float maxHeight;

float getHeight(ivec3 heightMapCoord) {
    vec4 heightMapValue = imageLoad(heightMap, heightMapCoord);

    return heightMapValue.r * maxHeight;
}

vec3 calculateNormalSobel(int layer) {
    ivec3 faceTexCoord = ivec3(texCoord, layer);

    // Calculate height of neighbors using cube positions
    float topLeft = getHeight(faceTexCoord + ivec3(-1, -1, 0));
    float top = getHeight(faceTexCoord + ivec3(0, -1, 0));
    float topRight = getHeight(faceTexCoord + ivec3(1, -1, 0));
    float right = getHeight(faceTexCoord + ivec3(1, 0, 0));
    float bottomRight = getHeight(faceTexCoord + ivec3(1, 1, 0));
    float bottom = getHeight(faceTexCoord + ivec3(0, 1, 0));
    float bottomLeft = getHeight(faceTexCoord + ivec3(-1, 1, 0));
    float left = getHeight(faceTexCoord + ivec3(-1, 0, 0));

    // Apply sobel filter
    vec3 normal;
    // Horizontal sobel
    // 1  0  -1
    // 2  0  -2
    // 1  0  -1
    normal.x = topLeft - topRight + (2.0 * left) - (2.0 * right) + bottomLeft - bottomRight;

    // Vertical sobel
    // 1  2  1
    // 0  0  0
    //-1 -2 -1
    normal.z = topLeft + (2.0 * top) + topRight - bottomLeft - (2.0 * bottom) - bottomRight;

    float normalStrength = 5.0;
    normal.y = 1.0 / normalStrength;

    normal = normalize(normal);

    // Convert normal from range [-1, 1] to range [0, 1]
    normal = (normal + 1.0f) * 0.5f;

    return normal;
}

vec3 calculateNormalCentralDifferences(int layer) {
    ivec3 faceTexCoord = ivec3(texCoord, layer);

    // Calculate height of neighbors using cube positions
    float top = getHeight(faceTexCoord + ivec3(0, -1, 0));
    float right = getHeight(faceTexCoord + ivec3(1, 0, 0));
    float bottom = getHeight(faceTexCoord + ivec3(0, 1, 0));
    float left = getHeight(faceTexCoord + ivec3(-1, 0, 0));

    vec3 normal = normalize(vec3(
        left - right,
        2.0,
        top - bottom
    ));

    normal = (normal + 1.0f) * 0.5f;

    return normal;
}

void main() {
    outFragColor1 = vec4(calculateNormalSobel(0), 1.0);
    outFragColor2 = vec4(calculateNormalSobel(1), 1.0);
    outFragColor3 = vec4(calculateNormalSobel(2), 1.0);
    outFragColor4 = vec4(calculateNormalSobel(3), 1.0);
    outFragColor5 = vec4(calculateNormalSobel(4), 1.0);
    outFragColor6 = vec4(calculateNormalSobel(5), 1.0);
}
