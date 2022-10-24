#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace slate {

    class Shader {
    private:
        unsigned int program_id;
        std::string vs_path;
        std::string fs_path;

        static std::string read_file(const std::string& path);
        static void check_compile_status(unsigned int shader_id, std::string debug_name);
        static void check_link_status(unsigned int program_id);

    public:
        Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
        ~Shader();

        void use() const;

        void set_uniform(const std::string& name, const float value) const;
        void set_uniform(const std::string& name, const glm::vec2& value) const;
        void set_uniform(const std::string& name, const glm::vec3& value) const;
        void set_uniform(const std::string& name, const glm::vec4& value) const;
        void set_uniform(const std::string& name, const glm::mat4& value) const;

        void set_uniform_block(const std::string& name, unsigned int block_id) const;
    };

    using ShaderPtr = std::shared_ptr<Shader>;

}