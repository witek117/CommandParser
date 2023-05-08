#pragma once

#include "CommandBase.hpp"
#include "PrintManager.hpp"

template <int count>
class CommandManager : public PrintManager {
    enum class READING_STATE : uint8_t {
        CLEAR,
        ADD_DATA,
        FOUND_COMMAND,
        SKIPPING,
    };

    READING_STATE state           = READING_STATE::CLEAR;
    size_t        commands_count  = count;
    size_t        received_bytes  = 0;
    uint8_t       skipping_index  = 0;
    char          cmd_buffer[100] = {0};

    typedef bool (*write_function_t)(const char* data, size_t length);
    typedef const char* (*read_function_t)(size_t& length);

    read_function_t readFunction;
    write_function_t writeFunction;

    using CommandContainer = std::array<ItemBase*, count>;
    CommandContainer commands;

  public:
    CommandManager(CommandContainer commands, read_function_t readFunction, write_function_t writeFunction) :
        commands(commands), readFunction(readFunction), writeFunction(writeFunction) {
    }

    inline void printData(const char* s, uint8_t length) override {
        writeFunction(s, length);
    }

    // void getInfo() {
    //     constexpr size_t infoBufferLength             = 100;
    //     char             infoBuffer[infoBufferLength] = {0};
    //     for (uint8_t i = 0; i < commands_count; i++) {
    //         std::memset(infoBuffer, 0, infoBufferLength);
    //         uint8_t len       = commands[i]->getInfo(infoBuffer, infoBufferLength);
    //         infoBuffer[len++] = '\n';
    //         infoBuffer[len++] = '\r';
    //         printData(infoBuffer, len);
    //     }
    // }

    void init() {
    }

    inline void add_to_buffer(uint8_t data) {
        if (received_bytes < sizeof(cmd_buffer)) {
            cmd_buffer[received_bytes++] = data;
        } else {
            state = READING_STATE::CLEAR;
        }
    }

    size_t readBufferLength = 0;
    size_t readBufferIndex = 0;
    const char* readBuffer = nullptr;
    size_t available() {
        if(readBufferIndex == readBufferLength) {
            readBufferIndex = 0;
            readBuffer = readFunction(readBufferLength);
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
                state          = READING_STATE::ADD_DATA;
                received_bytes = 0;
                break;
            case READING_STATE::ADD_DATA:
                avai = available();

                for (auto i = 0; i < avai; i++) {
                    char byte = read();

                    // check tabulator
                    if (byte == '\t') {
                        printHints(received_bytes);
                        continue;
                    }

                    // check backspace
                    if ((byte == 0x7f) && (received_bytes > 0)) {
                        received_bytes--;
                        break;
                    }

                    // check enter
                    if (byte == '\r' || byte == '\n') {
                        if (received_bytes == 0) {
                            continue;
                        }
                        add_to_buffer('\0');
                        state = READING_STATE::FOUND_COMMAND;
                        break;
                    }

                    // check escape
                    if (byte == 0x1b) {
                        skipping_index = 0;
                        state          = READING_STATE::SKIPPING;
                        break;
                    }
                    add_to_buffer(byte);
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
                        add_to_buffer(byte);
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

    void printHints(size_t length) {
        uint8_t match_commands = 0;
        uint8_t found_index    = 0;

        for (uint8_t i = 0; i < commands_count; i++) {
            if (getItem(i)->checkName(cmd_buffer, length, false)) {
                match_commands++;
                found_index = i;
            }
        }

        if (match_commands == 1) {
            auto match_command = commands[found_index];
            received_bytes     = match_command->getNameLen();
            std::memcpy(cmd_buffer, match_command->getName(), received_bytes);
        } else if (match_commands > 1) {
            received_bytes = length;
            print("\n\r");
            cmd_buffer[received_bytes] = 0;
            print(cmd_buffer);
        } else {
            received_bytes = length;
            print("\r");
            cmd_buffer[received_bytes] = 0;
            print(cmd_buffer);
        }
    }

    void parse() {
        // print("\n\r");
        uint8_t commandTitleLen = 0;
        ItemBase::getNextArg(cmd_buffer, commandTitleLen);
        uint8_t temporaryParseDepth = 0;
        for (uint8_t i = 0; i < commands_count; i++) {
            if (getItem(i)->parse(cmd_buffer, commandTitleLen, temporaryParseDepth)) {
                return;
            }
            if (temporaryParseDepth != 0) {
                break;
            }
        }

        print("undefined\n\r");
    }

};
