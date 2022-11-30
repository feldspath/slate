#pragma once

#include <slate/component/graphic/graphic_component.hpp>
#include <slate/scene/mesh/mesh.hpp>
#include <slate/display/shader/shader.hpp>

#include <string>
#include <vector>

struct DeformVertex : public slate::Vertex {
    unsigned int voxel_id;

    static std::vector<slate::ParamMetadata> vertex_info;
};


class Deformable : public slate::GraphicComponent {
private:
    slate::ShaderPtr shader;
    std::shared_ptr<slate::Mesh<DeformVertex>> mesh;
    std::shared_ptr<slate::Material> material;

public:
    Deformable(const std::string& voxelization_path, const std::string& mesh_path, slate::ShaderPtr shader);
    void update(const float dt) override;
    void render() override;
};