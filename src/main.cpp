#include <slate/slate.hpp>
#include <slate/component/graphic/mesh_renderer/mesh_renderer_component.hpp>
#include <slate/scene/camera/base/camera_base.hpp>
#include <slate/component/input/fps/fps_input_component.hpp>

#include <iostream>
#include <vector>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    slate::Renderer renderer(SCR_WIDTH, SCR_HEIGHT, "Slate");

    // Triangle
    std::vector<glm::vec3> vertices = {
        glm::vec3(0.5f,  0.5f, 0.0f),  // top right
        glm::vec3(0.5f, -0.5f, 0.0f),  // bottom right
        glm::vec3(-0.5f, -0.5f, 0.0f),  // bottom left
        glm::vec3(-0.5f,  0.5f, 0.0f)   // top left 
    };

    std::vector<unsigned int> indices = { 
        0, 1, 3,
        1, 2, 3
    };

    auto triangle = std::make_shared<slate::SlateObject>("triangle");
    triangle->add_component(std::make_shared<slate::MeshRendererComponent>(std::make_shared<slate::Mesh>(vertices, indices), renderer.get_default_shader()));

    // Camera
    slate::CameraPtr camera = std::make_shared<slate::CameraBase>();
    camera->add_component(std::make_shared<slate::FpsInputComponent>());

    // Scene
    slate::Scene scene;
    scene.add_slate_object(triangle);
    scene.add_slate_object(camera);

    auto triangle_instance = scene.slate_object_by_name("triangle");

    // render loop
    // -----------
    while (renderer.should_continue()) {
        scene.update();
        renderer.begin_frame();
        triangle_instance->transform.position = glm::vec3(0.0f, 0.0f, -3.0f - 2.0f * std::cos(glfwGetTime()));
        renderer.render(scene, camera);
        renderer.end_frame();
    }

    return 0;
}