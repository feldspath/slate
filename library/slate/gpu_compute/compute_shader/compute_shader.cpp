#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "compute_shader.hpp"

namespace slate {
    ComputeShader::ComputeShader(const std::string& compute_shader_path) : cs_path(compute_shader_path) {
        unsigned int cs_id = compile_shader(GL_COMPUTE_SHADER, read_file(compute_shader_path));

        // Program Linking
        program_id = glCreateProgram();
        glAttachShader(program_id, cs_id);
        glLinkProgram(program_id);

        check_link_status();

        // Delete Shaders
        glDeleteShader(cs_id);
    }

    ComputeShader::~ComputeShader() {
        glDeleteProgram(program_id);
    }

    void ComputeShader::dispatch(unsigned int width, unsigned int height, unsigned int depth) {
        use();
        glDispatchCompute(width, height, depth);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }
}