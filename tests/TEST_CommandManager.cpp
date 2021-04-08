#include <command_manager.h>
#include <Command.h>
#include <gmock/gmock.h>
#include <vector>

using ::testing::StrictMock;

class CommandBaseMock : public Command {
public:
    static int callbackTested;

    static void callback(const char *) {
        callbackTested++;
    }

    explicit CommandBaseMock(const char* name) : Command(name, callback) { }
};
int CommandBaseMock::callbackTested = 0;

static void enable_interrupts() {}
static void disable_interrupts() {}

static std::string printedString;
static void print_function(uint8_t c) {
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

    command_manager.run();

    EXPECT_EQ(printedString, std::string(""));
    EXPECT_EQ(CommandBaseMock::callbackTested, 2);
}

char data_table[3][100];

void callback1(const char* data) {
    static int data_len = 0;
    strcpy(data_table[data_len], data);
    data_len++;
}

TEST (COMMAND_MANAGER, multiCommands) {
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

void twoFloats(const char* data) {
    auto [f1, f2] = parser::get<float, float>(data);
    ff1 = f1;
    ff2 = f2;
}

TEST(COMMAND_MANAGER, twoFloats) {
    printedString = "";
    Command floats("floats", twoFloats);

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

TEST(COMMAND_MANAGER, print) {
    CommandManager<1> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.init();

    printedString = "";
    command_manager.print((uint16_t)123);
    EXPECT_EQ(printedString, std::string("123"));

    printedString = "";
    command_manager.print((uint32_t)123);
    EXPECT_EQ(printedString, std::string("123"));

    printedString = "";
    command_manager.print((int16_t)123);
    EXPECT_EQ(printedString, std::string("123"));

    printedString = "";
    command_manager.print((float)123);
    EXPECT_EQ(printedString, std::string("123.00"));

    printedString = "";
    command_manager.print("123");
    EXPECT_EQ(printedString, std::string("123"));

    printedString = "";
    command_manager.print("123", 3);
    EXPECT_EQ(printedString, std::string("123"));

    printedString = "";
    command_manager.print("123", 2);
    EXPECT_EQ(printedString, std::string("12"));
}

void info0Callback() { }
void info1Callback() { }
void info2Callback() { }

TEST(COMMAND_MANAGER, getInfoVoid) {
    CommandManager<3> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.init();

    Command_Void info0("info0", info0Callback);
    Command_Void info1("info1", info1Callback);
    Command_Void info2("info2", info2Callback);

    command_manager.addCommand(reinterpret_cast<Command *>(&info0));
    command_manager.addCommand(reinterpret_cast<Command *>(&info1));
    command_manager.addCommand(reinterpret_cast<Command *>(&info2));

    std::string compareString;
    printedString = "";

    compareString += "info0\tv\tf\n";
    compareString += "info1\tv\tf\n";
    compareString += "info2\tv\tf\n";

    command_manager.getInfo();
    EXPECT_TRUE(printedString == compareString);
}

void intCallback(int data) { (void)data; }
void floatCallback(float data) { (void)data; }
void doubleCallback(double data) { (void)data; }
void charCallback(char data) { (void)data; }
void charPointerCallback(const char* data) { (void)data; }


TEST(COMMAND_MANAGER, getInfoDifferentValues) {
    CommandManager<5> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.init();

    char names[5][10] = {"info0", "info1", "info2", "info3", "info4"};
    char valuesInfo[5][3] = {"i", "f", "d", "c", "c*"};
    char returnValues[5][3] = {"t", "f", "t", "f", "f"};

    Command_T1<int>     info0(names[0], intCallback, true);
    Command_T1<float>   info1(names[1], floatCallback, false);
    Command_T1<double>  info2(names[2], doubleCallback, true);
    Command_T1<char>    info3(names[3], charCallback);
    Command             info4(names[4], charPointerCallback);

    command_manager.addCommand(reinterpret_cast<Command *>(&info0));
    command_manager.addCommand(reinterpret_cast<Command *>(&info1));
    command_manager.addCommand(reinterpret_cast<Command *>(&info2));
    command_manager.addCommand(reinterpret_cast<Command *>(&info3));
    command_manager.addCommand(reinterpret_cast<Command *>(&info4));

    std::string compareString;
    printedString = "";

    for(int i = 0; i < 5; i++) {
        compareString += names[i];
        compareString += '\t';
        compareString += valuesInfo[i];
        compareString += '\t';
        compareString += returnValues[i];
        compareString += '\n';
    }

    command_manager.getInfo();
    EXPECT_TRUE(printedString == compareString);
}