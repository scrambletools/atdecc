#include "protocolAaAecpdu.hpp"
#include "esp_log.h"
#include <cassert>
#include <string>

/***********************************************************/
/* AaAecpdu class definition                              */
/***********************************************************/

static const char* TAG = "AaAecpdu";

AaAecpdu::AaAecpdu(bool const isResponse) noexcept
{
    setMessageType(isResponse ? AecpMessageType::ADDRESS_ACCESS_RESPONSE : AecpMessageType::ADDRESS_ACCESS_COMMAND);
    //setAecpSpecificDataLength(HeaderLength);
}

AaAecpdu::~AaAecpdu() noexcept {}

void AaAecpdu::serialize(uint8_t* buffer, size_t length) const
{
    if (length < HeaderLength + _tlvDataLength) {
        ESP_LOGI(TAG, "Buffer size is too small for serialization");
        return;
    }

    // First serialize the parent AECPDU
    Aecpdu::serialize(buffer, length);

    size_t offset = Aecpdu::HEADER_LENGTH;

    // TLV count
    uint16_t tlvCount = static_cast<uint16_t>(_tlvData.size());
    memcpy(buffer + offset, &tlvCount, sizeof(tlvCount));
    offset += sizeof(tlvCount);

    // Serialize TLVs
    for (const auto& tlv : _tlvData)
    {
        uint16_t mode_length = static_cast<uint16_t>(((static_cast<uint16_t>(tlv.getMode()) << 12) & 0xF000) | (tlv.size() & 0x0FFF));
        memcpy(buffer + offset, &mode_length, sizeof(mode_length));
        offset += sizeof(mode_length);

        uint64_t address = tlv.getAddress();
        memcpy(buffer + offset, &address, sizeof(address));
        offset += sizeof(address);

        memcpy(buffer + offset, tlv.data(), tlv.size());
        offset += tlv.size();
    }

    ESP_LOGI(TAG, "Serialization complete");
}

void AaAecpdu::deserialize(const uint8_t* buffer, size_t length)
{
    if (length < HeaderLength) {
        ESP_LOGI(TAG, "Buffer size is too small for deserialization");
        return;
    }

    // First deserialize the parent AECPDU
    Aecpdu::deserialize(buffer, length);

    size_t offset = Aecpdu::HEADER_LENGTH;
    uint16_t tlvCount;

    memcpy(&tlvCount, buffer + offset, sizeof(tlvCount));
    offset += sizeof(tlvCount);

    for (uint16_t i = 0; i < tlvCount; ++i)
    {
        uint16_t mode_length;
        uint64_t address;

        memcpy(&mode_length, buffer + offset, sizeof(mode_length));
        offset += sizeof(mode_length);

        AaMode mode = static_cast<AaMode>((mode_length & 0xF000) >> 12);
        uint16_t length = mode_length & 0x0FFF;

        memcpy(&address, buffer + offset, sizeof(address));
        offset += sizeof(address);

        Tlv tlv(mode, address, length);
        memcpy(tlv.data(), buffer + offset, length);
        offset += length;

        _tlvData.push_back(std::move(tlv));
        _tlvDataLength += TlvHeaderLength + length;
    }

    ESP_LOGI(TAG, "Deserialization complete");
}

Aecpdu::UniquePointer AaAecpdu::responseCopy() const
{
    if (getMessageType() != AecpMessageType::ADDRESS_ACCESS_COMMAND) {
        ESP_LOGI(TAG, "Cannot create a response from a non-ADDRESS_ACCESS_COMMAND message");
        return UniquePointer{nullptr, nullptr};
    }

    auto deleter = [](Aecpdu* self)
    {
        static_cast<AaAecpdu*>(self)->destroy();
    };

    auto response = UniquePointer(new AaAecpdu(*this), deleter);
    auto& aa = static_cast<AaAecpdu&>(*response);

    // Change the message type to ADDRESS_ACCESS_RESPONSE
    aa.setMessageType(AecpMessageType::ADDRESS_ACCESS_RESPONSE);

    return response;
}

/** Entry point */
AaAecpdu* AaAecpdu::createRawAaAecpdu(bool const isResponse) noexcept
{
    return new AaAecpdu(isResponse);
}

/** Destroy method for COM-like interface */
void AaAecpdu::destroy() noexcept
{
    delete this;
}