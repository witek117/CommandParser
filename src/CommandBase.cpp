#include "CommandBase.hpp"
#include <cstring>

bool CommandBase::parse(PrintManager* print, const char* data, uint8_t& depth) {
    printer = print;
    if (check_name(data) == ItemBase::Match::ALL) {
        depth++;
        args_begin = data + nameLen + 1;
        callback_handler(args_begin);
        return true;
    }
    return false;
}
