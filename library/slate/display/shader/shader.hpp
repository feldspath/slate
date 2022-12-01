#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <slate/gpu_compute/shader/shader_base.hpp>

namespace slate {

    class Shader : public ShaderBase {
    private:
        std::string vs_path;
        std::string fs_path;

    public:
        Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
        ~Shader();
    };

    using ShaderPtr = std::shared_ptr<Shader>;
}