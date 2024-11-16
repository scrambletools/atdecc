#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLAVTPDU_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLAVTPDU_HPP_

#pragma once

#include <stdint.h>
#include <string.h>
#include <string>
#include "entity.hpp"
#include "serialization.hpp"
#include "protocolDefines.hpp"

// Minimal implementation of Ethernet header for ESP-IDF
class EtherLayer2
{
public:
    // Ethernet header size as per ATDECC specification
    static constexpr size_t Length = 14; /* DestMacAddress + SrcMacAddress + EtherType */

		// Multicast MAC address used for AVB discovery messages
    const uint8_t Multicast_Mac_Address[6] = { 0x91, 0xe0, 0xf0, 0x01, 0x00, 0x00 };
    
    // Setters for Ethernet header fields
    void setDestAddress(MacAddress const& destAddress) noexcept { 
      this->destAddress = destAddress; 
    }
		void setSrcAddress(MacAddress const& srcAddress) noexcept { 
      this->srcAddress = srcAddress; 
    }
		void setEtherType(uint16_t const etherType) noexcept { 
      this->etherType = etherType;
    }
		
		// Getters for Ethernet header fields
    MacAddress getDestAddress() const noexcept { 
      return destAddress; 
    }
		MacAddress getSrcAddress() const noexcept { 
      return srcAddress; 
    }
		uint16_t getEtherType() const noexcept { 
      return etherType; 
    }

		// Serialization method to convert the header structure to a buffer for transmission
    void serialize(SerBuffer& buffer) const;

    // Deserialization method to populate the header structure from a buffer
    void deserialize(const uint8_t* buffer);

    // Note: Life cycle methods exposed for internal uses, not meant to be used outside the library
    /** Constructor */
    EtherLayer2() noexcept = default;

    /** Destructor */
    virtual ~EtherLayer2() noexcept = default;

protected:
    // Ethernet header fields
    MacAddress destAddress{}; // 48 bits
    MacAddress srcAddress{}; // 48 bits
    uint16_t etherType{};
};

// Minimal implementation of AVTPDU for ESP-IDF
class Avtpdu : public EtherLayer2
{
public:    
    // Setters for AVTPDU fields
    void setCD(bool const cd) noexcept { 
      this->cd = cd; 
    }
    void setSubType(uint8_t const subType) noexcept { 
      this->subType = subType; 
    }
		void setHeaderSpecific(bool const headerSpecific) noexcept { 
      this->headerSpecific = headerSpecific; 
    }
		void setVersion(uint8_t const version) noexcept { 
      this->version = version; 
    }

    // Getters for AVTPDU fields
    bool getCD() const noexcept { 
      return cd; 
    }
    uint8_t getSubType() const noexcept { 
      return subType; 
    }
		bool getHeaderSpecific() const noexcept { 
      return headerSpecific; 
    }
		uint8_t getVersion() const noexcept { 
      return version; 
    }

protected:
    /** Constructor */
    Avtpdu() noexcept;

    /** Destructor */
    virtual ~Avtpdu() noexcept override = default;

    // AVTPDU header fields
    bool cd{}; // control data
    uint8_t subType{};
    bool headerSpecific{}; // streamid valid
    uint8_t version{};

private:
	// Hide EtherLayer2 const data
	using EtherLayer2::setEtherType;
	using EtherLayer2::getEtherType;
};

// Minimal implementation of AVTPDU control for ESP-IDF
class AvtpduControl : public Avtpdu
{
public:
  // AVTPDU control header size as per ATDECC specification
  static constexpr size_t HeaderLength = 12; /* CD + SubType + StreamValid + Version + ControlData + Status + ControlDataLength + StreamID */
  
  // Setters for AVTPDU control fields
  void setStreamValid(bool const streamValid) noexcept { 
    Avtpdu::setHeaderSpecific(streamValid); 
  }
  void setControlData(uint8_t const controlData) noexcept { 
    //this->controlData = static_cast<uint8_t>(controlData); }
    this->controlData = controlData; 
  }
  void setStatus(uint8_t const status) noexcept { 
    this->status = status; 
  }
  void setControlDataLength(uint16_t const controlDataLength) noexcept { 
    this->controlDataLength = controlDataLength; 
  }
  void setStreamID(uint64_t const streamID) noexcept { 
    this->streamID = streamID; 
  }

  // Getters for AVTPDU control fields
  bool getStreamValid() const noexcept { 
    return Avtpdu::getHeaderSpecific(); 
  }
  uint8_t getControlData() const noexcept { 
    return controlData; 
  }
  uint8_t getStatus() const noexcept { 
    return status; 
  }
  uint16_t getControlDataLength() const noexcept { 
    return controlDataLength; 
  }
  uint64_t getStreamID() const noexcept { 
    return streamID; 
  }

  // Serialization method to convert the ADPDU control structure to a buffer for transmission
  void serialize(SerBuffer& buffer) const noexcept;

  // Deserialization method to populate the ADPDU control structure from a buffer
  void deserialize(const uint8_t* buffer);

protected:
	/** Constructor */
	AvtpduControl() noexcept;

	/**  Destructor */
	virtual ~AvtpduControl() noexcept override = default;

  // AVTPDU control header fields
  uint8_t  controlData{0};
  uint8_t  status{0};
  uint16_t controlDataLength{0};
  uint64_t streamID{};

private:
	// Hide renamed Avtpdu data
	using Avtpdu::setHeaderSpecific;
	using Avtpdu::getHeaderSpecific;
	// Hide Avtpdu const data
	using Avtpdu::setCD;
	using Avtpdu::setVersion;
	using Avtpdu::getCD;
	using Avtpdu::getVersion;
};

/** Serialization helper template */
template<class FrameType, typename... Ts>
void serialize(EtherLayer2 const& frame, Ts&&... params)
{
	static_cast<FrameType const&>(frame).serialize(std::forward<Ts>(params)...);
}

/** Deserialization helper template */
template<class FrameType, typename... Ts>
void deserialize(EtherLayer2* const frame, Ts&&... params)
{
	static_cast<FrameType*>(frame)->deserialize(std::forward<Ts>(params)...);
}

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLAVTPDU_HPP_ */