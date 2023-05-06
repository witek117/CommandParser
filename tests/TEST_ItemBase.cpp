#include <ItemBase.hpp>

#include <gmock/gmock.h>
#include <vector>


TEST(ITEMBASE, match) {
    ItemBase item("new_item", "description");

    EXPECT_TRUE(item.match("new", 3));
    EXPECT_FALSE(item.match("new", 3, true));
    EXPECT_TRUE(item.match("new_item", 8, true));
}

TEST(ITEMBASE, getLen) {
    ItemBase item("new_item", "description");

    EXPECT_EQ(item.getLen("item"), 4);
    EXPECT_EQ(item.getLen("item "), 4);
}

TEST(ITEMBASE, getNextArg) {
    ItemBase item("new_item", "description");

    uint8_t argLen;

    const char* buff1 = "  test1  ";
    EXPECT_EQ(item.getNextArg(buff1, argLen), &buff1[2]);
    EXPECT_EQ(argLen, 5);

    const char* buff2 = "";
    EXPECT_EQ(item.getNextArg(buff2, argLen), nullptr);
    EXPECT_EQ(argLen, 0);

    const char* buff3 = "test3";
    EXPECT_EQ(item.getNextArg(buff3, argLen), buff3);
    EXPECT_EQ(argLen, 5);
}

TEST(ITEMBASE, parse) {
    ItemBase item("new_item", "description");
    uint8_t parseDepth = 0;

    EXPECT_FALSE(item.parse("test", 0, parseDepth));
}
