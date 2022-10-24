#pragma once

// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

// Modules
#include <slate/display/shader/shader.hpp>
#include <slate/scene/scene.hpp>
#include <slate/display/renderer/renderer.hpp>
#include <slate/input/input.hpp>

// Base objects
#include <slate/scene/camera/camera_base.hpp>
#include <slate/scene/light/light_base.hpp>

// Essential objects
#include <slate/scene/light/point/point_light.hpp>

// Essential components
#include <slate/component/graphic/mesh_renderer/mesh_renderer_component.hpp>
#include <slate/component/input/fps/fps_input_component.hpp>

// Misc
#include "helper_dirs.hpp"
