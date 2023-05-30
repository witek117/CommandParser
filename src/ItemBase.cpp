#include "ItemBase.hpp"

const char* ItemBase::get_arg(const char* data) {
    if (data == nullptr) {
        return nullptr;
    }
    const char* ptr = data;
    while (true) {
        if (*ptr == '\0') {
            return nullptr;
        }

        if (*ptr == ' ') {
            ptr++;
        } else {
            break;
        }
    }
    return ptr;
}

const char* ItemBase::get_arg(const char* data, uint8_t& argSize) {
    argSize         = 0;
    const char* ptr = get_arg(data);
    if (ptr == nullptr) {
        return nullptr;
    }

    for (uint8_t i = 0; i < 20; i++) {
        if (ptr[i] == ' ' || ptr[i] == '\0') {
            argSize = i;
            return ptr;
        }
    }
    return nullptr;
}

const char* ItemBase::get_name() const {
    return name;
}

size_t ItemBase::get_name_len() const {
    return nameLen;
}

ItemBase::Match ItemBase::check_name(const char* data) const {
    uint8_t i = 0;
    for (; i < nameLen; i++) {
        if (data[i] != name[i]) {
            break;
        }

        if (data[i] == ' ' || data[i] == '\0') {
            break;
        }
    }

    if (i == 0) {
        return Match::NO;
    }

    if (i == nameLen) {
        return (data[nameLen] != '\0' && data[nameLen] != ' ') ? Match::NO : Match::ALL;
    }

    return Match::PART;
}
