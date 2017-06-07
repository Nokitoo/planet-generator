#version 420 core

layout (location = 0) in vec3 inCubePosition;
layout (location = 1) in vec3 inSpherePosition;
layout (location = 2) in vec3 inWidthDir;
layout (location = 3) in vec3 inHeightDir;
layout (location = 4) in float inQuadTreelevel;

layout (location = 0) out vec3 outPos;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec3 outCubeMapCoord;
layout (location = 3) out vec3 outTangent;
layout (location = 4) out vec3 outBitangent;

uniform mat4 view;
uniform mat4 proj;
uniform float planetSize;
uniform samplerCube heightMap;
uniform samplerCube normalMap;

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

float getQuadSize() {
    float squareSize = planetSize;
    for (int i = 0; i < inQuadTreelevel; ++i) {
        squareSize = squareSize / 2.0;
    }

    return squareSize;
}

void calculateTangent() {
    float quadSize = getQuadSize();

    vec3 rightCubePos = getNormalizedCubeCoord(inCubePosition + (inWidthDir * quadSize));
    vec3 topCubePos = getNormalizedCubeCoord(inCubePosition + (inHeightDir * quadSize));

    vec3 rightSpherePos = mapCubeToSphere(rightCubePos) * planetSize;
    vec3 topSpherePos = mapCubeToSphere(topCubePos) * planetSize;

    rightSpherePos += normalize(rightSpherePos) + getHeight(rightCubePos);
    topSpherePos += normalize(topSpherePos) + getHeight(topCubePos);

    outTangent = normalize(rightSpherePos - outPos);
    outBitangent = normalize(topSpherePos - outPos);
}

void main()
{
    outPos = inSpherePosition;

    // Convert position to range [-1.0, 1.0]
    outCubeMapCoord = getNormalizedCubeCoord(inCubePosition);

    vec3 vertexNormal = normalize(inSpherePosition);

    outNormal = normalize(inSpherePosition);

    // Add height to out position
    outPos += (vertexNormal * getHeight(outCubeMapCoord));

    gl_Position = proj * view * vec4(outPos, 1.0);

    calculateTangent();
}
