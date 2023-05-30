#pragma once

#include "ItemBase.hpp"
#include "PrintManager.hpp"
#include <array>

class CommandSetBase : public ItemBase {
  public:
    CommandSetBase(const char* name, const char* description) : ItemBase(name, description) {
    }

    virtual bool parse(PrintManager* print, const char* data, uint8_t& depth) override;

    virtual int print_hints(PrintManager& print, ParseBuffer& buffer, uint8_t& depth) override;

    virtual ItemBase* get_item(int index) = 0;

    virtual int get_count() const = 0;
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

    virtual int get_count() const {
        return count;
    }

    virtual ItemBase* get_item(int index) {
        return commands[index];
    }
};
