#pragma once

#include "SimpleStream.h"
#include "cyclicBuffer.hpp"
#include <utility>
#include "Command.h"
#include "PrintManager.h"

template<int size>
class CommandManager : public PrintManager {
    enum class READING_STATE : uint8_t {
        CLEAR,
        ADD_DATA,
        FOUND_COMMAND,
    };

    READING_STATE state = READING_STATE::CLEAR;

    SimpleStream &stream;

    static const uint8_t maxCommandsCount = size;
    CommandTemplate *commands[maxCommandsCount] = {0};

    uint8_t commandTitleLen = 0;
    uint8_t commandsCount = 0;
public:
    CommandManager(SimpleStream &stream) : stream(stream) {
    }

    inline void printData(const char *s, uint8_t length) override {
        stream.write((uint8_t *) s, length);
    }

    void getInfo() {
        char infoBuffer[100] = {0};
        for (uint8_t i = 0; i < commandsCount; i++) {
            memset(infoBuffer, 0, sizeof(infoBuffer));
            uint8_t len = commands[i]->getInfo(infoBuffer);
            infoBuffer[len++] = '\n';
            printData(infoBuffer, len);
        }
    }

    void init() {
        stream.flush();
    }

    bool run() {
        static char cmd_buffer[100];
        static char *data;
        size_t length;

        switch (state) {
            case READING_STATE::CLEAR:
                data = cmd_buffer;
                state = READING_STATE::ADD_DATA;
                break;
            case READING_STATE::ADD_DATA:
                length = stream.available();

                for (size_t i = 0; i < length; i++) {
                    *data = stream.read();
                    if (*data == endChar) {
                        *data = '\0';
                        state = READING_STATE::FOUND_COMMAND;
                        break;
                    }
                    data++;
                }
                break;

            case READING_STATE::FOUND_COMMAND:
                parse(cmd_buffer);
                state = READING_STATE::CLEAR;
                break;

        }

        return true;
    }

    void parse(char *data) {
        for (uint8_t i = 0; i < 20; i++) {
            if (data[i] == ' ' || data[i] == '\0') {
                data[i] = '\0';
                commandTitleLen = i;
                break;
            }
        }

        for (uint8_t i = 0; i < commandsCount; i++) {
            if (commands[i]->parse(data, commandTitleLen)) {
                return;
            }
        }

        if (strcmp(data, "?") == 0) {
            getInfo();
            return;
        }

        print("undefined\n");
    }

    void addCommand(CommandTemplate *command) {
        if (commandsCount < maxCommandsCount) {
            commands[commandsCount] = command;
            commandsCount++;
        }
    }
};
