#pragma once

#include "CommandBase.hpp"
#include "SimpleStream.h"
#include "PrintManager.h"

class CommandManager : public PrintManager {
    enum class READING_STATE : uint8_t {
        CLEAR,
        ADD_DATA,
        FOUND_COMMAND,
        SKIPPING,
    };

    READING_STATE state           = READING_STATE::CLEAR;
    CommandBase** commands        = nullptr;
    int           size            = 0;
    SimpleStream* stream          = nullptr;
    size_t        commands_count  = 0;
    size_t        received_bytes  = 0;
    uint8_t       skipping_index  = 0;
    char          cmd_buffer[100] = {0};

  public:
    CommandManager(CommandBase** commands, int size, SimpleStream* stream) : commands(commands), size(size), stream(stream) {
    }

    inline void printData(const char* s, uint8_t length) override {
        stream->write((uint8_t*)s, length);
    }

    void getInfo() {
        constexpr size_t infoBufferLength             = 100;
        char             infoBuffer[infoBufferLength] = {0};
        for (uint8_t i = 0; i < commands_count; i++) {
            std::memset(infoBuffer, 0, infoBufferLength);
            uint8_t len       = commands[i]->getInfo(infoBuffer, infoBufferLength);
            infoBuffer[len++] = '\n';
            infoBuffer[len++] = '\r';
            printData(infoBuffer, len);
        }
    }

    void init() {
        stream->init();
        stream->flush();
    }

    inline void add_to_buffer(uint8_t data) {
        if (received_bytes < sizeof(cmd_buffer)) {
            cmd_buffer[received_bytes++] = data;
        } else {
            state = READING_STATE::CLEAR;
        }
    }

    bool run() {
        switch (state) {
            case READING_STATE::CLEAR:
                state          = READING_STATE::ADD_DATA;
                received_bytes = 0;
                break;
            case READING_STATE::ADD_DATA:
                for (auto i = 0; i < stream->available(); i++) {
                    uint8_t byte = stream->read();

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
                if (stream->available() == 0) {
                    break;
                }

                if (skipping_index == 0) {
                    uint8_t byte = stream->read();
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
                    stream->read();
                    skipping_index = 0;
                    state          = READING_STATE::ADD_DATA;
                }
                break;
        }

        return true;
    }

    void printHints(size_t length) {
        uint8_t match_commands = 0;
        uint8_t found_index    = 0;

        for (uint8_t i = 0; i < commands_count; i++) {
            if (std::memcmp(cmd_buffer, commands[i]->getName(), length) == 0) {
                if (match_commands == 0) {
                    print("\r");
                }

                found_index = i;
                if (match_commands != 0) {
                    print("\t");
                }
                print(commands[i]->getName());
                match_commands++;
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
        uint8_t commandTitleLen = 0;
        print("\n\r");

        char* data = cmd_buffer;

        for (uint8_t i = 0; i < 20; i++) {
            if (data[i] == ' ' || data[i] == '\0') {
                data[i]         = '\0';
                commandTitleLen = i;
                break;
            }
        }

        for (uint8_t i = 0; i < commands_count; i++) {
            if (commands[i]->parse(data, commandTitleLen)) {
                return;
            }
        }

        if (strcmp(data, "?") == 0) {
            getInfo();
            return;
        }

        print("undefined\n\r");
    }

    void addCommand(CommandBase* command) {
        if (commands_count < size) {
            commands[commands_count] = command;
            commands_count++;
        }
    }
};
