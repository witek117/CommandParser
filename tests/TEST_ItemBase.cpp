#include <ItemBase.hpp>
#include <gmock/gmock.h>
#include <vector>

class PrintManagerMock : public PrintManager {
    virtual void print_data(const char* s, std::size_t len) {
        (void)s;
        (void)len;
    }
};

static PrintManagerMock mock_print;

TEST(ITEMBASE, check_name) {
    ItemBase item("new_item", "description");

    EXPECT_TRUE(item.check_name("new") == ItemBase::Match::PART);
    EXPECT_TRUE(item.check_name("new_item") == ItemBase::Match::ALL);
    EXPECT_TRUE(item.check_name("test") == ItemBase::Match::NO);
}

TEST(ITEMBASE, get_arg) {
    ItemBase item("new_item", "description");

    const char* buff1 = "  test1  ";
    EXPECT_EQ(item.get_arg(buff1), &buff1[2]);

    const char* buff2 = "";
    EXPECT_EQ(item.get_arg(buff2), nullptr);

    const char* buff3 = "test3";
    EXPECT_EQ(item.get_arg(buff3), buff3);
}

TEST(ITEMBASE, parse) {
    ItemBase    item("new_item", "description");
    std::size_t parse_depth = 0;

    EXPECT_FALSE(item.parse(&mock_print, "test", parse_depth));
}
