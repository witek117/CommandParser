


class ParseBuffer {
    char   buffer[100]    = {0};
    size_t received_bytes = 0;

  public:
    ParseBuffer() {
    }
    ~ParseBuffer() {
    }

    inline void push(const char data) {
        if (data == ' ' && buffer[received_bytes - 1] == ' ') {
            return;
        }
        if (received_bytes < sizeof(buffer)) {
            buffer[received_bytes++] = data;
        }
    }


    inline void push(const char* data, uint8_t length) {
        // TODO checking length
        std::memcpy(&buffer[received_bytes], data, length);
        received_bytes += length;
    }

    void clear() {
        received_bytes = 0;
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
        return buffer;
    }
};
