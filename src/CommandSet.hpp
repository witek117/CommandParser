#pragma once

#include "ItemBase.hpp"
#include <array>

#include <iostream>

class CommandSetBase : public ItemBase {
  public:
    virtual uint8_t getInfo(char* buffer, size_t bufferLength) {
        (void)buffer;
        (void)bufferLength;
        return 0;
    }

    virtual int getCount() const = 0;

    virtual bool parse(const char* data, size_t dataLen, uint8_t& parseDepth) {
        if (match(data, dataLen, true)) {
            parseDepth++;
            uint8_t temporaryParseDepth = parseDepth;            
            data += dataLen;
            uint8_t len = 0;
            const char* ptr = getNextArg(data, len);
            if (len == 0 || ptr == nullptr) {
                return false;
            }
            for (int i = 0; i < getCount(); i++) {
                if (getCommand(i)->parse(ptr, len, temporaryParseDepth)) {
                    parseDepth = temporaryParseDepth;
                    return true;
                }
                if (temporaryParseDepth != parseDepth) {
                    parseDepth = temporaryParseDepth;
                    return false;
                }
            }
        }
        return false;
    }

    virtual ItemBase* getCommand(int index) = 0;

    CommandSetBase(const char* name, const char* description) : ItemBase(name, description) {
    }
};

template<int count> class CommandSet : public CommandSetBase {
  public:
    using CommandContainer = std::array<ItemBase*, count>;

    CommandContainer commands;
    constexpr CommandSet(const char* name, const char* description, CommandContainer commands) : CommandSetBase(name, description), commands(commands) {
    }

    virtual int getCount() const {
        return count;
    }

    virtual ItemBase* getCommand(int index) {
        return commands[index];
    }
};
