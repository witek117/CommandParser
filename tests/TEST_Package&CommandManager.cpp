#include "Package.h"
#include "Stream.h"
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
static Stream stream;

Command one("one", one_callback);
Command two("two", two_callback);

TEST(PACKAGE_AND_COMMAND, create) {
    Package package(stream);
    CommandManager<2> command_manager(package, &enable_interrupts, &disable_interrupts);
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
        stream.pushToRXBuffer(txData[i]);
    }
    printedString = "";
    for(int i = 0; i < 100; i++) {
        package.run();
        command_manager.run();
    }

    EXPECT_EQ(printedString, std::string("one"));

    txData[7] = 0; // create error

    for(uint8_t i = 0; i < 8; i++) {
        stream.pushToRXBuffer(txData[i]);
    }

    printedString = "";

    for(int i = 0; i < 100; i++) {
        package.run();
        command_manager.run();
    }

    EXPECT_EQ(printedString, std::string(""));
}

TEST(PACKAGE_AND_COMMAND, numbers) {
    Package package(stream);
    CommandManager<2> command_manager(package, &enable_interrupts, &disable_interrupts);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();

    uint8_t myText[] = "two 123\n";
    uint8_t myTextLength = strlen(reinterpret_cast<const char *>(myText));
    uint8_t txData [100] = {0};

    txData[0] = 0xc8;
    txData[1] = myTextLength;
    memcpy(&txData[2], myText, myTextLength);

    uint16_t crc = Package::getCRC(txData, 2, myTextLength);

    txData[myTextLength + 2] = Package::getFirstByte(crc);
    txData[myTextLength + 3] = Package::getSecondByte(crc);

    for(uint8_t i = 0; i < (myTextLength + 4); i++) {
        stream.pushToRXBuffer(txData[i]);
    }

    myInt = 0;

    for(int i = 0; i < 100; i++) {
        package.run();
        command_manager.run();
    }

    EXPECT_EQ(myInt, 123);
}

TEST(PACKAGE_AND_COMMAND, undefined) {
    Package package(stream);
    CommandManager<2> command_manager(package, &enable_interrupts, &disable_interrupts);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();

    uint8_t myText[] = "three 123\n";
    uint8_t myTextLength = strlen(reinterpret_cast<const char *>(myText));
    uint8_t txData [100] = {0};

    txData[0] = 0xc8;
    txData[1] = myTextLength;
    memcpy(&txData[2], myText, myTextLength);

    uint16_t crc = Package::getCRC(txData, 2, myTextLength);

    txData[myTextLength + 2] = Package::getFirstByte(crc);
    txData[myTextLength + 3] = Package::getSecondByte(crc);

    printedString = "";

    for(uint8_t i = 0; i < (myTextLength + 4); i++) {
        stream.pushToRXBuffer(txData[i]);
    }

    for(int i = 0; i < 100; i++) {
        package.run();
        command_manager.run();
    }
    EXPECT_EQ(stream.getTxBuffer(), std::string("\xC8\nundefined\nH\t"));
}

TEST(PACKAGE_AND_COMMAND, without_crc) {
    Package package(stream);
    CommandManager<2> command_manager(package, &enable_interrupts, &disable_interrupts);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();
    uint8_t myText[] = "one\n";

    for(unsigned char i : myText) {
        stream.pushToRXBuffer(i);
    }

    for(int i = 0; i < 100; i++) {
        package.run();
        command_manager.run();
    }

    EXPECT_EQ(stream.getTxBuffer(), std::string(""));
}

TEST(PACKAGE_AND_COMMAND, print_function_buffer) {
    Package package(stream);
    CommandManager<2> command_manager(package, &enable_interrupts, &disable_interrupts);
    command_manager.addCommand(&one);
    command_manager.addCommand(&two);

    command_manager.init();

    uint8_t myText[] = "three 123\n";
    uint8_t myTextLength = strlen(reinterpret_cast<const char *>(myText));
    uint8_t txData [100] = {0};

    txData[0] = 0xc8;
    txData[1] = myTextLength;
    memcpy(&txData[2], myText, myTextLength);

    uint16_t crc = Package::getCRC(txData, 2, myTextLength);

    txData[myTextLength + 2] = Package::getFirstByte(crc);
    txData[myTextLength + 3] = Package::getSecondByte(crc);

    for(uint8_t i = 0; i < (myTextLength + 4); i++) {
        stream.pushToRXBuffer(txData[i]);
    }

    for(int i = 0; i < 100; i++) {
        package.run();
        command_manager.run();
    }
    EXPECT_EQ(stream.getTxBuffer(), std::string("\xC8\nundefined\nH\t"));
}
