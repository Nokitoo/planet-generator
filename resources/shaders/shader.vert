#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragColor;
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

    return pos;
}

float getHeight() {
    vec4 heightMapValue = texture(heightMap, cubeMapCoord);
    float height = heightMapValue.r + heightMapValue.g + heightMapValue.b;

    if (wireframe == 1) {
        return (height * heightStrength) + 0.05;
    }

    return height * heightStrength;
}

void main()
{
    fragPos = inPosition;

    // Convert position to range [-1.0, 1.0]
    cubeMapCoord = (fragPos + (planetSize / 2.0)) / planetSize * 2.0 - 1.0;

    // Map cube position [-1.0, 1.0] to sphere position [-1.0, 1.0]
    // and scale [-1.0, 1.0] position to planet scale
    fragPos = mapCubeToSphere(cubeMapCoord) * planetSize;

    // TODO: normal don't take into account height (How to calculate ?)
    fragNormal = normalize(fragPos);

    // Add height to frag position
    fragPos += (fragNormal * getHeight());

    gl_Position = proj * view * vec4(fragPos, 1.0);
    fragColor = inColor;
}
