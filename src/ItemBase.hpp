#pragma once

#include <cstdint>
#include <cstring>

class ItemBase {
  protected:
    const char* name;
    size_t      nameLen;
    const char* description;
    size_t      descriptionLen;

  public:
    static const char* getNextArg(const char* data);

    static const char* getNextArg(const char* data, uint8_t& argSize);

    ItemBase(const char* name, const char* description) : 
        name(name), nameLen(std::strlen(name)), description(description), descriptionLen(strlen(description)) {
    }

    const char* getName() const;

    size_t getNameLen() const;

    bool checkName(const char* data, size_t length, bool allLength = false) const;

    virtual uint8_t getInfo(char* buffer, size_t bufferLength) {
        (void)buffer;
        (void)bufferLength;
        return 0;
    }

    virtual bool parse(const char* data, size_t data_len, uint8_t& parseDepth) {
        (void)data;
        (void)data_len;
        (void)parseDepth;
        return false;
    }
};
