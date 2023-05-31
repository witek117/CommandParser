#pragma once

#include <cstdint>
#include <cstddef>

class ParseBuffer {
    char        buffer[100]    = {0};
    std::size_t received_bytes = 0;
    std::size_t read_index     = 0;

  public:
    ParseBuffer() {
    }
    ~ParseBuffer() {
    }

    void push(const char data);

    void push(const char* data, std::size_t len);

    void push_at(const char* data, std::size_t len, std::size_t index);

    void terminate();

    void clear();

    void pop();

    void reset_read() {
        read_index = 0;
    }

    std::size_t size() const;

    const char* get() const;

    const char* get(std::size_t index) const;

    std::size_t get_read_index() const;

    void increase_read_index(std::size_t index);

    char get_last() const {
        return buffer[received_bytes - 1];
    }
};
