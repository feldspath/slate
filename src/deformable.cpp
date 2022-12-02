#include "deformable.hpp"

#include <slate/utils/model_loader/model_loader.hpp>
#include <slate/scene/object/slate_object.hpp>

std::vector<slate::ParamMetadata> DeformVertex::vertex_info = {
   slate::ParamMetadata {.size = 1, .type = GL_UNSIGNED_INT, .offset = (void*)offsetof(DeformVertex, voxel_id) },
};

float random_float() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

Deformable::Deformable(const std::string& voxelization_path, const std::string& mesh_path, slate::ShaderPtr shader) : shader(shader) {
    slate::ModelLoader model_loader(mesh_path);
    const slate::RawMesh raw_mesh = model_loader.get_meshes()[0];
    material = raw_mesh.material.lock();

    std::vector<DeformVertex> vertices(raw_mesh.positions.size());

    int vertex_idx = 0;
    for (auto& v : vertices) {
        v.position = raw_mesh.positions[vertex_idx];
        v.normal = raw_mesh.normals[vertex_idx];
        v.uv = raw_mesh.uvs[vertex_idx];
        v.voxel_id = vertex_idx;
        std::cout << v.voxel_id << '\n';
        vertex_idx++;
    }

    mesh = std::make_shared<slate::Mesh<DeformVertex>>(vertices, raw_mesh.indices, material, DeformVertex::vertex_info);

    std::vector<float> deformations(3*vertices.size());
    for (int i = 0; i < vertices.size(); ++i) {
        deformations[3 * i] = random_float();
        deformations[3 * i + 1] = random_float();
        deformations[3 * i + 1] = random_float();
    }
    voxel_deformations = std::make_shared <slate::SSBO<float>>(deformations);
}

void Deformable::update(const float dt) {

}

void Deformable::render() {
    auto obj = get_target();
    if (!obj)  return;
    shader->use();
    shader->bind_ssbo(voxel_deformations->get_id(), 0);
    const glm::mat4 model_matrix = obj->transform.frame_matrix();
    shader->set_uniform("model_matrix", model_matrix);
    mesh->draw(shader);
}