#pragma once

#include <stdint.h>
#include <string.h>
#include <string>
#include "protocolDefines.hpp"

// Minimal implementation for ADPDU without AVTPDU for ESP-IDF
class Adpdu
{
public:
    // ADPDU size as per ATDECC specification
    static constexpr size_t Length = 56;

    // Multicast MAC address used for AVB discovery messages
    const uint8_t Multicast_Mac_Address[6] = { 0x91, 0xe0, 0xf0, 0x01, 0x00, 0x00 };

    // Constructor to initialize default ADPDU fields
    Adpdu() noexcept;

    // Destructor
    ~Adpdu() noexcept;

    // Setters for ADPDU fields
    void setMessageType(uint8_t messageType) noexcept { this->messageType = static_cast<uint8_t>(messageType); }
    void setValidTime(uint8_t validTime) noexcept { this->validTime = validTime; }
    void setEntityID(uint64_t entityID) noexcept { this->entityID = entityID; }
    void setEntityModelID(uint64_t entityModelID) noexcept { this->entityModelID = entityModelID; }
    void setEntityCapabilities(uint32_t capabilities) noexcept { this->entityCapabilities = capabilities; }
    void setTalkerStreamSources(uint16_t sources) noexcept { this->talkerStreamSources = sources; }
    void setTalkerCapabilities(uint16_t capabilities) noexcept { this->talkerCapabilities = capabilities; }
    void setListenerStreamSinks(uint16_t sinks) noexcept { this->listenerStreamSinks = sinks; }
    void setListenerCapabilities(uint16_t capabilities) noexcept { this->listenerCapabilities = capabilities; }
    void setAvailableIndex(uint32_t index) noexcept { this->availableIndex = index; }
    void setGptpGrandmasterID(uint64_t id) noexcept { this->gptpGrandmasterID = id; }
    void setGptpDomainNumber(uint8_t number) noexcept { this->gptpDomainNumber = number; }
    void setIdentifyControlIndex(uint16_t index) noexcept { this->identifyControlIndex = index; }
    void setInterfaceIndex(uint16_t index) noexcept { this->interfaceIndex = index; }
    void setAssociationID(uint64_t id) noexcept { this->associationID = id; }

    // Getters for ADPDU fields
    uint8_t getMessageType() const noexcept { return messageType; }
    uint8_t getValidTime() const noexcept { return validTime; }
    uint64_t getEntityID() const noexcept { return entityID; }
    uint64_t getEntityModelID() const noexcept { return entityModelID; }
    uint32_t getEntityCapabilities() const noexcept { return entityCapabilities; }
    uint16_t getTalkerStreamSources() const noexcept { return talkerStreamSources; }
    uint16_t getTalkerCapabilities() const noexcept { return talkerCapabilities; }
    uint16_t getListenerStreamSinks() const noexcept { return listenerStreamSinks; }
    uint16_t getListenerCapabilities() const noexcept { return listenerCapabilities; }
    uint32_t getAvailableIndex() const noexcept { return availableIndex; }
    uint64_t getGptpGrandmasterID() const noexcept { return gptpGrandmasterID; }
    uint8_t getGptpDomainNumber() const noexcept { return gptpDomainNumber; }
    uint16_t getIdentifyControlIndex() const noexcept { return identifyControlIndex; }
    uint16_t getInterfaceIndex() const noexcept { return interfaceIndex; }
    uint64_t getAssociationID() const noexcept { return associationID; }

    // Serialization method to convert the ADPDU structure to a buffer for transmission
    void serialize(uint8_t* buffer) const noexcept;

    // Deserialization method to populate the ADPDU structure from a buffer
    void deserialize(const uint8_t* buffer);

    // Copy method to create a deep copy of the current ADPDU instance
    Adpdu* copy() const;

    // Entry point for creating a new ADPDU instance
    static Adpdu* createRawAdpdu() noexcept;

    // Destroy method to delete the current ADPDU instance
    void destroy() noexcept;
    
    // Utility methods to easily create and send messages
    static Adpdu createEntityAvailableMessage(uint64_t entityID, uint64_t entityModelID, uint32_t entityCapabilities);
    static Adpdu createEntityDepartingMessage(uint64_t entityID, uint32_t availableIndex);
    static Adpdu createEntityDiscoverMessage();

//private:
    // ADPDU header fields
    uint8_t messageType = 0;
    uint8_t validTime = 0;
    uint64_t entityID = 0;
    uint64_t entityModelID = 0;
    uint32_t entityCapabilities = 0;
    uint16_t talkerStreamSources = 0;
    uint16_t talkerCapabilities = 0;
    uint16_t listenerStreamSinks = 0;
    uint16_t listenerCapabilities = 0;
    uint32_t availableIndex = 0;
    uint64_t gptpGrandmasterID = 0;
    uint8_t gptpDomainNumber = 0;
    uint16_t identifyControlIndex = 0;
    uint16_t interfaceIndex = 0;
    uint64_t associationID = 0;
};