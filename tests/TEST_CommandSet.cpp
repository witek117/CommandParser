#include <CommandSet.hpp>
#include <Command.hpp>
#include <PrintManager.hpp>
#include <ParseBuffer.hpp>
#include <helper.hpp>
#include <gmock/gmock.h>
#include <vector>

static helper::PrintManagerMock mock_print;

TEST(COMMANDSET, check_name) {
    Command my_command(
        "myCommand",
        "desc",
        +[](CommandBase& cmd) {
            (void)cmd;
        });
    CommandSet<1> command_set("new_cmd", "description", {&my_command});

    EXPECT_TRUE(command_set.check_name("new") == ItemBase::Match::PART);
    EXPECT_TRUE(command_set.check_name("new_cmd") == ItemBase::Match::ALL);
    EXPECT_TRUE(command_set.check_name("new_cmdd") == ItemBase::Match::NO);
    EXPECT_TRUE(command_set.check_name("test") == ItemBase::Match::NO);
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
    CommandSet<2> command_set("command_set", "description", {&command1, &command2});

    std::size_t parse_depth = 0;

    auto retVal = command_set.parse(&mock_print, "command_set", parse_depth);

    EXPECT_FALSE(retVal);
    EXPECT_EQ(parse_depth, 1);

    parse_depth  = 0;
    auto retVal2 = command_set.parse(&mock_print, "command_set command1", parse_depth);
    EXPECT_EQ(parse_depth, 2);
    EXPECT_TRUE(retVal2);

    CommandSet<2> command_set2("command_set2", "description", {&command_set, &command3});

    parse_depth  = 0;
    auto retVal3 = command_set2.parse(&mock_print, "command_set2 command_set command1", parse_depth);
    EXPECT_EQ(parse_depth, 3);
    EXPECT_TRUE(retVal3);

    parse_depth  = 0;
    auto retVal4 = command_set2.parse(&mock_print, "command_set2 command3", parse_depth);
    EXPECT_EQ(parse_depth, 2);
    EXPECT_TRUE(retVal4);

    parse_depth  = 0;
    auto retVal5 = command_set2.parse(&mock_print, "command_set2 command_set command", parse_depth);
    EXPECT_EQ(parse_depth, 2);
    EXPECT_FALSE(retVal5);
}

TEST(COMMANDSET, print_hints) {
    mock_print.clear();

    CommandSet<3> command_set("command_set", "description", {&command1, &command2, &command4});
    ParseBuffer   buffer;

    std::size_t depth = 0;

    buffer.push("command1 1 2 3", 14);

    command_set.print_hints(mock_print, buffer, depth);

    buffer.clear();
    buffer.push("te", 2);

    command_set.print_hints(mock_print, buffer, depth);
    EXPECT_TRUE(0 == std::memcmp(buffer.get(), command4.get_name(), command4.get_name_len()))
        << "\"" << buffer.get() << "\" is not \"" << command4.get_name() << "\"";

    buffer.clear();
    buffer.push("com", 3);
    command_set.print_hints(mock_print, buffer, depth);

    auto splitted = helper::remove_blank(helper::split(mock_print.get(), "\r\n"));

    for (auto s : splitted) {
        EXPECT_TRUE(s.find("com") != std::string::npos);
    }
}

TEST(COMMANDSET, printHintsDeep) {
    mock_print.clear();

    CommandSet<2> command_set1("command_set1", "description 1", {&command1, &command2});
    CommandSet<2> command_set2("command_set2", "description 2", {&command3, &command4});
    CommandSet<3> command_set3("command_set3", "description 3", {&command_set1, &command_set2, &command1});

    ParseBuffer buffer;

    std::size_t depth = 0;
    buffer.push("com", 3);
    buffer.terminate();
    command_set3.print_hints(mock_print, buffer, depth);

    auto splitted1 = helper::remove_blank(helper::split(mock_print.get(), "\n\r"));
    EXPECT_EQ(splitted1.size(), 3);

    const char* expected1[3] = {"command_set1\tdescription 1", "command_set2\tdescription 2", "command1\tdesc"};

    for (std::size_t i = 0; i < splitted1.size(); i++) {
        EXPECT_STREQ(splitted1.at(i).c_str(), expected1[i]);
    }

    buffer.clear();
    mock_print.clear();
    buffer.push("command_set1", 12);
    buffer.terminate();
    command_set3.print_hints(mock_print, buffer, depth);

    auto splitted2 = helper::remove_blank(helper::split(mock_print.get(), "\n\r"));
    EXPECT_EQ(splitted2.size(), 2);

    const char* expected2[2] = {"command1\tdesc", "command2\tdesc"};

    for (std::size_t i = 0; i < splitted2.size(); i++) {
        EXPECT_STREQ(splitted2.at(i).c_str(), expected2[i]);
    }

    buffer.clear();
    mock_print.clear();
    buffer.push("command1", 8);
    buffer.terminate();

    command_set3.print_hints(mock_print, buffer, depth);

    auto splitted3 = helper::remove_blank(helper::split(mock_print.get(), "\n\r"));
    EXPECT_EQ(splitted3.size(), 1);

    const char* expected3 = {"command1\tdesc"};

    EXPECT_STREQ(splitted3.at(0).c_str(), expected3);
}
