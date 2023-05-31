#include "ItemBase.hpp"

const char* ItemBase::get_arg(const char* data) {
    if (data == nullptr) {
        return nullptr;
    }
    const char* ptr = data;
    while (true) {
        if (*ptr == '\0') {
            return nullptr;
        }

        if (*ptr == ' ') {
            ptr++;
        } else {
            break;
        }
    }
    return ptr;
}

const char* ItemBase::get_name() const {
    return name;
}

const char* ItemBase::get_description() const {
    return description;
}
std::size_t ItemBase::get_name_len() const {
    return name_len;
}

std::size_t ItemBase::get_description_len() const {
    return description_len;
}

ItemBase::Match ItemBase::check_name(const char* data) const {
    std::size_t i = 0;
    for (; i < name_len; i++) {
        if (data[i] != name[i]) {
            break;
        }

        if (data[i] == ' ' || data[i] == '\0') {
            break;
        }
    }

    if (i == 0) {
        return Match::NO;
    }

    if (i == name_len) {
        return (data[name_len] != '\0' && data[name_len] != ' ') ? Match::NO : Match::ALL;
    }

    return Match::PART;
}

void ItemBase::get_info(PrintManager& print) {
    print.print(get_name());
    print.print('\t');
    print.print(get_description());
    print.print("\n\r");
}

bool ItemBase::parse(PrintManager* print, const char* data, std::size_t& depth) {
    printer = print;
    (void)data;
    (void)depth;
    return false;
}

int ItemBase::print_hints(PrintManager& print, ParseBuffer& buffer, std::size_t& depth) {
    print.print('\t');
    print.print(get_description());
    print.print("\n\r");
    (void)buffer;
    (void)depth;
    return 1;
}

PrintManager* ItemBase::get_printer() const {
    return printer;
}
