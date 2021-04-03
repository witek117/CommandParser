#include "Package.h"
#include <gmock/gmock.h>

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
