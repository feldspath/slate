#include <iostream>
#include <memory>

#include "window.hpp"
#include "callbacks.hpp"

static void APIENTRY dl_debug_output(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

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
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
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

        // Debug
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(dl_debug_output, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
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