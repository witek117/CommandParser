#pragma once

#include "ItemBase.hpp"
#include "PrintManager.hpp"
#include <array>
#include "esp_log.h"

class CommandSetBase : public ItemBase {
  public:
    CommandSetBase(const char* name, const char* description) : ItemBase(name, description) {
    }

    virtual uint8_t getInfo(char* buffer, size_t bufferLength) override;

    virtual bool parse(PrintManager* print, const char* data, size_t dataLen, uint8_t& parseDepth) override;

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

    virtual int getCount() const {
        return count;
    }

    virtual ItemBase* getItem(int index) {
        return commands[index];
    }

    int printHints(PrintManager& print, char* data, size_t length) {
        uint8_t match_commands = 0;
        // uint8_t found_index    = 0;

        // if (length == 0) {
        //     print.print("\n\r");
        //     for (auto& item : commands) {
        //         print.print(item->getName());
        //         print.print('\t');
        //         print.print(item->getDescription());
        //         print.print("\n\r");
        //     }
        //     return 0;
        // }

        // uint8_t     name_len = 0;
        // const char* name     = ItemBase::getNextArg(data, name_len);

        // ItemBase::Match lastMatch = ItemBase::Match::NO;

        // for (uint8_t i = 0; i < getCount(); i++) {
        //     auto item = getItem(i);
        //     if (item == nullptr) {
        //         continue;
        //     }

        //     lastMatch = item->checkName(name);

        //     if (lastMatch != ItemBase::Match::NO) {
        //         match_commands++;
        //         found_index = i;
        //     }
        // }

        // ESP_LOGI("CommandSet", "match_commands %d", match_commands);

        // if (match_commands == 1) {
        //     auto match_command = getItem(found_index);

        //     if (name[name_len] == ' ') {
        //         name_len += 1;
        //     }

        //     auto received_bytes = match_command->getNameLen();
        //     std::memcpy(data, match_command->getName(), received_bytes);
        //     // data[received_bytes] = 0;
        //     // print.print("\r\n");
        //     match_command->printHints(print, (char*)(name + name_len), length - name_len);
        //     // print.print(data);
        //     // print("\n\r");
        // }
        return match_commands;
    }
};
