#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>

class RawTransmitCommand {
public:
    enum class newLineAtIndex : uint8_t {
        ZERO = (1u << 7u),
        ONE = (1u << 6u),
        TWO = (1u << 5u),
        THREE = (1u << 4u),
    };
protected:
    uint8_t dataBuffer[32] = {0};
    uint8_t *valueBeginIndex = nullptr;
    uint8_t index = 0;
public:
    constexpr static char commandEnd = '\n';
protected:
    inline void constructorForTwoByte() {
        dataBuffer[index++] = ' ';
        valueBeginIndex = dataBuffer + index;
        index += 3;
        dataBuffer[index++] = '\n';
    }

    inline void constructorForFourByte() {
        dataBuffer[index++] = ' ';
        valueBeginIndex = dataBuffer + index;
        index += 5;
        dataBuffer[index++] = '\n';
    }

    inline void constructorForOneByte() {
        dataBuffer[index++] = ' ';
        valueBeginIndex = dataBuffer + index;
        index += 2;
        dataBuffer[index++] = commandEnd;
    }

    inline void setValueForFourByte(uint8_t* data) {
        uint8_t dataInfo = 0;

        if (data[0] == commandEnd) {
            dataInfo = static_cast<uint8_t>(newLineAtIndex::ZERO);
            data[0] = ' ';
        }

        if (data[1] == commandEnd) {
            dataInfo |= static_cast<uint8_t>(newLineAtIndex::ONE);
            data[1] = ' ';
        }

        if (data[2] == commandEnd) {
            dataInfo |= static_cast<uint8_t>(newLineAtIndex::TWO);
            data[2] = ' ';
        }

        if (data[3] == commandEnd) {
            dataInfo |= static_cast<uint8_t>(newLineAtIndex::THREE);
            data[3] = ' ';
        }

        *valueBeginIndex = dataInfo;
        memcpy(valueBeginIndex + 1, reinterpret_cast<const void *>(data), 4);
    }

    inline void setValueForTwoByte(uint8_t* data) {
        uint8_t dataInfo = 0;

        if (data[0] == commandEnd) {
            dataInfo = static_cast<uint8_t>(newLineAtIndex::ZERO);
            data[0] = ' ';
        }

        if (data[1] == commandEnd) {
            dataInfo |= static_cast<uint8_t>(newLineAtIndex::ONE);
            data[1] = ' ';
        }
        *valueBeginIndex = dataInfo;
        memcpy(valueBeginIndex + 1, reinterpret_cast<const void *>(data), 2);
    }

    inline void setValueForOneByte(uint8_t data) {
        uint8_t dataInfo = 0;

        if (data == commandEnd) {
            dataInfo = static_cast<uint8_t>(newLineAtIndex::ZERO);
            *(valueBeginIndex + 1) = ' ';
        } else {
            *(valueBeginIndex + 1) = data;
        }
        *valueBeginIndex = dataInfo;
    }


    RawTransmitCommand (const char * commandName) {
        index = strlen(commandName);
        memcpy(reinterpret_cast<void *>(dataBuffer), commandName, index);
        valueBeginIndex = dataBuffer + index;
    }
public:
    const uint8_t* getCommandData(uint8_t *commandLength) {
        *commandLength = index;
        return dataBuffer;
    }
};

class RawVoidTransmitCommand : public RawTransmitCommand {
public:
    RawVoidTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
        dataBuffer[index++] = commandEnd;
        dataBuffer[index++] = '\0';
    }
};

template<typename T>
class RawTemplateTransmitCommand : public RawTransmitCommand {
public:
    RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
//        std::cout << "Data buffer " << dataBuffer << std::endl;
    }

    void setValue(T value) {
        (void) value;
    }
};

template<> RawTemplateTransmitCommand<uint32_t>::RawTemplateTransmitCommand(const char *commandName);
template<> void RawTemplateTransmitCommand<uint32_t>::setValue(uint32_t value);
template<> RawTemplateTransmitCommand<int32_t>::RawTemplateTransmitCommand(const char *commandName);
template<> void RawTemplateTransmitCommand<int32_t>::setValue(int32_t value);
template<> RawTemplateTransmitCommand<uint16_t>::RawTemplateTransmitCommand(const char *commandName);
template<> void RawTemplateTransmitCommand<uint16_t>::setValue(uint16_t value);
template<> RawTemplateTransmitCommand<int16_t>::RawTemplateTransmitCommand(const char *commandName);
template<> void RawTemplateTransmitCommand<int16_t>::setValue(int16_t value);
template<> RawTemplateTransmitCommand<uint8_t>::RawTemplateTransmitCommand(const char *commandName);
template<> void RawTemplateTransmitCommand<uint8_t>::setValue(uint8_t value);
template<> RawTemplateTransmitCommand<int8_t>::RawTemplateTransmitCommand(const char *commandName);
template<> void RawTemplateTransmitCommand<int8_t>::setValue(int8_t value);
template<> RawTemplateTransmitCommand<char>::RawTemplateTransmitCommand(const char *commandName);
template<> void RawTemplateTransmitCommand<char>::setValue(char value);