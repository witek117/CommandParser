#pragma once

#include "ItemBase.hpp"
#include "PrintManager.hpp"
#include <array>

class CommandSetBase : public ItemBase {
  public:
    CommandSetBase(const char* name, const char* description) : ItemBase(name, description) {
    }

    virtual bool parse(PrintManager* print, const char* data, uint8_t& parseDepth) override;

    virtual int printHints(PrintManager& print, ParseBuffer& buffer, uint8_t& depth) override;

    virtual ItemBase* getItem(int index) = 0;

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

    constexpr CommandSet(CommandContainer commands) : CommandSetBase("", ""), commands(commands) {
    }

    virtual int getCount() const {
        return count;
    }

    virtual ItemBase* getItem(int index) {
        return commands[index];
    }
};
