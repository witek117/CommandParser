#include <CommandSet.hpp>
#include <Command.hpp>
#include <PrintManager.hpp>
#include <ParseBuffer.hpp>
#include <helper.hpp>

#include <gmock/gmock.h>
#include <vector>

static helper::PrintManagerMock mockPrint;

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
    EXPECT_TRUE(commandSet.checkName("new_cmdd") == ItemBase::Match::NO);
    EXPECT_TRUE(commandSet.checkName("test") == ItemBase::Match::NO);
}

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

Command command3(
    "command3",
    "desc",
    +[](CommandBase& cmd) {
        (void)cmd;
    });

Command command4(
    "test1",
    "desc",
    +[](CommandBase& cmd) {
        (void)cmd;
    });

TEST(COMMANDSET, parse) {
    CommandSet<2> commandSet("commandSet", "description", {&command1, &command2});

    uint8_t parseDepth = 0;

    auto retVal = commandSet.parse(&mockPrint, "commandSet", parseDepth);

    EXPECT_FALSE(retVal);
    EXPECT_EQ(parseDepth, 1);

    parseDepth   = 0;
    auto retVal2 = commandSet.parse(&mockPrint, "commandSet command1", parseDepth);
    EXPECT_EQ(parseDepth, 2);
    EXPECT_TRUE(retVal2);

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

TEST(COMMANDSET, printHints) {
    mockPrint.clear();

    CommandSet<3> commandSet("commandSet", "description", {&command1, &command2, &command4});
    ParseBuffer   buffer;

    uint8_t depth = 0;

    buffer.push("command1 1 2 3", 14);

    commandSet.printHints(mockPrint, buffer, depth);

    buffer.clear();
    buffer.push("te", 2);

    commandSet.printHints(mockPrint, buffer, depth);
    EXPECT_TRUE(0 == std::memcmp(buffer.get(), command4.getName(), command4.getNameLen()))
        << "\"" << buffer.get() << "\" is not \"" << command4.getName() << "\"";

    buffer.clear();
    buffer.push("com", 3);
    commandSet.printHints(mockPrint, buffer, depth);

    auto splitted = helper::remove_blank(helper::split(mockPrint.get(), "\r\n"));

    for (auto s : splitted) {
        EXPECT_TRUE(s.find("com") != std::string::npos);
    }
}
