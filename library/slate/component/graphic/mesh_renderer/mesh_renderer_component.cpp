#include "mesh_renderer_component.hpp"
#include "slate/scene/object/slate_object.hpp"

namespace slate {
    MeshRendererComponent::MeshRendererComponent(MeshPtr mesh_, ShaderPtr shader_) : mesh(mesh_), shader(shader_) {}
    
    void MeshRendererComponent::render() {
        // shader->use();
        auto obj = target.lock();
        if (!obj) {
            std::cout << "Error::MeshRenderComponent::Render: expired pointer\n";
        }
        const glm::mat4 model_matrix = obj->transform.frame_matrix();
        shader->set_uniform("model_matrix", model_matrix);
        // std::cout << "model: \n";
        // std::cout << model_matrix[0][0] << model_matrix[0][1] << model_matrix[0][2] << model_matrix[0][3] << '\n';
        // std::cout << model_matrix[1][0] << model_matrix[1][1] << model_matrix[1][2] << model_matrix[1][3] << '\n';
        // std::cout << model_matrix[2][0] << model_matrix[2][1] << model_matrix[2][2] << model_matrix[2][3] << '\n';
        // std::cout << model_matrix[3][0] << model_matrix[3][1] << model_matrix[3][2] << model_matrix[3][3] << '\n';
        mesh->draw();
    }
}