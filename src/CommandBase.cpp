#include "CommandBase.hpp"
#include <cstring>

uint8_t CommandBase::getInfo(char* buffer, size_t bufferLength) {
    uint8_t offset = 0;

    if ((offset + nameLen) < bufferLength) {
        std::memcpy(&buffer[offset], name, nameLen);
        offset += nameLen;
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

    if ((offset + descriptionLen) < bufferLength) {
        std::memcpy(&buffer[offset], description, descriptionLen);
        offset += descriptionLen;
    } else {
        std::memcpy(&buffer[offset], description, (bufferLength - offset - 1));
        return bufferLength - 1;
    }
    return offset;
}

bool CommandBase::parse(PrintManager* print, const char* data, size_t dataLen, uint8_t& parseDepth) {
    printer = print;
    if (checkName(data) == ItemBase::Match::ALL) {
        parseDepth++;
        argsBegin = data + nameLen + 1;
        callback_handler(argsBegin);
        return true;
    }
    return false;
}
