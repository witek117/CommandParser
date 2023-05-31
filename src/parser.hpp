#pragma once

#include <string_view>

namespace parser {
    template<class T>
    T parse(std::string_view const& s);

} // namespace parser
