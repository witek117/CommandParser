#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>

class PrintManager {
    void parseValue(uint32_t value, uint8_t buffIndexEnd = BUFF_SIZE - 1);

    void printBuffer();

    void addMinus();

  protected:
    static const uint8_t BUFF_SIZE       = 15;
    uint8_t              i               = 0;
    char                 buff[BUFF_SIZE] = {0};

    virtual void printData(const char* s, uint8_t length) = 0;

  public:
    inline void print(char s) {
        printData(&s, 1);
    }

    inline void print(const char* s, uint8_t length) {
        printData(s, length);
    }

    size_t print(const char s[]);

    void print(uint32_t value);

    void print(uint16_t value);

    void print(int32_t value);

    void print(int16_t value);

    void print(float value);
};
