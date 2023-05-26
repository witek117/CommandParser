// #include "Stream.h"
#include "CommandManager.hpp"
#include "Command.hpp"
#include <gmock/gmock.h>

// static Stream stream;

char data_table[3][100];

void callback1(CommandBase& cmd, const char* data) {
    (void)cmd;
    static int data_len = 0;
    strcpy(data_table[data_len], data);
    data_len++;
}

std::string trim(std::string data) {
    const char* notData = " \n\r\t";
    auto        start   = data.find_first_not_of(notData);
    auto        stop    = data.find_last_not_of(notData);

    if (start != std::string::npos && stop != std::string::npos) {
        return data.substr(start, stop - start + 1);
    }
    return "";
}

using ::testing::StrictMock;

std::string transmitted;
std::string received;

const char* readFunction(size_t& length) {
    static std::string myData;
    myData.clear();

    myData.append(transmitted);
    transmitted.clear();

    length = myData.size();
    // std::cout << "myData" << std::endl;
    // std::cout << myData << std::endl;
    return myData.c_str();
}

bool writeFunction(const char* data, size_t length) {
    received.append(data, length);
    return false;
}

Config config{.readFunction = readFunction, .writeFunction = writeFunction};

TEST(COMMAND_MANAGER, multiCommands) {
    Command jeden("jeden", "desc", callback1);
    Command dwa("dwa", "desc", callback1);

    CommandManager<2> command_manager(config, {&jeden, &dwa});

    command_manager.init();

    transmitted.append("jeden sdfg\ndwa 2345\n");

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }


    EXPECT_EQ(std::string("sdfg"), std::string(data_table[0]));
    EXPECT_EQ(std::string("2345"), std::string(data_table[1]));
}


float ff1, ff2;

void twoFloats(CommandBase& cmd, const char* data) {
    (void)cmd;
    auto [f1, f2] = CommandBase::get<float, float>(data);
    ff1           = f1;
    ff2           = f2;
}

TEST(COMMAND_MANAGER, twoFloats) {
    Command floats("floats", "desc", twoFloats);

    CommandManager<1> command_manager(config, {&floats});

    command_manager.init();

    transmitted.append("floats 2.456 3.654\n");

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }

    EXPECT_EQ(ff1, (float)(2.456));
    EXPECT_EQ(ff2, (float)(3.654));
}


int functionNUmber = 0;

void question1(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    functionNUmber = 1;
}

void question2(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    functionNUmber = 2;
}

void question3(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    functionNUmber = 3;
}

void question4(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    functionNUmber = 4;
}

TEST(COMMAND_MANAGER, question) {
    Command q1("t?", "t", question1);
    Command q2("n?", "n", question2);
    Command q3("t", "t", question3);
    Command q4("n", "n", question4);

    CommandManager<4> command_manager(config, {&q1, &q2, &q3, &q4});

    command_manager.init();

    transmitted.append("t 1\n");

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }
    EXPECT_EQ(functionNUmber, 3);

    // transmitted.append("t?\n");

    // for (uint8_t i = 0; i < 6; i++) {
    //     command_manager.run();
    // }
    // EXPECT_EQ(functionNUmber, 1);
}

TEST(COMMAND_MANAGER, undefined) {
    Command           q4("n", "n", question4);
    CommandManager<1> command_manager(config, {&q4});


    command_manager.init();

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }

    EXPECT_EQ(trim(received), std::string(""));

    transmitted.append("t 1\n");

    for (uint8_t i = 0; i < 6; i++) {
        command_manager.run();
    }

    EXPECT_EQ(trim(received), std::string("undefined"));
}


TEST(COMMAND_MANAGER, print) {
    Command           q4("n", "n", question4);
    CommandManager<1> command_manager(config, {&q4});

    command_manager.init();

    received.clear();

    command_manager.print((uint16_t)123);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    command_manager.print((uint32_t)123);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    command_manager.print((int16_t)123);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    command_manager.print((float)123);
    EXPECT_EQ(received, std::string("123.00"));
    received.clear();

    command_manager.print("123");
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    command_manager.print("123", 3);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    command_manager.print("123", 2);
    EXPECT_EQ(received, std::string("12"));
    received.clear();
}

// void info0Callback(CommandBase& cmd) {
// }

// void info1Callback(CommandBase& cmd) {
// }

// void info2Callback(CommandBase& cmd) {
// }

// TEST(COMMAND_MANAGER, getInfoVoid) {
//     Command info0("info0", "desc0", info0Callback);
//     Command info1("info1", "desc1", info1Callback);
//     Command info2("info2", "desc2", info2Callback);
//     CommandManager<3> command_manager({&info0, info1, info2}, readFunction, writeFunction);
//     command_manager.init();

//     std::string compareString;

//     compareString += "info0\tv\tf\n";
//     compareString += "info1\tv\tf\n";
//     compareString += "info2\tv\tf\n";

//     command_manager.getInfo();
//     EXPECT_TRUE(stream.getTxBuffer() == compareString);
// }

// void intCallback(int data) {
//     (void)data;
// }

// void floatCallback(float data) {
//     (void)data;
// }

// void doubleCallback(double data) {
//     (void)data;
// }

// void charCallback(char data) {
//     (void)data;
// }

// void charPointerCallback(const char* data) {
//     (void)data;
// }

// TEST(COMMAND_MANAGER, getInfoDifferentValues) {
//     CommandManager<5> command_manager(stream);
//     command_manager.init();

//     char names[5][10]       = {"info0", "info1", "info2", "info3", "info4"};
//     char valuesInfo[5][3]   = {"i", "f", "d", "c", "c*"};
//     char returnValues[5][3] = {"t", "f", "t", "f", "f"};

//     Command info0(names[0], intCallback, true);
//     Command info1(names[1], floatCallback, false);
//     Command info2(names[2], doubleCallback, true);
//     Command info3(names[3], charCallback);
//     Command info4(names[4], charPointerCallback);

//     command_manager.addCommand(&info0);
//     command_manager.addCommand(&info1);
//     command_manager.addCommand(&info2);
//     command_manager.addCommand(&info3);
//     command_manager.addCommand(&info4);

//     std::string compareString;

//     for (int i = 0; i < 5; i++) {
//         compareString += names[i];
//         compareString += '\t';
//         compareString += valuesInfo[i];
//         compareString += '\t';
//         compareString += returnValues[i];
//         compareString += '\n';
//     }
//     command_manager.getInfo();

//     std::string originalString = stream.getTxBuffer();

//     EXPECT_EQ(originalString.size(), compareString.size());

//     for (int i = 0; i < originalString.size(); i++) {
//         EXPECT_EQ(originalString[i], compareString[i]) << (int)i << std::endl;
//     }

//     const char* data = "?\n";
//     for (size_t i = 0; i < strlen(data); i++) {
//         stream.pushToRXBuffer(data[i]);
//     }

//     for (uint8_t i = 0; i < 6; i++) {
//         command_manager.run();
//     }
//     EXPECT_TRUE(stream.getTxBuffer() == compareString);
// }
