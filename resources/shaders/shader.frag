#version 420 core

layout (location = 0) in vec3 fragPos;
layout (location = 1) in vec3 fragNormal;
layout (location = 2) in vec3 cubeMapCoord;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

uniform samplerCube heightMap;
uniform samplerCube normalMap;
uniform float planetSize;
uniform float maxHeight;

out vec4 outFragColor;

uniform vec3 lightDir = vec3(0.0, -0.5, -0.5);

float getHeight(vec3 heightMapCoord) {
    vec4 heightMapValue = texture(heightMap, heightMapCoord);

    return heightMapValue.r * maxHeight;
}

vec3 getNormal() {
    // Convert normal from range [0, 1] to range [-1, 1]
    vec3 worldNormal = 2.0f * texture(normalMap, cubeMapCoord).rgb - 1.0f;
    worldNormal = normalize(worldNormal);

    // Construct tangent, bitangent, normal matrix
    mat3 TBN = mat3(inTangent, normalize(fragNormal), inBitangent);

    return normalize(TBN * worldNormal);
}

vec3 getAmbient(vec3 color) {
    return color * 0.2;
}

vec3 getDiffuse(vec3 color) {
    vec3 normal = getNormal();

    float diff = max(dot(normalize(-lightDir), normal), 0.0);

    return color * diff;
}

void main()
{
    vec3 color = vec3(0.5);
    vec3 ambient = getAmbient(color);
    vec3 diffuse = getDiffuse(color);

    outFragColor = vec4(ambient + diffuse, 1.0);
}
