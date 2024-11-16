#include "protocolAdpdu.hpp"
#include <cstring> // for memcpy
#include <cassert> // for assert
#include "esp_log.h"

/***********************************************************/
/* Adpdu class definition                                  */
/***********************************************************/

MacAddress const Adpdu::Multicast_Mac_Address{ { 0x91, 0xe0, 0xf0, 0x01, 0x00, 0x00 } };

// Constructor to initialize default ADPDU fields
Adpdu::Adpdu() noexcept
{
    // set the avtp control fields for an adpdu
    AvtpduControl::setSubType(AVTP_SUBTYPE_ADP);
	AvtpduControl::setStreamValid(0);
	AvtpduControl::setControlDataLength(Length);

    // Initialize all fields to zero or default values
    // messageType = 0;
    // validTime = 0;
    // entityID = 0;
    // entityModelID = 0;
    // entityCapabilities = 0;
    // talkerStreamSources = 0;
    // talkerCapabilities = 0;
    // listenerStreamSinks = 0;
    // listenerCapabilities = 0;
    // availableIndex = 0;
    // gptpGrandmasterID = 0;
    // gptpDomainNumber = 0;
    // identifyControlIndex = 0;
    // interfaceIndex = 0;
    // associationID = 0;
}

// Destructor
Adpdu::~Adpdu() noexcept = default;

// Serialize the ADPDU fields into a buffer for transmission
//void Adpdu::serialize(uint8_t* buffer) const noexcept
void Adpdu::serialize(SerBuffer& buffer) const noexcept
{
    // Reserved fields
    uint32_t reserved0 = {0u};
    uint32_t reserved1 = {0u};

    auto const previousSize = buffer.size();

    // load data into buffer
    buffer << entityModelID << entityCapabilities;
	buffer << talkerStreamSources << talkerCapabilities;
	buffer << listenerStreamSinks << listenerCapabilities;
	buffer << controllerCapabilities;
	buffer << availableIndex;
	buffer << gptpGrandmasterID << static_cast<std::uint32_t>(((gptpDomainNumber << 24) & 0xff000000) | (reserved0 & 0x00ffffff));
	buffer << identifyControlIndex << interfaceIndex << associationID << reserved1;

    // check that buffer size change is correct
    if ((buffer.size() - previousSize) != Length) {
        ESP_LOGI("ADPDU", "Serialize error: buffer is %d but should be %d", buffer.size() - previousSize, Length);
    }
}

// Deserialize the ADPDU from a buffer
void Adpdu::deserialize(const uint8_t* buffer)
{
    // Ensure that the buffer contains enough data to deserialize
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're reading from
    size_t offset = 0;

    // // FIXME missing fields
    // // Copy fields from the buffer in the correct order
    // memcpy(&messageType, buffer + offset, sizeof(messageType));
    // offset += sizeof(messageType);
    // memcpy(&validTime, buffer + offset, sizeof(validTime));
    // offset += sizeof(validTime);
    // memcpy(&entityID, buffer + offset, sizeof(entityID));
    // offset += sizeof(entityID);
    // memcpy(&entityModelID, buffer + offset, sizeof(entityModelID));
    // offset += sizeof(entityModelID);
    // memcpy(&entityCapabilities, buffer + offset, sizeof(entityCapabilities));
    // offset += sizeof(entityCapabilities);
    // memcpy(&talkerStreamSources, buffer + offset, sizeof(talkerStreamSources));
    // offset += sizeof(talkerStreamSources);
    // memcpy(&talkerCapabilities, buffer + offset, sizeof(talkerCapabilities));
    // offset += sizeof(talkerCapabilities);
    // memcpy(&listenerStreamSinks, buffer + offset, sizeof(listenerStreamSinks));
    // offset += sizeof(listenerStreamSinks);
    // memcpy(&listenerCapabilities, buffer + offset, sizeof(listenerCapabilities));
    // offset += sizeof(listenerCapabilities);
    // memcpy(&availableIndex, buffer + offset, sizeof(availableIndex));
    // offset += sizeof(availableIndex);
    // memcpy(&gptpGrandmasterID, buffer + offset, sizeof(gptpGrandmasterID));
    // offset += sizeof(gptpGrandmasterID);
    // memcpy(&gptpDomainNumber, buffer + offset, sizeof(gptpDomainNumber));
    // offset += sizeof(gptpDomainNumber);
    // offset += (3); // 24 bits reserved
    // memcpy(&identifyControlIndex, buffer + offset, sizeof(identifyControlIndex));
    // offset += sizeof(identifyControlIndex);
    // memcpy(&interfaceIndex, buffer + offset, sizeof(interfaceIndex));
    // offset += sizeof(interfaceIndex);
    // memcpy(&associationID, buffer + offset, sizeof(associationID));
    // // 32 bits reserved
}

// Entry point for creating a new ADPDU instance
Adpdu* Adpdu::createRawAdpdu() noexcept
{
    return new Adpdu(); // Manual memory management in ESP-IDF (use with care)
}

// Destroy method to delete the current ADPDU instance
void Adpdu::destroy() noexcept
{
    delete this; // Free manually allocated memory
}