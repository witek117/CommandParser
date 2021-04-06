#pragma once

#include "cyclicBuffer.hpp"
#include <functional>
#include <utility>
#include "Command.h"
#include "Package.h"



class PrintManager {
protected:
    char buff[10] = {0};
    virtual void printData(const char *s, uint8_t length) = 0;
public:
    inline void print(const char *s, uint8_t length) {
        printData(s,length);
    }

    size_t print(const char s[]) {
        size_t length = strlen(s);
        printData(s, length);
        return length;
    }

    void print(uint16_t value) {
        printData(buff, sprintf(buff, "%d", value));
    }

    void print(uint32_t value) {
        printData(buff, sprintf(buff, "%lu", value));
    }

    void print(int16_t value) {
        printData(buff, sprintf(buff, "%d", value));
    }

    void print(float value) {
        printData(buff, sprintf(buff, "%.2f", value));
    }

    static const char endChar = '\n';
};

class ReadManager {
public:
    CyclicBuffer_data<uint8_t , 50> bufferRx;
    uint8_t commandsInBuffer = 0;

    void putChar(char c) {
        bufferRx.append(c);
        if (c == PrintManager::endChar) {
            commandsInBuffer++;
        }
    }
};

template <int size>
class CommandManager : public PrintManager {
public:
    void(*printFunction)(uint8_t) = nullptr;
    ReadManager reader;
protected:
    void(*enableInterrupts)() = nullptr;
    void(*disableInterrupts)() = nullptr;

    static const uint8_t maxCommandsCount = size;
    Command *commands[maxCommandsCount] = {0};

    uint8_t commandTitleLen = 0;
    uint8_t commandsCount = 0;
public:
    explicit CommandManager(void(*enableInterrupts)(), void(*disableInterrupts)(), void(*printFunction_m)(uint8_t) ) :
            printFunction(printFunction_m), enableInterrupts(enableInterrupts), disableInterrupts(disableInterrupts) {
    }

    inline void printData(const char *s, uint8_t length) override {
        for(uint8_t i = 0; i < length; i++) {
            printFunction(s[i]);
        }
    }

    void init() {}

    bool run() {
        disableInterrupts();
        uint8_t commandsInFifoLocal = reader.commandsInBuffer;
        reader.commandsInBuffer = 0;
        enableInterrupts();

        while (commandsInFifoLocal--) {
            commandTitleLen = 0;
            char* cmdBuffer = copyFromFifoToBuffer();
            parse(cmdBuffer);
        }
        return true;
    }

    void parse(char* data) {
        for (uint8_t i = 0; i < 20; i ++) {
            if (data[i] == ' ' || data[i] == '\0') {
                data[i] = '\0';
                commandTitleLen = i;
                break;
            }
        }

        for(uint8_t i = 0; i < commandsCount; i++) {
            if(commands[i]->parse(data, commandTitleLen)) {
                return;
            }
        }
        print("undefined\n");
    }

    char *copyFromFifoToBuffer() {
        static std::array<char, 100> cmd_buffer;
        auto it = cmd_buffer.begin();
        disableInterrupts();
        while (reader.bufferRx.getSize() != 0 && it != cmd_buffer.end()) {
            *it = reader.bufferRx.get();
            if(*it == endChar) {
                *it = '\0';
                break;
            }
            it++;
        }
        enableInterrupts();
        return cmd_buffer.data();
    }

    void addCommand(Command* command) {
        if (commandsCount < maxCommandsCount) {
            commands[commandsCount] = command;
            commandsCount++;
        }
    }
};

template <int size>
class PackageAndCommandManager : public Package, public CommandManager<size> {
public:
    PackageAndCommandManager(void(*enableInterrupts)(), void(*disableInterrupts)(), void(*printFunction_m)(uint8_t)) :
        CommandManager<size>(enableInterrupts, disableInterrupts, printFunction_m) {
    }

    void useValidData() override {
        for (uint8_t i = 0 ; i < length; i++) {
            CommandManager<size>::reader.putChar(packageRxBuffer[i]);
        }
    }

    void printData(const char *s, uint8_t length) override {
        uint8_t packetLength = createPackage((char*)s, length);

        for(uint8_t i = 0; i < packetLength; i++) {
            CommandManager<size>::printFunction((uint8_t)packageTxBuffer[i]);
        }
    }
};

