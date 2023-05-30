#pragma once

#include "ItemBase.hpp"
#include "PrintManager.hpp"
#include "ParseBuffer.hpp"
#include "CommandSet.hpp"
#include <array>

class ShellBase : public PrintManager, public ParseBuffer {
    enum class READING_STATE : uint8_t {
        CLEAR,
        ADD_DATA,
        FOUND_COMMAND,
        SKIPPING,
    };

  public:
    struct Config {
        typedef bool (*write_function_t)(const char* data, size_t length);
        typedef const char* (*read_function_t)(size_t& length);

        read_function_t  readFunction;
        write_function_t writeFunction;
    };

    virtual void print_hints() = 0;

    virtual void parse() = 0;

    READING_STATE state            = READING_STATE::CLEAR;
    uint8_t       skipping_index   = 0;
    size_t        readBufferLength = 0;
    size_t        readBufferIndex  = 0;
    const char*   readBuffer       = nullptr;

    Config config;

    ShellBase(Config config) : config(config) {
    }

    void init() {
    }

    inline void print_data(const char* s, uint8_t length) override {
        config.writeFunction(s, length);
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
                        ParseBuffer::push('\0');
                        print_hints();
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
                        parse();
                        state = READING_STATE::CLEAR;
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
};

template<int count>
class Shell : public ShellBase {
    CommandSet<count> commandSet;

  public:
    Shell(Config config, std::array<ItemBase*, count> commands) : ShellBase(config), commandSet(commands) {
    }

    void print_hints() {
        uint8_t temporaryParseDepth = 0;
        if (commandSet.print_hints(*this, *this, temporaryParseDepth)) {
            return;
        }

        return;
    }

    void parse() {
        PrintManager::print("\n\r");
        uint8_t temporaryParseDepth = 0;
        if (commandSet.parse(this, ParseBuffer::get(), temporaryParseDepth)) {
            return;
        }

        ParseBuffer::clear();
        PrintManager::print("undefined\n\r");
    }
};
