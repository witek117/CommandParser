#include "PrintManager.hpp"

size_t PrintManager::print(const char s[]) {
    size_t length = strlen(s);
    printData(s, length);
    return length;
}

void PrintManager::parseValue(uint32_t value, uint8_t buffIndexEnd) {
    i = buffIndexEnd;
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

void PrintManager::printBuffer() {
    printData(buff + i, BUFF_SIZE - i);
}

void PrintManager::addMinus() {
    buff[--i] = '-';
}

void PrintManager::print(uint32_t value) {
    parseValue(value);
    printBuffer();
}

void PrintManager::print(uint16_t value) {
    parseValue((uint32_t)value);
    printBuffer();
}

void PrintManager::print(int32_t value) {
    parseValue((uint32_t)std::abs(value));
    if (value < 0) {
        addMinus();
    }
    printBuffer();
}

void PrintManager::print(int16_t value) {
    print((int32_t)value);
}

void PrintManager::print(float value) {
    memset(buff, 0, BUFF_SIZE);
    float        tempValue = std::abs(value);
    unsigned int valueInt  = static_cast<unsigned int>(tempValue + 0.01);
    parseValue(valueInt, BUFF_SIZE - 4);

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
        addMinus();
    }
    printBuffer();
}
