#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texture_coordinates;
in vec3 texcoords_anim;

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

uniform vec3 font_colour;
uniform sampler2D alphabet_texture;
// uniform bool isText;

void main() {
    float texture_value = texture(alphabet_texture, texture_coordinates).r;

    // if (!isText) {
    if (texture_value != 0) {
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
        // fragColor = vec4(0, 0, 0, 1);
        discard;
    }

    // } else {
    //     fragColor = vec4(texture_value, texture_value, 1, 1);
    // }
}
