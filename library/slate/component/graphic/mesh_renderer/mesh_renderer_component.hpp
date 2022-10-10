#pragma once

#include "../graphic_component.hpp"
#include <slate/scene/mesh/mesh.hpp>
#include <slate/shader/shader.hpp>

namespace slate {
    class MeshRendererComponent : public GraphicComponent {
    private:
        MeshPtr mesh;
        ShaderPtr shader;
    
    public:
        MeshRendererComponent(MeshPtr mesh_, ShaderPtr shader_ = nullptr);
        
        void render() override;
        void update() override {}
    };
}