#include "protocolAemAecpdu.hpp"
#include "esp_log.h"
#include <cstring> // memcpy

/***********************************************************/
/* AemAecpdu class definition                              */
/***********************************************************/

AemAecpdu::AemAecpdu(bool const isResponse) noexcept
{
    setMessageType(isResponse ? AecpMessageType::AEM_RESPONSE : AecpMessageType::AEM_COMMAND);
    //setAecpSpecificDataLength(HEADER_LENGTH);
}

AemAecpdu::~AemAecpdu() noexcept {}

void AemAecpdu::setUnsolicited(bool const unsolicited) noexcept
{
    _unsolicited = unsolicited;
}

void AemAecpdu::setCommandType(AemCommandType const commandType) noexcept
{
    _commandType = commandType;
}

void AemAecpdu::setCommandSpecificData(void const* const commandSpecificData, size_t const commandSpecificDataLength)
{
    // Check Aecp does not exceed maximum allowed length
    if (commandSpecificDataLength > MAXIMUM_PAYLOAD_BUFFER_LENGTH)
    {
        ESP_LOGE("AEM_AECPDU", "AEM payload too big");
        return;
    }

    _commandSpecificDataLength = commandSpecificDataLength;
    if (_commandSpecificDataLength > 0 && commandSpecificData != nullptr)
    {
        std::memcpy(_commandSpecificData.data(), commandSpecificData, _commandSpecificDataLength);
    }
    // Update parent's specific data length field
    //setAecpSpecificDataLength(HEADER_LENGTH + commandSpecificDataLength);
}

bool AemAecpdu::getUnsolicited() const noexcept
{
    return _unsolicited;
}

AemCommandType AemAecpdu::getCommandType() const noexcept
{
    return _commandType;
}

AemAecpdu::Payload AemAecpdu::getPayload() const noexcept
{
    return std::make_pair(_commandSpecificData.data(), _commandSpecificDataLength);
}

void AemAecpdu::serialize(uint8_t* buffer, size_t length) const
{
    size_t offset = 0;

    // Serialize AECPDU common header first
    Aecpdu::serialize(buffer, length);

    offset += Aecpdu::HEADER_LENGTH;

    // Serialize unsolicited bit and command type into 2 bytes (16 bits)
    if (offset + 2 > length) {
        ESP_LOGE("SERIALIZE", "Not enough space in buffer");
        return;
    }

    uint16_t u_ct = static_cast<uint16_t>(((_unsolicited << 15) & 0x8000) | (static_cast<uint16_t>(_commandType) & 0x7fff));
    buffer[offset] = static_cast<uint8_t>((u_ct >> 8) & 0xFF);  // High byte
    buffer[offset + 1] = static_cast<uint8_t>(u_ct & 0xFF);     // Low byte
    offset += 2;

    // Serialize the command-specific data
    auto payloadLength = _commandSpecificDataLength;
    if (payloadLength > MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH) {
        ESP_LOGW("SERIALIZE", "Payload size exceeds maximum allowed value of %zu for AemCommandType %u, clamping buffer down from %zu",
                 MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH, static_cast<uint16_t>(_commandType), payloadLength);
        payloadLength = std::min(payloadLength, MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH);
    }

    if (offset + payloadLength > length) {
        ESP_LOGE("SERIALIZE", "Not enough space in buffer to serialize payload");
        return;
    }

    std::memcpy(buffer + offset, _commandSpecificData.data(), payloadLength);
    offset += payloadLength;

    // Validate if the size matches expected size
    if (offset != (Aecpdu::HEADER_LENGTH + HEADER_LENGTH + payloadLength)) {
        ESP_LOGE("SERIALIZE", "Packed buffer length does not match expected length");
    }
}

void AemAecpdu::deserialize(const uint8_t* buffer, size_t length)
{
    size_t offset = 0;

    // Deserialize AECPDU common header first
    Aecpdu::deserialize(buffer, length);
    offset += Aecpdu::HEADER_LENGTH;

    // Check if there are enough bytes to deserialize the AEM-specific header
    if (offset + 2 > length) {
        ESP_LOGE("DESERIALIZE", "Not enough data in buffer for deserialization");
    }

    // Deserialize unsolicited bit and command type (16 bits)
    uint16_t u_ct = (static_cast<uint16_t>(buffer[offset]) << 8) | static_cast<uint16_t>(buffer[offset + 1]);
    offset += 2;

    _unsolicited = ((u_ct & 0x8000) >> 15) != 0;
    _commandType = static_cast<AemCommandType>(u_ct & 0x7fff);

    // Ensure control data length is at least the minimum expected
    /*auto const minCDL = Aecpdu::HEADER_LENGTH + HEADER_LENGTH;
    if (_controlDataLength < minCDL) {
        ESP_LOGW("DESERIALIZE", "ControlDataLength field value too small for AEM-AECPDU, expected at least %u, but got %u", minCDL, _controlDataLength);
    }

    // Determine the size of the command-specific data
    _commandSpecificDataLength = _controlDataLength - minCDL;*/

    // Ensure there is enough data to read the payload
    if (_commandSpecificDataLength > (length - offset)) {
        ESP_LOGW("DESERIALIZE", "Not enough data to deserialize command-specific data");
    }

    // Clamp the command-specific data length if it exceeds the maximum allowed
    if (_commandSpecificDataLength > MAXIMUM_RECV_PAYLOAD_BUFFER_LENGTH) {
        ESP_LOGW("DESERIALIZE", "Payload size exceeds maximum allowed value of %zu, clamping buffer down from %zu",
                 MAXIMUM_RECV_PAYLOAD_BUFFER_LENGTH, _commandSpecificDataLength);
        _commandSpecificDataLength = std::min(_commandSpecificDataLength, MAXIMUM_RECV_PAYLOAD_BUFFER_LENGTH);
    }

    // Deserialize the payload
    std::memcpy(_commandSpecificData.data(), buffer + offset, _commandSpecificDataLength);
}

Aecpdu::UniquePointer AemAecpdu::responseCopy() const
{
    if (getMessageType() != AecpMessageType::AEM_COMMAND)
    {
        ESP_LOGE("AEM_AECPDU", "Invalid command type for response");
        return UniquePointer{nullptr, nullptr};
    }

    auto deleter = [](Aecpdu* self)
    {
        static_cast<AemAecpdu*>(self)->destroy();
    };

    // Create a response message as a copy of this
    UniquePointer response(new AemAecpdu(*this), deleter);
    
    // Change the message type to be an AEM_RESPONSE
    response->setMessageType(AecpMessageType::AEM_RESPONSE);

    return response;
}

AemAecpdu* AemAecpdu::createRawAemAecpdu(bool const isResponse) noexcept
{
    return new AemAecpdu(isResponse);
}

void AemAecpdu::destroy()
{
    delete this;
}
