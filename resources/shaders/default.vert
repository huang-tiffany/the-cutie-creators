#version 330 core

layout(location = 0) in vec4 objectPosition;
layout(location = 1) in vec3 objectNormal;

out vec3 worldPosition;
out vec3 worldNormal;
uniform mat4 model;

uniform mat4 transp_inv_model_matrix;
uniform mat4 mvp_matrix;

out vec2 texture_coordinates;
out vec3 texcoords_anim;

uniform int alphabet_texture_width;
uniform int alphabet_texture_height;

void main(void)
{
    // if (!isText) { // uncomment to visualize text. don't forget to draw it in realtime.cpp
    worldPosition = vec3(model * objectPosition);
    worldNormal = vec3(transp_inv_model_matrix * vec4(normalize(objectNormal), 0));
    gl_Position = mvp_matrix * objectPosition;
    texture_coordinates = vec2(worldPosition[0] / float(alphabet_texture_width / 100.f) + 0.5, worldPosition[2] / float(alphabet_texture_height / 100.f) + 0.5); // Same as using z and w... i.e. vertex.zw
    // } else {
    //     gl_Position = vec4(objectPosition.x/2, objectPosition.y, 0.0, 1.0);
    //     texture_coordinates = vec2(objectPosition[2], objectPosition[3]); // Same as using z and w... i.e. vertex.zw
    // }
}
