#include <CommandBase.hpp>

#include <gmock/gmock.h>

TEST(COMMANDBASE, get) {
    auto [f, g, h] = CommandBase::get<int, float, int>("12 5.763 344");
    EXPECT_EQ(f, 12);
    EXPECT_EQ(g, 5.763f);
    EXPECT_EQ(h, 344);

    auto [x1, y1] = CommandBase::get<float, float>("2.345 4.235");
    EXPECT_EQ(x1, 2.345f);
    EXPECT_EQ(y1, 4.235f);

    auto [x2, y2] = CommandBase::get<int, int>("-234 5432");
    EXPECT_EQ(x2, -234);
    EXPECT_EQ(y2, 5432);

    auto [x3] = CommandBase::get<int>("2345");
    EXPECT_EQ(x3, 2345);
}


class MockCommandBase : public CommandBase {
  public:
    void setArgs(const char* args) {
        argsBegin = args;
    }

    MockCommandBase(uint8_t argsCount) : CommandBase("", "", false, argsCount) {
    }

    virtual void callback_handler(const char* data) {
        (void)data;
    }

    virtual uint8_t getValuesInfo(char* buffer) {
        (void)buffer;
        return 0;
    }
};

TEST(COMMANDBASE, getArgCount) {
    MockCommandBase command(3);

    command.setArgs("ab cd ef gh");
    EXPECT_EQ(command.getArgCount(), 4);

    command.setArgs("ab cd ef");
    EXPECT_EQ(command.getArgCount(), 3);

    command.setArgs("  ab cd ef");
    EXPECT_EQ(command.getArgCount(), 3);
    EXPECT_TRUE(command.checkArgsCount());

    command.setArgs("  ab cd ef  ");
    EXPECT_EQ(command.getArgCount(), 3);

    command.setArgs(" a bc def ghij     ");
    EXPECT_EQ(command.getArgCount(), 4);
    EXPECT_FALSE(command.checkArgsCount());
}
