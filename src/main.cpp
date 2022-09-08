#include "slate/slate.hpp"

#include <iostream>
#include <vector>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    slate::Window window(SCR_WIDTH, SCR_HEIGHT, "Slate");
    auto shader = std::make_shared<slate::Shader>("src/shaders/default.vs", "src/shaders/default.fs");
    slate::Mesh::set_default_shader(shader);

    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f), // left  
        glm::vec3(0.5f, -0.5f, 0.0f), // right 
        glm::vec3(0.0f,  0.5f, 0.0f)  // top   
    };

    slate::Mesh triangle(vertices);


    // render loop
    // -----------
    while (window.should_continue()) {
        window.begin_frame();

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        triangle.draw();

        window.end_frame();
    }

    return 0;
}