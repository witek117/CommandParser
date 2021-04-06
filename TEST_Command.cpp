#include <Command.h>
#include <gmock/gmock.h>
#include <vector>

static int int0 = 0;
static float float0 = 0.0f;
static char char0 = '0';
void oneInt(int data) {
    int0 = data;
}

TEST(COMMAND, commandInt) {
    Command_T1<int> myInt("myInt", oneInt);

    myInt.parse((char*)"myInt 5", 5);

    EXPECT_EQ(int0, 5);
}

void intFloat(int data0, float data1) {
    int0 = data0;
    float0 = data1;
}

TEST(COMMAND, commandIntFloat) {
    Command_T2<int, float> myInt("myInt", intFloat);

    myInt.parse((char*)"myInt 5 2.54", 5);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);
}

void intFloatChar(int data0, float data1, char data2) {
    int0 = data0;
    float0 = data1;
    char0 = data2;
}

TEST(COMMAND, commandIntFloatChar) {
    Command_T3<int, float, char> myInt("myInt", intFloatChar);

    myInt.parse((char*)"myInt 5 2.54 i", 5);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);
    EXPECT_EQ(char0, 'i');
}

char buffer[20] = {0};
void myCommandCallback(const char* data) {
    strcpy(buffer, data);
}

TEST(COMMAND, commandChar) {
    memset(buffer, 0, sizeof(buffer));
    Command myCommand("myCommand", myCommandCallback);
    myCommand.parse((char*)"myCommand command", 9);
    EXPECT_STREQ(buffer, "command");
}


void voidCallback() {
    int0 = 10;
}

TEST(COMMAND, commandVoid) {
    int0 = 0;
    Command_Void myCommand("myCommand", voidCallback);
    myCommand.parse((char*)"myCommand", 9);
    EXPECT_EQ(int0, 10);
}
