#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include <slate/display/glad/glad.hpp>

#include "compute_shader.hpp"

namespace slate {
    ComputeShader::ComputeShader(const std::string& compute_shader_path, const std::vector<float>& data) : cs_path(compute_shader_path) {
        // Retrieve code
        const std::string cs_str = read_file(compute_shader_path);

        const char* cs_code = cs_str.c_str();

        // Vertex Shader Compilation
        unsigned int cs_id;
        cs_id = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(cs_id, 1, &cs_code, NULL);
        glCompileShader(cs_id);

        check_compile_status(cs_id, "Compute");

        // Program Linking
        program_id = glCreateProgram();
        glAttachShader(program_id, cs_id);
        glLinkProgram(program_id);

        check_link_status(program_id);

        // Delete Shaders
        glDeleteShader(cs_id);

        // SSBO
        glGenBuffers(1, &ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    ComputeShader::ComputeShader() : program_id(-1) {}


    ComputeShader::~ComputeShader() {
        glDeleteProgram(program_id);
        glDeleteBuffers(1, &ssbo);
    }

    void ComputeShader::use() const {
        glUseProgram(program_id);
    }

    void ComputeShader::dispatch(unsigned int width, unsigned int height, unsigned int depth) {
        use();
        glDispatchCompute(width, height, depth);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }

    void ComputeShader::read_data(unsigned int start, unsigned int end, std::vector<float> data) {
        data = std::vector<float>(end-start);
        glGetNamedBufferSubData(ssbo, sizeof(float) * start, sizeof(float) * data.size(), data.data());
    }


    std::string ComputeShader::read_file(const std::string& path) {
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

    void ComputeShader::check_compile_status(unsigned int shader_id, std::string debug_name) {
        int success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(shader_id, 512, NULL, info_log);
            std::cerr << "Error::Shader::" << debug_name << "::Compilation: " << info_log << '\n';
        }
    }

    void ComputeShader::check_link_status(unsigned int program_id) {
        int success;
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(program_id, 512, NULL, info_log);
            std::cerr << "Error::Shader::Program::Linking: " << info_log << '\n';
        }
    }

    void ComputeShader::set_uniform(const std::string& name, const float value) const {
        glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
    }

    void ComputeShader::set_uniform(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
    }

    void ComputeShader::set_uniform(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
    }

    void ComputeShader::set_uniform(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(program_id, name.c_str()), 1, &value[0]);
    }

    void ComputeShader::set_uniform(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    void ComputeShader::set_uniform_block(const std::string& name, unsigned int block_id) const {
        glUniformBlockBinding(program_id, glGetUniformBlockIndex(program_id, name.c_str()), block_id);
    }

}