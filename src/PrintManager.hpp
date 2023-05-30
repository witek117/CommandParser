#pragma once

#include <cstdint>
#include <cstring>
#include <cmath>

class PrintManager {
    void parse_value(uint32_t value, uint8_t buffIndexEnd = BUFF_SIZE - 1);

    void print_buffer();

    void add_minus();

  protected:
    static const uint8_t BUFF_SIZE       = 15;
    uint8_t              i               = 0;
    char                 buff[BUFF_SIZE] = {0};

    virtual void print_data(const char* s, uint8_t length) = 0;

  public:
    inline void print(char s) {
        print_data(&s, 1);
    }

    inline void print(const char* s, uint8_t length) {
        print_data(s, length);
    }

    size_t print(const char s[]);

    void print(uint32_t value);

    void print(uint16_t value);

    void print(int32_t value);

    void print(int16_t value);

    void print(float value);
};
