#version 330 core

// in vec2 fragUV;
// uniform sampler2D textureSampler;
// uniform bool togglePerPixelTexture;
// uniform bool toggleKernelTexture;
// uniform int fboWidth;
// uniform int fboHeight;
// out vec4 fragColor;

// void main()
// {
//     fragColor = texture(textureSampler, fragUV);
//     if (togglePerPixelTexture) {
//         fragColor = vec4(1.0) - fragColor;
//     } if (toggleKernelTexture) {
//         vec3 blur = vec3(0.0);
//         for (int i = -2; i <= 2; i++) {
//             for (int j = -2; j <= 2; j++) {
//                 vec2 sampleUV = fragUV + vec2(i / float(fboWidth), j / float(fboHeight));
//                 blur += texture(textureSampler, sampleUV).rgb;
//             }
//         }
//         blur /= float(25);
//         fragColor = vec4(blur, 1.0);
//     }
// }

void main() {}
