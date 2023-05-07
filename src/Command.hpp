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

    template<typename R0, typename... R>
    static constexpr uint8_t checkType(char* buffer, uint8_t len) {
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

    Command(const char* name, const char* description, void (*handler)(CommandBase&, T...), bool shouldReturnValue = false) :
        CommandBase(name, description, shouldReturnValue, sizeof...(T)), handler(handler) {
    }
};
