#include <Command.hpp>
#include <helper.hpp>

#include <gmock/gmock.h>
#include <vector>

static char  buffer[20] = {0};
static int   int0       = 0;
static float float0     = 0.0f;
static char  char0      = '0';

static helper::PrintManagerMock mockPrint;

void oneInt(CommandBase& cmd, int data) {
    (void)cmd;
    int0 = data;
}

TEST(COMMAND, commandInt) {
    Command myInt("myInt", "desc", oneInt);
    uint8_t parseDepth = 0;

    myInt.parse(&mockPrint, (char*)"myInt 5", parseDepth);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(parseDepth, 1);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myInt.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "i");
    EXPECT_EQ(len, 1);
}

void intFloat(CommandBase& cmd, int data0, float data1) {
    (void)cmd;
    int0   = data0;
    float0 = data1;
}

TEST(COMMAND, commandIntFloat) {
    Command myInt("myInt", "desc", intFloat);

    uint8_t parseDepth = 0;

    myInt.parse(&mockPrint, (char*)"myInt 5 2.54", parseDepth);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);
    EXPECT_EQ(parseDepth, 1);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myInt.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "if");
    EXPECT_EQ(len, 2);
}

void intFloatChar(CommandBase& cmd, int data0, float data1, char data2) {
    (void)cmd;
    int0   = data0;
    float0 = data1;
    char0  = data2;
}

TEST(COMMAND, commandIntFloatChar) {
    Command myInt("myInt", "desc", intFloatChar);

    uint8_t parseDepth = 0;

    myInt.parse(&mockPrint, (char*)"myInt 5 2.54 i", parseDepth);

    EXPECT_EQ(int0, 5);
    EXPECT_EQ(float0, 2.54f);
    EXPECT_EQ(char0, 'i');
    EXPECT_EQ(parseDepth, 1);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myInt.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "ifc");
    EXPECT_EQ(len, 3);
}

TEST(COMMAND, commandIntFloatCharMissingArg) {
    Command myInt("myInt", "desc", intFloatChar);

    uint8_t parseDepth = 0;

    myInt.parse(&mockPrint, (char*)"myInt 6 2.57", parseDepth);

    EXPECT_EQ(int0, 6);
    EXPECT_EQ(float0, 2.57f);
    EXPECT_EQ(char0, '\0');
    EXPECT_EQ(parseDepth, 1);
}

void doubleCallbakc(CommandBase& cmd, double d0, double d1, double d2) {
    (void)cmd;
    d0 = d1 = d2;
}

TEST(COMMAND, commandDouble) {
    Command doubleCommand("myInt", "desc", doubleCallbakc);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = doubleCommand.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "ddd");
    EXPECT_EQ(len, 3);
}

void myCommandCallback(CommandBase& cmd, const char* data) {
    (void)cmd;
    strcpy(buffer, data);
}

TEST(COMMAND, commandChar) {
    memset(buffer, 0, sizeof(buffer));
    Command myCommand("myCommand", "desc", myCommandCallback);
    uint8_t parseDepth = 0;
    myCommand.parse(&mockPrint, (char*)"myCommand command", parseDepth);
    EXPECT_STREQ(buffer, "command");
    EXPECT_EQ(parseDepth, 1);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myCommand.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "c*");
    EXPECT_EQ(len, 2);
}

void voidCallback(CommandBase& cmd) {
    (void)cmd;
    int0 = 10;
}

TEST(COMMAND, commandVoid) {
    int0 = 0;
    Command myCommand("myCommand", "desc", voidCallback);
    uint8_t parseDepth = 0;
    myCommand.parse(&mockPrint, (char*)"myCommand", parseDepth);
    EXPECT_EQ(int0, 10);
    EXPECT_EQ(parseDepth, 1);

    memset(buffer, 0, sizeof(buffer));
    uint8_t len = myCommand.getValuesInfo(buffer);
    EXPECT_STREQ(buffer, "v");
    EXPECT_EQ(len, 1);
}

TEST(COMMAND, getInfo) {
    int0 = 0;
    Command                  myCommand("myCommand", "desc", voidCallback);
    helper::PrintManagerMock mockPrint;

    myCommand.getInfo(mockPrint);

    EXPECT_STREQ(mockPrint.get().c_str(), "myCommand\tdesc\n\r");
}

TEST(COMMAND, commandIntFloatCharMissing) {
    Command myInt("missing", "desc", intFloatChar);

    uint8_t parseDepth = 0;

    myInt.parse(&mockPrint, (char*)"missing", parseDepth);

    EXPECT_EQ(int0, 0);
    EXPECT_EQ(float0, 0.0f);
    EXPECT_EQ(char0, '\0');
    EXPECT_EQ(parseDepth, 1);
}

bool gatArgumentCountCalled = false;
void gatArgumentCount(CommandBase& cmd, int i, float j, char k) {
    (void)i;
    (void)j;
    (void)k;
    EXPECT_EQ(cmd.getArgsCount(), 3);
    gatArgumentCountCalled = true;
}

TEST(COMMAND, gatArgumentCount) {
    int0 = 0;
    Command myCommand("myCommand", "desc", gatArgumentCount);
    uint8_t parseDepth     = 0;
    gatArgumentCountCalled = false;
    myCommand.parse(&mockPrint, (char*)"myCommand 7 3.43 hello", parseDepth);

    EXPECT_TRUE(gatArgumentCountCalled);
}
