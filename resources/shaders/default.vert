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
