# Slate

Slate is a lightweight OpenGL engine which aims at providing basic support for scripting and rendering. It works on Linux and Windows only, as OpenGL is now deprecated for MacOS.

## Download

Make sure you have a working git installation and clone the repository via SSH:

```
git clone git@github.com:feldspath/slate.git
```

or via HTTPS:

```
git clone https://github.com/feldspath/slate.git
```

## Configure / Build

### Linux

On Linux, the dependencies can be installed with the following commands:

```
sudo apt update
sudo apt install cmake build-essential libglfw3-dev pkg-config libassimp-dev
```

Configure the project with CMake using the following commands inside the project repository:

```
mkdir build
cd build
cmake ..
```

Build by running in the build folder:

```
make
```

and run using the project name, by default:

```
./slate
```

### Windows

The required libraries are pre-compiled and nothing has to be installed, you only need to install [CMake](https://cmake.org/download/). Use it to setup the project, either with the GUI, or using the following commands inside the repository:

```
mkdir build
cd build
cmake ..
```

Run the `.sln` located in the build folder with Visual Studio. There you can select the build configuration (Debug, Release, RelWithDebInfo). Set the project as the startup target (right-click the project name on the left panel -> Set as Startup Project). You can now build and run the project.

## Overview

### Math

Math is handled using the GLM library. The manual can be found [here](https://github.com/g-truc/glm/blob/0.9.9.2/doc/manual.pdf). Slate uses its vectors `glm::vec3`, its matrices `glm::mat4` and its quaternions `glm::quat`.

### Slate Object

Every object used in a scene is a `SlateObject`. They should not be specified with subclasses, but components should be added to them in order to specify their behaviour. Each component has to be created via `std::make_shared`. For example:

```cpp
camera->add_component(std::make_shared<slate::FpsInputComponent>());
```

`void SlateObject::update()` may be overridden to define the behaviour of a component. Finally, always create `SlateObjectPtr` and not `SlateObject`, it is required to add them to the scene.


### Scene

The scene contains every object that is relevant: meshes, camera, lights... Objects are associated with their unique name in the scene, and can be retrieved that way.

`scene.update()` updates every component of the objects it contains, so make sure to add every object to the scene before running the renderer.

### Mesh / Model

A 3D model can be imported, put the `.obj` file in the resources folder and use

```cpp
slate::ModelPtr model = std::make_shared<slate::Model>(std::string(ROOT_DIR) + "resources/my_obj.obj");
```

Textured models are not yet supported, only single-color materials.

To render the model, it has to be linked to a `MeshRendererComponent`:

```cpp
slate::SlateObjectPtr obj = std::make_shared<slate::SlateObject>("cube");
obj->add_component(std::make_shared<slate::MeshRendererComponent>(model, renderer.get_default_shader()));
scene.add(obj);
```

### Camera

A `Camera` is a `SlateObject` so components can be assigned to it. Be sure to add a camera to the scene before running the renderer.

### Light

`Light` is also a `SlateObject` and can have components. Only one type of light is implemented for now: `PointLight`. Its fields (`power` and `color`) are public. The default shader can handle up to 32 lights. 

### Renderer

The renderer is the main object in Slate. Initialize it with:

```cpp
slate::Renderer renderer(window_width, window_height, "Window name");
```

Once every object is set up, run the main loop with

```cpp
renderer.run(scene);
```

### Shader

For now, shaders are not easily customizable. The only shader which can be retrieved is by querying the renderer:

```cpp
renderer.get_default_shader();
```

This shader has to be passed to the `MeshRendererComponent`.

### Input

Keyboard input is handled via a singleton. To check if a specific key is pressed:

```cpp
Input::get().key_pressed(GLFW_KEY_W);
```

Refer to [GLFW Keyboard keys](https://www.glfw.org/docs/3.3/group__keys.html) for a complete list.  

Mouse input is implemented via an subject/observer pattern, the `on_notify` callback function has to be overridden:

```cpp
void FpsInputComponent::on_notify(Event event) {
    if (event.type != EventType::MOUSE_MOVE)
        return;

    glm::vec2 mouse_pos(event.read_float_arg("mouse_x"), event.read_float_arg("mouse_y"));
    {...}
}
```

For a complete example of handling input, check the `FpsInputComponent` located in library/slate/component/input/fps/.