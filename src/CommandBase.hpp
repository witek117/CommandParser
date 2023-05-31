#pragma once

#include "ItemBase.hpp"
#include "Parser.hpp"
#include <tuple>

class CommandBase : public ItemBase {
    const bool        should_return_value = false;
    const std::size_t parameters_count    = 0;

  protected:
    const char* args = nullptr;

    virtual void        callback_handler(const char* data) = 0;
    virtual std::size_t get_values_info(char* buffer)      = 0;

  public:
    std::size_t get_args_count() const;

    bool check_args_count() const;

    CommandBase(const char* name, const char* description, bool should_return_value, std::size_t parameters_count) :
        ItemBase(name, description), should_return_value(should_return_value), parameters_count(parameters_count) {
    }

    virtual bool parse(PrintManager* print, const char* data, std::size_t& depth) override;

  protected:
    template<typename Function, typename Tuple, std::size_t... I>
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
            auto p = (char*)get_arg(s.data() + spacePos);
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
    static constexpr std::size_t check_type(char* buffer, std::size_t len) {
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
            return check_type<R...>(buffer, len);
        } else {
            return len;
        }
    }
};
