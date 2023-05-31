#include "Shell.hpp"
#include <array>

void ShellBase::print_data(const char* s, std::size_t len) {
    config.write_function(s, len);
}

std::size_t ShellBase::available() {
    if (read_buffer_index == read_buffer_len) {
        read_buffer_index = 0;
        read_buffer       = config.read_function(read_buffer_len);
    }

    return read_buffer_len - read_buffer_index;
}

char ShellBase::read() {
    return read_buffer[read_buffer_index++];
}

bool ShellBase::run() {
    std::size_t avai = 0;

    switch (state) {
        case READING_STATE::CLEAR:
            state = READING_STATE::ADD_DATA;
            ParseBuffer::clear();
            break;
        case READING_STATE::ADD_DATA:
            avai = available();

            for (auto i = 0; i < avai; i++) {
                char byte = read();

                // check tabulator
                if (byte == '\t' || byte == '?') {
                    print_hints();
                    continue;
                }

                // check backspace
                if (byte == 0x7f) {
                    ParseBuffer::pop();
                    break;
                }

                // check enter
                if (byte == '\r' || byte == '\n') {
                    if (ParseBuffer::size() == 0) {
                        continue;
                    }
                    parse();
                    state = READING_STATE::CLEAR;
                    break;
                }

                // check escape
                if (byte == 0x1b) {
                    skipping_index = 0;
                    state          = READING_STATE::SKIPPING;
                    break;
                }

                ParseBuffer::push(byte);
            }
            break;
        case READING_STATE::SKIPPING:
            if (available() == 0) {
                break;
            }

            if (skipping_index == 0) {
                std::size_t byte = read();
                if (byte == 0x5b) {
                    skipping_index++;
                } else if (byte == 0x1b) {
                } else {
                    skipping_index = 0;
                    ParseBuffer::push(byte);
                    state = READING_STATE::ADD_DATA;
                }
            }

            if (skipping_index == 1) {
                read();
                skipping_index = 0;
                state          = READING_STATE::ADD_DATA;
            }
            break;
    }

    return true;
}
