#version 420 core

layout (location = 0) in vec3 inCubePosition;
layout (location = 1) in vec3 inSpherePosition;
layout (location = 2) in vec3 inWidthDir;
layout (location = 3) in vec3 inHeightDir;
layout (location = 4) in float inQuadTreelevel;

layout (location = 0) out vec3 outPos;
layout (location = 1) out flat float outQuadTreelevel;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outCubeMapCoord;

uniform mat4 view;
uniform mat4 proj;
uniform float planetSize;
uniform samplerCube heightMap;

uniform float maxHeight;

// Formulas: http://mathproofs.blogspot.kr/2005/07/mapping-cube-to-sphere.html
vec3 mapCubeToSphere(vec3 pos)
{
    float x2 = pos.x * pos.x;
    float y2 = pos.y * pos.y;
    float z2 = pos.z * pos.z;

    pos.x = pos.x * sqrt(1.0 - (y2 * 0.5) - (z2 * 0.5) + ((y2 * z2) / 3.0));
    pos.y = pos.y * sqrt(1.0 - (z2 * 0.5) - (x2 * 0.5) + ((z2 * x2) / 3.0));
    pos.z = pos.z * sqrt(1.0 - (x2 * 0.5) - (y2 * 0.5) + ((x2 * y2) / 3.0));

    return normalize(pos);
}

float getHeight(vec3 heightMapCoord) {
    vec4 heightMapValue = texture(heightMap, heightMapCoord);

    return heightMapValue.r * maxHeight;
}

vec3 getNormalizedCubeCoord(vec3 worldCubeCoord) {
    return normalize((worldCubeCoord + (planetSize / 2.0)) / planetSize * 2.0 - 1.0);
}

vec3 getNormal() {
    float quadSize = pow(planetSize, 1.0 / inQuadTreelevel) * 2.0;

    /* To calculate the normal, we need to calculate the tangent and bitangent
     * which is done with neighbor positions
     *         o
     *         |
     *     o - x - o
     *         |
     *         o
    */
    // Calculate normalized cube position of neighbors
    vec3 leftCubePos = getNormalizedCubeCoord(inCubePosition - (inWidthDir * quadSize));
    vec3 rightCubePos = getNormalizedCubeCoord(inCubePosition + (inWidthDir * quadSize));
    vec3 topCubePos = getNormalizedCubeCoord(inCubePosition + (inHeightDir * quadSize));
    vec3 bottomCubePos = getNormalizedCubeCoord(inCubePosition - (inHeightDir * quadSize));

    // Calculate height of neighbors using cube positions
    float leftCubePosHeight = getHeight(leftCubePos);
    float rightCubePosHeight = getHeight(rightCubePos);
    float topCubePosHeight = getHeight(topCubePos);
    float bottomCubePosHeight = getHeight(bottomCubePos);

    // Calculate sphere position of neighbors
    vec3 leftSpherePos = mapCubeToSphere(leftCubePos) * planetSize;
    vec3 rightSpherePos = mapCubeToSphere(rightCubePos) * planetSize;
    vec3 topSpherePos = mapCubeToSphere(topCubePos) * planetSize;
    vec3 bottomSpherePos = mapCubeToSphere(bottomCubePos) * planetSize;

    // Add height on neighbors sphere positions
    leftSpherePos += (leftCubePosHeight * normalize(leftSpherePos));
    rightSpherePos += (rightCubePosHeight * normalize(rightSpherePos));
    topSpherePos += (topCubePosHeight * normalize(topSpherePos));
    bottomSpherePos += (bottomCubePosHeight * normalize(bottomSpherePos));

    vec3 tangent = normalize(rightSpherePos - leftSpherePos);
    vec3 bitangent = normalize(topSpherePos - bottomSpherePos);
    vec3 normal = normalize(cross(tangent, bitangent));

    return normal;
}

void main()
{
    outPos = inSpherePosition;

    // Convert position to range [-1.0, 1.0]
    outCubeMapCoord = getNormalizedCubeCoord(inCubePosition);

    vec3 vertexNormal = normalize(outPos);

    outNormal = getNormal();
    outQuadTreelevel = inQuadTreelevel;

    // Add height to out position
    outPos += (vertexNormal * getHeight(outCubeMapCoord));

    gl_Position = proj * view * vec4(outPos, 1.0);
}
