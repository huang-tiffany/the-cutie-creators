#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
// in vec3 points[1000];
// uniform sampler2D pointTexture;
uniform int numPoints;
out vec4 fragColor;
uniform float ka;
uniform float kd;
uniform float ks;
uniform vec4 lightPos[8];
uniform vec4 lightDir[8];
uniform vec4 lightColors[8];
uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform int lightType[8];
uniform float a[8];
uniform float b[8];
uniform float c[8];
uniform float angle[8];
uniform float penumbra[8];
uniform int numLights;
uniform float shininess;
uniform vec4 cameraPos;
// uniform vec2 screenSize;

// const vec3 offsets[27] = vec3[](
//     vec3(0, 0, 0),
//     vec3(0, 0, 1),
//     vec3(-1, 1, 1),
//     vec3(-1, 0, 1),
//     vec3(-1, -1, 1),
//     vec3(0, 1, 1),
//     vec3(0, -1, 1),
//     vec3(1, 1, 1),
//     vec3(1, 0, 1),
//     vec3(1, -1, 1),
//     vec3(0, 0, -1),
//     vec3(-1, 1, -1),
//     vec3(-1, 0, -1),
//     vec3(-1, -1, -1),
//     vec3(0, 1, -1),
//     vec3(0, -1, -1),
//     vec3(1, 1, -1),
//     vec3(1, 0, -1),
//     vec3(1, -1, -1),
//     vec3(-1, 1, 0),
//     vec3(-1, 0, 0),
//     vec3(-1, -1, 0),
//     vec3(0, 1, 0),
//     vec3(0, -1, 0),
//     vec3(1, 1, 0),
//     vec3(1, 0, 0),
//     vec3(1, -1, 0)
// );

// float maxComponent(vec3 vec) {
//     return max(vec.x, max(vec.y, vec.z));
// }

// float minComponent(vec3 vec) {
//     return min(vec.x, min(vec.y, vec.z));
// }

// vec3 getPointFromTexture(int index) {
//     return texelFetch(pointTexture, ivec2(index, 0), 0).xyz;
// }

// float worley(vec3 samplePos) {
//     int numCells = 10;
//     vec3 cellID = floor(samplePos * numCells);
//     float minSqrDst = 1;

//     for (int cellOffsetIndex = 0; cellOffsetIndex < 27; cellOffsetIndex++) {
//         vec3 adjID = cellID + offsets[cellOffsetIndex];
//         if (minComponent(adjID) == -1 || maxComponent(adjID) == numCells) {
//             vec3 wrappedID = mod(adjID + numCells, float(numCells));
//             int adjCellIndex = int(wrappedID[0]) + numCells * (int(wrappedID[1]) + int(wrappedID[2]) * numCells);
//             vec3 wrappedPoint = getPointFromTexture(adjCellIndex);
//             for (int wrapOffsetIndex = 0; wrapOffsetIndex < 27; wrapOffsetIndex ++) {
//                 vec3 sampleOffset = (samplePos - (wrappedPoint + offsets[wrapOffsetIndex]));
//                 minSqrDst = min(minSqrDst, dot(sampleOffset, sampleOffset));
//             }
//         } else {
//             int adjCellIndex = int(adjID[0]) + numCells * (int(adjID[1]) + int(adjID[2]) * numCells);
//             vec3 sampleOffset = samplePos - getPointFromTexture(adjCellIndex);
//             minSqrDst = min(minSqrDst, dot(sampleOffset, sampleOffset));
//         }
//     }
//     return sqrt(minSqrDst);
// }

