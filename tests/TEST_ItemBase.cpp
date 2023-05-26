#include <ItemBase.hpp>

#include <gmock/gmock.h>
#include <vector>

class PrintManagerMock : public PrintManager {
    virtual void printData(const char* s, uint8_t length) {
        (void)s;
        (void)length;
    }
};

static PrintManagerMock mockPrint;

TEST(ITEMBASE, checkName) {
    ItemBase item("new_item", "description");

    EXPECT_TRUE(item.checkName("new") == ItemBase::Match::PART);
    EXPECT_TRUE(item.checkName("new_item") == ItemBase::Match::ALL);
    EXPECT_TRUE(item.checkName("test") == ItemBase::Match::NO);
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
    uint8_t  parseDepth = 0;

    EXPECT_FALSE(item.parse(&mockPrint, "test", parseDepth));
}
