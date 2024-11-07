#pragma once

#include <stdint.h>
#include <string.h>
#include <string>
#include "protocolDefines.hpp"

/** Ethernet frame payload minimum size */
static constexpr size_t EthernetPayloadMinimumSize = 46;

// Minimal implementation of Ethernet header for ESP-IDF
class EtherHeader
{
public:
    // Ethernet header size as per ATDECC specification
    static constexpr size_t Length = 14; /* DestMacAddress + SrcMacAddress + EtherType */

		// Multicast MAC address used for AVB discovery messages
    const uint8_t Multicast_Mac_Address[6] = { 0x91, 0xe0, 0xf0, 0x01, 0x00, 0x00 };

    // Constructor to initialize default Ethernet header fields
    EtherHeader() noexcept;

    // Destructor
    ~EtherHeader() noexcept;
    
    // Setters for Ethernet header fields
    void setDestAddress(uint8_t destAddress) noexcept { this->destAddress = static_cast<uint8_t>(destAddress); }
		void setSrcAddress(uint8_t srcAddress) noexcept { this->srcAddress = static_cast<uint8_t>(srcAddress); }
		void setEtherType(uint8_t etherType) noexcept { this->etherType = static_cast<uint8_t>(etherType); }
		
		// Getters for Ethernet header fields
    uint8_t getDestAddress() const noexcept { return destAddress; }
		uint8_t getSrcAddress() const noexcept { return srcAddress; }
		uint8_t getEtherType() const noexcept { return etherType; }

		// Serialization method to convert the header structure to a buffer for transmission
    void serialize(uint8_t* buffer) const noexcept;

    // Deserialization method to populate the header structure from a buffer
    void deserialize(const uint8_t* buffer);

    // Copy method to create a deep copy of the current header instance
    EtherHeader* copy() const;

    // Entry point for creating a new header instance
    static EtherHeader* createRawEtherHeader() noexcept;

    // Destroy method to delete the current header instance
    void destroy() noexcept;

//private:
    // Ethernet header fields
    uint64_t destAddress = 0; // 48 bits
    uint64_t srcAddress = 0; // 48 bits
    uint16_t etherType = 0;
};

// Minimal implementation of AVTPDU for ESP-IDF
class Avtpdu
{
public:
    // Constructor to initialize default AVTPDU fields
    Avtpdu() noexcept;

    // Destructor
    ~Avtpdu() noexcept;
    
    // Setters for AVTPDU fields
    void setCD(bool cd) noexcept { this->cd = static_cast<uint8_t>(cd); }
    void setSubType(uint8_t subType) noexcept { this->subType = subType; }
		void setHeaderSpecific(bool headerSpecific) noexcept { this->headerSpecific = headerSpecific; }
		void setVersion(uint8_t version) noexcept { this->version = version; }

    // Getters for AVTPDU fields
    bool getCD() const noexcept { return cd; }
    uint8_t getSubType() const noexcept { return subType; }
		bool getHeaderSpecific() const noexcept { return headerSpecific; }
		uint8_t getVersion() const noexcept { return version; }

    // Serialization method to convert the ADPDU structure to a buffer for transmission
    void serialize(uint8_t* buffer) const noexcept;

    // Deserialization method to populate the ADPDU structure from a buffer
    void deserialize(const uint8_t* buffer);

    // Copy method to create a deep copy of the current ADPDU instance
    Avtpdu* copy() const;

    // Entry point for creating a new AVTPDU instance
    static Avtpdu* createRawAvtpdu() noexcept;

    // Destroy method to delete the current ADPDU instance
    void destroy() noexcept;

//private:
    // AVTPDU header fields
    bool cd = 0;
    uint8_t subType = 0;
    bool headerSpecific = 0;
    uint64_t version = 0;
};

// Minimal implementation of AVTPDU control for ESP-IDF
class AvtpduControl
{
public:
    // AVTPDU control header size as per ATDECC specification
    static constexpr size_t Length = 12; /* CD + SubType + StreamValid + Version + ControlData + Status + ControlDataLength + StreamID */

    // Constructor to initialize default AVTPDU control fields
    AvtpduControl() noexcept;

    // Destructor
    ~AvtpduControl() noexcept;
    
    // Setters for AVTPDU control fields
    void setControlData(uint8_t controlData) noexcept { this->controlData = static_cast<uint8_t>(controlData); }
    void setStatus(uint8_t status) noexcept { this->status = status; }
		void setControlDataLength(uint8_t controlDataLength) noexcept { this->controlDataLength = controlDataLength; }
		void setStreamID(uint8_t streamID) noexcept { this->streamID = streamID; }

    // Getters for AVTPDU control fields
    uint8_t getControlData() const noexcept { return controlData; }
    uint8_t getStatus() const noexcept { return status; }
		uint8_t getControlDataLength() const noexcept { return controlDataLength; }
		uint8_t getStreamID() const noexcept { return streamID; }

    // Serialization method to convert the ADPDU control structure to a buffer for transmission
    void serialize(uint8_t* buffer) const noexcept;

    // Deserialization method to populate the ADPDU control structure from a buffer
    void deserialize(const uint8_t* buffer);

    // Copy method to create a deep copy of the current ADPDU control instance
    AvtpduControl* copy() const;

    // Entry point for creating a new ADPDU control instance
    static AvtpduControl* createRawAvtpduControl() noexcept;

    // Destroy method to delete the current ADPDU control instance
    void destroy() noexcept;

//private:
    // AVTPDU control header fields
    uint8_t  controlData = 0;
		uint8_t  status = 0;
    uint16_t controlDataLength = 0;
    uint64_t streamID = 0;
};