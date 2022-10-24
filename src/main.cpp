#include <slate/slate.hpp>
#include <slate/component/graphic/mesh_renderer/mesh_renderer_component.hpp>
#include <slate/scene/camera/base/camera_base.hpp>
#include <slate/component/input/fps/fps_input_component.hpp>
#include <slate/scene/light/point/point_light.hpp>
#include <slate/scene/light/light_base.hpp>

#include <iostream>
#include <vector>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    slate::Renderer renderer(SCR_WIDTH, SCR_HEIGHT, "Slate");

    // Object
    slate::ModelPtr model = std::make_shared<slate::Model>(std::string(ROOT_DIR) + std::string("resources/test/test.obj"));
    slate::SlateObjectPtr obj = std::make_shared<slate::SlateObject>("cube");
    obj->add_component(std::make_shared<slate::MeshRendererComponent>(model, renderer.get_default_shader()));

    // Camera
    slate::CameraPtr camera = std::make_shared<slate::CameraBase>();
    camera->add_component(std::make_shared<slate::FpsInputComponent>());

    // Light
    slate::LightPtr light = std::make_shared<slate::PointLight>();
    light->transform.position = glm::vec3(3.0f, 2.0f, 2.0f);

    // Scene
    slate::Scene scene;
    scene.add(camera);
    scene.add(obj);
    scene.add(light);

    light->power = 40.0f;
    renderer.clear_color = glm::vec3(0.02f, 0.02f, 0.03f);

    // render loop
    // -----------
    while (renderer.should_continue()) {
        scene.update();
        renderer.begin_frame();
        renderer.render(scene, camera);
        renderer.end_frame();
    }

    return 0;
}