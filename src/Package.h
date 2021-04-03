#pragma once


class Package {
    uint8_t length = 0;
    const uint8_t startWord = 0xc8;

    uint8_t rxBufferLen = 0;
    uint8_t crcIndex = 0;
    uint16_t receivedCrc = 0;
    uint8_t txPackageLength = 0;

    enum class State : uint8_t {
        START = 0,
        GET_LENGTH = 1,
        GET_DATA = 2,
        GET_CRC = 3,
    };

    State state = State::START;
public:
    uint8_t rxBuffer[100] = {0};
    uint8_t txBuffer[100] = {0};

    uint8_t getLength() {
        return length;
    }

    static uint16_t updateCRC(uint8_t byte, uint16_t crc) {
        // CRC-CCITT without LUT
        byte ^= (uint8_t)(crc & 0x00ffu);
        byte ^= (uint8_t)(byte << 4u);
        return ((((uint16_t)byte << 8u) | (uint8_t)(crc >> 8u)) ^ (uint8_t)(byte >> 4u) ^ ((uint16_t)byte << 3u));
    }

    virtual void useValidData() = 0;

    bool putChar(uint8_t c) {
        switch (state) {
            case State::START:
                if (c == startWord) {
                    state = State::GET_LENGTH;
                    rxBufferLen = 0;
                }
                break;
            case State::GET_LENGTH:
                length = c;
                state = State::GET_DATA;
                rxBufferLen = 0;
                break;
            case State::GET_DATA:
                rxBuffer[rxBufferLen++] = c;
                if (rxBufferLen == length) {
                    state = State::GET_CRC;
                    crcIndex = 0;
                }
                break;
            case State::GET_CRC:
                if (crcIndex == 0) {
                    receivedCrc = c << 8u;
                    crcIndex = 1;
                } else if (crcIndex == 1) {
                    receivedCrc |= c;

                    uint16_t myCrc = 0xffff;
                    for (uint8_t i = 0; i < length; i++) {
                        myCrc = updateCRC(rxBuffer[i], myCrc);
                    }
                    if (myCrc == receivedCrc) {
                        useValidData();
                    }
                    state = State::START;
                }
                break;
            default:
                length = 0;
        }

        return true;
    }

    uint8_t createPackage(char* data, uint8_t length) {
        txPackageLength = 0;
        uint8_t offset = 0;
        txBuffer[0] = startWord;
        txBuffer[1] = length;
        offset = 2;
        memcpy(txBuffer + offset, data,  length);

        uint16_t crc = 0xFFFF;
        for(uint8_t i = offset; i < length + offset; i++) {
            crc = updateCRC(txBuffer[i], crc);
        }
        offset += length;
        txBuffer[offset] = crc >> 8u;
        txBuffer[offset + 1] = crc & 0xffu;
        txPackageLength = offset + 2;
        return txPackageLength;
    }

};