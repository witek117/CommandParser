#include "ItemBase.hpp"

const char* ItemBase::getNextArg(const char* data) {
    if(data == nullptr) {
        return nullptr;
    }
    const char* ptr = data;
    while(true) {
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
    argSize = 0;
    const char* ptr = getNextArg(data);
    if(ptr == nullptr) {
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

bool ItemBase::checkName(const char* data, size_t length, bool allLength) const {
    if (allLength && (nameLen != length)) {
        return false;
    }

    if (length > nameLen) {
        return false;
    }

    return std::memcmp(data, name, length) == 0;
}
