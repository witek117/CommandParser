#pragma once

#include <cstdint>
#include <cstddef>
class PrintManager {
    void parse_value(uint32_t value, std::size_t buff_index_end = BUFF_SIZE - 1);

    void print_buffer();

    void add_minus();

  protected:
    static const std::size_t BUFF_SIZE       = 15;
    std::size_t              i               = 0;
    char                     buff[BUFF_SIZE] = {0};

    virtual void print_data(const char* s, std::size_t len) = 0;

  public:
    inline void print(char s) {
        print_data(&s, 1);
    }

    inline void print(const char* s, std::size_t len) {
        print_data(s, len);
    }

    std::size_t print(const char s[]);

    void print(uint32_t value);

    void print(uint16_t value);

    void print(int32_t value);

    void print(int16_t value);

    void print(float value);
};
