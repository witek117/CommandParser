#pragma once

#include "CommandBase.hpp"
#include "Parser.hpp"

template<typename... T>
class Command : public CommandBase {
    void (*handler)(CommandBase&, T...) = nullptr;

    inline void callback_handler(const char* data) override {
        (void)data;
        if constexpr (sizeof...(T) > 0) {
            call(handler, static_cast<CommandBase&>(*this), get<T...>(data));
        } else {
            handler(static_cast<CommandBase&>(*this));
        }
    }

  public:
    std::size_t get_values_info(char* buffer) override {
        if constexpr (sizeof...(T) > 0) {
            return check_type<T...>(buffer, 0);
        } else {
            buffer[0] = 'v';
            return 1;
        }
        return ' ';
    }

    Command(const char* name,
            const char* description,
            void (*handler)(CommandBase&, T...),
            bool should_return_value = false) :
        CommandBase(name, description, should_return_value, sizeof...(T)), handler(handler) {
    }
};
