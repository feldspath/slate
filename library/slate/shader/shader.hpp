#pragma once

#include "../glad/glad.hpp"

#include <string>
#include <memory>


namespace slate {

    class Shader {
    private:
        unsigned int program_id;
        std::string vs_path;
        std::string fs_path;

        static std::string read_file(std::string path);
        static void check_compile_status(unsigned int shader_id, std::string debug_name);
        static void check_link_status(unsigned int program_id);
    
    public:
        Shader(std::string vertex_shader_path, std::string fragment_shader_path);
        ~Shader();
        
        void use() const;
        void reload();
    };

    using ShaderPtr = std::shared_ptr<Shader>;

}