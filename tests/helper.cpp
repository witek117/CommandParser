#include "helper.hpp"

namespace helper {
    std::vector<std::string> split(std::string data, std::string delimiter) {
        size_t                   pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string              token;
        std::vector<std::string> res;

        while ((pos_end = data.find(delimiter, pos_start)) != std::string::npos) {
            token     = data.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(data.substr(pos_start));
        return res;
    }

    std::string trim(std::string data) {
        const char* notData = " \n\r\t";
        auto        start   = data.find_first_not_of(notData);
        auto        stop    = data.find_last_not_of(notData);

        if (start != std::string::npos && stop != std::string::npos) {
            return data.substr(start, stop - start + 1);
        }
        return "";
    }

    std::vector<std::string> remove_blank(std::vector<std::string> data) {
        std::vector<std::string> ret;

        for (auto d : data) {
            d = trim(d);

            if (d.size()) {
                ret.push_back(d);
            }
        }
        return ret;
    }
} // namespace helper
