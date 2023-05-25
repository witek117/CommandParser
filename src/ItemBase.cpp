#include "ItemBase.hpp"

const char* ItemBase::getNextArg(const char* data) {
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

const char* ItemBase::getNextArg(const char* data, uint8_t& argSize) {
    argSize         = 0;
    const char* ptr = getNextArg(data);
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

const char* ItemBase::getName() const {
    return name;
}

size_t ItemBase::getNameLen() const {
    return nameLen;
}

ItemBase::Match ItemBase::checkName(const char* data) const {
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
        return Match::ALL;
    }

    return Match::PART;


    // if (allLength && (nameLen != length)) {
    //     return false;
    // }

    // if (length > nameLen) {
    //     return false;
    // }

    // return std::memcmp(data, name, length) == 0 ? Match::ALL : Match::NO;
}
