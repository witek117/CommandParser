#include <Parser.hpp>
#include <gmock/gmock.h>

TEST(PARSER, value_float) {
    auto d1 = parser::parse<double>("4.5324");
    EXPECT_EQ(d1, 4.5324);

    auto d2 = parser::parse<int>("-1234");
    EXPECT_EQ(d2, -1234);

    auto d3 = parser::parse<uint32_t>("1234");
    EXPECT_EQ(d3, 1234);

    auto d4 = parser::parse<float>("-0.321");
    EXPECT_EQ(d4, -0.321f);

    auto d5 = parser::parse<char>("212");
    EXPECT_EQ(d5, '2');

    auto d6 = parser::parse<const char*>("Hello");
    EXPECT_EQ(d6, "Hello");

    parser::parse<void>("void");
}
