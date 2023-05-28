#include "CommandSet.hpp"

bool CommandSetBase::parse(PrintManager* print, const char* data, uint8_t& parseDepth) {
    printer = print;
    if (getNameLen() == 0 || checkName(data) == ItemBase::Match::ALL) {
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


int CommandSetBase::printHints(PrintManager& print, ParseBuffer& buffer, uint8_t& depth) {
    uint8_t         match_commands = 0;
    uint8_t         found_index    = 0;
    const char*     ptr            = ItemBase::getNextArg(buffer.get());
    ItemBase::Match lastMatch      = ItemBase::Match::NO;

    if (ptr == nullptr) {
        for (uint8_t i = 0; i < getCount(); i++) {
            auto item = getItem(i);
            if (item == nullptr) {
                continue;
            }
            item->getInfo(print);
        }
    }

    for (uint8_t i = 0; i < getCount(); i++) {
        auto item = getItem(i);
        if (item == nullptr) {
            continue;
        }

        lastMatch = item->checkName(ptr);

        if (lastMatch == ItemBase::Match::ALL) {
            depth++;
            buffer.increaseReadIndex(item->getNameLen() + 1);
            item->printHints(print, buffer, depth);
            return 1;
        } else if (lastMatch == ItemBase::Match::PART) {
            match_commands++;
            found_index = i;
        }
    }

    if (match_commands == 1) {
        depth++;
        auto item = getItem(found_index);
        buffer.push_at_read_index(item->getName(), item->getNameLen());
        buffer.push(' ');
        buffer.push('\0');
    }

    if (match_commands > 1) {
        for (uint8_t i = 0; i < getCount(); i++) {
            auto item = getItem(i);
            if (item == nullptr) {
                continue;
            }

            if (item->checkName(ptr) == ItemBase::Match::PART) {
                item->getInfo(print);
            }
        }
    }

    return match_commands;
}
