#include <CommandSet.hpp>
#include <Command.hpp>

#include <gmock/gmock.h>
#include <vector>

void function() {

}

// TEST(COMMANDSET, match) {
//     Command myCommand("myCommand", "desc", +[](){}, false);
//     CommandSet<1> commandSet("new_item", "description", {&myCommand});

//     EXPECT_TRUE(commandSet.match("new", 3));
//     EXPECT_FALSE(commandSet.match("new", 3, true));
//     EXPECT_TRUE(commandSet.match("new_item", 8, true));
// }

TEST(COMMANDSET, parse) {
    Command command1("command1", "desc", +[](ItemBase* item){(void)item; });
    Command command2("command2", "desc", +[](ItemBase* item){(void)item; });
    CommandSet<2> commandSet("commandSet", "description", {&command1, &command2});

    uint8_t parseDepth = 0;

    auto retVal = commandSet.parse("commandSet", 10, parseDepth);

    EXPECT_FALSE(retVal);
    EXPECT_EQ(parseDepth, 1);

    parseDepth = 0;
    auto retVal2 = commandSet.parse("commandSet command1", 10, parseDepth);
    EXPECT_EQ(parseDepth, 2);
    EXPECT_TRUE(retVal2);
    
    Command command3("command3", "desc", +[](ItemBase* item){(void)item; });
    CommandSet<2> commandSet2("commandSet2", "description", {&commandSet, &command3});

    parseDepth = 0;
    auto retVal3 = commandSet2.parse("commandSet2 commandSet command1", 11, parseDepth);
    EXPECT_EQ(parseDepth, 3);
    EXPECT_TRUE(retVal3);

    parseDepth = 0;
    auto retVal4 = commandSet2.parse("commandSet2 command3", 11, parseDepth);
    EXPECT_EQ(parseDepth, 2);
    EXPECT_TRUE(retVal4);

    parseDepth = 0;
    auto retVal5 = commandSet2.parse("commandSet2 commandSet command", 11, parseDepth);
    EXPECT_EQ(parseDepth, 2);
    EXPECT_FALSE(retVal5);
}
