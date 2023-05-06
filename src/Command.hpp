#pragma once

#include "CommandBase.hpp"
#include "parser.hpp"

template<typename... T>
class Command : public CommandBase {
    void (*handler)(CommandBase&, T...) = nullptr;

    template<class R0, class... R>
    constexpr std::tuple<R0, R...> get(std::string_view const& s) {
        const auto val      = parser::parse<R0>(s);
        const auto spacePos = s.find_first_of(' ', 0);
        char*      ptr      = (char*)"";

        if (spacePos != std::string_view::npos) {
            auto p = (char*)getNextArg(s.data() + spacePos);
            if (p != nullptr) {
                ptr = p;
            }
        }

        if constexpr (sizeof...(R) > 0) {
            return std::tuple_cat(std::make_tuple(val), get<R...>(ptr));
        } else {
            return std::make_tuple(val);
        }
    }

    inline void callback_handler(const char* data) override {
        (void)data;
        if constexpr (sizeof...(T) > 0) {
            call(handler, static_cast<CommandBase&>(*this), get<T...>(data));
        } else {
            handler(static_cast<CommandBase&>(*this));
        }
    }

    template<typename R0, typename... R>
    constexpr uint8_t checkType(char* buffer, uint8_t len) {
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
