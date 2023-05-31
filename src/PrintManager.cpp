#include "PrintManager.hpp"
#include <cmath>
#include <cstring>

std::size_t PrintManager::print(const char s[]) {
    std::size_t len = std::strlen(s);
    print_data(s, len);
    return len;
}

void PrintManager::parse_value(uint32_t value, std::size_t buff_index_end) {
    i = buff_index_end;
    if (value == 0) {
        buff[i] = '0';
    }

    for (; i > 0; i--) {
        uint16_t modulo = value % 10;
        buff[i]         = modulo + '0';

        value = value / 10;
        if (value == 0) {
            break;
        }
    }
}

void PrintManager::print_buffer() {
    print_data(buff + i, BUFF_SIZE - i);
}

void PrintManager::add_minus() {
    buff[--i] = '-';
}

void PrintManager::print(uint32_t value) {
    parse_value(value);
    print_buffer();
}

void PrintManager::print(uint16_t value) {
    parse_value((uint32_t)value);
    print_buffer();
}

void PrintManager::print(int32_t value) {
    parse_value((uint32_t)std::abs(value));
    if (value < 0) {
        add_minus();
    }
    print_buffer();
}

void PrintManager::print(int16_t value) {
    print((int32_t)value);
}

void PrintManager::print(float value) {
    memset(buff, 0, BUFF_SIZE);
    float        tempValue = std::abs(value);
    unsigned int valueInt  = static_cast<unsigned int>(tempValue + 0.01);
    parse_value(valueInt, BUFF_SIZE - 4);

    tempValue -= valueInt;
    tempValue *= 100.0f;

    valueInt = static_cast<unsigned int>(tempValue + 0.5);

    if (valueInt > 99) {
        valueInt = 0;
    }

    buff[BUFF_SIZE - 3] = '.';
    buff[BUFF_SIZE - 2] = (valueInt / 10) + '0';
    buff[BUFF_SIZE - 1] = (valueInt % 10) + '0';

    if (value < 0.0f) {
        add_minus();
    }
    print_buffer();
}
