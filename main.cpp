#include <vector>
#include <gmock/gmock.h>
#include "gtest/gtest.h"
#include <gmock/gmock-spec-builders.h>
#include <Command.h>
#include <command_manager.h>
#include "Package.h"

using ::testing::StrictMock;

class CommandBaseMock : public Command {
public:
    MOCK_METHOD(void, callback, (const char *));

    explicit CommandBaseMock(const char* name) : Command(name, ([this](const char* ch) {this->callback(ch);})) { }
};

TEST(COMMAND, create) {
    CommandBaseMock start("start");
    const char* data = "start 12345";
    EXPECT_CALL(start, callback(data + 6));
    EXPECT_EQ(start.parse((char*)data, 5), true);
    EXPECT_CALL(start, callback(::testing::_)).Times(0);
    EXPECT_EQ(start.parse((char*)"data", 5), false);
}

TEST(COMMAND, value_float) {
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

void enable_interrupts() {}

void disable_interrupts() {}


std::string printedString;
void print_function(uint8_t c) {
    printedString += c;
}

TEST (COMMAND_MANAGER, basic) {
    printedString = "";
    CommandBaseMock jeden("jeden");
    CommandBaseMock dwa("dwa");

    CommandManager<2> command_manager(&enable_interrupts, &disable_interrupts, &print_function);

    command_manager.addCommand(&jeden);
    command_manager.addCommand(&dwa);

    command_manager.init();

    const char * data = "jeden sdfg\ndwa 2345\n";

    for (size_t i =0; i < strlen(data); i++) {
        command_manager.reader.putChar(data[i]);
    }

    EXPECT_CALL(jeden, callback(::testing::_));
    EXPECT_CALL(dwa, callback(::testing::_));

    command_manager.run();

    EXPECT_EQ(printedString, std::string(""));
}

char data_table[3][100];

void callback1(const char* data) {
    static int data_len = 0;
    strcpy(data_table[data_len], data);
    data_len++;
}

TEST (COMMAND_MANAGER, multi_commands) {
    printedString = "";
    Command jeden("jeden", callback1);
    Command dwa("dwa", callback1);

    CommandManager<2> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.addCommand(&jeden);
    command_manager.addCommand(&dwa);

    command_manager.init();

    const char * data = "jeden sdfg\ndwa 2345\n";

    for (size_t i =0; i < strlen(data); i++) {
        command_manager.reader.putChar(data[i]);
    }

    command_manager.run();

    EXPECT_EQ(std::string("sdfg"), std::string(data_table[0]));
    EXPECT_EQ(std::string("2345"), std::string(data_table[1]));
    EXPECT_EQ(printedString, std::string(""));
}

float ff1, ff2;

void two_floats(const char* data) {
    auto [f1, f2] = parser::get<float, float>(data);
    ff1 = f1;
    ff2 = f2;
}

TEST(COMMAND_MANAGER, two_floats) {
    printedString = "";
    Command floats("floats", two_floats);

    CommandManager<1> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.addCommand(&floats);

    command_manager.init();

    const char * data = "floats 2.456 3.654\n";

    for (size_t i =0; i < strlen(data); i++) {
        command_manager.reader.putChar(data[i]);
    }

    command_manager.run();

    EXPECT_EQ(ff1, (float)(2.456));
    EXPECT_EQ(ff2, (float)(3.654));
    EXPECT_EQ(printedString, std::string(""));
}

int functionNUmber = 0;
void question1(const char * data) {
    (void) data;
    functionNUmber = 1;
}

void question2(const char * data) {
    (void) data;
    functionNUmber = 2;
}

void question3(const char * data) {
    (void) data;
    functionNUmber = 3;
}

void question4(const char * data) {
    (void) data;
    functionNUmber = 4;
}

TEST(COMMAND_MANAGER, question) {

    Command q1("t?", question1);
    Command q2("n?", question2);
    Command q3("t", question3);
    Command q4("n", question4);

    CommandManager<4> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.addCommand(&q1);
    command_manager.addCommand(&q2);
    command_manager.addCommand(&q3);
    command_manager.addCommand(&q4);

    command_manager.init();

    const char * data1 = "t 1\n";
    for (size_t i =0; i < strlen(data1); i++) {
        command_manager.reader.putChar(data1[i]);
    }
    command_manager.run();
    EXPECT_EQ(functionNUmber, 3);

    const char * data2 = "t?\n";
    for (size_t i =0; i < strlen(data2); i++) {
        command_manager.reader.putChar(data2[i]);
    }
    command_manager.run();
    EXPECT_EQ(functionNUmber, 1);
}

TEST(COMMAND_MANAGER, undefined) {
    CommandManager<1> command_manager(&enable_interrupts, &disable_interrupts, &print_function);

    command_manager.init();

    printedString = "";
    command_manager.run();

    EXPECT_EQ(printedString, std::string(""));

    Command q4("n", question4);
    command_manager.addCommand(&q4);

    const char * data1 = "t 1\n";
    for (size_t i =0; i < strlen(data1); i++) {
        command_manager.reader.putChar(data1[i]);
    }
    printedString = "";
    command_manager.run();
    EXPECT_EQ(printedString, std::string("undefined\n"));
}

bool gotValidData = false;
class TestPackage : public Package {
    void useValidData() {
        gotValidData = true;
    }
};

TEST(PACKAGE, create_package) {
    TestPackage package;
    const char *data = "this is the test";
    uint8_t dataLength = sizeof(data);
    uint8_t packageLength = package.createPackage(const_cast<char *>(data), dataLength);
    EXPECT_EQ(dataLength + 4, packageLength);

    EXPECT_EQ(0xc8, package.txBuffer[0]);
    EXPECT_EQ(dataLength, package.txBuffer[1]);

    uint16_t crc = 0xFFFF;
    for(int i = 0; i < dataLength; i++) {
        EXPECT_EQ(data[i], package.txBuffer[i + 2]);
        crc = Package::updateCRC(data[i], crc);
    }

    EXPECT_EQ(crc >> 8u, package.txBuffer[packageLength - 2]);
    EXPECT_EQ(crc & 0xffu, package.txBuffer[packageLength - 1]);
}

TEST(PACKAGE, receive_package) {
    gotValidData = false;
    TestPackage package;
    uint8_t myData [] = "this is the test 1234\n";
    uint16_t crc = 0xffff;

    package.putChar(0xc8);
    package.putChar((uint8_t) sizeof(myData));

    for (unsigned char i : myData) {
        package.putChar(i);
        crc = Package::updateCRC(i, crc);
    }
    package.putChar(crc >> 8u);
    package.putChar(crc & 0xffu);

    EXPECT_EQ(gotValidData, true);
}

TEST(PACKAGE, create_receive) {
    gotValidData = false;
    TestPackage package;
    const char *data = "this is the test";
    uint8_t dataLength = sizeof(data);
    uint8_t packageLength = package.createPackage(const_cast<char *>(data), dataLength);

    for (uint8_t i = 0; i < packageLength; i++) {
        package.putChar(package.txBuffer[i]);
    }

    EXPECT_EQ(gotValidData, true);
}
