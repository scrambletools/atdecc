#include "protocolAdpdu.hpp"
#include <cstring> // for memcpy
#include <cassert> // for assert
#include "esp_log.h"

/***********************************************************/
/* Adpdu class definition                                  */
/***********************************************************/

// Constructor to initialize default ADPDU fields
Adpdu::Adpdu() noexcept
{
    // Initialize all fields to zero or default values
    messageType = 0;
    validTime = 0;
    entityID = 0;
    entityModelID = 0;
    entityCapabilities = 0;
    talkerStreamSources = 0;
    talkerCapabilities = 0;
    listenerStreamSinks = 0;
    listenerCapabilities = 0;
    availableIndex = 0;
    gptpGrandmasterID = 0;
    gptpDomainNumber = 0;
    identifyControlIndex = 0;
    interfaceIndex = 0;
    associationID = 0;
}

// Destructor
Adpdu::~Adpdu() noexcept = default;

// Serialize the ADPDU fields into a buffer for transmission
void Adpdu::serialize(uint8_t* buffer) const noexcept
{
    // Ensure that the buffer is large enough to hold the ADPDU
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're writing
    size_t offset = 0;

    // Copy fields into the buffer in the correct order
    memcpy(buffer + offset, &messageType, sizeof(messageType));
    offset += sizeof(messageType);
    memcpy(buffer + offset, &validTime, sizeof(validTime));
    offset += sizeof(validTime);
    memcpy(buffer + offset, &entityID, sizeof(entityID));
    offset += sizeof(entityID);
    memcpy(buffer + offset, &entityModelID, sizeof(entityModelID));
    offset += sizeof(entityModelID);
    memcpy(buffer + offset, &entityCapabilities, sizeof(entityCapabilities));
    offset += sizeof(entityCapabilities);
    memcpy(buffer + offset, &talkerStreamSources, sizeof(talkerStreamSources));
    offset += sizeof(talkerStreamSources);
    memcpy(buffer + offset, &talkerCapabilities, sizeof(talkerCapabilities));
    offset += sizeof(talkerCapabilities);
    memcpy(buffer + offset, &listenerStreamSinks, sizeof(listenerStreamSinks));
    offset += sizeof(listenerStreamSinks);
    memcpy(buffer + offset, &listenerCapabilities, sizeof(listenerCapabilities));
    offset += sizeof(listenerCapabilities);
    memcpy(buffer + offset, &availableIndex, sizeof(availableIndex));
    offset += sizeof(availableIndex);
    memcpy(buffer + offset, &gptpGrandmasterID, sizeof(gptpGrandmasterID));
    offset += sizeof(gptpGrandmasterID);
    memcpy(buffer + offset, &gptpDomainNumber, sizeof(gptpDomainNumber));
    offset += sizeof(gptpDomainNumber);
    memcpy(buffer + offset, &identifyControlIndex, sizeof(identifyControlIndex));
    offset += sizeof(identifyControlIndex);
    memcpy(buffer + offset, &interfaceIndex, sizeof(interfaceIndex));
    offset += sizeof(interfaceIndex);
    memcpy(buffer + offset, &associationID, sizeof(associationID));
}

// Deserialize the ADPDU from a buffer
void Adpdu::deserialize(const uint8_t* buffer)
{
    // Ensure that the buffer contains enough data to deserialize
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're reading from
    size_t offset = 0;

    // Copy fields from the buffer in the correct order
    memcpy(&messageType, buffer + offset, sizeof(messageType));
    offset += sizeof(messageType);
    memcpy(&validTime, buffer + offset, sizeof(validTime));
    offset += sizeof(validTime);
    memcpy(&entityID, buffer + offset, sizeof(entityID));
    offset += sizeof(entityID);
    memcpy(&entityModelID, buffer + offset, sizeof(entityModelID));
    offset += sizeof(entityModelID);
    memcpy(&entityCapabilities, buffer + offset, sizeof(entityCapabilities));
    offset += sizeof(entityCapabilities);
    memcpy(&talkerStreamSources, buffer + offset, sizeof(talkerStreamSources));
    offset += sizeof(talkerStreamSources);
    memcpy(&talkerCapabilities, buffer + offset, sizeof(talkerCapabilities));
    offset += sizeof(talkerCapabilities);
    memcpy(&listenerStreamSinks, buffer + offset, sizeof(listenerStreamSinks));
    offset += sizeof(listenerStreamSinks);
    memcpy(&listenerCapabilities, buffer + offset, sizeof(listenerCapabilities));
    offset += sizeof(listenerCapabilities);
    memcpy(&availableIndex, buffer + offset, sizeof(availableIndex));
    offset += sizeof(availableIndex);
    memcpy(&gptpGrandmasterID, buffer + offset, sizeof(gptpGrandmasterID));
    offset += sizeof(gptpGrandmasterID);
    memcpy(&gptpDomainNumber, buffer + offset, sizeof(gptpDomainNumber));
    offset += sizeof(gptpDomainNumber);
    memcpy(&identifyControlIndex, buffer + offset, sizeof(identifyControlIndex));
    offset += sizeof(identifyControlIndex);
    memcpy(&interfaceIndex, buffer + offset, sizeof(interfaceIndex));
    offset += sizeof(interfaceIndex);
    memcpy(&associationID, buffer + offset, sizeof(associationID));
}

// Copy method to create a deep copy of the current ADPDU instance
Adpdu* Adpdu::copy() const
{
    // Create a new Adpdu object and copy the current object's data into it
    Adpdu* newAdpdu = new Adpdu();
    memcpy(newAdpdu, this, sizeof(Adpdu));
    return newAdpdu;
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

// Create an "Entity Available" message
Adpdu Adpdu::createEntityAvailableMessage(uint64_t entityID, uint64_t entityModelID, uint32_t entityCapabilities)
{
    Adpdu adpdu;
    adpdu.setMessageType(static_cast<uint8_t>(AdpMessageType::ENTITY_AVAILABLE));
    adpdu.setEntityID(entityID);
    adpdu.setEntityModelID(entityModelID);
    adpdu.setEntityCapabilities(entityCapabilities);
    
    return adpdu;
}

// Create an "Entity Departing" message
Adpdu Adpdu::createEntityDepartingMessage(uint64_t entityID, uint32_t availableIndex)
{
    Adpdu adpdu;
    adpdu.setMessageType(static_cast<uint8_t>(AdpMessageType::ENTITY_DEPARTING));
    adpdu.setEntityID(entityID);
    adpdu.setAvailableIndex(availableIndex);
    return adpdu;
}

// Create an "Entity Discover" message
Adpdu Adpdu::createEntityDiscoverMessage()
{
    Adpdu adpdu;
    adpdu.setMessageType(static_cast<uint8_t>(AdpMessageType::ENTITY_DISCOVER));
    return adpdu;
}