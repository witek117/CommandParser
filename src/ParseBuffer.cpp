#include "ParseBuffer.hpp"
#include <cstring>

void ParseBuffer::push(const char data) {
    if ((data == ' ') && (buffer[received_bytes - 1] == ' ')) {
        return;
    }
    if (received_bytes < sizeof(buffer)) {
        buffer[received_bytes] = data;
    }
    received_bytes++;
}

void ParseBuffer::push(const char* data, std::size_t len) {
    // TODO checking len
    std::memcpy(&buffer[received_bytes], data, len);
    received_bytes += len;
}

void ParseBuffer::push_at(const char* data, std::size_t len, std::size_t index) {
    std::memcpy(&buffer[index], data, len);
    received_bytes = index + len;
}

void ParseBuffer::terminate() {
    if (buffer[received_bytes] != '\0') {
        buffer[received_bytes] = '\0';
    }
}

void ParseBuffer::clear() {
    received_bytes = 0;
    read_index     = 0;
}

void ParseBuffer::pop() {
    read_index = 0;
    if (received_bytes > 0) {
        received_bytes--;
    }
}

std::size_t ParseBuffer::size() const {
    return received_bytes;
}

const char* ParseBuffer::get() const {
    return buffer;
}

const char* ParseBuffer::get(std::size_t index) const {
    return &buffer[index];
}

std::size_t ParseBuffer::get_read_index() const {
    return read_index;
}

void ParseBuffer::increase_read_index(std::size_t index) {
    read_index += index;
}
