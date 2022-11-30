#pragma once

#include <slate/component/graphic/graphic_component.hpp>
#include <slate/scene/mesh/mesh.hpp>
#include <slate/display/shader/shader.hpp>

#include <string>
#include <vector>

struct DeformVertex : public slate::Vertex {
    unsigned int voxel_id;
};

static std::vector<slate::ParamMetadata> vertex_info = {
    slate::ParamMetadata { .size = 1, .type = GL_UNSIGNED_INT, .offset = (void*)offsetof(DeformVertex, voxel_id)}
};

class Deformable : public slate::GraphicComponent {
private:
    slate::ShaderPtr shader;
    std::shared_ptr<slate::Mesh<DeformVertex>> mesh;

public:
    Deformable(const std::string& voxelization_path, const std::string& mesh_path);
    void update(const float dt) override;
    void render() override;
};