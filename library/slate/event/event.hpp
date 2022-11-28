#pragma once

#include <vector>
#include <map>
#include <string>

namespace slate {
    enum class EventType {
        WINDOW_RESIZE,
        MOUSE_MOVE,
        CHRONO_DONE,
        COUNT
    };

    struct EventArg {
        enum class ArgType {
            INTEGER,
            FLOAT,
            BOOL,
            COUNT
        };

        ArgType type;

        union {
            int as_integer;
            float as_float;
            bool as_bool;
        };
    };
    
    struct Event {
        EventType type;
        std::map<std::string, EventArg> arguments;

        void add_integer_arg(std::string name, int value);
        void add_float_arg(std::string name, float value);
        void add_bool_arg(std::string name, bool value);

        int read_integer_arg(std::string name) const;
        float read_float_arg(std::string name) const;
        bool read_bool_arg(std::string name) const;
        
    private:
        void error_arg_not_found(std::string arg_name) const;
        void error_arg_wrong_type(std::string arg_name) const;


    };
}