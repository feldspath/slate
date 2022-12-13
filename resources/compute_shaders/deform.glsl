#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

// mapping 3D vertex coord to 1D vertex index
layout(std430, binding = 0) buffer Mapping {
    int vertex_mapping[];
};

// 3n x r matrix, reduction basis
layout(std430, binding = 1) buffer Basis {
    float reduction_basis[];
};

// vector q
layout(std430, binding = 2) buffer Vector {
    float reduced_vector[];
};

layout(rgba32f, binding = 0) uniform image3D vertex_displacement;

uniform int width;
uniform int height;
uniform int depth;

uniform int r;

void main() {
    ivec3 id = ivec3(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y, gl_GlobalInvocationID.z);
    if (id.x >= width || id.y >= height || id.z >= depth) return;

    uint idx = id.x + id.y * width + id.z * width * height;
    int v = vertex_mapping[idx];
    if (v == -1) return;

    float displ[3];
    for (int k = 0; k < 3; ++k) {
        displ[k] = 0.0;
        for (int i = 0; i < r; ++i) {
            int col = i;
            int row = 3*v+k;
            int index = row * r + col;
            displ[k] += reduction_basis[index] * reduced_vector[i];
        }
    }

    // imageStore(vertex_displacement, id, vec4(1.0, 1.0, 1.0, 1.0));
    imageStore(vertex_displacement, id, vec4(displ[0], displ[1], displ[2], 1.0));
}
