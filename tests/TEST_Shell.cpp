#include "Shell.hpp"
#include "Command.hpp"
#include "helper.hpp"
#include <gmock/gmock.h>

char data_table[3][100];

void callback1(CommandBase& cmd, const char* data) {
    (void)cmd;
    static int data_len = 0;
    std::strcpy(data_table[data_len], data);
    data_len++;
}

using ::testing::StrictMock;

std::string transmitted;
std::string received;

const char* read_function(std::size_t& len) {
    static std::string myData;
    myData.clear();

    myData.append(transmitted);
    transmitted.clear();

    len = myData.size();
    return myData.c_str();
}

bool write_function(const char* data, std::size_t len) {
    received.append(data, len);
    return false;
}

ShellBase::Config config{.read_function = read_function, .write_function = write_function};

TEST(SHELL, multi_commands) {
    Command jeden("jeden", "desc", callback1);
    Command dwa("dwa", "desc", callback1);

    Shell<2> shell(config, {&jeden, &dwa});

    shell.init();

    transmitted.append("jeden sdfg\ndwa 2345\n");

    for (std::size_t i = 0; i < 6; i++) {
        shell.run();
    }


    EXPECT_EQ(std::string("sdfg"), std::string(data_table[0]));
    EXPECT_EQ(std::string("2345"), std::string(data_table[1]));
}


float ff1, ff2;

void two_floats(CommandBase& cmd, const char* data) {
    (void)cmd;
    auto [f1, f2] = CommandBase::get<float, float>(data);
    ff1           = f1;
    ff2           = f2;
}

TEST(SHELL, two_floats) {
    Command floats("floats", "desc", two_floats);

    Shell<1> shell(config, {&floats});

    shell.init();

    transmitted.append("floats 2.456 3.654\n");

    for (std::size_t i = 0; i < 6; i++) {
        shell.run();
    }

    EXPECT_EQ(ff1, (float)(2.456));
    EXPECT_EQ(ff2, (float)(3.654));
}


int function_number = 0;

void question1(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    function_number = 1;
}

void question2(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    function_number = 2;
}

void question3(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    function_number = 3;
}

void question4(CommandBase& cmd, const char* data) {
    (void)cmd;
    (void)data;
    function_number = 4;
}

TEST(SHELL, question) {
    Command q1("t?", "t", question1);
    Command q2("n?", "n", question2);
    Command q3("t", "t", question3);
    Command q4("n", "n", question4);

    Shell<4> shell(config, {&q1, &q2, &q3, &q4});

    shell.init();

    transmitted.append("t 1\n");

    for (std::size_t i = 0; i < 6; i++) {
        shell.run();
    }
    EXPECT_EQ(function_number, 3);

    // transmitted.append("t?\n");

    // for (std::size_t i = 0; i < 6; i++) {
    //     shell.run();
    // }
    // EXPECT_EQ(functionNUmber, 1);
}

TEST(SHELL, undefined) {
    Command  q4("n", "n", question4);
    Shell<1> shell(config, {&q4});


    shell.init();

    for (std::size_t i = 0; i < 6; i++) {
        shell.run();
    }

    EXPECT_EQ(helper::trim(received), std::string(""));

    transmitted.append("t 1\n");

    for (std::size_t i = 0; i < 6; i++) {
        shell.run();
    }

    EXPECT_EQ(helper::trim(received), std::string("undefined"));
}


TEST(SHELL, print) {
    Command  q4("n", "n", question4);
    Shell<1> shell(config, {&q4});

    shell.init();

    received.clear();

    shell.print((uint16_t)123);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    shell.print((uint32_t)123);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    shell.print((int16_t)123);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    shell.print((float)123);
    EXPECT_EQ(received, std::string("123.00"));
    received.clear();

    shell.print("123");
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    shell.print("123", 3);
    EXPECT_EQ(received, std::string("123"));
    received.clear();

    shell.print("123", 2);
    EXPECT_EQ(received, std::string("12"));
    received.clear();
}
