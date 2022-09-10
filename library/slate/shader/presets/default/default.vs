#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}