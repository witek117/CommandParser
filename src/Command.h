#pragma once

#include <utility>
#include <tuple>
#include <array>
#include <cstring>
#include <string_view>

namespace parser {
    template<class T>
    T parse(std::string_view s);

    template<class T0, class... T>
    std::tuple<T0, T...> get(std::string_view const s) {
        const auto spacePos = s.find_first_of(' ', 0);

        auto val = parse<T0>(s);

        if constexpr (sizeof ...(T) > 0) {
            return std::tuple_cat(std::make_tuple(val), get<T...>(s.substr(spacePos + 1)));
        } else {
            return std::make_tuple(val);
        }
    }

    template<typename T, int size>
    std::array<T, size> get(const char *data) {
        std::array<T, size> return_data{};

        if constexpr (std::is_same<T, float>()) {
            char *end;

            for (int i = 0; i < size; i++) {
                return_data[i] = strtod(data, &end);
                data = end;
            }
        } else if constexpr (std::is_same<int, T>() || std::is_same<uint32_t, T>()) {
            char *end;

            for (int i = 0; i < size; i++) {
                return_data[i] = strtol(data, &end, 10);
                data = end;
            }
        } else if constexpr (std::is_same<char, T>()) {
            char *end;

            for (int i = 0; i < size; i++) {
                return_data[i] = data[0];
                data = end;
            }
        }
        return return_data;
    }
}

class CommandTemplate {
    const char *name;
    size_t name_len = 0;
    bool shouldReturnValue = false;

protected:
    virtual void callback_handler(const char *data) = 0;

    virtual uint8_t getValuesInfo(char *buffer) = 0;

public:
    uint8_t getInfo(char *buffer) {
        uint8_t offset = 0;
        memcpy(buffer, name, name_len);
        offset += name_len;
        buffer[offset++] = '\t';
        offset += getValuesInfo(buffer + offset);
        buffer[offset++] = '\t';
        buffer[offset++] = shouldReturnValue ? 't' : 'f';
        return offset;
    }

    CommandTemplate(const char *name, bool shouldReturnValue) : name(name), name_len(strlen(name)),
                                                                shouldReturnValue(shouldReturnValue) {}

    bool parse(char *data, size_t data_len) {
        if (data_len != name_len) {
            return false;
        }

        if (memcmp(data, name, name_len) == 0) {
            callback_handler((char *) data + name_len + 1);
            return true;
        }
        return false;
    }

protected:
    template<typename Function, typename Tuple, size_t ... I>
    auto call(Function f, Tuple t, std::index_sequence<I ...>) {
        return f(std::get<I>(t) ...);
    }

    template<typename Function, typename Tuple>
    auto call(Function f, Tuple t) {
        static constexpr auto size = std::tuple_size<Tuple>::value;
        return call(f, t, std::make_index_sequence<size>{});
    }
};

template<typename ...T>
class Command : public CommandTemplate {
    void (*handler)(T...) = nullptr;

    inline void callback_handler(const char *data) override {
        if constexpr(sizeof...(T) > 0) {
            call(handler, parser::get<T...>(data));
        } else {
            handler();
        }
    }

    template<typename R0, typename ...R>
    uint8_t checkType(char *buffer, uint8_t len) {
        if constexpr(std::is_same<R0, float>()) {
            buffer[len] = 'f';
        } else if constexpr(std::is_same<R0, double>()) {
            buffer[len] = 'd';
        } else if constexpr(std::is_same<R0, void>()) {
            buffer[len] = 'v';
        } else if constexpr(std::is_same<R0, int>()) {
            buffer[len] = 'i';
        } else if constexpr(std::is_same<R0, char>()) {
            buffer[len] = 'c';
        } else if constexpr(std::is_same<R0, const char *>()) {
            buffer[len++] = 'c';
            buffer[len] = '*';
        } else if constexpr(std::is_same<R0, bool>()) {
            buffer[len] = 'b';
        } else {
            buffer[len] = 'u';
        }

        len++;
        if constexpr(sizeof... (R) > 0) {
            return checkType<R...>(buffer, len);
        } else {
            return len;
        }
    }

public:
    uint8_t getValuesInfo(char *buffer) override {
        if constexpr(sizeof...(T) > 0) {
            return checkType<T...>(buffer, 0);
        } else {
            buffer[0] = 'v';
            return 1;
        }
    }

    Command(const char *name, void (*handler)(T...), bool shouldReturnValue = false) : CommandTemplate(name, shouldReturnValue), handler(handler) {}
};
