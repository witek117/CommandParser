#pragma once

#include "PrintManager.hpp"
#include <cstdint>
#include <cstring>

class ItemBase {
  protected:
    const char* name;
    size_t      nameLen;
    const char* description;
    size_t      descriptionLen;
    PrintManager* printer;

  public:
    PrintManager* getPrinter() {
        return printer;
    }

    static const char* getNextArg(const char* data);

    static const char* getNextArg(const char* data, uint8_t& argSize);

    ItemBase(const char* name, const char* description) : 
        name(name), nameLen(std::strlen(name)), description(description), descriptionLen(strlen(description)) {
    }

    const char* getName() const;
    const char* getDescription() const {
        return description;
    }

    size_t getNameLen() const;


    virtual uint8_t getInfo(char* buffer, size_t bufferLength) {
        (void)buffer;
        (void)bufferLength;
        return 0;
    }

    virtual bool parse(PrintManager* print, const char* data, size_t data_len, uint8_t& parseDepth) {
        printer = print;
        (void)data;
        (void)data_len;
        (void)parseDepth;
        return false;
    }

    virtual int printHints(PrintManager& print, char* data, size_t length) {
        print.print(getName());
        print.print('\t');
        print.print(getDescription());
        print.print("\n\r");
        return 1;
    }

    enum class Match {
        NO, ALL, PART
    };

    Match checkName(const char* data) const;
};
