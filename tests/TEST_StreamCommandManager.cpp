#include "Stream.h"
#include "CommandManager.hpp"
#include <gmock/gmock.h>

static Stream stream;

char data_table[3][100];

void callback1(const char *data) {
    static int data_len = 0;
    strcpy(data_table[data_len], data);
    data_len++;
}

using ::testing::StrictMock;

TEST (COMMAND_MANAGER, multiCommands) {
    Command jeden("jeden", "desc", callback1);
    Command dwa("dwa", "desc", callback1);

    CommandManager<2> command_manager(stream);
    command_manager.addCommand(&jeden);
    command_manager.addCommand(&dwa);

    command_manager.init();

    const char *data = "jeden sdfg\ndwa 2345\n";

    for (size_t i = 0; i < strlen(data); i++) {
        stream.pushToRXBuffer(data[i]);
    }

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }


    EXPECT_EQ(std::string("sdfg"), std::string(data_table[0]));
    EXPECT_EQ(std::string("2345"), std::string(data_table[1]));
    EXPECT_EQ(stream.getTxBuffer(), std::string(""));
}

float ff1, ff2;

void twoFloats(const char *data) {
    auto[f1, f2] = parser::get<float, float>(data);
    ff1 = f1;
    ff2 = f2;
}

TEST(COMMAND_MANAGER, twoFloats) {
    Command floats("floats", twoFloats);

    CommandManager<1> command_manager(stream);
    command_manager.addCommand(&floats);

    command_manager.init();

    const char *data = "floats 2.456 3.654\n";

    for (size_t i = 0; i < strlen(data); i++) {
        stream.pushToRXBuffer(data[i]);
    }

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }

    EXPECT_EQ(ff1, (float) (2.456));
    EXPECT_EQ(ff2, (float) (3.654));
    EXPECT_EQ(stream.getTxBuffer(), std::string(""));
}

int functionNUmber = 0;

void question1(const char *data) {
    (void) data;
    functionNUmber = 1;
}

void question2(const char *data) {
    (void) data;
    functionNUmber = 2;
}

void question3(const char *data) {
    (void) data;
    functionNUmber = 3;
}

void question4(const char *data) {
    (void) data;
    functionNUmber = 4;
}

TEST(COMMAND_MANAGER, question) {
    Command q1("t?", question1);
    Command q2("n?", question2);
    Command q3("t", question3);
    Command q4("n", question4);

    CommandManager<4> command_manager(stream);
    command_manager.addCommand(&q1);
    command_manager.addCommand(&q2);
    command_manager.addCommand(&q3);
    command_manager.addCommand(&q4);

    command_manager.init();

    const char *data1 = "t 1\n";
    for (size_t i = 0; i < strlen(data1); i++) {
        stream.pushToRXBuffer(data1[i]);
    }

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }
    EXPECT_EQ(functionNUmber, 3);

    const char *data2 = "t?\n";
    for (size_t i = 0; i < strlen(data2); i++) {
        stream.pushToRXBuffer(data2[i]);
    }

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }
    EXPECT_EQ(functionNUmber, 1);
}

TEST(COMMAND_MANAGER, undefined) {
    CommandManager<1> command_manager(stream);

    command_manager.init();
    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }

    EXPECT_EQ(stream.getTxBuffer(), std::string(""));

    Command q4("n", question4);
    command_manager.addCommand(&q4);

    stream.flush();
    const char *data1 = "t 1\n";
    for (size_t i = 0; i < strlen(data1); i++) {
        stream.pushToRXBuffer(data1[i]);
    }

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }
    EXPECT_EQ(stream.getTxBuffer(), std::string("undefined\n"));
}

TEST(COMMAND_MANAGER, print) {
    CommandManager<1> command_manager(stream);
    command_manager.init();

    command_manager.print((uint16_t) 123);
    EXPECT_EQ(stream.getTxBuffer(), std::string("123"));

    command_manager.print((uint32_t) 123);
    EXPECT_EQ(stream.getTxBuffer(), std::string("123"));

    command_manager.print((int16_t) 123);
    EXPECT_EQ(stream.getTxBuffer(), std::string("123"));

    command_manager.print((float) 123);
    EXPECT_EQ(stream.getTxBuffer(), std::string("123.00"));

    command_manager.print("123");
    EXPECT_EQ(stream.getTxBuffer(), std::string("123"));

    command_manager.print("123", 3);
    EXPECT_EQ(stream.getTxBuffer(), std::string("123"));

    command_manager.print("123", 2);
    EXPECT_EQ(stream.getTxBuffer(), std::string("12"));
}

void info0Callback() {}

void info1Callback() {}

void info2Callback() {}

TEST(COMMAND_MANAGER, getInfoVoid) {
    CommandManager<3> command_manager(stream);
    command_manager.init();

    Command info0("info0", info0Callback);
    Command info1("info1", info1Callback);
    Command info2("info2", info2Callback);

    command_manager.addCommand(&info0);
    command_manager.addCommand(&info1);
    command_manager.addCommand(&info2);

    std::string compareString;

    compareString += "info0\tv\tf\n";
    compareString += "info1\tv\tf\n";
    compareString += "info2\tv\tf\n";

    command_manager.getInfo();
    EXPECT_TRUE(stream.getTxBuffer() == compareString);
}

void intCallback(int data) { (void) data; }

void floatCallback(float data) { (void) data; }

void doubleCallback(double data) { (void) data; }

void charCallback(char data) { (void) data; }

void charPointerCallback(const char *data) { (void) data; }

TEST(COMMAND_MANAGER, getInfoDifferentValues) {
    CommandManager<5> command_manager(stream);
    command_manager.init();

    char names[5][10] = {"info0", "info1", "info2", "info3", "info4"};
    char valuesInfo[5][3] = {"i", "f", "d", "c", "c*"};
    char returnValues[5][3] = {"t", "f", "t", "f", "f"};

    Command info0(names[0], intCallback, true);
    Command info1(names[1], floatCallback, false);
    Command info2(names[2], doubleCallback, true);
    Command info3(names[3], charCallback);
    Command info4(names[4], charPointerCallback);

    command_manager.addCommand(&info0);
    command_manager.addCommand(&info1);
    command_manager.addCommand(&info2);
    command_manager.addCommand(&info3);
    command_manager.addCommand(&info4);

    std::string compareString;

    for (int i = 0; i < 5; i++) {
        compareString += names[i];
        compareString += '\t';
        compareString += valuesInfo[i];
        compareString += '\t';
        compareString += returnValues[i];
        compareString += '\n';
    }
    command_manager.getInfo();

    std::string originalString = stream.getTxBuffer();

    EXPECT_EQ(originalString.size(), compareString.size());

    for (int i = 0; i < originalString.size(); i++) {
        EXPECT_EQ(originalString[i], compareString[i]) << (int) i << std::endl;
    }

    const char *data = "?\n";
    for (size_t i = 0; i < strlen(data); i++) {
        stream.pushToRXBuffer(data[i]);
    }

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }
    EXPECT_TRUE(stream.getTxBuffer() == compareString);
}
