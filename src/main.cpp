#include <slate/slate.hpp>

#include <iostream>
#include <vector>

#include "scaling.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    slate::Renderer renderer(SCR_WIDTH, SCR_HEIGHT, "Slate");

    // Object
    slate::ModelPtr model = std::make_shared<slate::Model>(std::string(ROOT_DIR) + "resources/test/test.obj");
    slate::SlateObjectPtr obj = std::make_shared<slate::SlateObject>("cube");
    obj->add_component(std::make_shared<slate::MeshRendererComponent>(model, renderer.get_default_shader()));
    obj->add_component(std::make_shared<Scaling>());

    // Camera
    slate::CameraPtr camera = std::make_shared<slate::Camera>();
    camera->add_component(std::make_shared<slate::FpsInputComponent>());

    // Light
    slate::LightPtr light = std::make_shared<slate::PointLight>();
    light->transform.position = glm::vec3(3.0f, 2.0f, 2.0f);

    // Scene
    slate::Scene scene;
    scene.add(camera);
    scene.add(obj);
    scene.add(light);

    // Setting some parameters
    light->power = 40.0f;
    renderer.clear_color = glm::vec3(0.02f, 0.02f, 0.03f);
    obj->transform.position = glm::vec3(0.0f, 0.0f, -2.0f);

    // render loop
    renderer.run(scene);


    return 0;
}