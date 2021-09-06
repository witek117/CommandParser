#include "CommandSender.h"
#include <gmock/gmock.h>

TEST(CommandSender, createRawCommand) {
    RawVoidTransmitCommand rawCommand("rawCommand");

    uint8_t len;
    const char *data = reinterpret_cast<const char *>(rawCommand.getCommandData(&len));

    EXPECT_STREQ(data, "rawCommand\n");
    EXPECT_EQ(len, 12);
}

TEST(CommandSender, createUint32RawCommand) {
    RawTemplateTransmitCommand<uint32_t> rawCommand("rawCommand");

    uint32_t value = 0x01D401D4; // it is 0x01D4

    rawCommand.setValue(value);
    uint8_t len;
    const auto *data = reinterpret_cast<const uint8_t *>(rawCommand.getCommandData(&len));

    constexpr uint8_t refLen = 18;
    uint8_t referenceData[refLen] = "rawCommand \0\xD4\x01\xD4\x01\n";

    // test value with no new line
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData) - 1; ++i) {
        EXPECT_EQ(referenceData[i], data[i]) << "data and referenceData differ at index " << i;
    }

    // test value with new line at index zero
    uint8_t referenceData2[refLen] = "rawCommand \x10\x20\x20\x20 \n";
    rawCommand.setValue(0x0A202020);
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData2) - 1; ++i) {
        EXPECT_EQ(referenceData2[i], data[i]) << "data and referenceData differ at index " << i;
    }
//
    // test value with new line at index one
    uint8_t referenceData3[refLen] = "rawCommand \x20\x20\x20 \x20\n";
    rawCommand.setValue(0x200A2020);
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData3) - 1; ++i) {
        EXPECT_EQ(referenceData3[i], data[i]) << "data and referenceData differ at index " << i;
    }

    // test value with new line at index two
    uint8_t referenceData4[refLen] = "rawCommand \x40\x20 \x20\x20\n";
    rawCommand.setValue(0x20200A20);
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData4) - 1; ++i) {
        EXPECT_EQ(referenceData4[i], data[i]) << "data and referenceData differ at index " << i;
    }

    // test value with new line at index three
    uint8_t referenceData5[refLen] = "rawCommand \x80 \x20\x20\x20\n";
    rawCommand.setValue(0x2020200A);
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData5) - 1; ++i) {
        EXPECT_EQ(referenceData5[i], data[i]) << "data and referenceData differ at index " << i;
    }
}

TEST(CommandSender, createInt16RawCommand) {
    RawTemplateTransmitCommand<uint16_t> rawCommand("rawCommand");

    int16_t value = 468; // it is 0x01D4

    rawCommand.setValue(value);
    uint8_t len;
    const auto *data = reinterpret_cast<const uint8_t *>(rawCommand.getCommandData(&len));

    constexpr uint8_t refLen = 16;
    uint8_t referenceData[refLen] = "rawCommand \0\xD4\x01\n";

    // test value with no new line
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData) - 1; ++i) {
        EXPECT_EQ(referenceData[i], data[i]) << "data and referenceData differ at index " << i;
    }

    // test value with new line at index zero
    uint8_t referenceData2[refLen] = "rawCommand \x80 \x20\n";
    rawCommand.setValue(0x200A);
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData2) - 1; ++i) {
        EXPECT_EQ(referenceData2[i], data[i]) << "data and referenceData differ at index " << i;
    }

    // test value with new line at index one
    uint8_t referenceData3[refLen] = "rawCommand \x40\x20 \n";
    rawCommand.setValue(0x0A20);
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData3) - 1; ++i) {
        EXPECT_EQ(referenceData3[i], data[i]) << "data and referenceData differ at index " << i;
    }

    // test value with new line at index zero and one
    uint8_t referenceData4[refLen] = "rawCommand \xC0  \n";
    rawCommand.setValue(0x0A0A);
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData4) - 1; ++i) {
        EXPECT_EQ(referenceData4[i], data[i]) << "data and referenceData differ at index " << i;
    }
}

TEST(CommandSender, createUint8RawCommand) {
    RawTemplateTransmitCommand<uint8_t> rawCommand("rawCommand");

    uint8_t value = 28; // it is 0x1C

    rawCommand.setValue(value);
    uint8_t len;
    const auto *data = rawCommand.getCommandData(&len);

    constexpr uint8_t refLen = 15;
    uint8_t referenceData[refLen] = "rawCommand \0\x1C\n";

    // test value witch no new line
    ASSERT_EQ(len, refLen - 1) << "data and referenceData are of unequal length";
    for (int i = 0; i < sizeof(referenceData) - 1; ++i) {
        EXPECT_EQ(referenceData[i], data[i]) << "data and referenceData differ at index " << i;
    }

    uint8_t referenceData2[refLen] = "rawCommand \x80 \n";
    // test value with new line at index zero
    rawCommand.setValue('\n');
    data = rawCommand.getCommandData(&len);
    ASSERT_EQ(len, refLen - 1) << "data and referenceData2 are of unequal length";
    for (int i = 0; i < sizeof(referenceData2) - 1; ++i) {
        EXPECT_EQ(referenceData2[i], data[i]) << "data and referenceData2 differ at index " << i;
    }
}
