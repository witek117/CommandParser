#pragma once

#include "PrintManager.hpp"
#include "ParseBuffer.hpp"
#include <cstdint>
#include <cstring>

class ItemBase {
  public:
    enum class Match { NO, ALL, PART };

  protected:
    const char*   name;
    std::size_t   name_len;
    const char*   description;
    std::size_t   description_len;
    PrintManager* printer;

  public:
    ItemBase(const char* name, const char* description) :
        name(name), name_len(std::strlen(name)), description(description), description_len(strlen(description)) {
    }

    const char* get_name() const;

    const char* get_description() const;

    void get_info(PrintManager& print);

    PrintManager* get_printer() const;

    static const char* get_arg(const char* data);

    std::size_t get_name_len() const;

    std::size_t get_description_len() const;

    virtual bool parse(PrintManager* print, const char* data, std::size_t& depth);

    virtual int print_hints(PrintManager& print, ParseBuffer& buffer, std::size_t& depth);

    Match check_name(const char* data) const;
};
