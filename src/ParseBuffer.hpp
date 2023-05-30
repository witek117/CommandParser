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

    void push_at(const char* data, size_t length, size_t index) {
        std::memcpy(&buffer[index], data, length);
        received_bytes = index + length;
    }

    void terminate() {
        if (buffer[received_bytes] != '\0') {
            push('\0');
        }
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

    size_t size() const {
        return received_bytes;
    }

    const char* get() const {
        return buffer;
    }

    const char* get(size_t index) const {
        return &buffer[index];
    }

    size_t get_read_index() const {
        return readIndex;
    }

    void increase_read_index(size_t index) {
        readIndex += index;
    }
};
