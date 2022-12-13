#pragma once

#include <slate/component/graphic/graphic_component.hpp>
#include <slate/scene/mesh/mesh.hpp>
#include <slate/display/shader/shader.hpp>
#include <slate/gpu_compute/compute_shader/compute_shader.hpp>

#include <string>
#include <vector>
#include <map>

class Deformable : public slate::GraphicComponent {
private:
    slate::ShaderPtr shader;
    std::shared_ptr<slate::ComputeShader> cs;
    
    // Mesh
    slate::MeshPtr mesh;
    std::shared_ptr<slate::Material> material;

    // Voxelization
    int width, height, depth, n_vertices;
    glm::vec3 origin;
    float voxel_size;
    std::vector<int> voxel_vertices_indices;

    std::shared_ptr<slate::SSBO> vertex_mapping;
    GLuint vertex_offsets_tex;


    std::shared_ptr<slate::SSBO> voxel_deformations;
    
    void import_voxelization(const std::string& path);

public:
    Deformable(const std::string& voxelization_path, const std::string& mesh_path, slate::ShaderPtr shader);
    ~Deformable();
    void update(const float dt) override;
    void render() override;
};