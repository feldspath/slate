#pragma once

#include "../graphic_component.hpp"
#include <slate/scene/model/model.hpp>
#include <slate/shader/shader.hpp>

namespace slate {
    class MeshRendererComponent : public GraphicComponent {
    private:
        ModelPtr model;
        ShaderPtr shader;
    
    public:
        MeshRendererComponent(ModelPtr mesh_, ShaderPtr shader_ = nullptr);
        
        void render() override;
        void update() override {}
    };
}