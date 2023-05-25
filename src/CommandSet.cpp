#include "CommandSet.hpp"

uint8_t CommandSetBase::getInfo(char* buffer, size_t bufferLength) {
    (void)buffer;
    (void)bufferLength;
    return 0;
}

bool CommandSetBase::parse(PrintManager* print, const char* data, uint8_t& parseDepth) {
    printer = print;
    if (checkName(data) == ItemBase::Match::ALL) {
        parseDepth++;
        uint8_t temporaryParseDepth = parseDepth;
        data += ItemBase::getNameLen();
        const char* ptr = getNextArg(data);
        if (ptr == nullptr) {
            return false;
        }
        for (int i = 0; i < getCount(); i++) {
            if (getItem(i)->parse(print, ptr, temporaryParseDepth)) {
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
