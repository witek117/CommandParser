#pragma once

#include "ItemBase.hpp"
#include "Parser.hpp"
#include <tuple>

class CommandBase : public ItemBase {
    const bool    shouldReturnValue = false;
    const uint8_t parametersCount   = 0;

  protected:
    const char* argsBegin = nullptr;

    virtual void    callback_handler(const char* data) = 0;
    virtual uint8_t getValuesInfo(char* buffer)        = 0;

  public:
    uint8_t getArgCount() const {
        uint8_t     argsCount = 0;
        const char* ptr       = argsBegin;
        while (true) {
            ptr = getNextArg(ptr);
            if (ptr == nullptr) {
                break;
            }
            argsCount++;
            ptr++;

            while (true) {
                if (*ptr == ' ' || *ptr == '\0') {
                    break;
                }
                ptr++;
            }
        }
        return argsCount;
    };

    bool checkArgsCount() const {
        return getArgCount() == parametersCount;
    }

    virtual uint8_t getInfo(char* buffer, size_t bufferLength);

    CommandBase(const char* name, const char* description, bool shouldReturnValue, uint8_t parametersCount) :
        ItemBase(name, description), shouldReturnValue(shouldReturnValue), parametersCount(parametersCount) {
    }

    virtual bool parse(PrintManager* print, const char* data, uint8_t& parseDepth) override;

  protected:
    template<typename Function, typename Tuple, size_t... I>
    static constexpr auto call(Function f, CommandBase& cmd, Tuple t, std::index_sequence<I...>) {
        return f(cmd, std::get<I>(t)...);
    }

    template<typename Function, typename Tuple>
    static constexpr auto call(Function f, CommandBase& cmd, Tuple t) {
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
};
