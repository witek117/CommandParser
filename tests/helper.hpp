#include <vector>
#include <string>
#include <PrintManager.hpp>

namespace helper {
    std::vector<std::string> split(std::string s, std::string delimiter);

    std::string trim(std::string data);

    std::vector<std::string> remove_blank(std::vector<std::string> data);

    class PrintManagerMock : public PrintManager {
      public:
        std::string  received;
        virtual void printData(const char* s, uint8_t length) {
            for (int i = 0; i < length; i++) {
                received.push_back(s[i]);
            }
        }
        std::string& get() {
            return received;
        }

        void clear() {
            received.clear();
        }
    };
} // namespace helper
