#include "CommandSet.hpp"

bool CommandSetBase::parse(PrintManager* print, const char* data, std::size_t& depth) {
    printer = print;
    if ((get_name_len() == 0) || (check_name(data) == ItemBase::Match::ALL)) {
        depth++;
        std::size_t temporary_depth = depth;
        data += ItemBase::get_name_len();
        const char* ptr = get_arg(data);
        if (ptr == nullptr) {
            return false;
        }
        for (auto i = 0; i < get_count(); i++) {
            if (get_item(i)->parse(print, ptr, temporary_depth)) {
                depth = temporary_depth;
                return true;
            }
            if (temporary_depth != depth) {
                depth = temporary_depth;
                return false;
            }
        }
    }
    return false;
}

int CommandSetBase::print_hints(PrintManager& print, ParseBuffer& buffer, std::size_t& depth) {
    std::size_t     match_commands = 0;
    std::size_t     found_index    = 0;
    const char*     ptr            = ItemBase::get_arg(buffer.get(buffer.get_read_index()));
    ItemBase::Match last_match     = ItemBase::Match::NO;

    if (ptr == nullptr || buffer.size() == 0) {
        if (buffer.get_last() != ' ' && buffer.size()) {
            buffer.push(' ');
            buffer.terminate();
        }

        for (auto i = 0; i < get_count(); i++) {
            auto item = get_item(i);
            if (item == nullptr) {
                continue;
            }
            item->get_info(print);
        }
        depth++;
        return 0;
    }

    for (auto i = 0; i < get_count(); i++) {
        auto item = get_item(i);
        if (item == nullptr) {
            continue;
        }

        last_match = item->check_name(ptr);

        if (last_match == ItemBase::Match::ALL) {
            depth++;
            buffer.increase_read_index(item->get_name_len() + 1);
            return item->print_hints(print, buffer, depth);
        } else if (last_match == ItemBase::Match::PART) {
            match_commands++;
            found_index = i;
        }
    }

    if (match_commands == 1) {
        depth++;
        auto item = get_item(found_index);
        buffer.push_at(item->get_name(), item->get_name_len(), buffer.get_read_index());
        buffer.push(' ');
        buffer.terminate();
    }

    if (match_commands > 1) {
        for (auto i = 0; i < get_count(); i++) {
            auto item = get_item(i);
            if (item == nullptr) {
                continue;
            }

            if (item->check_name(ptr) == ItemBase::Match::PART) {
                item->get_info(print);
            }
        }
    }

    return match_commands;
}
