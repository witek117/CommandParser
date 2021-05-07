#include "Package.h"
#include <Command.h>
#include <command_manager.h>

#include <gmock/gmock.h>

static void enable_interrupts() {}
static void disable_interrupts() {}
static std::string printedString;
static int myInt = 0;
static void one_callback(const char* data) {
    (void) data;
    printedString = "one";
}
static void two_callback(const char* data) {
    myInt = parser::parse<int>(data);
}

static void print_function(uint8_t c) {
    printedString += c;
}

static void print_function_buffer(uint8_t* c, uint16_t len) {
    for(uint16_t i =0; i < len; i++) {
        printedString += c[i];
    }
}

Command one("one", one_callback);
Command two("two", two_callback);

class TestPackage : public Package {
    void useValidData() {

    }
};

TEST(PACKAGE_AND_COMMAND, create) {
    PackageAndCommandManager<2> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);
    command_manager.init();

    uint8_t txData [100] = {0};

    txData[0] = 0xc8;
    txData[1] = 4;
    txData[2] = 'o';
    txData[3] = 'n';
    txData[4] = 'e';
    txData[5] = '\n';

    uint16_t crc = Package::getCRC(txData, 2, 4);

    txData[6] = Package::getFirstByte(crc);
    txData[7] = Package::getSecondByte(crc);

    for(uint8_t i = 0; i < 8; i++) {
        command_manager.putChar(txData[i]);
    }
    printedString = "";
    command_manager.run();
    EXPECT_EQ(printedString, std::string("one"));

    txData[7] = 0; // create error
    for(uint8_t i = 0; i < 8; i++) {
        command_manager.putChar(txData[i]);
    }
    printedString = "";
    command_manager.run();
    EXPECT_EQ(printedString, std::string(""));
}

TEST(PACKAGE_AND_COMMAND, numbers) {
    PackageAndCommandManager<2> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();

    uint8_t myText[] = "two 123\n";
    uint8_t myTextLength = sizeof(myText);
    uint8_t txData [100] = {0};

    txData[0] = 0xc8;
    txData[1] = myTextLength;
    memcpy(&txData[2], myText, myTextLength);

    uint16_t crc = Package::getCRC(txData, 2, myTextLength);

    txData[myTextLength + 2] = Package::getFirstByte(crc);
    txData[myTextLength + 3] = Package::getSecondByte(crc);

    for(uint8_t i = 0; i < (myTextLength + 4); i++) {
        command_manager.putChar(txData[i]);
    }

    myInt = 0;
    command_manager.run();
    EXPECT_EQ(myInt, 123);
}

TEST(PACKAGE_AND_COMMAND, undefined) {
    PackageAndCommandManager<2> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();

    uint8_t myText[] = "three 123\n";
    uint8_t myTextLength = sizeof(myText);
    uint8_t txData [100] = {0};

    txData[0] = 0xc8;
    txData[1] = myTextLength;
    memcpy(&txData[2], myText, myTextLength);

    uint16_t crc = Package::getCRC(txData, 2, myTextLength);

    txData[myTextLength + 2] = Package::getFirstByte(crc);
    txData[myTextLength + 3] = Package::getSecondByte(crc);

    printedString = "";

    for(uint8_t i = 0; i < (myTextLength + 4); i++) {
        command_manager.putChar(txData[i]);
    }

    command_manager.run();
    EXPECT_EQ(printedString, std::string("\xC8\nundefined\nH\t"));
}

TEST(PACKAGE_AND_COMMAND, without_crc) {
    PackageAndCommandManager<2> command_manager(&enable_interrupts, &disable_interrupts, &print_function);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();

    uint8_t myText[] = "one\n";

    for(unsigned char i : myText) {
        command_manager.putChar(i);
    }
    printedString = "";
    command_manager.run();
    EXPECT_EQ(printedString, std::string(""));
}

TEST(PACKAGE_AND_COMMAND, print_function_buffer) {
    PackageAndCommandManager<2> command_manager(&enable_interrupts, &disable_interrupts, &print_function_buffer);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();

    uint8_t myText[] = "three 123\n";
    uint8_t myTextLength = sizeof(myText);
    uint8_t txData [100] = {0};

    txData[0] = 0xc8;
    txData[1] = myTextLength;
    memcpy(&txData[2], myText, myTextLength);

    uint16_t crc = Package::getCRC(txData, 2, myTextLength);

    txData[myTextLength + 2] = Package::getFirstByte(crc);
    txData[myTextLength + 3] = Package::getSecondByte(crc);

    printedString = "";

    for(uint8_t i = 0; i < (myTextLength + 4); i++) {
        command_manager.putChar(txData[i]);
    }

    command_manager.run();
    EXPECT_EQ(printedString, std::string("\xC8\nundefined\nH\t"));
}
