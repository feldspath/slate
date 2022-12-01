#include <fstream>
#include <iostream>
#include <sstream>

#include "shader_base.hpp"

namespace slate {
    void ShaderBase::use() const {
        glUseProgram(program_id);
    }

    std::string ShaderBase::read_file(const std::string& path) {
        std::string file_text;
        std::ifstream file(path);

        if (file.is_open()) {
            std::stringstream stream;
            stream << file.rdbuf();
            file.close();
            file_text = stream.str();
        }
        else {
            std::cerr << "Error::Shader: Failed to open file " << path << '\n';
        }

        return file_text;
    }

    void ShaderBase::check_compile_status(unsigned int shader_id, std::string debug_name) {
        int success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(shader_id, 512, NULL, info_log);
            std::cerr << "Error::Shader::" << debug_name << "::Compilation: " << info_log << '\n';
        }
    }

    void ShaderBase::check_link_status(unsigned int id) {
        int success;
        std::cout << "checking link status\n";
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(id, 512, NULL, info_log);
            std::cerr << "Error::Shader::Program::Linking: " << info_log << '\n';
        }
        std::cout << "ok\n";

    }

    unsigned int ShaderBase::compile_shader(GLenum type, const std::string& code) {
        std::string name;
        switch (type) {
            case GL_FRAGMENT_SHADER:
                name = "Fragment";
                break;
            case GL_VERTEX_SHADER:
                name = "Vertex";
                break;
            case GL_COMPUTE_SHADER:
                name = "Compute";
                break;
            default:
                std::cerr << "Error::Shader::compile_shader: Invalid GLenum type\n";
                abort();
        }
        unsigned int shader_id = glCreateShader(type);
        const char* char_code = code.c_str();
        glShaderSource(shader_id, 1, &char_code, NULL);
        glCompileShader(shader_id);

        check_compile_status(shader_id, name);

        return shader_id;
    }

    void ShaderBase::set_uniform(const std::string& name, const float value) const {
        glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
    }

    void ShaderBase::set_uniform(const std::string& name, const int value) const {
        glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
    }

    void ShaderBase::set_uniform(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
    }

    void ShaderBase::set_uniform(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
    }

    void ShaderBase::set_uniform(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
    }

    void ShaderBase::set_uniform(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void ShaderBase::set_uniform_block(const std::string& name, unsigned int block_id) const {
        glUniformBlockBinding(program_id, glGetUniformBlockIndex(program_id, name.c_str()), block_id);
    }

}