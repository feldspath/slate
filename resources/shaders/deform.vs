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
uniform int width;
uniform int height;
uniform int depth;
uniform vec3 origin;
uniform float voxel_size;

layout(binding = 0) uniform sampler3D vertex_displacement;

out vec3 fragPos;
out vec3 fragNormal;

void main() {
    vec3 uvw = vec3((aPos - origin).x / (width * voxel_size), (aPos - origin).y / (height * voxel_size), (aPos - origin).z / (depth * voxel_size));
    uvw += vec3(0.5 / width, 0.5 / height, 0.5 / depth);
    vec3 displacement = vec3(texture(vertex_displacement, uvw));
    fragNormal = vec3(model_matrix * vec4(aNorm, 0.0f));
    fragPos = vec3(model_matrix * vec4(aPos+displacement, 1.0f));
    gl_Position = projection_matrix * view_matrix * vec4(fragPos, 1.0f);
}