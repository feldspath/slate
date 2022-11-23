#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {
    class ComputeShader {
    private:
        unsigned int program_id;
        unsigned int ssbo;
        std::string cs_path;

        static std::string read_file(const std::string& path);
        static void check_compile_status(unsigned int shader_id, std::string debug_name);
        static void check_link_status(unsigned int program_id);

    public:
        ComputeShader();
        ComputeShader(const std::string& compute_sahder_path, const std::vector<float>& data);
        ~ComputeShader();

        void use() const;
        void dispatch(unsigned int width, unsigned int height, unsigned int depth);
        void read_data(unsigned int start, unsigned int end, std::vector<float> data);

        void set_uniform(const std::string& name, const float value) const;
        void set_uniform(const std::string& name, const glm::vec2& value) const;
        void set_uniform(const std::string& name, const glm::vec3& value) const;
        void set_uniform(const std::string& name, const glm::vec4& value) const;
        void set_uniform(const std::string& name, const glm::mat4& value) const;

        void set_uniform_block(const std::string& name, unsigned int block_id) const;
    };
}