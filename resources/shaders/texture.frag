#version 330 core

in vec2 fragUV;
uniform sampler2D textureSampler;
uniform bool togglePerPixelTexture;
uniform bool toggleKernelTexture;
uniform int fboWidth;
uniform int fboHeight;
out vec4 fragColor;

void main()
{
    fragColor = texture(textureSampler, fragUV);
}
