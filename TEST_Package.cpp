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

    EXPECT_EQ(0xc8, package.packageTxBuffer[0]);
    EXPECT_EQ(dataLength, package.packageTxBuffer[1]);

    for(int i = 0; i < dataLength; i++) {
        EXPECT_EQ(data[i], package.packageTxBuffer[i + 2]);
    }

    uint16_t crc = Package::getCRC((uint8_t *) data, 0, dataLength);

    EXPECT_EQ(Package::getFirstByte(crc), package.packageTxBuffer[packageLength - 2]);
    EXPECT_EQ(Package::getSecondByte(crc), package.packageTxBuffer[packageLength - 1]);
}

TEST(PACKAGE, receive_package) {
    gotValidData = false;
    TestPackage package;
    uint8_t myData [] = "this is the test 1234\n";
    uint16_t crc = Package::getCRC(myData, 0, (uint8_t) sizeof(myData));

    package.putChar(0xc8);
    package.putChar((uint8_t) sizeof(myData));

    for (unsigned char i : myData) {
        package.putChar(i);
    }
    package.putChar(Package::getFirstByte(crc));
    package.putChar(Package::getSecondByte(crc));

    EXPECT_EQ(gotValidData, true);
}

TEST(PACKAGE, create_receive) {

    TestPackage package;
    const char *data = "this is the test";
    uint8_t dataLength = sizeof(data);
    uint8_t packageLength = package.createPackage(const_cast<char *>(data), dataLength);

    gotValidData = false;
    for (uint8_t i = 0; i < packageLength; i++) {
        package.putChar(package.packageTxBuffer[i]);
    }
    EXPECT_EQ(gotValidData, true);

    gotValidData = false;
    for (uint8_t i = 0; i < packageLength; i++) {
        package.putChar(package.packageTxBuffer[i]);
    }
    EXPECT_EQ(gotValidData, true);

    uint8_t temp_byte = package.packageTxBuffer[packageLength - 1];
    package.packageTxBuffer[packageLength - 1] = 4; // Create CRC error
    gotValidData = false;
    for (uint8_t i = 0; i < packageLength; i++) {
        package.putChar(package.packageTxBuffer[i]);
    }
    EXPECT_EQ(gotValidData, false);

    package.packageTxBuffer[packageLength - 1] = temp_byte;
    gotValidData = false;
    for (uint8_t i = 0; i < packageLength; i++) {
        package.putChar(package.packageTxBuffer[i]);
    }
    EXPECT_EQ(gotValidData, true);
}

TEST(PACKAGE, create_receive_error) {
    gotValidData = false;
    TestPackage package;
    const char *data = "this is the test";
    uint8_t dataLength = sizeof(data);
    uint8_t packageLength = package.createPackage(const_cast<char *>(data), dataLength);

    package.packageTxBuffer[packageLength - 1] = 4; // Create CRC error

    for (uint8_t i = 0; i < packageLength; i++) {
        package.putChar(package.packageTxBuffer[i]);
    }

    EXPECT_EQ(gotValidData, false);
}


TEST(PACKAGE, CRC) {
    uint8_t byte = 117;
    uint16_t crc = 0xFFFF;


    // CRC-CCITT without LUT
    byte ^= (uint8_t)(crc & 0x00ffu);
    byte ^= (uint8_t)(byte << 4u);
    crc = (((uint16_t)((uint16_t)byte << 8u) | (uint16_t)((uint8_t)crc >> 8u)) ^ (uint8_t)(byte >> 4u) ^ ((uint16_t)byte << 3u));

    std::cout << crc << std::endl;
//
//    return (((uint16_t)((uint16_t)byte << 8u) | (uint16_t)((uint8_t)crc >> 8u)) ^ (uint8_t)(byte >> 4u) ^ ((uint16_t)byte << 3u));

}
