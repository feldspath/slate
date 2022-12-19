#include "punch.hpp"
#include "deformable.hpp"

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
            auto target_pos = def->get_target()->transform.position;
            target_pos.y = 0.0f;
            const auto punch_dir = glm::normalize(target_pos - player_pos);
            const float distance2 = glm::distance2(target_pos, player_pos);
            if (distance2 > max_dist * max_dist) continue;
            def->add_external_force(500, force * punch_dir);
        }
    }
}
