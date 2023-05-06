#pragma once

#include "CommandBase.hpp"
#include "parser.hpp"

template<typename... T> class Command : public CommandBase {
    void (*handler)(ItemBase*, T...) = nullptr;

    inline void callback_handler(const char* data) override {
        (void)data;
        if constexpr (sizeof...(T) > 0) {
            call(handler, this, parser::get<T...>(data));
        } else {
            handler(this);
        }
    }

    template<typename R0, typename... R> constexpr uint8_t checkType(char* buffer, uint8_t len) {
        if constexpr (std::is_same<R0, float>()) {
            buffer[len] = 'f';
        } else if constexpr (std::is_same<R0, double>()) {
            buffer[len] = 'd';
        } else if constexpr (std::is_same<R0, void>()) {
            buffer[len] = 'v';
        } else if constexpr (std::is_same<R0, int>() || std::is_same<R0, uint32_t>()) {
            buffer[len] = 'i';
        } else if constexpr (std::is_same<R0, char>()) {
            buffer[len] = 'c';
        } else if constexpr (std::is_same<R0, const char*>()) {
            buffer[len++] = 'c';
            buffer[len]   = '*';
        } else if constexpr (std::is_same<R0, bool>()) {
            buffer[len] = 'b';
        } else {
            buffer[len] = 'u';
        }

        len++;
        if constexpr (sizeof...(R) > 0) {
            return checkType<R...>(buffer, len);
        } else {
            return len;
        }
    }

  public:
    uint8_t getValuesInfo(char* buffer) override {
        if constexpr (sizeof...(T) > 0) {
            return checkType<T...>(buffer, 0);
        } else {
            buffer[0] = 'v';
            return 1;
        }
        return ' ';
    }

    Command(const char* name, const char* description, void (*handler)(ItemBase*, T...), bool shouldReturnValue = false) : CommandBase(name, description, shouldReturnValue, sizeof...(T)), handler(handler) {
    }
};
