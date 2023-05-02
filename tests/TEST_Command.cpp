#include <Command.hpp>

#include <gmock/gmock.h>
#include <vector>

static char buffer[20] = {0};
static int int0 = 0;
static float float0 = 0.0f;
static char char0 = '0';

void oneInt(int data) {
    int0 = data;
}

TEST(COMMAND, commandInt) {
    Command myInt("myInt", "desc", oneInt);

    myInt.parse((char *) "myInt 5", 5);

    EXPECT_EQ(int0, 5);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myInt.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "i");
    EXPECT_EQ(len, 1);
}

void intFloat(int data0, float data1) {
    int0 = data0;
    float0 = data1;
}

TEST(COMMAND, commandIntFloat) {
    Command myInt("myInt", "desc", intFloat);

    myInt.parse((char *) "myInt 5 2.54", 5);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myInt.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "if");
    EXPECT_EQ(len, 2);
}

void intFloatChar(int data0, float data1, char data2) {
    int0 = data0;
    float0 = data1;
    char0 = data2;
}

TEST(COMMAND, commandIntFloatChar) {
    Command myInt("myInt", "desc", intFloatChar);

    myInt.parse((char *) "myInt 5 2.54 i", 5);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);
    EXPECT_EQ(char0, 'i');

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myInt.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "ifc");
    EXPECT_EQ(len, 3);
}

void doubleCallbakc(double d0, double d1, double d2) {
    d0 = d1 = d2;
}

TEST(COMMAND, commandDouble) {
    Command doubleCommand("myInt", "desc", doubleCallbakc);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = doubleCommand.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "ddd");
    EXPECT_EQ(len, 3);
}

void myCommandCallback(const char *data) {
    strcpy(buffer, data);
}

TEST(COMMAND, commandChar) {
    memset(buffer, 0, sizeof(buffer));
    Command myCommand("myCommand", "desc", myCommandCallback);
    myCommand.parse((char *) "myCommand command", 9);
    EXPECT_STREQ(buffer, "command");

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myCommand.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "c*");
    EXPECT_EQ(len, 2);
}

void voidCallback() {
    int0 = 10;
}

TEST(COMMAND, commandVoid) {
    int0 = 0;
    Command myCommand("myCommand", "desc", voidCallback);
    myCommand.parse((char *) "myCommand", 9);
    EXPECT_EQ(int0, 10);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myCommand.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "v");
    EXPECT_EQ(len, 1);
}

TEST(COMMAND, getInfo) {
    int0 = 0;
    Command myCommand("myCommand", "desc", voidCallback);

    memset(buffer, 0, sizeof(buffer));
    myCommand.getInfo(buffer, sizeof(buffer));
    // EXPECT_STREQ(buffer, "myCommand\tv\tf");
}