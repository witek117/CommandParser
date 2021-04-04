#pragma once

#include "cyclicBuffer.hpp"
#include <functional>
#include <utility>
#include "Command.h"
#include "Package.h"

constexpr char endChar = '\n';

class PrintManager {
protected:
    char buff[10] = {0};
public:
    virtual void print(const char *s, uint8_t length) = 0;

protected:
    size_t print_s(const char s[]) {
        size_t length = strlen(s);
        print(s, length);
        return length;
    }

public:
    void print(uint16_t value) {
        print(buff, sprintf(buff, "%d", value));
    }

    void print(uint32_t value) {
        print(buff, sprintf(buff, "%lu", value));
    }

    void print(int16_t value) {

        print(buff, sprintf(buff, "%d", value));
    }

    void print(float value) {
        print(buff, sprintf(buff, "%.2f", value));
    }
};

class ReadManager {
public:
    CyclicBuffer_data<uint8_t , 50> buffer_rx;
    uint8_t commands_in_buffer = 0;

    void putChar(char c) {
        buffer_rx.append(c);
        if (c == endChar) {
            commands_in_buffer++;
        }
    }
};

template <int size>
class CommandManager : public PrintManager {
public:
    void(*print_function)(uint8_t) = nullptr;
    ReadManager reader;
//    ReadManager reader;
protected:
    void(*enable_interrupts)() = nullptr;
    void(*disable_interrupts)() = nullptr;



    static const uint8_t maxCommandsCount = size;
    Command *commands[maxCommandsCount] = {0};

    uint8_t command_title_len = 0;
    uint8_t commandsCount = 0;
public:
    explicit CommandManager(void(*enable_interrupts)(), void(*disable_interrupts)(), void(*print_function_m)(uint8_t) ) :
            enable_interrupts(enable_interrupts), disable_interrupts(disable_interrupts), print_function(print_function_m) {
    }

    inline void print(const char *s, uint8_t length) override {
        for(uint8_t i = 0; i < length; i++) {
            print_function(s[i]);
        }
    }

    inline void print(const char s[]){
        print_s(s);
    }

    void init() {}

    bool run() {
        disable_interrupts();
        uint8_t commands_in_fifo_local = reader.commands_in_buffer;
        reader.commands_in_buffer = 0;
        enable_interrupts();

        while (commands_in_fifo_local--) {
            command_title_len = 0;
            char* cmd_buffer = copy_from_fifo_to_buffer();
            parse(cmd_buffer);
        }
        return true;
    }

    void parse(char* data) {
        for (uint8_t i = 0; i < 20; i ++) {
            if (data[i] == ' ' || data[i] == '\0') {
                data[i] = '\0';
                command_title_len = i;
                break;
            }
        }

        for(uint8_t i = 0; i < commandsCount; i++) {
            if(commands[i]->parse(data, command_title_len)) {
                return;
            }
        }
        print("undefined\n");
    }

    char *copy_from_fifo_to_buffer() {
        static std::array<char, 100> cmd_buffer;
        auto it = cmd_buffer.begin();
        disable_interrupts();
        while (reader.buffer_rx.getSize() != 0 && it != cmd_buffer.end()) {
            *it = reader.buffer_rx.get();
            if(*it == endChar) {
                *it = '\0';
                break;
            }
            it++;
        }
        enable_interrupts();
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
    PackageAndCommandManager(void(*enable_interrupts)(), void(*disable_interrupts)(), void(*print_function_m)(uint8_t)) :
        CommandManager<size>(enable_interrupts, disable_interrupts, print_function_m) {
    }

    void useValidData() override {
        for (uint8_t i = 0 ; i < length; i++) {
            CommandManager<size>::reader.putChar(packageRxBuffer[i]);
        }
    }

    void print(const char *s, uint8_t length) override {
        uint8_t packetLength = createPackage((char*)s, length);

        for(uint8_t i = 0; i < packetLength; i++) {
            CommandManager<size>::print_function((uint8_t)packageTxBuffer[i]);
        }
    }

};

