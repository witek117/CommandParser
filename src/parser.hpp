#pragma once
#include <cstdint>
#include <string_view>
#include <type_traits>
#include <tuple>
#include <cstdlib>

namespace parser {
    template<class T> T parse(std::string_view const& s);

    template<class T0, class... T> constexpr std::tuple<T0, T...> get(std::string_view const& s) {
        const auto spacePos = s.find_first_of(' ', 0);

        auto val = parse<T0>(s);

        if constexpr (sizeof...(T) > 0) {
            return std::tuple_cat(std::make_tuple(val), get<T...>(s.substr(spacePos + 1)));
        } else {
            return std::make_tuple(val);
        }
    }

    template<typename T, int size> constexpr std::array<T, size> get(const char* data) {
        std::array<T, size> return_data{};

        if constexpr (std::is_same<T, float>()) {
            char* end;

            for (int i = 0; i < size; i++) {
                return_data[i] = strtod(data, &end);
                data           = end;
            }
        } else if constexpr (std::is_same<int, T>() || std::is_same<uint32_t, T>()) {
            char* end;

            for (int i = 0; i < size; i++) {
                return_data[i] = strtol(data, &end, 10);
                data           = end;
            }
        } else if constexpr (std::is_same<char, T>()) {
            for (int i = 0; i < size; i++) {
                return_data[i] = data[0];
            }
        }
        return return_data;
    }
} // namespace parser