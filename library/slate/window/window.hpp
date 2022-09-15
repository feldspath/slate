#pragma once

#include <string>

#include <slate/glad/glad.hpp>
#include <GLFW/glfw3.h>

#include <slate/event/observer.hpp>
#include <slate/camera/base/camera_base.hpp>


namespace slate {
    class Window : public Observer {
    private:
        GLFWwindow* id;
        unsigned int width;
        unsigned int height;

    public:
        Window(unsigned int width_, unsigned int height_, const std::string& name);
        ~Window();

        void process_input();
        void end_frame();
        bool should_close() const;

        unsigned int get_width() const;
        unsigned int get_height() const;

        void on_notify(Event event) override;

        void disable_cursor();
        void enable_cursor();
    };
}
