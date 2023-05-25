#include "CommandSet.hpp"

uint8_t CommandSetBase::getInfo(char* buffer, size_t bufferLength) {
    (void)buffer;
    (void)bufferLength;
    return 0;
}

bool CommandSetBase::parse(PrintManager* print, const char* data, size_t dataLen, uint8_t& parseDepth) {
    printer = print;
    if (checkName(data) == ItemBase::Match::ALL) {
        parseDepth++;
        uint8_t temporaryParseDepth = parseDepth;
        data += dataLen;
        uint8_t     len = 0;
        const char* ptr = getNextArg(data, len);
        if (len == 0 || ptr == nullptr) {
            return false;
        }
        for (int i = 0; i < getCount(); i++) {
            if (getItem(i)->parse(print, ptr, len, temporaryParseDepth)) {
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
