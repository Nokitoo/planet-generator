#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec3 inNormal;

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragColor;
layout (location = 2) out vec3 fragNormal;

uniform mat4 view;
uniform mat4 proj;
uniform float planetSize;

// Formulas: http://mathproofs.blogspot.kr/2005/07/mapping-cube-to-sphere.html
vec3 mapCubeToSphere(vec3 pos)
{
    float x2 = pos.x * pos.x;
    float y2 = pos.y * pos.y;
    float z2 = pos.z * pos.z;

    pos.x = pos.x * sqrt(1.0 - (y2 * 0.5) - (z2 * 0.5) + ((y2 * z2) / 3.0));
    pos.y = pos.y * sqrt(1.0 - (z2 * 0.5) - (x2 * 0.5) + ((z2 * x2) / 3.0));
    pos.z = pos.z * sqrt(1.0 - (x2 * 0.5) - (y2 * 0.5) + ((x2 * y2) / 3.0));

    // Convert from range [-1.0, 1.0] to range [0.0, planetSize]
    pos.xy = (pos.xy / 2.0 + 0.5) * planetSize;
    pos.z = (pos.z / 2.0 + 0.5) * -planetSize;
    return pos;
}

void main()
{
    fragPos = inPosition;

    // Convert position to range [-1.0, 1.0]
    fragPos.xy = fragPos.xy / planetSize * 2.0 - 1.0;
    fragPos.z = fragPos.z / -planetSize * 2.0 - 1.0;

    fragPos = mapCubeToSphere(fragPos);
    gl_Position = proj * view * vec4(fragPos, 1.0);
    fragColor = inColor;
    fragNormal = normalize(fragPos);
}
