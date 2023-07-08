#pragma once

#include "ItemBase.hpp"
#include "PrintManager.hpp"
#include "ParseBuffer.hpp"
#include "CommandSet.hpp"
#include <array>

class ShellBase : public PrintManager, public ParseBuffer {
  public:
    struct backend_t {
        typedef bool (*write_function_t)(const char* data, std::size_t len);
        typedef const char* (*read_function_t)(std::size_t& len);

        read_function_t  p_read;
        write_function_t p_write;
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

    backend_t backend;

  public:
    virtual int print_hints_cmd(PrintManager& print, ParseBuffer& buffer, std::size_t& depth) = 0;

    virtual bool parse_cmd(PrintManager* print, const char* data, std::size_t& depth) = 0;

    void print_hints();

    void parse();

    ShellBase(backend_t backend) : backend(backend) {
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
    Shell(backend_t backend, std::array<ItemBase*, count> commands) : ShellBase(backend), command_set(commands) {
    }

    int print_hints_cmd(PrintManager& print, ParseBuffer& buffer, std::size_t& depth) override {
        return command_set.print_hints(print, buffer, depth);
    }

    bool parse_cmd(PrintManager* print, const char* data, std::size_t& depth) override {
        return command_set.parse(print, data, depth);
    }
};
