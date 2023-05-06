#include "CommandBase.hpp"
#include <cstring>

uint8_t CommandBase::getInfo(char* buffer, size_t bufferLength) {
    uint8_t offset = 0;

    if ((offset + name_len) < bufferLength) {
        std::memcpy(&buffer[offset], name, name_len);
        offset += name_len;
    } else {
        std::memcpy(&buffer[offset], name, (bufferLength - offset - 1));
        return bufferLength - 1;
    }

    if ((offset + 1) < bufferLength) {
        buffer[offset++] = '\t';
    } else {
        return offset;
    }

    if ((offset + 4) < bufferLength) {
        offset += getValuesInfo(buffer + offset);
    } else {
        return offset;
    }

    if ((offset + 1) < bufferLength) {
        buffer[offset++] = '\t';
    } else {
        return offset;
    }

    if ((offset + 1) < bufferLength) {
        buffer[offset++] = shouldReturnValue ? 't' : 'f';
    } else {
        return offset;
    }

    if ((offset + 1) < bufferLength) {
        buffer[offset++] = '\t';
    } else {
        return offset;
    }

    if ((offset + description_len) < bufferLength) {
        std::memcpy(&buffer[offset], description, description_len);
        offset += description_len;
    } else {
        std::memcpy(&buffer[offset], description, (bufferLength - offset - 1));
        return bufferLength - 1;
    }
    return offset;
}

bool CommandBase::parse(const char* data, size_t dataLen, uint8_t& parseDepth) {
    if (match(data, dataLen, true)) {
        parseDepth++;
        argsBegin = data + name_len;
        callback_handler(argsBegin + 1);
        return true;
    }
    return false;
}
