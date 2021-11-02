#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>

class PrintManager {
protected:
    static const uint8_t BUFF_SIZE = 15;
    uint8_t i = 0;
    char buff[BUFF_SIZE] = {0};
    virtual void printData(const char *s, uint8_t length) = 0;

public:
    inline void print(const char *s, uint8_t length) {
        printData(s, length);
    }

    size_t print(const char s[]) {
        size_t length = strlen(s);
        printData(s, length);
        return length;
    }

private:
    void parseValue(uint32_t value, uint8_t buffIndexEnd = BUFF_SIZE - 1) {
        i = buffIndexEnd;
        if (value == 0) {
            buff[i] = '0';
        }

        for (i; i > 0; i--) {
            uint16_t modulo = value % 10;
            buff[i] = modulo + '0';

            value = value / 10;
            if (value == 0) {
                break;
            }
        }
    }

    void printBuffer() {
        printData(buff + i, BUFF_SIZE - i);
    }

    void addMinus() {
        buff[--i] = '-';
    }

public:
    void print(uint32_t value) {
        parseValue(value);
        printBuffer();
    }

    void print(uint16_t value) {
        parseValue((uint32_t) value);
        printBuffer();
    }

    void print(int32_t value) {
        parseValue((uint32_t) std::abs(value));
        if (value < 0) { addMinus(); }
        printBuffer();
    }

    void print(int16_t value) {
        print((int32_t) value);
    }

    void print(float value) {
        memset(buff, 0, BUFF_SIZE);
        float tempValue = std::abs(value);
        unsigned int valueInt = static_cast<unsigned int>(tempValue + 0.01);
        parseValue(valueInt, BUFF_SIZE - 4);

        tempValue -= valueInt;
        tempValue *= 100.0f;

        valueInt = static_cast<unsigned int>(tempValue + 0.5);

        if (valueInt > 99) { valueInt = 0; }

        buff[BUFF_SIZE - 3] = '.';
        buff[BUFF_SIZE - 2] = (valueInt / 10) + '0';
        buff[BUFF_SIZE - 1] = (valueInt % 10) + '0';

        if (value < 0.0f) { addMinus(); }
        printBuffer();
    }

    static const char endChar = '\n';
};

