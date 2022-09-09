#include "../glad/glad.hpp"
#include "shader.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

namespace slate {
    Shader::Shader(std::string vertex_shader_path, std::string fragment_shader_path) : vs_path(vertex_shader_path), fs_path(fragment_shader_path) {
        // Retrieve code
        const auto vs_str = read_file(vertex_shader_path);
        const auto fs_str = read_file(fragment_shader_path);

        const char* vs_code = vs_str.c_str();
        const char* fs_code = fs_str.c_str();

        // Vertex Shader Compilation
        unsigned int vs_id;
        vs_id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs_id, 1, &vs_code, NULL);
        glCompileShader(vs_id);

        check_compile_status(vs_id, "Vertex");
        
        // Fragment Shader Compilation
        unsigned int fs_id;
        fs_id = glCreateShader(GL_FRAGMENT_SHADER); 
        glShaderSource(fs_id, 1, &fs_code, NULL);
        glCompileShader(fs_id);

        check_compile_status(fs_id, "Fragment");

        // Program Linking
        program_id = glCreateProgram();
        glAttachShader(program_id, vs_id);
        glAttachShader(program_id, fs_id);
        glLinkProgram(program_id);

        check_link_status(program_id);

        // Delete Shaders
        glDeleteShader(vs_id);
        glDeleteShader(fs_id);
    }

    Shader::~Shader() {
        glDeleteProgram(program_id);
    }

    void Shader::use() const {
        glUseProgram(program_id);
    }

    std::string Shader::read_file(std::string path) {
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

    void Shader::check_compile_status(unsigned int shader_id, std::string debug_name) {
        int success;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(shader_id, 512, NULL, info_log);
            std::cerr << "Error::Shader::" << debug_name << "::Compilation: " << info_log << '\n';
        }
    }

    void Shader::check_link_status(unsigned int program_id) {
        int success;
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(program_id, 512, NULL, info_log);
            std::cerr << "Error::Shader::Program::Linking: " << info_log << '\n';
        }
    }
}