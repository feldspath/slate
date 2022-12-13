#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer ssbo {
    int vertex_mapping[];
};
layout(rgba32f, binding = 0) uniform image3D vertex_displacement;

uniform int width;
uniform int height;
uniform int depth;

void main() {
    ivec3 id = ivec3(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y, gl_GlobalInvocationID.z);
    if (id.x >= width || id.y >= height || id.z >= depth) return;

    uint idx = id.x + id.y * width + id.z * width * height;
    int v = vertex_mapping[idx];
    if (v == -1) return;

    imageStore(vertex_displacement, id, vec4(1.0, 1.0, 1.0, 1.0));
}
