#pragma once
#include <tuple>
#include <cstring>
#include <inttypes.h>

class CommandBase {
    const char* name;
    size_t      name_len = 0;
    const char* description;
    size_t      description_len   = 0;
    bool        shouldReturnValue = false;
    uint8_t     parametersCount   = 0;

  protected:
    virtual void    callback_handler(const char* data) = 0;
    virtual uint8_t getValuesInfo(char* buffer)        = 0;

  public:
    const char* getName() const {
        return name;
    }

    size_t getNameLen() const {
        return name_len;
    }

    uint8_t getInfo(char* buffer, size_t bufferLength);

    CommandBase(const char* name, const char* description, bool shouldReturnValue, uint8_t parametersCount) :
        name(name), name_len(std::strlen(name)), description(description), description_len(strlen(description)), shouldReturnValue(shouldReturnValue), parametersCount(parametersCount) {
    }

    bool parse(char* data, size_t data_len);

  protected:
    template<typename Function, typename Tuple, size_t... I> constexpr auto call(Function f, Tuple t, std::index_sequence<I...>) {
        return f(std::get<I>(t)...);
    }

    template<typename Function, typename Tuple> constexpr auto call(Function f, Tuple t) {
        constexpr auto size = std::tuple_size<Tuple>::value;
        return call(f, t, std::make_index_sequence<size>{});
    }
};
