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
        } else if constexpr (std::is_same<char, T>()) {
            char *end;

            for (int i=0; i < size; i++) {
                return_data[i] = data[0];
                data = end;
            }
        }
        return return_data;
    }
}

class CommandTemplate {
    const char * name;
    size_t  name_len = 0;
    bool shouldReturnValue = false;

protected:
    virtual void callback_handler(const char* data) = 0;
    virtual uint8_t getValuesInfo(char* buffer) = 0;

public:
    uint8_t getInfo(char* buffer) {
        uint8_t offset = 0;
        memcpy(buffer, name, name_len);
        offset += name_len;
        buffer[offset++] = '\t';
        offset += getValuesInfo(buffer + offset);
        buffer[offset++] = '\t';
        buffer[offset++] = shouldReturnValue ? 't' : 'f';
        return offset;
    }

    explicit CommandTemplate(const char* name, bool shouldReturnValue) : name(name), name_len(strlen(name)), shouldReturnValue(shouldReturnValue){ }

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
    uint8_t getValuesInfo(char* buffer) override {
        buffer[0] = 'c';
        buffer[1] = '*';
        return 2;
    }

    Command(const char* name, void (*handler)(const char*), bool shouldReturnValue = false) : CommandTemplate(name, shouldReturnValue), handler(handler) { }
};

class Command_Void : public CommandTemplate{
    void (*handler)() = nullptr;
    inline void callback_handler(const char* data) override {
        (void) data;
        handler();
    }

public:
    uint8_t getValuesInfo(char* buffer) override {
        buffer[0] = 'v';
        return 1;
    }

    Command_Void(const char* name, void (*handler)(), bool shouldReturnValue = false) : CommandTemplate(name, shouldReturnValue), handler(handler) { }
};

template<class T0>
class Command_T1 : public CommandTemplate{
    void (*handler)(T0) = nullptr;
    inline void callback_handler(const char* data) override {
        auto [t0] = parser::get<T0>(data);
        handler(t0);
    }

public:
    uint8_t getValuesInfo(char* buffer) override {
        buffer[0] = typeid(T0).name()[0];
        return 1;
    }

    Command_T1(const char* name, void (*handler)(T0), bool shouldReturnValue = false) : CommandTemplate(name, shouldReturnValue), handler(handler) { }
};

template<typename T0, typename T1>
class Command_T2 : public CommandTemplate{
    void (*handler)(T0, T1) = nullptr;
    inline void callback_handler(const char* data) override {
        auto [t0, t1] = parser::get<T0, T1>(data);
        handler(t0, t1);
    }

public:
    uint8_t getValuesInfo(char* buffer) override {
        buffer[0] = typeid(T0).name()[0];
        buffer[1] = typeid(T1).name()[0];
        return 2;
    }

    Command_T2(const char* name, void (*handler)(T0, T1), bool shouldReturnValue = false) : CommandTemplate(name, shouldReturnValue), handler(handler) { }
};

template<typename T0, typename T1, typename T2>
class Command_T3 : public CommandTemplate{
    void (*handler)(T0, T1, T2) = nullptr;
    inline void callback_handler(const char* data) override {
        auto [t0, t1, t2] = parser::get<T0, T1, T2>(data);
        handler(t0, t1, t2);
    }

public:
    uint8_t getValuesInfo(char* buffer) override {
        buffer[0] = typeid(T0).name()[0];
        buffer[1] = typeid(T1).name()[0];
        buffer[2] = typeid(T2).name()[0];
        return 3;
    }

    Command_T3(const char* name, void (*handler)(T0, T1, T2), bool shouldReturnValue = false) : CommandTemplate(name, shouldReturnValue), handler(handler) { }
};
