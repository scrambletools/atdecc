#include "protocolAecpdu.hpp"
#include <cstring>  // For std::memcpy
#include "esp_log.h"

static const char* TAG = "Aecpdu";

/** Default constructor */
Aecpdu::Aecpdu() noexcept
    : _targetEntityID(), _controllerEntityID(), _sequenceID(0), _messageType(AecpMessageType::UNKNOWN), _status(AecpStatus::UNKNOWN)
{
}

/** Serialize the AECPDU to a buffer */
void Aecpdu::serialize(uint8_t* buffer, size_t length) const
{
    if (buffer == nullptr || length < HEADER_LENGTH)
    {
        // Error handling: buffer is null or insufficient length
        ESP_LOGE(TAG, "Buffer is null or length is insufficient for serialization.");
        return;
    }

    // Serialize common fields
    size_t offset = 0;
    std::memcpy(buffer + offset, &_targetEntityID, sizeof(_targetEntityID));
    offset += sizeof(_targetEntityID);
    std::memcpy(buffer + offset, &_controllerEntityID, sizeof(_controllerEntityID));
    offset += sizeof(_controllerEntityID);
    std::memcpy(buffer + offset, &_sequenceID, sizeof(_sequenceID));
    offset += sizeof(_sequenceID);
    std::memcpy(buffer + offset, &_messageType, sizeof(_messageType));
    offset += sizeof(_messageType);
    std::memcpy(buffer + offset, &_status, sizeof(_status));
    offset += sizeof(_status);

    ESP_LOGI(TAG, "Serialization complete. Serialized data length: %d bytes", offset);

    // Add other specific serialization logic if needed
}

/** Deserialize the AECPDU from a buffer */
void Aecpdu::deserialize(const uint8_t* buffer, size_t length)
{
    if (buffer == nullptr || length < HEADER_LENGTH)
    {
        // Error handling: buffer is null or insufficient length
        ESP_LOGE(TAG, "Buffer is null or length is insufficient for deserialization.");
        return;
    }

    // Deserialize common fields
    size_t offset = 0;
    std::memcpy(&_targetEntityID, buffer + offset, sizeof(_targetEntityID));
    offset += sizeof(_targetEntityID);
    std::memcpy(&_controllerEntityID, buffer + offset, sizeof(_controllerEntityID));
    offset += sizeof(_controllerEntityID);
    std::memcpy(&_sequenceID, buffer + offset, sizeof(_sequenceID));
    offset += sizeof(_sequenceID);
    std::memcpy(&_messageType, buffer + offset, sizeof(_messageType));
    offset += sizeof(_messageType);
    std::memcpy(&_status, buffer + offset, sizeof(_status));
    offset += sizeof(_status);

    ESP_LOGI(TAG, "Deserialization complete. Deserialized data length: %d bytes", offset);

    // Add other specific deserialization logic if needed
}

/** Create specific AECP message types based on the provided message type */
Aecpdu::UniquePointer Aecpdu::createAemMessage(AecpMessageType messageType)
{
    UniquePointer aecpdu(new Aecpdu(), &Aecpdu::destroy);
    aecpdu->setMessageType(messageType);
    ESP_LOGI(TAG, "Created AECP message with type: %d", static_cast<int>(messageType));
    return aecpdu;
}

/** Destroy method for UniquePointer */
void Aecpdu::destroy(Aecpdu* aecpdu)
{
    if (aecpdu != nullptr)
    {
        ESP_LOGI(TAG, "Destroying AECP message.");
        delete aecpdu;
    }
    else
    {
        ESP_LOGW(TAG, "Attempted to destroy a null AECP message.");
    }
}