#include "Command.h"

namespace parser {

    template<> double parse<double>(std::string_view const s) {
        return atof(s.data());
    }

    template<> int parse<int>(std::string_view const s) {
        return atoi(s.data());
    }

    template<> float parse<float>(std::string_view const s) {
        return atof(s.data());
    }

    template<> char parse<char>(std::string_view const s) {
        return s[0];
    }

    template<> void parse<void>(std::string_view const s) {
        (void) s;
    }
}