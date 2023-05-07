#include "PrintManager.h"
#include <string>
#include <gmock/gmock.h>

class Print : public PrintManager {
    std::string dataFromApp = "";

  public:
    std::string getDataFromApp() {
        return dataFromApp;
    }

    void flush() {
        dataFromApp.clear();
    }


    void printData(const char* s, uint8_t length) override {
        for (int i = 0; i < length; i++) {
            dataFromApp += s[i];
        }
    }
};

TEST(PrintManager, print_uint32_t) {
    Print printer;
    printer.flush();

    printer.print((uint32_t)123456);
    EXPECT_EQ(printer.getDataFromApp(), std::string("123456"));
    printer.flush();

    printer.print((uint32_t)0);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0"));
    printer.flush();

    printer.print((uint32_t)std::numeric_limits<uint32_t>::max());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<uint32_t>::max()));
    printer.flush();

    printer.print((uint32_t)std::numeric_limits<uint32_t>::min());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<uint32_t>::min()));
    printer.flush();

    // should false
    printer.print((uint32_t)std::numeric_limits<uint32_t>::min());
    EXPECT_FALSE(printer.getDataFromApp() == std::to_string(std::numeric_limits<uint32_t>::max()));
    printer.flush();
}

TEST(PrintManager, print_uint16_t) {
    Print printer;
    printer.flush();

    printer.print((uint16_t)12345);
    EXPECT_EQ(printer.getDataFromApp(), std::string("12345"));
    printer.flush();

    printer.print((uint16_t)0);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0"));
    printer.flush();

    printer.print((uint16_t)std::numeric_limits<uint16_t>::max());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<uint16_t>::max()));
    printer.flush();

    printer.print((uint16_t)std::numeric_limits<uint16_t>::min());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<uint16_t>::min()));
    printer.flush();

    // should false
    printer.print((uint16_t)std::numeric_limits<uint16_t>::min());
    EXPECT_FALSE(printer.getDataFromApp() == std::to_string(std::numeric_limits<uint16_t>::max()));
    printer.flush();
}

TEST(PrintManager, print_uint8_t) {
    Print printer;
    printer.flush();

    printer.print((uint8_t)123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("123"));
    printer.flush();

    printer.print((uint8_t)0);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0"));
    printer.flush();

    printer.print((uint8_t)std::numeric_limits<uint8_t>::max());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<uint8_t>::max()));
    printer.flush();

    printer.print((uint8_t)std::numeric_limits<uint8_t>::min());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<uint8_t>::min()));
    printer.flush();

    // should false
    printer.print((uint8_t)std::numeric_limits<uint8_t>::min());
    EXPECT_FALSE(printer.getDataFromApp() == std::to_string(std::numeric_limits<uint8_t>::max()));
    printer.flush();
}

TEST(PrintManager, print_int32_t) {
    Print printer;
    printer.flush();

    printer.print((int32_t)-123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("-123"));
    printer.flush();

    printer.print((int32_t)123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("123"));
    printer.flush();

    printer.print((int32_t)0);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0"));
    printer.flush();

    printer.print((int32_t)std::numeric_limits<int32_t>::max());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int32_t>::max()));
    printer.flush();

    printer.print((int32_t)std::numeric_limits<int32_t>::min());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int32_t>::min()));
    printer.flush();

    // should false
    printer.print((int32_t)std::numeric_limits<int32_t>::min());
    EXPECT_FALSE(printer.getDataFromApp() == std::to_string(std::numeric_limits<int32_t>::max()));
    printer.flush();
}

TEST(PrintManager, print_int16_t) {
    Print printer;
    printer.flush();

    printer.print((int16_t)-123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("-123"));
    printer.flush();

    printer.print((int16_t)123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("123"));
    printer.flush();

    printer.print((int16_t)0);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0"));
    printer.flush();

    printer.print((int16_t)std::numeric_limits<int16_t>::max());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int16_t>::max()));
    printer.flush();

    printer.print((int16_t)std::numeric_limits<int16_t>::min());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int16_t>::min()));
    printer.flush();

    // should false
    printer.print((int16_t)std::numeric_limits<int16_t>::min());
    EXPECT_FALSE(printer.getDataFromApp() == std::to_string(std::numeric_limits<int16_t>::max()));
    printer.flush();
}

TEST(PrintManager, print_int8_t) {
    Print printer;
    printer.flush();

    printer.print((int8_t)-123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("-123"));
    printer.flush();

    printer.print((int8_t)123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("123"));
    printer.flush();

    printer.print((int8_t)0);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0"));
    printer.flush();

    printer.print((int8_t)std::numeric_limits<int8_t>::max());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int8_t>::max()));
    printer.flush();

    printer.print((int8_t)std::numeric_limits<int8_t>::min());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int8_t>::min()));
    printer.flush();

    // should false
    printer.print((int8_t)std::numeric_limits<int8_t>::min());
    EXPECT_FALSE(printer.getDataFromApp() == std::to_string(std::numeric_limits<int8_t>::max()));
    printer.flush();
}

TEST(PrintManager, print_int) {
    Print printer;
    printer.flush();

    printer.print((int)-123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("-123"));
    printer.flush();

    printer.print((int)123);
    EXPECT_EQ(printer.getDataFromApp(), std::string("123"));
    printer.flush();

    printer.print((int)0);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0"));
    printer.flush();

    printer.print((int)std::numeric_limits<int>::max());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int>::max()));
    printer.flush();

    printer.print((int)std::numeric_limits<int>::min());
    EXPECT_EQ(printer.getDataFromApp(), std::to_string(std::numeric_limits<int>::min()));
    printer.flush();

    // should false
    printer.print((int)std::numeric_limits<int>::min());
    EXPECT_FALSE(printer.getDataFromApp() == std::to_string(std::numeric_limits<int>::max()));
    printer.flush();
}

TEST(PrintManager, print_float) {
    Print printer;
    printer.flush();

    printer.print((float)-123.0f);
    EXPECT_EQ(printer.getDataFromApp(), std::string("-123.00"));
    printer.flush();

    printer.print((float)123.0f);
    EXPECT_EQ(printer.getDataFromApp(), std::string("123.00"));
    printer.flush();

    printer.print((float)0.0f);
    EXPECT_EQ(printer.getDataFromApp(), std::string("0.00"));
    printer.flush();

    char  buffer[40]   = {0};
    float testingFloat = -0.01f;
    for (uint32_t i = 0; i < 300; i++) {
        memset(buffer, 0, 40);
        testingFloat += 0.01f;
        std::sprintf(buffer, "%.2f", testingFloat);
        printer.print(testingFloat);
        EXPECT_EQ(printer.getDataFromApp(), std::string(buffer));
        printer.flush();
    }

    printer.print(-4324.54f);
    EXPECT_EQ(printer.getDataFromApp(), std::string("-4324.54"));
    printer.flush();

    printer.print(4324.54f);
    EXPECT_EQ(printer.getDataFromApp(), std::string("4324.54"));
    printer.flush();
}
