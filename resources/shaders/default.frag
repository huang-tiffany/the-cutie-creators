// #version 330 core

// in vec3 worldPosition;
// in vec3 worldNormal;
// out vec4 fragColor;
// uniform float ka;
// uniform float kd;
// uniform float ks;
// uniform vec4 lightPos[8];
// uniform vec4 lightDir[8];
// uniform vec4 lightColors[8];
// uniform vec4 cAmbient;
// uniform vec4 cDiffuse;
// uniform vec4 cSpecular;
// uniform int lightType[8];
// uniform float a[8];
// uniform float b[8];
// uniform float c[8];
// uniform float angle[8];
// uniform float penumbra[8];
// uniform int numLights;
// uniform float shininess;
// uniform vec4 cameraPos;

// void main() {
//     fragColor = ka * cAmbient;
//     for (int i = 0; i < numLights; ++i) {
//         float att = 1.f;
//         vec3 currLightDir = normalize(vec3(lightPos[i]) - worldPosition);
//         vec4 lightIntensity = lightColors[i];
//         float dist = length(vec3(lightPos[i]) - worldPosition);
//         if (lightType[i] == 0) {
//             currLightDir = -normalize(vec3(lightDir[i]));
//         } else if (lightType[i] == 1) {
//             att = min(1.f, 1.f / (a[i] + b[i] * dist + c[i] * dist * dist));
//         } else if (lightType[i] == 2) {
//             att = min(1.f, 1.f / (a[i] + b[i] * dist + c[i] * dist * dist));
//             float x = acos(dot(-currLightDir, normalize(vec3(lightDir[i]))));
//             float innerAngle = angle[i] - penumbra[i];
//             float falloff = -2.f * pow((x - innerAngle) / (angle[i] - innerAngle), 3.f) + 3.f * pow((x - innerAngle) / (angle[i] - innerAngle), 2.f);
//             if (x <= innerAngle) {
//                 lightIntensity = lightColors[i];
//             } else if (x > innerAngle && x <= angle[i]) {
//                 lightIntensity *= (1.f - falloff);
//             } else {
//                 lightIntensity = vec4(0, 0, 0, 1);
//             }
//         }
//         float nl = clamp(dot(normalize(worldNormal), currLightDir), 0.0, 1.0);
//         float diffuse = kd * nl;
//         fragColor += att * diffuse * lightIntensity * cDiffuse;
//         vec3 r = normalize(reflect(-currLightDir, normalize(worldNormal)));
//         vec3 v = normalize(vec3(cameraPos) - worldPosition);
//         float rv = clamp(dot(r, v), 0.0, 1.0);
//         float specular;
//         if (shininess <= 0.0) {
//             specular = 0.0;
//         } else {
//             specular = ks * pow(rv, shininess);
//         }
//         fragColor += att * specular * lightColors[i] * cSpecular;
//     }
//     fragColor.a = 1.0;
// }


#version 330 core

uniform vec3 font_colour;
uniform sampler2D alphabet_texture;

in vec2 texture_coordinates;
in vec3 texcoords_anim;

out vec4 fragment_colour;

void main(void)
{
        float texture_value = texture(alphabet_texture, texture_coordinates).r;

        // Enable this if-statement for 2D window-positioned text
        // -------------------------------------------------------------------------
         if (texture_value == 1) // Fully opaque character pixels.
         {
                 fragment_colour = vec4(font_colour / 255, texture_value);
                         // fragment_colour = vec4(255 / 255, 255 / 255, 255 / 255, 1.0);
         }
         else if (texture_value == 0) // Fully transparent, i.e. background pixels (Note: you cannot use the "discard" method as used for 3D text further down, if colouring in the font's background)
         {
                 fragment_colour = vec4(1, 0, 0, 1);
                         // fragment_colour = vec4(85.0 / 255, 160.0 / 255, 155.0 / 255, 1.0);
         }
         else // Anti-aliased character pixels.
         {
                 fragment_colour = vec4(font_colour / 255, texture_value);
                         // fragment_colour = vec4(255 / 255, 255 / 255, 255 / 255, 1.0);
         }

        // Enable these three lines instead for 3D animated text
        // ----------------------------------------------------------------------
        // fragment_colour = vec4((texcoords_anim * 2) + vec3(0.25, 0.25, 0.25), texture_value);

        // if (texture_value == 0)
                //  discard;
}
