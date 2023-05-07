#include "Parser.hpp"
#include <cstdlib>

namespace parser {
    template<>
    double parse<double>(std::string_view const& s) {
        return atof(s.data());
    }

    template<>
    int parse<int>(std::string_view const& s) {
        return atoi(s.data());
    }

    template<>
    uint32_t parse<uint32_t>(std::string_view const& s) {
        return (uint32_t)atoi(s.data());
    }

    template<>
    float parse<float>(std::string_view const& s) {
        return atof(s.data());
    }

    template<>
    char parse<char>(std::string_view const& s) {
        return s[0];
    }

    template<>
    const char* parse<const char*>(std::string_view const& s) {
        return (const char*)&s[0];
    }

    template<>
    void parse<void>(std::string_view const& s) {
        (void)s;
    }
} // namespace parser
