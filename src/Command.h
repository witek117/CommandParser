#pragma once

#include <utility>
#include <tuple>
#include <array>
#include <cstring>
#include <string_view>

namespace parser {
    template<class T> T parse(std::string_view s);

    template<class T0, class... T>  std::tuple<T0, T...>  get(std::string_view const s) {
        const auto spacePos = s.find_first_of(' ', 0);

        auto val =  parse<T0>(s);

        if constexpr (sizeof ...(T)) {
            return std::tuple_cat(std::make_tuple(val), get<T...>(s.substr(spacePos+ 1)));
        } else {
            return std::make_tuple(val);
        }
    }

    template<typename T, int size>
    std::array<T, size> get (const char * data) {
        std::array<T, size> return_data {};

        if constexpr (std::is_same<T, float>() ) {
            char *end;

            for (int i=0; i < size; i++) {
                return_data[i] = strtod(data, &end);
                data = end;
            }
        } else if constexpr (std::is_same<int, T>() || std::is_same<uint32_t, T>()) {
            char *end;

            for (int i=0; i < size; i++) {
                return_data[i] = strtol(data, &end, 10);
                data = end;
            }
        }
        return return_data;
    }
}

class CommandTemplate {
    const char * name;
    size_t  name_len = 0;
public:
    virtual void callback_handler(const char* data) = 0;
public:
    explicit CommandTemplate(const char* name) : name(name), name_len(strlen(name)){ }

    bool parse(char * data, size_t data_len) {
        if (data_len != name_len) {
            return false;
        }

        if (memcmp(data, name, name_len) == 0) {
            callback_handler((char *)data + name_len + 1);
            return true;
        }
        return false;
    }
};

class Command : public CommandTemplate{
    void (*handler)(const char* data) = nullptr;
    inline void callback_handler(const char* data) override {
        handler(data);
    }
public:
    Command(const char* name, void (*handler)(const char* data)) : CommandTemplate(name), handler(handler) { }
};

template<typename T0>
class Command_T1 : public CommandTemplate{
    void (*handler)(T0) = nullptr;
    inline void callback_handler(const char* data) override {
        auto [t0, t1] = parser::parse<T0>(data);
        handler(t0, t1);
    }
public:
    Command_T1(const char* name, void (*handler)(T0)) : CommandTemplate(name), handler(handler) { }
};

template<typename T0, typename T1>
class Command_T2 : public CommandTemplate{
    void (*handler)(T0, T1) = nullptr;
    inline void callback_handler(const char* data) override {
        auto [t0, t1] = parser::parse<T0, T1>(data);
        handler(t0, t1);
    }
public:
    Command_T2(const char* name, void (*handler)(T0, T1)) : CommandTemplate(name), handler(handler) { }
};

template<typename T0, typename T1, typename T2>
class Command_T3 : public CommandTemplate{
    void (*handler)(T0, T1, T2) = nullptr;
    inline void callback_handler(const char* data) override {
        auto [t0, t1, t2] = parser::parse<T0, T1, T2>(data);
        handler(t0, t1, t2);
    }
public:
    Command_T3(const char* name, void (*handler)(T0, T1, T2)) : CommandTemplate(name), handler(handler) { }
};
