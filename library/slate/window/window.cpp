#include <iostream>
#include <memory>

#include "window.hpp"
#include "callbacks.hpp"


namespace slate {
    static void framebuffer_size_callback(GLFWwindow*, int width, int height) {
        Event e;
        e.type = EventType::WINDOW_RESIZE;
        e.add_integer_arg("width", width);
        e.add_integer_arg("height", height);
        Callback::get().window_resize.notify(e);
    }

    static void mouse_callback(GLFWwindow*, double xpos, double ypos) {
        Event e;
        e.type = EventType::MOUSE_MOVE;
        e.add_float_arg("mouse_x", static_cast<float>(xpos));
        e.add_float_arg("mouse_y", static_cast<float>(ypos));
        Callback::get().mouse_move.notify(e);
    }

    Window::Window(unsigned int width_, unsigned int height_, const std::string& name) : width(width_), height(height_) {
        // GLFW init
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // Window Creation
        id = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
        if (id == NULL) {
            std::cerr << "Error::Window:: Failed to create GLFW window\n";;
            glfwTerminate();
            exit(-1);
        }
        glfwMakeContextCurrent(id);
        glfwSetFramebufferSizeCallback(id, framebuffer_size_callback);
        glfwSetCursorPosCallback(id, mouse_callback);

        // GLAD init
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Error::Window: Failed to initialize GLAD" << std::endl;
            exit(-1);
        }
    }

    Window::~Window() {
        glfwTerminate();
    }

    void Window::process_input() {
        if (key_pressed(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(id, true);
    }

    bool Window::key_pressed(unsigned int key) const {
        return (glfwGetKey(id, key) == GLFW_PRESS);
    }

    void Window::end_frame() {
        glfwSwapBuffers(id);
        glfwPollEvents();
    }

    bool Window::should_close() const {
        return glfwWindowShouldClose(id);
    }

    unsigned int Window::get_width() const {
        return width;
    }

    unsigned int Window::get_height() const {
        return height;
    }

    void Window::on_notify(Event event) {
        width = event.read_integer_arg("width");
        height = event.read_integer_arg("height");
        glViewport(0, 0, width, height);
    }

    void Window::disable_cursor() {
        glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::enable_cursor() {
        glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    
}