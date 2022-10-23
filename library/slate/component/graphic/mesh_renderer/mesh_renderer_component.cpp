#include "mesh_renderer_component.hpp"
#include "slate/scene/object/slate_object.hpp"

namespace slate {
    MeshRendererComponent::MeshRendererComponent(ModelPtr mesh_, ShaderPtr shader_) : model(mesh_), shader(shader_) {}

    void MeshRendererComponent::render() {
        auto obj = target();
        if (!obj)  return;

        shader->use();
        const glm::mat4 model_matrix = obj->transform.frame_matrix();
        shader->set_uniform("model_matrix", model_matrix);
        model->draw(shader);
    }
}