#pragma once

#include "PrintManager.hpp"
#include "ParseBuffer.hpp"
#include <cstdint>
#include <cstring>

class ItemBase {
  protected:
    const char*   name;
    size_t        nameLen;
    const char*   description;
    size_t        descriptionLen;
    PrintManager* printer;

  public:
    PrintManager* get_printer() const {
        return printer;
    }

    static const char* get_arg(const char* data);

    static const char* get_arg(const char* data, uint8_t& argSize);

    ItemBase(const char* name, const char* description) :
        name(name), nameLen(std::strlen(name)), description(description), descriptionLen(strlen(description)) {
    }

    const char* get_name() const;
    const char* get_description() const {
        return description;
    }

    size_t get_name_len() const;

    void get_info(PrintManager& print) {
        print.print(get_name());
        print.print('\t');
        print.print(get_description());
        print.print("\n\r");
    }

    virtual bool parse(PrintManager* print, const char* data, uint8_t& parseDepth) {
        printer = print;
        (void)data;
        (void)parseDepth;
        return false;
    }

    virtual int print_hints(PrintManager& print, ParseBuffer& buffer, uint8_t& depth) {
        get_info(print);
        (void)buffer;
        (void)depth;
        return 1;
    }

    enum class Match { NO, ALL, PART };

    Match check_name(const char* data) const;
};
