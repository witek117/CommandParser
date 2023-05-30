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

    EXPECT_TRUE(commandSet.check_name("new") == ItemBase::Match::PART);
    EXPECT_TRUE(commandSet.check_name("new_cmd") == ItemBase::Match::ALL);
    EXPECT_TRUE(commandSet.check_name("new_cmdd") == ItemBase::Match::NO);
    EXPECT_TRUE(commandSet.check_name("test") == ItemBase::Match::NO);
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

TEST(COMMANDSET, print_hints) {
    mockPrint.clear();

    CommandSet<3> commandSet("commandSet", "description", {&command1, &command2, &command4});
    ParseBuffer   buffer;

    uint8_t depth = 0;

    buffer.push("command1 1 2 3", 14);

    commandSet.print_hints(mockPrint, buffer, depth);

    buffer.clear();
    buffer.push("te", 2);

    commandSet.print_hints(mockPrint, buffer, depth);
    EXPECT_TRUE(0 == std::memcmp(buffer.get(), command4.get_name(), command4.get_name_len()))
        << "\"" << buffer.get() << "\" is not \"" << command4.get_name() << "\"";

    buffer.clear();
    buffer.push("com", 3);
    commandSet.print_hints(mockPrint, buffer, depth);

    auto splitted = helper::remove_blank(helper::split(mockPrint.get(), "\r\n"));

    for (auto s : splitted) {
        EXPECT_TRUE(s.find("com") != std::string::npos);
    }
}

TEST(COMMANDSET, printHintsDeep) {
    mockPrint.clear();

    CommandSet<2> commandSet1("commandSet1", "description 1", {&command1, &command2});
    CommandSet<2> commandSet2("commandSet2", "description 2", {&command3, &command4});
    CommandSet<3> commandSet3("commandSet3", "description 3", {&commandSet1, &commandSet2, &command1});

    ParseBuffer buffer;

    uint8_t depth = 0;
    buffer.push("com", 3);
    commandSet3.print_hints(mockPrint, buffer, depth);

    auto splitted1 = helper::remove_blank(helper::split(mockPrint.get(), "\n\r"));
    EXPECT_EQ(splitted1.size(), 3);

    const char* expected1[3] = {"commandSet1\tdescription 1", "commandSet2\tdescription 2", "command1\tdesc"};

    for (int i = 0; i < splitted1.size(); i++) {
        EXPECT_STREQ(splitted1.at(i).c_str(), expected1[i]);
    }

    buffer.clear();
    mockPrint.clear();
    buffer.push("commandSet1", 11);
    commandSet3.print_hints(mockPrint, buffer, depth);

    auto splitted2 = helper::remove_blank(helper::split(mockPrint.get(), "\n\r"));
    EXPECT_EQ(splitted2.size(), 2);

    const char* expected2[2] = {"command1\tdesc", "command2\tdesc"};

    for (int i = 0; i < splitted2.size(); i++) {
        EXPECT_STREQ(splitted2.at(i).c_str(), expected2[i]);
    }

    buffer.clear();
    mockPrint.clear();
    buffer.push("command1", 8);

    commandSet3.print_hints(mockPrint, buffer, depth);

    auto splitted3 = helper::remove_blank(helper::split(mockPrint.get(), "\n\r"));
    EXPECT_EQ(splitted3.size(), 1);

    const char* expected3 = {"command1\tdesc"};

    EXPECT_STREQ(splitted3.at(0).c_str(), expected3);
}
