#include <slate/slate.hpp>

#include <iostream>
#include <vector>

#include "deformable.hpp"
#include "cs_test.hpp"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main()
{
    slate::Renderer renderer(SCR_WIDTH, SCR_HEIGHT, "Slate");

    // Deform Shader
    slate::ShaderPtr deform_shader = std::make_shared<slate::Shader>(std::string(ROOT_DIR) + "resources/shaders/deform.vs", std::string(ROOT_DIR) + "resources/shaders/deform.fs");

    // Cube
    //slate::SlateObjectPtr cube = std::make_shared<slate::SlateObject>("cube");
    //cube->add_component(std::make_shared<Deformable>("", std::string(ROOT_DIR) + "resources/test/test.obj", deform_shader));

    // Cactus
    slate::SlateObjectPtr cactus = std::make_shared<slate::SlateObject>("armadillo");
    cactus->add_component(std::make_shared<Deformable>("", std::string(ROOT_DIR) + "resources/models/cactus_small.off", deform_shader));
    cactus->transform.scale = glm::vec3(10.0f);

    // Camera
    slate::CameraPtr camera = std::make_shared<slate::Camera>();
    camera->add_component(std::make_shared<slate::FpsInputComponent>());

    // Light
    slate::LightPtr light = std::make_shared<slate::PointLight>();
    light->transform.position = glm::vec3(3.0f, 2.0f, 2.0f);

    // Scene
    slate::Scene scene;
    scene.add(camera);
    //scene.add(cube);
    scene.add(cactus);
    scene.add(light);

    // Setting some parameters
    light->power = 40.0f;
    renderer.clear_color = glm::vec3(0.02f, 0.02f, 0.03f);
    //cube->transform.position = glm::vec3(0.0f, 0.0f, -2.0f);
    cactus->transform.position = glm::vec3(0.0f, -3.0f, 2.0f);
    cactus->transform.rotation = glm::angleAxis(-glm::pi<float>() * 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

    // render loop
    renderer.run(scene);


    return 0;
}