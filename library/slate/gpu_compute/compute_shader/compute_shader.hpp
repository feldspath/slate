#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shader/shader_base.hpp"

namespace slate {
    class ComputeShader : public ShaderBase {
    private:
        std::string cs_path;

    public:
        ComputeShader(const std::string& compute_shader_path);
        ~ComputeShader();

        void dispatch(unsigned int width, unsigned int height, unsigned int depth);
    };
}