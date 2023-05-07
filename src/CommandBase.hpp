#pragma once

#include "ItemBase.hpp"
#include "Parser.hpp"
#include <tuple>

class CommandBase : public ItemBase {
    bool        shouldReturnValue = false;
    uint8_t     parametersCount   = 0;
    const char* argsBegin         = nullptr;

  protected:
    virtual void    callback_handler(const char* data) = 0;
    virtual uint8_t getValuesInfo(char* buffer)        = 0;

  public:
    uint8_t getArgLen() {
        return 0;
    };

    virtual uint8_t getInfo(char* buffer, size_t bufferLength);

    CommandBase(const char* name, const char* description, bool shouldReturnValue, uint8_t parametersCount) :
        ItemBase(name, description), shouldReturnValue(shouldReturnValue), parametersCount(parametersCount) {
    }

    virtual bool parse(const char* data, size_t dataLen, uint8_t& parseDepth);

  protected:
    template<typename Function, typename Tuple, size_t... I>
    constexpr auto call(Function f, CommandBase& cmd, Tuple t, std::index_sequence<I...>) {
        return f(cmd, std::get<I>(t)...);
    }

    template<typename Function, typename Tuple>
    constexpr auto call(Function f, CommandBase& cmd, Tuple t) {
        constexpr auto size = std::tuple_size<Tuple>::value;
        return call(f, cmd, t, std::make_index_sequence<size>{});
    }

  public:
    template<class R0, class... R>
    static constexpr std::tuple<R0, R...> get(std::string_view const& s) {
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
};
