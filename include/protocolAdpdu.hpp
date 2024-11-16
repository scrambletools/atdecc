#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLADPDU_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLADPDU_HPP_

#pragma once

#include <stdint.h>
#include <string.h>
#include <string>
#include "protocolAvtpdu.hpp"

// Minimal implementation of ADPDU for ESP-IDF
class Adpdu : public AvtpduControl
{
public:
    // ADPDU control data size as per ATDECC spec
    static constexpr size_t Length = 56;

    using UniquePointer = std::unique_ptr<Adpdu, void (*)(Adpdu*)>;

    // Multicast MAC address used for AVB discovery messages
    static MacAddress const Multicast_Mac_Address;

	/**
	* @brief Factory method to create a new Adpdu.
	* @details Creates a new Adpdu as a unique pointer.
	* @return A new Adpdu as a Adpdu::UniquePointer.
	*/
	static UniquePointer create() noexcept
	{
		auto deleter = [](Adpdu* self)
		{
			self->destroy();
		};
		return UniquePointer(createRawAdpdu(), deleter);
	}

    // Constructor to initialize default ADPDU fields
    Adpdu() noexcept;

    // Destructor
    ~Adpdu() noexcept;
    
    // Setters for ADPDU fields
    void setMessageType(AdpMessageType const messageType) noexcept { 
        //this->messageType = static_cast<uint8_t>(messageType);
        AvtpduControl::setControlData((uint8_t)messageType);
    }
    void setValidTime(uint8_t const validTime) noexcept { 
        AvtpduControl::setStatus(validTime);
    }
    void setEntityID(UniqueIdentifier const entityID) noexcept { 
        AvtpduControl::setStreamID(entityID.getValue()); 
    }
    void setEntityModelID(UniqueIdentifier const entityModelID) noexcept { 
        this->entityModelID = entityModelID; 
    }
    void setEntityCapabilities(EntityCapabilities const capabilities) noexcept { 
        this->entityCapabilities = capabilities; 
    }
    void setTalkerStreamSources(uint16_t const sources) noexcept { 
        this->talkerStreamSources = sources; 
    }
    void setTalkerCapabilities(TalkerCapabilities const capabilities) noexcept { 
        this->talkerCapabilities = capabilities; 
    }
    void setListenerStreamSinks(uint16_t const sinks) noexcept { 
        this->listenerStreamSinks = sinks; 
    }
    void setListenerCapabilities(ListenerCapabilities const capabilities) noexcept { 
        this->listenerCapabilities = capabilities; 
    }
    void setControllerCapabilities(ControllerCapabilities const capabilities) noexcept { 
        this->controllerCapabilities = capabilities; 
    }
    void setAvailableIndex(uint32_t const index) noexcept { 
        this->availableIndex = index; 
    }
    void setGptpGrandmasterID(UniqueIdentifier const id) noexcept { 
        this->gptpGrandmasterID = id; 
    }
    void setGptpDomainNumber(uint8_t const number) noexcept { 
        this->gptpDomainNumber = number; 
    }
    // void setCurrentConfigurationIndex(uint16_t const index) noexcept { 
    //     this->currentConfigurationIndex = index; 
    // }
    void setIdentifyControlIndex(ControlIndex const index) noexcept { 
        this->identifyControlIndex = index; 
    }
    void setInterfaceIndex(AvbInterfaceIndex const index) noexcept { 
        this->interfaceIndex = index; 
    }
    void setAssociationID(UniqueIdentifier const id) noexcept { 
        this->associationID = id; 
    }

    // Getters for ADPDU fields
    AdpMessageType getMessageType() const noexcept { 
        return AdpMessageType(AvtpduControl::getControlData());
    }
    uint8_t getValidTime() const noexcept { 
        return AvtpduControl::getStatus();
    }
    UniqueIdentifier getEntityID() const noexcept { 
        return UniqueIdentifier{ AvtpduControl::getStreamID() };
    }
    UniqueIdentifier getEntityModelID() const noexcept { 
        return entityModelID; 
    }
    EntityCapabilities getEntityCapabilities() const noexcept { 
        return entityCapabilities; 
    }
    uint16_t getTalkerStreamSources() const noexcept { 
        return talkerStreamSources; 
    }
    TalkerCapabilities getTalkerCapabilities() const noexcept { 
        return talkerCapabilities; 
    }
    uint16_t getListenerStreamSinks() const noexcept { 
        return listenerStreamSinks;
    }
    ListenerCapabilities getListenerCapabilities() const noexcept { 
        return listenerCapabilities; 
    }
    ControllerCapabilities getControllerCapabilities() const noexcept { 
        return controllerCapabilities; 
    }
    uint32_t getAvailableIndex() const noexcept { 
        return availableIndex; 
    }
    UniqueIdentifier getGptpGrandmasterID() const noexcept { 
        return gptpGrandmasterID; 
    }
    uint8_t getGptpDomainNumber() const noexcept { 
        return gptpDomainNumber; 
    }
    // uint16_t getCurrentConfigurationIndex() const noexcept { 
    //     return currentConfigurationIndex; 
    //}
    ControlIndex getIdentifyControlIndex() const noexcept { 
        return identifyControlIndex; 
    }
    AvbInterfaceIndex getInterfaceIndex() const noexcept { 
        return interfaceIndex; 
    }
    UniqueIdentifier getAssociationID() const noexcept { 
        return associationID; 
    }

    // Serialization method to convert the ADPDU structure to a buffer for transmission
    //void serialize(uint8_t* buffer) const noexcept;
    void serialize(SerBuffer& buffer) const;

    // Deserialization method to populate the ADPDU structure from a buffer
    void deserialize(const uint8_t* buffer);

    // Copy method to create a deep copy of the current ADPDU instance
    //Adpdu* copy() const;

    /** Copy method */
	UniquePointer copy() const;

    // Entry point for creating a new ADPDU instance
    static Adpdu* createRawAdpdu() noexcept;

    // Destroy method to delete the current ADPDU instance
    void destroy() noexcept;

    // Adpdu header data
	UniqueIdentifier entityModelID{};
	EntityCapabilities entityCapabilities{};
	uint16_t talkerStreamSources{};
	TalkerCapabilities talkerCapabilities{};
	uint16_t listenerStreamSinks{};
	ListenerCapabilities listenerCapabilities{};
	ControllerCapabilities controllerCapabilities{};
	uint32_t availableIndex{};
	UniqueIdentifier gptpGrandmasterID{};
	uint8_t gptpDomainNumber{ 0u };
	// Reserved 24bits
	ControlIndex identifyControlIndex{ 0u };
	AvbInterfaceIndex interfaceIndex{ 0u };
	UniqueIdentifier associationID{};
	// Reserved 32bits

private:
	// Hide renamed AvtpduControl data
	using AvtpduControl::setControlData;
	using AvtpduControl::setControlDataLength;
	using AvtpduControl::setStatus;
	using AvtpduControl::setStreamID;
	using AvtpduControl::getControlData;
	using AvtpduControl::getControlDataLength;
	using AvtpduControl::getStatus;
	using AvtpduControl::getStreamID;
	// Hide Avtpdu const data
	using Avtpdu::setSubType;
	using Avtpdu::getSubType;
	// Hide AvtpduControl const data
	using AvtpduControl::setStreamValid;
	using AvtpduControl::getStreamValid;
};

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLADPDU_HPP_ */