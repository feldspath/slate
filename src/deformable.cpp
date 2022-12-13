#include "deformable.hpp"

#include <slate/utils/model_loader/model_loader.hpp>
#include <slate/scene/object/slate_object.hpp>
#include <slate/utils/performance/performance.hpp>
#include <slate/helper_dirs.hpp>

#include <fstream>

float random_float() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Deformable::Deformable(const std::string& voxelization_path, const std::string& mesh_path, slate::ShaderPtr shader) : shader(shader) {
    // MESH
    slate::ModelLoader model_loader(mesh_path);
    const slate::RawMesh raw_mesh = model_loader.get_meshes()[0];
    material = raw_mesh.material.lock();

    std::vector<slate::Vertex> vertices(raw_mesh.positions.size());
    int vertex_idx = 0;
    for (auto& vertex : vertices) {
        vertex.position = raw_mesh.positions[vertex_idx];
        vertex.normal = raw_mesh.normals[vertex_idx];
        vertex.uv = raw_mesh.uvs[vertex_idx];
        vertex_idx++;
    }
    mesh = std::make_shared<slate::Mesh<slate::Vertex>>(vertices, raw_mesh.indices, material);

    // VOXELIZATION
    import_voxelization(std::string(ROOT_DIR) + "resources/voxelizations/armv.txt");
    vertex_mapping = std::make_shared <slate::SSBO>(sizeof(float) * voxel_vertices_indices.size(), GL_MAP_READ_BIT, voxel_vertices_indices.data());

    // 3D texture of displacements
    glGenTextures(1, &vertex_offsets_tex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_3D, vertex_offsets_tex);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA32F, width, height, depth, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindImageTexture(0, vertex_offsets_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    // SIMULATION
    import_simulation(std::string(ROOT_DIR) + "resources/voxelizations/arms.txt");
    reduction_basis_ssbo = std::make_shared<slate::SSBO>(sizeof(float) * reduction_basis.size(), GL_MAP_READ_BIT, reduction_basis.data());

    // Shader
    cs = std::make_shared<slate::ComputeShader>(std::string(ROOT_DIR) + "resources/compute_shaders/deform.glsl");
}

Deformable::~Deformable() {
    glDeleteTextures(1, &vertex_offsets_tex);
}

void Deformable::update(const float dt) {
    slate::Benchmark bench("Deform update");
    cs->use();
    cs->bind_ssbo(vertex_mapping->get_id(), 0);
    cs->bind_ssbo(reduction_basis_ssbo->get_id(), 1);
    cs->set_uniform("width", width);
    cs->set_uniform("height", height);
    cs->set_uniform("depth", depth);
    cs->set_uniform("r", r);
    cs->dispatch(width, height, depth);
}

void Deformable::render() {
    auto obj = get_target();
    if (!obj)  return;

    shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, vertex_offsets_tex);

    // Model Matrix
    const glm::mat4 model_matrix = obj->transform.frame_matrix();
    shader->set_uniform("model_matrix", model_matrix);

    // Voxelization
    shader->set_uniform("width", width);
    shader->set_uniform("height", height);
    shader->set_uniform("depth", depth);
    shader->set_uniform("origin", origin);
    shader->set_uniform("voxel_size", voxel_size);

    mesh->draw(shader);
}

void Deformable::import_voxelization(const std::string& path) {
    std::fstream file;
    file.open(path, std::ios_base::in);

    file >> origin.x >> origin.y >> origin.z;
    file >> voxel_size;
    file >> width >> height >> depth >> n_vertices;
    voxel_vertices_indices.resize(width * height * depth, -1);

    for (int i = 0; i < n_vertices; ++i) {
        int x, y, z;
        file >> x >> y >> z;
        file >> voxel_vertices_indices[z * width * height + y * width + x];
    }

    file.close();
}

void Deformable::import_simulation(const std::string& path) {
    std::fstream file;
    file.open(path, std::ios_base::in);

    // Reduction Basis
    int rows, cols;
    file >> rows >> cols;
    reduction_basis.resize(rows * cols);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            file >> reduction_basis[i*cols + j];
        }
    }

    // Polynomials
    file >> r;
    std::vector<Polynomial> polynomials(r, Polynomial(r));
    for (auto& polynomial : polynomials) {
        // linear terms
        for (int i = 0; i < r; i++) {
            file >> polynomial.get_linear(i);
        }

        // quadratic terms
        for (int i = 0; i < r; i++) {
            for (int j = i; j < r; j++) {
                file >> polynomial.get_quadratic(i, j);
            }
        }

        // cubic terms
        for (int i = 0; i < r; i++) {
            for (int j = i; j < r; j++)
                for (int k = j; k < r; k++) {
                    file >> polynomial.get_cubic(i, j, k);
                }
        }
    }

    file.close();

    polynomial_generator = std::make_shared<PolynomialGenerator>(polynomials);
}