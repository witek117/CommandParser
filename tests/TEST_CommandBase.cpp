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
    void set_args(const char* data) {
        args = data;
    }

    MockCommandBase(std::size_t args_count) : CommandBase("", "", false, args_count) {
    }

    virtual void callback_handler(const char* data) {
        (void)data;
    }

    virtual std::size_t get_values_info(char* buffer) {
        (void)buffer;
        return 0;
    }
};

TEST(COMMANDBASE, get_args_count) {
    MockCommandBase command(3);

    command.set_args("ab cd ef gh");
    EXPECT_EQ(command.get_args_count(), 4);

    command.set_args("ab cd ef");
    EXPECT_EQ(command.get_args_count(), 3);

    command.set_args("  ab cd ef");
    EXPECT_EQ(command.get_args_count(), 3);
    EXPECT_TRUE(command.check_args_count());

    command.set_args("  ab cd ef  ");
    EXPECT_EQ(command.get_args_count(), 3);

    command.set_args(" a bc def ghij     ");
    EXPECT_EQ(command.get_args_count(), 4);
    EXPECT_FALSE(command.check_args_count());
}
