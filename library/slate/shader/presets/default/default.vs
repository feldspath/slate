#version 460 core

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
    mat4 projection_matrix;
    mat4 view_matrix;
};
uniform mat4 model_matrix;


void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
}