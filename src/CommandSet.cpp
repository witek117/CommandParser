#include "CommandSet.hpp"

uint8_t CommandSetBase::getInfo(char* buffer, size_t bufferLength) {
    (void)buffer;
    (void)bufferLength;
    return 0;
}

bool CommandSetBase::parse(const char* data, size_t dataLen, uint8_t& parseDepth) {
    if (checkName(data, dataLen, true)) {
        parseDepth++;
        uint8_t temporaryParseDepth = parseDepth;
        data += dataLen;
        uint8_t     len = 0;
        const char* ptr = getNextArg(data, len);
        if (len == 0 || ptr == nullptr) {
            return false;
        }
        for (int i = 0; i < getCount(); i++) {
            if (getCommand(i)->parse(ptr, len, temporaryParseDepth)) {
                parseDepth = temporaryParseDepth;
                return true;
            }
            if (temporaryParseDepth != parseDepth) {
                parseDepth = temporaryParseDepth;
                return false;
            }
        }
    }
    return false;
}
