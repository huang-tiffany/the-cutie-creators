#version 330 core

layout(location = 0) in vec3 objectPosition;
layout(location = 1) in vec3 objectNormal;

out vec3 worldPosition;
out vec3 worldNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    worldPosition = vec3(model * vec4(objectPosition, 1.0));
    worldNormal = mat3(transpose(inverse(model))) * normalize(objectNormal);
    gl_Position = proj * view * model * vec4(objectPosition, 1.0);
}


// #version 330 core

// layout (location = 0) in vec4 vertex;

// uniform mat4 view;
// uniform mat4 projection;
// uniform mat4 animate;

// out vec2 texture_coordinates;
// out vec3 texcoords_anim;

// void main(void)
// {
//         texture_coordinates = vec2(vertex[2], vertex[3]); // Same as using z and w... i.e. vertex.zw

//         // Enable this line for 2D window-positioned text
//         // -------------------------------------------------------------
//         gl_Position = projection * view * animate * vec4(vertex.xy, 0.0, 1.0);

//         // Enable these two lines instead for 3D animated text
//         // --------------------------------------------------------------------
//         // texcoords_anim = vec3(animate * vec4(vertex.xy, 0.0, 1)); // Used simply to animate the colours.
//         // gl_Position = projection * view * animate * vec4(vertex.xy, 0.0, 1.0);
// }
