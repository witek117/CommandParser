#pragma once

#include "ItemBase.hpp"
#include <tuple>
#include <inttypes.h>

class CommandBase : public ItemBase {
    bool    shouldReturnValue = false;
    uint8_t parametersCount   = 0;
    const char* argsBegin = nullptr;

  protected:
    virtual void    callback_handler(const char* data) = 0;
    virtual uint8_t getValuesInfo(char* buffer)        = 0;

  public:
    virtual uint8_t getInfo(char* buffer, size_t bufferLength);

    CommandBase(const char* name, const char* description, bool shouldReturnValue, uint8_t parametersCount) :
        ItemBase(name, description), shouldReturnValue(shouldReturnValue), parametersCount(parametersCount) {
    }

    virtual bool parse(const char* data, size_t dataLen, uint8_t& parseDepth);

  protected:
    template<typename Function, typename Tuple, size_t... I> constexpr auto call(Function f, ItemBase* item, Tuple t, std::index_sequence<I...>) {
        return f(item, std::get<I>(t)...);
    }

    template<typename Function, typename Tuple> constexpr auto call(Function f, ItemBase* item, Tuple t) {
        constexpr auto size = std::tuple_size<Tuple>::value;
        return call(f, item, t, std::make_index_sequence<size>{});
    }
};
