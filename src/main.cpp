#include <slate/slate.hpp>

#include <iostream>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "deformable.hpp"
#include "punch.hpp"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

slate::SlateObjectPtr create_cactus() {
    // Deform Shader
    static slate::ShaderPtr deform_shader = std::make_shared<slate::Shader>(std::string(ROOT_DIR) + "resources/shaders/deform.vs", std::string(ROOT_DIR) + "resources/shaders/deform.fs");
    static slate::MeshPtr cactus_mesh = std::make_shared<slate::Mesh<slate::Vertex>>(std::string(ROOT_DIR) + "resources/models/cactus_highres_centered.obj");
    static int cactus_count = 0;

    slate::SlateObjectPtr cactus = std::make_shared<slate::SlateObject>(std::string("cactus") + std::to_string(cactus_count++));
    cactus->add_component(std::make_shared<Deformable>(cactus_mesh, deform_shader));
    return cactus;
}

int main()
{
    slate::Renderer renderer(SCR_WIDTH, SCR_HEIGHT, "Slate");

    // Camera
    slate::CameraPtr camera = std::make_shared<slate::Camera>();
    camera->add_component(std::make_shared<slate::FpsInputComponent>());
    camera->add_component(std::make_shared<PunchComponent>());

    // Light
    slate::LightPtr light = std::make_shared<slate::PointLight>();
    light->transform.position = glm::vec3(3.0f, 2.0f, 2.0f);

    // Scene
    auto scene = std::make_shared<slate::Scene>();
    scene->add(camera);

    auto cactus = create_cactus();
    cactus->transform.scale = glm::vec3(2.0f);
    cactus->transform.position = glm::vec3(-1.0f, -1.0f, -1.0f);
    cactus->get_component<Deformable>()->active = true;
    scene->add(cactus);

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            auto cactus = create_cactus();
            cactus->transform.scale = glm::vec3(2.0f);
            cactus->transform.position = glm::vec3(-1.0f + j * 1.0f, -1.0f, i * 1.0f);
            cactus->transform.rotation = glm::angleAxis(static_cast<float>(static_cast<float>(rand()) * 2.0f * M_PI / RAND_MAX), glm::vec3(0.0f, 1.0f, 0.0f));
            scene->add(cactus);
        }
    }
    scene->add(light);

    // Setting some parameters
    light->power = 80.0f;
    renderer.clear_color = glm::vec3(0.5f, 0.6f, 0.8f);

    // render loop
    renderer.run(scene);


    return 0;
}