#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texture_coordinates;
in vec3 texcoords_anim;

out vec4 fragColor;
uniform float ka;
uniform float kd;
uniform float ks;
uniform vec4 lightPos[4];
uniform vec4 lightDir[4];
uniform vec4 lightColors[4];
uniform vec4 cAmbient;
uniform vec4 cDiffuse;
uniform vec4 cSpecular;
uniform int lightType[4];
uniform float a[4];
uniform float b[4];
uniform float c[4];
uniform float angle[4];
uniform float penumbra[4];
uniform int numLights;
uniform float shininess;
uniform vec4 cameraPos;

uniform vec3 font_colour;
uniform sampler2D alphabet_texture;
// uniform bool isText;
uniform bool isFog;

float basicFog() {
    vec3 cameraProj = vec3(cameraPos);
    cameraProj.y = 0.0;

    vec3 pixelProj = worldPosition;
    pixelProj.y = 0.0;

    float fogEnd = 60.0;
    float deltaD = length(cameraProj - pixelProj) / fogEnd;

    float deltaY = 0.0f;
    float density = 0.0f;

    float fogTop = 0.015;

    float heightFactor = 0.0;
    heightFactor = exp(-pow(worldPosition.y - fogTop, 2.0) * 10.0);

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
    float texture_value = texture(alphabet_texture, texture_coordinates).r;

    // if (!isText) {
    if (texture_value != 0) {
        if (!isFog) {
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
        } else {
            float fog = basicFog();
            fragColor = vec4(0.41, 0.45, 0.5, 1.0);
            vec3 fogColor = vec3(0.53, 0.58, 0.64);
            fragColor = mix(fragColor, vec4(fogColor, 1.0), fog);
            fragColor.a = mix(0.1, 1.0, fog);
        }
    } else {
        // fragColor = vec4(0, 0, 0, 1);
        discard;
    }

    // } else {
    //     fragColor = vec4(texture_value, texture_value, 1, 1);
    // }
}
