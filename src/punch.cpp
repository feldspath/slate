#include "punch.hpp"
#include "deformable.hpp"

#include <random>
#include <algorithm>

#include <slate/input/input.hpp>
#include <slate/scene/scene.hpp>


void PunchComponent::update(std::shared_ptr<slate::Scene> scene, const float) {
    auto obj = get_target();
    if (!obj) return;

    auto player_pos = get_target()->transform.position;
    player_pos.y = 0.0f;

    auto deformables = scene->components_by_type<Deformable>();
    if (slate::Input::get().key_pressed(GLFW_KEY_SPACE)) {
        int count = 0;
        for (auto& def : deformables) {
            if (!def->active) continue;
            auto target_pos = def->get_target()->transform.position;
            target_pos.y = 0.0f;
            const auto punch_dir = glm::normalize(target_pos - player_pos);
            const float distance2 = glm::distance2(target_pos, player_pos);
            if (distance2 > max_dist * max_dist) continue;
            def->add_external_force(100, force * punch_dir);
        }
    }

    static int spawned_step = 0;
    if (slate::Input::get().key_pressed(GLFW_KEY_Q) && !q_pressed) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(deformables.begin(), deformables.end(), g);

        int to_spawn = std::pow(2, spawned_step++);
        for (auto& def : deformables) {
            if (!def->active) {
                def->active = true;
                --to_spawn;
                if (to_spawn == 0) break;
            }
        }
        q_pressed = true;

    }
    else if (!slate::Input::get().key_pressed(GLFW_KEY_Q)) {
        q_pressed = false;
    }

}
