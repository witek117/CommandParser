#pragma once

#include "ItemBase.hpp"
#include "PrintManager.hpp"
#include "ParseBuffer.hpp"
#include "CommandSet.hpp"
#include <array>

class ShellBase : public PrintManager, public ParseBuffer {
  public:
    struct Config {
        typedef bool (*write_function_t)(const char* data, std::size_t len);
        typedef const char* (*read_function_t)(std::size_t& len);

        read_function_t  read_function;
        write_function_t write_function;
    };

  private:
    enum class READING_STATE : std::size_t {
        CLEAR,
        ADD_DATA,
        SKIPPING,
    };
    READING_STATE state             = READING_STATE::CLEAR;
    std::size_t   skipping_index    = 0;
    std::size_t   read_buffer_len   = 0;
    std::size_t   read_buffer_index = 0;
    const char*   read_buffer       = nullptr;

    Config config;

  public:
    virtual void print_hints() = 0;

    virtual void parse() = 0;

    ShellBase(Config config) : config(config) {
    }

    void init() {
    }

    void print_data(const char* s, std::size_t len) override;

    std::size_t available();

    char read();

    bool run();
};

template<int count>
class Shell : public ShellBase {
    CommandSet<count> command_set;

  public:
    Shell(Config config, std::array<ItemBase*, count> commands) : ShellBase(config), command_set(commands) {
    }

    void print_hints() {
        ParseBuffer::terminate();
        std::size_t temporaryparse_depth = 0;
        PrintManager::print("\n\r");

        command_set.print_hints(*this, *this, temporaryparse_depth);

        ParseBuffer::reset_read();
        PrintManager::print(ParseBuffer::get());

        return;
    }

    void parse() {
        ParseBuffer::terminate();
        PrintManager::print("\n\r");
        std::size_t temporaryparse_depth = 0;
        if (command_set.parse(this, ParseBuffer::get(), temporaryparse_depth)) {
            ParseBuffer::clear();
            return;
        }

        ParseBuffer::clear();
        PrintManager::print("undefined\n\r");
    }
};
