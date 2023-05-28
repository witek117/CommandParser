#pragma once

class ParseBuffer {
    char   buffer[100]    = {0};
    size_t received_bytes = 0;
    size_t readIndex      = 0;

  public:
    ParseBuffer() {
    }
    ~ParseBuffer() {
    }

    void push(const char data) {
        if (data == ' ' && buffer[received_bytes - 1] == ' ') {
            return;
        }
        if (received_bytes < sizeof(buffer)) {
            buffer[received_bytes++] = data;
        }
    }

    void push(const char* data, uint8_t length) {
        // TODO checking length
        std::memcpy(&buffer[received_bytes], data, length);
        received_bytes += length;
    }

    void push_at_read_index(const char* data, uint8_t length) {
        std::memcpy(&buffer[readIndex], data, length);
        received_bytes = readIndex + length;
    }

    void clear() {
        received_bytes = 0;
        readIndex      = 0;
    }

    void pop() {
        if (received_bytes > 0) {
            received_bytes--;
        }
    }

    int size() {
        return received_bytes;
    }

    const char* get() {
        return buffer + readIndex;
    }

    void increaseReadIndex(size_t index) {
        readIndex += index;
    }
};
