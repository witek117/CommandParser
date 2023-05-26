#include <CommandSet.hpp>
#include <Command.hpp>
#include <PrintManager.hpp>

#include <gmock/gmock.h>
#include <vector>

void function() {
}

class PrintManagerMock : public PrintManager {
    virtual void printData(const char* s, uint8_t length) {
        (void)s;
        (void)length;
    }
};

static PrintManagerMock mockPrint;

TEST(COMMANDSET, checkName) {
    Command myCommand(
        "myCommand",
        "desc",
        +[](CommandBase& cmd) {
            (void)cmd;
        });
    CommandSet<1> commandSet("new_cmd", "description", {&myCommand});

    EXPECT_TRUE(commandSet.checkName("new") == ItemBase::Match::PART);
    EXPECT_TRUE(commandSet.checkName("new_cmd") == ItemBase::Match::ALL);
    EXPECT_TRUE(commandSet.checkName("test") == ItemBase::Match::NO);
}

TEST(COMMANDSET, parse) {
    Command command1(
        "command1",
        "desc",
        +[](CommandBase& cmd) {
            (void)cmd;
        });
    Command command2(
        "command2",
        "desc",
        +[](CommandBase& cmd) {
            (void)cmd;
        });
    CommandSet<2> commandSet("commandSet", "description", {&command1, &command2});

    uint8_t parseDepth = 0;

    auto retVal = commandSet.parse(&mockPrint, "commandSet", parseDepth);

    EXPECT_FALSE(retVal);
    EXPECT_EQ(parseDepth, 1);

    parseDepth   = 0;
    auto retVal2 = commandSet.parse(&mockPrint, "commandSet command1", parseDepth);
    EXPECT_EQ(parseDepth, 2);
    EXPECT_TRUE(retVal2);

    Command command3(
        "command3",
        "desc",
        +[](CommandBase& cmd) {
            (void)cmd;
        });
    CommandSet<2> commandSet2("commandSet2", "description", {&commandSet, &command3});

    parseDepth   = 0;
    auto retVal3 = commandSet2.parse(&mockPrint, "commandSet2 commandSet command1", parseDepth);
    EXPECT_EQ(parseDepth, 3);
    EXPECT_TRUE(retVal3);

    parseDepth   = 0;
    auto retVal4 = commandSet2.parse(&mockPrint, "commandSet2 command3", parseDepth);
    EXPECT_EQ(parseDepth, 2);
    EXPECT_TRUE(retVal4);

    parseDepth   = 0;
    auto retVal5 = commandSet2.parse(&mockPrint, "commandSet2 commandSet command", parseDepth);
    EXPECT_EQ(parseDepth, 2);
    EXPECT_FALSE(retVal5);
}
