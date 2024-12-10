#version 330 core

layout(location = 0) in vec4 objectPosition;
layout(location = 1) in vec3 objectNormal;

out vec3 worldPosition;
out vec3 worldNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform bool isText;

out vec2 texture_coordinates;
out vec3 texcoords_anim;


void main(void)
{
    if (!isText) {
        worldPosition = vec3(model * objectPosition);
        worldNormal = mat3(transpose(inverse(model))) * normalize(objectNormal);
        gl_Position = proj * view * model * objectPosition;
        texture_coordinates = vec2(objectPosition[0], objectPosition[2]); // Same as using z and w... i.e. vertex.zw
    } else {
        // Enable this line for 2D window-positioned text
        // -------------------------------------------------------------
        gl_Position = proj * view * model * vec4(objectPosition.xy, 0.0, 1.0);
        texture_coordinates = vec2(objectPosition[2], objectPosition[3]); // Same as using z and w... i.e. vertex.zw
    }


}