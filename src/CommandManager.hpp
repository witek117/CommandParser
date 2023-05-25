#pragma once

#include "ItemBase.hpp"
#include "PrintManager.hpp"
#include "ParseBuffer.hpp"
#include <array>

#include "esp_log.h"

static const char* TAG = "manager";

struct Config {
    typedef bool (*write_function_t)(const char* data, size_t length);
    typedef const char* (*read_function_t)(size_t& length);

    read_function_t  readFunction;
    write_function_t writeFunction;
};

template<int count>
class CommandManager : public PrintManager, ParseBuffer {
    using CommandContainer = std::array<ItemBase*, count>;

    enum class READING_STATE : uint8_t {
        CLEAR,
        ADD_DATA,
        FOUND_COMMAND,
        SKIPPING,
    };

    READING_STATE    state          = READING_STATE::CLEAR;
    size_t           commands_count = count;
    uint8_t          skipping_index = 0;
    Config           config;
    CommandContainer commands;
    size_t           readBufferLength = 0;
    size_t           readBufferIndex  = 0;
    const char*      readBuffer       = nullptr;

  public:
    CommandManager(Config config, CommandContainer commands) : config(config), commands(commands) {
    }

    inline void printData(const char* s, uint8_t length) override {
        config.writeFunction(s, length);
    }

    void init() {
    }

    size_t available() {
        if (readBufferIndex == readBufferLength) {
            readBufferIndex = 0;
            readBuffer      = config.readFunction(readBufferLength);
        }

        return readBufferLength - readBufferIndex;
    }

    char read() {
        return readBuffer[readBufferIndex++];
    }

    bool run() {
        size_t avai = 0;

        switch (state) {
            case READING_STATE::CLEAR:
                state = READING_STATE::ADD_DATA;
                ParseBuffer::clear();
                break;
            case READING_STATE::ADD_DATA:
                avai = available();

                for (auto i = 0; i < avai; i++) {
                    char byte = read();

                    // check tabulator
                    if (byte == '\t' || byte == '?') {
                        printHints();
                        continue;
                    }

                    // check backspace
                    if (byte == 0x7f) {
                        ParseBuffer::pop();
                        break;
                    }

                    // check enter
                    if (byte == '\r' || byte == '\n') {
                        if (ParseBuffer::size() == 0) {
                            continue;
                        }
                        ParseBuffer::push('\0');
                        state = READING_STATE::FOUND_COMMAND;
                        break;
                    }

                    // check escape
                    if (byte == 0x1b) {
                        skipping_index = 0;
                        state          = READING_STATE::SKIPPING;
                        break;
                    }
                    ParseBuffer::push(byte);
                }
                break;
            case READING_STATE::FOUND_COMMAND:
                parse();
                state = READING_STATE::CLEAR;
                break;
            case READING_STATE::SKIPPING:
                if (available() == 0) {
                    break;
                }

                if (skipping_index == 0) {
                    uint8_t byte = read();
                    if (byte == 0x5b) {
                        skipping_index++;
                    } else if (byte == 0x1b) {
                    } else {
                        skipping_index = 0;
                        ParseBuffer::push(byte);
                        state = READING_STATE::ADD_DATA;
                    }
                }

                if (skipping_index == 1) {
                    read();
                    skipping_index = 0;
                    state          = READING_STATE::ADD_DATA;
                }
                break;
        }

        return true;
    }

    ItemBase* getItem(uint8_t index) {
        return commands[index];
    }

    int printHints() {
        // auto* data   = cmd_buffer;
        // auto  length = received_bytes;

        // uint8_t match_commands = 0;
        // uint8_t found_index    = 0;


        // // ESP_LOGI(TAG, "length %d", length);

        // if (length == 0) {
        //     print("\n\r");
        //     for (auto& item : commands) {
        //         print(item->getName());
        //         print('\t');
        //         print(item->getDescription());
        //         print("\n\r");
        //     }
        //     return 0;
        // }

        // uint8_t     name_len = 0;
        // const char* name     = ItemBase::getNextArg(data, name_len);

        // for (uint8_t i = 0; i < commands_count; i++) {
        //     auto item = getItem(i);
        //     if (item == nullptr) {
        //         continue;
        //     }

        //     if (item->checkName(name, name_len, false)) {
        //         match_commands++;
        //         found_index = i;
        //     }
        // }

        // ESP_LOGI("CommandManager", "match_commands %d", match_commands);

        // if (match_commands == 1) {
        //     auto match_command = getItem(found_index);

        //     if (name[name_len] == ' ') {
        //         name_len += 1;
        //     }

        //     // received_bytes     = match_command->getNameLen();
        //     ParseBuffer::clear();
        //     ParseBuffer::push(match_command->getName(), match_command->getNameLen());
        //     ParseBuffer::push(' ');

        //     print("\r\n");
        //     auto ret = match_command->printHints(*this, (char*)(&cmd_buffer[received_bytes]), length - name_len);

        //     // if (ret != 1) {
        //     //     data[length] = 0;
        //     // }

        //     ESP_LOGW(TAG, "Have %d, %s", received_bytes, cmd_buffer);
        //     print(data);
        //     // print("\n\r");
        // } else if (match_commands > 1) {
        //     received_bytes = length;
        //     print("\n\r");
        //     data[received_bytes] = 0;
        //     print(data);
        //     // for(auto& item : commands) {
        //     //     print(item->getName());
        //     //     print('\t');
        //     //     print(item->getDescription());
        //     //     print("\n\r");
        //     // }
        // } else {
        //     received_bytes = length;
        //     print("\r");
        //     data[received_bytes] = 0;
        //     print(data);
        // }


        return 1;
    }

    void parse() {
        ESP_LOGW(TAG, "parse %d, %s", ParseBuffer::size(), ParseBuffer::get());

        print("\n\r");
        uint8_t commandTitleLen = 0;
        ItemBase::getNextArg(ParseBuffer::get(), commandTitleLen);
        uint8_t temporaryParseDepth = 0;
        for (uint8_t i = 0; i < commands_count; i++) {
            auto item = getItem(i);
            if (item == nullptr) {
                continue;
            }
            if (item->parse(this, ParseBuffer::get(), temporaryParseDepth)) {
                return;
            }
            if (temporaryParseDepth != 0) {
                break;
            }
        }

        ParseBuffer::clear();

        print("undefined\n\r");
    }
};