float basicFog() {
    vec3 cameraProj = vec3(cameraPos);
    cameraProj.y = 0.0;

    vec3 pixelProj = worldPosition;
    pixelProj.y = 0.0;

    float fogEnd = 100.0;
    float deltaD = length(cameraProj - pixelProj) / fogEnd;

    float deltaY = 0.0f;
    float density = 0.0f;

    float fogTop = 10.0;
    float fogStart = 0.1;
    float fogEndHeight = -0.4;

    float heightFactor = 0.0;
    heightFactor = clamp(smoothstep(fogStart, fogEndHeight, worldPosition.y), 0.0, 1.0);

    if (cameraPos.y > fogTop) {
        if (worldPosition.y < fogTop) {
            deltaY = (fogTop - worldPosition.y) / fogTop;
            density = deltaY * deltaY * 0.5 * heightFactor;
        }
    } else {
        if (worldPosition.y < fogTop) {
            deltaY = abs(cameraPos.y - worldPosition.y) / fogTop;
            float deltaCamera = (fogTop - cameraPos.y) / fogTop;
            float densityCamera = deltaCamera * deltaCamera * 0.5;
            float deltaPixel = (fogTop - worldPosition.y) / fogTop;
            float densityPixel = deltaPixel * deltaPixel * 0.5;
            density = abs(densityCamera - densityPixel) * heightFactor;
        } else {
            deltaY = (fogTop - cameraPos.y) / fogTop;
            density = deltaY * deltaY * 0.5 * heightFactor;
        }
    }

    float fogDensity = 0.0;

    if (deltaY != 0) {
        fogDensity = (sqrt(1.0 + ((deltaD / deltaY) * (deltaD / deltaY)))) * density;
    }

    return exp(-fogDensity);
}

void main() {
    // vec3 newWorldPos = worldPosition;
    // newWorldPos[1] = 0;

    // if (newWorldPos[0] > 0.5 && newWorldPos[0] < 0.75) {
    fragColor = ka * cAmbient;
    for (int i = 0; i < numLights; ++i) {
        float att = 1.f;
        vec3 currLightDir = normalize(vec3(lightPos[i]) - worldPosition);
        vec4 lightIntensity = lightColors[i];
        float dist = length(vec3(lightPos[i]) - worldPosition);
        if (lightType[i] == 0) {
            currLightDir = -normalize(vec3(lightDir[i]));
        } else if (lightType[i] == 1) {
            att = min(1.f, 1.f / (a[i] + b[i] * dist + c[i] * dist * dist));
        } else if (lightType[i] == 2) {
            att = min(1.f, 1.f / (a[i] + b[i] * dist + c[i] * dist * dist));
            float x = acos(dot(-currLightDir, normalize(vec3(lightDir[i]))));
            float innerAngle = angle[i] - penumbra[i];
            float falloff = -2.f * pow((x - innerAngle) / (angle[i] - innerAngle), 3.f) + 3.f * pow((x - innerAngle) / (angle[i] - innerAngle), 2.f);
            if (x <= innerAngle) {
                lightIntensity = lightColors[i];
            } else if (x > innerAngle && x <= angle[i]) {
                lightIntensity *= (1.f - falloff);
            } else {
                lightIntensity = vec4(0, 0, 0, 1);
            }
        }
        float nl = clamp(dot(normalize(worldNormal), currLightDir), 0.0, 1.0);
        float diffuse = kd * nl;
        fragColor += att * diffuse * lightIntensity * cDiffuse;
        vec3 r = normalize(reflect(-currLightDir, normalize(worldNormal)));
        vec3 v = normalize(vec3(cameraPos) - worldPosition);
        float rv = clamp(dot(r, v), 0.0, 1.0);
        float specular;
        if (shininess <= 0.0) {
            specular = 0.0;
        } else {
            specular = ks * pow(rv, shininess);
        }
        fragColor += att * specular * lightColors[i] * cSpecular;
    }
    fragColor.a = 1.0;
<<<<<<< HEAD
    // } else {
    //     fragColor = vec4(0, 0, 0, 1);
=======

    if (worldPosition.y < 0.1) {
        float fog = basicFog();
        vec3 fogColor = vec3(0.62, 0.68, 0.75);
        fragColor = mix(vec4(fogColor, 1.0), fragColor, fog);
    }

    // if (worldPosition.y < 0.01) {
    //     vec2 fragUV = gl_FragCoord.xy / screenSize;
    //     vec3 samplePos = vec3(fragUV, 0.0);
    //     float noiseValue = worley(samplePos);
    //     fragColor = vec4(vec3(noiseValue), 1.0);
>>>>>>> d2fcf45 (basic fog)
    // }
}
