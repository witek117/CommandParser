#include "CommandBase.hpp"
#include <cstring>

bool CommandBase::parse(PrintManager* print, const char* data, std::size_t& depth) {
    printer = print;
    if (check_name(data) == ItemBase::Match::ALL) {
        depth++;
        args = data + name_len + 1;
        callback_handler(args);
        return true;
    }
    return false;
}

std::size_t CommandBase::get_args_count() const {
    std::size_t args_count = 0;
    const char* ptr        = args;
    while (true) {
        ptr = get_arg(ptr);
        if (ptr == nullptr) {
            break;
        }
        args_count++;
        ptr++;

        while (true) {
            if (*ptr == ' ' || *ptr == '\0') {
                break;
            }
            ptr++;
        }
    }
    return args_count;
};

bool CommandBase::check_args_count() const {
    return get_args_count() == parameters_count;
}
