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
