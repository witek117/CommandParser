#include "CommandSender.h"

template<> RawTemplateTransmitCommand<uint32_t>::RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
    constructorForFourByte();
}

template<> void RawTemplateTransmitCommand<uint32_t>::setValue(uint32_t value) {
    auto *data = reinterpret_cast<uint8_t *>(&value);
    setValueForFourByte(data);
}

template<> RawTemplateTransmitCommand<int32_t>::RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
    constructorForFourByte();
}

template<> void RawTemplateTransmitCommand<int32_t>::setValue(int32_t value) {
    auto *data = reinterpret_cast<uint8_t *>(&value);
    setValueForFourByte(data);
}

template<> RawTemplateTransmitCommand<uint16_t>::RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
    constructorForTwoByte();
}

template<> void RawTemplateTransmitCommand<uint16_t>::setValue(uint16_t value) {
    auto *data = reinterpret_cast<uint8_t *>(&value);
    setValueForTwoByte(data);
}

template<> RawTemplateTransmitCommand<int16_t>::RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
    constructorForTwoByte();
}

template<> void RawTemplateTransmitCommand<int16_t>::setValue(int16_t value) {
    auto *data = reinterpret_cast<uint8_t *>(&value);
    setValueForTwoByte(data);
}

template<> RawTemplateTransmitCommand<uint8_t>::RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
    constructorForOneByte();
}

template<> void RawTemplateTransmitCommand<uint8_t>::setValue(uint8_t value) {
    setValueForOneByte(value);
}

template<> RawTemplateTransmitCommand<int8_t>::RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
    constructorForOneByte();
}

template<> void RawTemplateTransmitCommand<int8_t>::setValue(int8_t value) {
    setValueForOneByte(static_cast<uint8_t>(value));
}

template<> RawTemplateTransmitCommand<char>::RawTemplateTransmitCommand(const char *commandName) : RawTransmitCommand(commandName) {
    constructorForOneByte();
}

template<> void RawTemplateTransmitCommand<char>::setValue(char value) {
    setValueForOneByte(static_cast<uint8_t>(value));
}

