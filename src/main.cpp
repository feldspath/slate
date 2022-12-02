#include <slate/slate.hpp>

#include <iostream>
#include <vector>

#include "deformable.hpp"
#include "cs_test.hpp"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    slate::Renderer renderer(SCR_WIDTH, SCR_HEIGHT, "Slate");

    // Object
    slate::SlateObjectPtr obj = std::make_shared<slate::SlateObject>("cube");
    slate::ShaderPtr deform_shader = std::make_shared<slate::Shader>(std::string(ROOT_DIR) + "resources/shaders/deform.vs", std::string(ROOT_DIR) + "resources/shaders/deform.fs");
    obj->add_component(std::make_shared<Deformable>("", std::string(ROOT_DIR) + "resources/test/test.obj", deform_shader));
    //obj->add_component(std::make_shared<Deformable>("", std::string(ROOT_DIR) + "resources/test/test.obj", renderer.get_default_shader()));
    //obj->add_component(std::make_shared<CSTest>());

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