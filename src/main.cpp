#include "slate/slate.hpp"

#include <iostream>
#include <vector>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    slate::Renderer renderer(SCR_WIDTH, SCR_HEIGHT, "Slate");

    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f), // left  
        glm::vec3(0.5f, -0.5f, 0.0f), // right 
        glm::vec3(0.0f,  0.5f, 0.0f)  // top   
    };
    auto triangle = std::make_shared<slate::Mesh>(vertices);
    auto transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -6.0f));

    renderer.add_instance(triangle, transform);

    // render loop
    // -----------
    while (renderer.should_continue()) {
        renderer.begin_frame();

        renderer.render();

        renderer.end_frame();
    }

    return 0;
}