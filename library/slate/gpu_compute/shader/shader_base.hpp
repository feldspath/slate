#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <slate/display/glad/glad.hpp>
#include "../ssbo/ssbo.hpp"

namespace slate {

    class ShaderBase {
    protected:
        unsigned int program_id;

        static std::string read_file(const std::string& path);
        static void check_compile_status(unsigned int shader_id, std::string debug_name);
        static unsigned int compile_shader(GLenum type, const std::string& code);
        void check_link_status();

    public:
        void use() const;

        void bind_ssbo(unsigned int ssbo_id, unsigned int index) const {
            use();
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, ssbo_id);
        }

        void set_uniform(const std::string& name, const float value) const;
        void set_uniform(const std::string& name, const int value) const;
        void set_uniform(const std::string& name, const glm::vec2& value) const;
        void set_uniform(const std::string& name, const glm::vec3& value) const;
        void set_uniform(const std::string& name, const glm::vec4& value) const;
        void set_uniform(const std::string& name, const glm::mat4& value) const;

        void set_uniform_block(const std::string& name, unsigned int block_id) const;
    };
}