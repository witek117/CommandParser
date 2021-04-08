#include <Command.h>
#include <gmock/gmock.h>


//class CommandBaseMock : public Command {
//public:
//    MOCK_METHOD(void, callback, (const char *));
//
//    explicit CommandBaseMock(const char* name) : Command(name, ([this](const char* ch) {this->callback(ch);})) { }
//};

TEST(PARSER, create) {
//    CommandBaseMock start("start");
//    const char* data = "start 12345";
//    EXPECT_CALL(start, callback(data + 6));
//    EXPECT_EQ(start.parse((char*)data, 5), true);
//    EXPECT_CALL(start, callback(::testing::_)).Times(0);
//    EXPECT_EQ(start.parse((char*)"data", 5), false);
}

TEST(PARSER, value_float) {
    const char *data0 = "12 5.763 344";
    auto[f, g, h] = parser::get<int, float, int>(data0);
    EXPECT_EQ(f, 12);
    EXPECT_EQ(g, (float)5.763);
    EXPECT_EQ(h, 344);

    const char* data1 = "2.345 4.235";
    auto [x1, y1] = parser::get<float, float>(data1);
    EXPECT_EQ(x1, (float)2.345);
    EXPECT_EQ(y1, (float)4.235);

    const char* data2 = "-234 5432";
    auto [x2, y2] = parser::get<int, int>(data2);
    EXPECT_EQ(x2, -234);
    EXPECT_EQ(y2, 5432);

    const char* data3 = "2345";
    auto [x3] = parser::get<int>(data3);
    EXPECT_EQ(x3, 2345);
}
