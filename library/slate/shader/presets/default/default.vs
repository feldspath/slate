#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

layout (std140) uniform Matrices {
    mat4 projection_matrix;
    mat4 view_matrix;
    mat4 view_matrix_inv;
};
uniform mat4 model_matrix;

out vec3 fragPos;
out vec3 fragNormal;

void main() {
    fragNormal = vec3(model_matrix * vec4(aNorm, 0.0f));
    fragPos = vec3(model_matrix * vec4(aPos, 1.0f));
    gl_Position = projection_matrix * view_matrix * vec4(fragPos, 1.0f);
}