#pragma once

#include "ItemBase.hpp"
#include <array>

class CommandSetBase : public ItemBase {
  public:
    CommandSetBase(const char* name, const char* description) : ItemBase(name, description) {
    }

    virtual uint8_t getInfo(char* buffer, size_t bufferLength) override;

    virtual bool parse(const char* data, size_t dataLen, uint8_t& parseDepth) override;

    virtual ItemBase* getCommand(int index) = 0;

    virtual int getCount() const = 0;
};

template<int count>
class CommandSet : public CommandSetBase {
  public:
    using CommandContainer = std::array<ItemBase*, count>;

    CommandContainer commands;
    
    constexpr CommandSet(const char* name, const char* description, CommandContainer commands) :
        CommandSetBase(name, description), commands(commands) {
    }

    virtual int getCount() const {
        return count;
    }

    virtual ItemBase* getCommand(int index) {
        return commands[index];
    }
};
