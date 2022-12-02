#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "shader.hpp"

namespace slate {
    Shader::Shader(const std::string& vertex_shader_path, const std::string& fragment_shader_path) : vs_path(vertex_shader_path), fs_path(fragment_shader_path) {
        // Vertex Shader Compilation
        unsigned int vs_id = compile_shader(GL_VERTEX_SHADER, read_file(vertex_shader_path));

        // Fragment Shader Compilation
        unsigned int fs_id = compile_shader(GL_FRAGMENT_SHADER, read_file(fragment_shader_path));

        // Program Linking
        program_id = glCreateProgram();
        glAttachShader(program_id, vs_id);
        glAttachShader(program_id, fs_id);
        glLinkProgram(program_id);

        check_link_status();

        // Delete Shaders
        glDeleteShader(vs_id);
        glDeleteShader(fs_id);
    }

    Shader::~Shader() {
        glDeleteProgram(program_id);
    }
}