#version 330 core

layout(location = 0) in vec4 objectPosition;
layout(location = 1) in vec3 objectNormal;

out vec3 worldPosition;
out vec3 worldNormal;
uniform mat4 text_model;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform bool isText;

out vec2 texture_coordinates;
out vec3 texcoords_anim;

uniform int alphabet_texture_width;
uniform int alphabet_texture_height;

void main(void)
{
    if (!isText) {
        worldPosition = vec3(model * objectPosition);
        worldNormal = mat3(transpose(inverse(model))) * normalize(objectNormal);
        gl_Position = proj * view * text_model * model * objectPosition;
        texture_coordinates = vec2(worldPosition[0] / float(alphabet_texture_width / 100.f) + 0.5, worldPosition[2] / float(alphabet_texture_height / 100.f) + 0.5); // Same as using z and w... i.e. vertex.zw
    } else {
        // Enable this line for 2D window-positioned text
        // -------------------------------------------------------------
        gl_Position = vec4(objectPosition.x/2, objectPosition.y, 0.0, 1.0);
        texture_coordinates = vec2(objectPosition[2], objectPosition[3]); // Same as using z and w... i.e. vertex.zw
    }


}
