#pragma once

#include <cstdint>
#include <cstring>

class ItemBase {
  protected:
    const char* name;
    size_t      name_len = 0;
    const char* description;
    size_t      description_len = 0;

  public:
    const char* getNextArg(const char* data) {
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

    const char* getNextArg(const char* data, uint8_t& argSize) {
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

    ItemBase(const char* name, const char* description) : name(name), name_len(std::strlen(name)), description(description), description_len(strlen(description)) {
    }

    const char* getName() const {
        return name;
    }

    size_t getNameLen() const {
        return name_len;
    }

    virtual uint8_t getInfo(char* buffer, size_t bufferLength) {
        (void)buffer;
        (void)bufferLength;
        return 0;
    }

    bool match(const char* data, size_t length, bool allLength = false) {
        if (allLength && (name_len != length)) {
            return false;
        }

        if (length > name_len) {
            return false;
        }

        return std::memcmp(data, name, length) == 0;
    }

    virtual bool parse(const char* data, size_t data_len, uint8_t& parseDepth) {
        (void)data;
        (void)data_len;
        (void)parseDepth;
        return false;
    }
};
