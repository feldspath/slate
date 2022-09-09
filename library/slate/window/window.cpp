#include "window.hpp"

#include <iostream>


namespace slate {
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
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
        if (glfwGetKey(id, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(id, true);
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
    
}