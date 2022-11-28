#include <iostream>

#include "event.hpp"

namespace slate {
    void Event::add_integer_arg(std::string name, int value) {
        EventArg arg;
        arg.type = EventArg::ArgType::INTEGER;
        arg.as_integer = value;
        arguments.insert(std::pair<std::string, EventArg>(name, arg));
    }

    void Event::add_float_arg(std::string name, float value) {
        EventArg arg;
        arg.type = EventArg::ArgType::FLOAT;
        arg.as_float = value;
        arguments.insert(std::pair<std::string, EventArg>(name, arg));
    }

    void Event::add_bool_arg(std::string name, bool value) {
        EventArg arg;
        arg.type = EventArg::ArgType::BOOL;
        arg.as_bool = value;
        arguments.insert(std::pair<std::string, EventArg>(name, arg));
    }

    int Event::read_integer_arg(std::string name) const {
        const auto& arg = arguments.find(name);
        if (arg == arguments.end()) {
            error_arg_not_found(name);
            return 0;
        }

        if (arg->second.type != EventArg::ArgType::INTEGER) {
            error_arg_wrong_type(name);
            return 0;
        }

        return arg->second.as_integer;        
    }

    float Event::read_float_arg(std::string name) const {
        const auto& arg = arguments.find(name);
        if (arg == arguments.end()) {
            error_arg_not_found(name);
            return 0.0f;
        }

        if (arg->second.type != EventArg::ArgType::FLOAT) {
            error_arg_wrong_type(name);
            return 0.0f;
        }

        return arg->second.as_float;  
    }

    bool Event::read_bool_arg(std::string name) const {
        const auto& arg = arguments.find(name);
        if (arg == arguments.end()) {
            error_arg_not_found(name);
            return false;
        }

        if (arg->second.type != EventArg::ArgType::BOOL) {
            error_arg_wrong_type(name);
            return false;
        }

        return arg->second.as_bool;  
    }

    void Event::error_arg_not_found(std::string arg_name) const {
        std::cerr << "Error::Event: argument " << arg_name << " not found in EventType " << static_cast<int>(type) << '\n';
        abort();
    }

    void Event::error_arg_wrong_type(std::string arg_name) const {
        std::cerr << "Warning::Event: argument " << arg_name << " of wrong type in EventType " << static_cast<int>(type) << '\n';
    }
}