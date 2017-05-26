#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inWidthDir;
layout (location = 2) in vec3 inHeightDir;
layout (location = 3) in float inQuadTreelevel;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out flat float fragQuadTreelevel;
layout (location = 2) out vec3 fragNormal;
layout (location = 3) out vec3 cubeMapCoord;

uniform mat4 view;
uniform mat4 proj;
uniform float planetSize;
uniform samplerCube heightMap;
uniform int wireframe;

uniform float heightStrength = 6.0;

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
    float height = heightMapValue.r + heightMapValue.g + heightMapValue.b;

    if (wireframe == 1) {
        return (height * heightStrength) + 0.05;
    }

    return height * heightStrength;
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
    vec3 leftCubePos = getNormalizedCubeCoord(inPosition - (inWidthDir * quadSize));
    vec3 rightCubePos = getNormalizedCubeCoord(inPosition + (inWidthDir * quadSize));
    vec3 topCubePos = getNormalizedCubeCoord(inPosition + (inHeightDir * quadSize));
    vec3 bottomCubePos = getNormalizedCubeCoord(inPosition - (inHeightDir * quadSize));

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
    fragPos = inPosition;

    // Convert position to range [-1.0, 1.0]
    cubeMapCoord = getNormalizedCubeCoord(fragPos);

    // Map cube position [-1.0, 1.0] to sphere position [-1.0, 1.0]
    // and scale [-1.0, 1.0] position to planet scale
    fragPos = mapCubeToSphere(cubeMapCoord) * planetSize;

    vec3 vertexNormal = normalize(fragPos);

    fragNormal = getNormal();
    fragQuadTreelevel = inQuadTreelevel;

    // Add height to frag position
    fragPos += (vertexNormal * getHeight(cubeMapCoord));

    gl_Position = proj * view * vec4(fragPos, 1.0);
}
