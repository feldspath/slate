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
    auto triangle_mesh = std::make_shared<slate::Mesh>(vertices);

    slate::Scene scene;
    scene.add_instance("triangle", triangle_mesh);

    auto triangle_instance = scene.get_instance_by_name("triangle");

    // render loop
    // -----------
    while (renderer.should_continue()) {
        renderer.begin_frame();

        auto transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f + 3.0f * sin(0.0f)));
        triangle_instance->get_transform() = transform;

        renderer.render(scene);

        renderer.end_frame();
    }

    return 0;
}