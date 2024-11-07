
#include "protocolAvtpdu.hpp"
#include "protocolDefines.hpp"
#include <cstring> // for memcpy
#include <cassert> // for assert
#include "esp_log.h"

/***********************************************************/
/* Ethernet header class definition                        */
/***********************************************************/

// Constructor to initialize default EtherHeader fields
EtherHeader::EtherHeader() noexcept
{
    // Initialize all fields to zero or default values
    destAddress = 0;
    srcAddress = 0;
    etherType = 0;
}

// Destructor
EtherHeader::~EtherHeader() noexcept = default;

// Serialize the EtherHeader fields into a buffer for transmission
void EtherHeader::serialize(uint8_t* buffer) const noexcept
{
    // Ensure that the buffer is large enough to hold the EtherHeader
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're writing
    size_t offset = 0;

    // Copy fields into the buffer in the correct order
    memcpy(buffer + offset, &destAddress, sizeof(destAddress));
    offset += sizeof(destAddress);
    memcpy(buffer + offset, &srcAddress, sizeof(srcAddress));
    offset += sizeof(srcAddress);
    memcpy(buffer + offset, &etherType, sizeof(etherType));
}

// Deserialize the EtherHeader from a buffer
void EtherHeader::deserialize(const uint8_t* buffer)
{
    // Ensure that the buffer contains enough data to deserialize
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're reading from
    size_t offset = 0;

    // Copy fields from the buffer in the correct order
    memcpy(&destAddress, buffer + offset, sizeof(destAddress));
    offset += sizeof(destAddress);
    memcpy(&srcAddress, buffer + offset, sizeof(srcAddress));
    offset += sizeof(srcAddress);
    memcpy(&etherType, buffer + offset, sizeof(etherType));
}

// Copy method to create a deep copy of the current EtherHeader instance
EtherHeader* EtherHeader::copy() const
{
    // Create a new EtherHeader object and copy the current object's data into it
    EtherHeader* newEtherHeader = new EtherHeader();
    memcpy(newEtherHeader, this, sizeof(EtherHeader));
    return newEtherHeader;
}

// Entry point for creating a new EtherHeader instance
EtherHeader* EtherHeader::createRawEtherHeader() noexcept
{
    return new EtherHeader(); // Manual memory management in ESP-IDF (use with care)
}

// Destroy method to delete the current EtherHeader instance
void EtherHeader::destroy() noexcept
{
    delete this; // Free manually allocated memory
}

/**************************************************/
/* AVTPDU class definition                        */
/**************************************************/

// Constructor to initialize default AVTPDU fields
Avtpdu::Avtpdu() noexcept
{
    // Initialize all fields to zero or default values
    cd = 0;
    subType = 0;
    headerSpecific = 0;
    version = 0;
}

// Destructor
Avtpdu::~Avtpdu() noexcept = default;

// Serialize the AVTPDU fields into a buffer for transmission
void Avtpdu::serialize(uint8_t* buffer) const noexcept
{
    // Ensure that the buffer is large enough to hold the AVTPDU
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're writing
    size_t offset = 0;

    // Copy fields into the buffer in the correct order
    memcpy(buffer + offset, &cd, sizeof(cd));
    offset += sizeof(cd);
    memcpy(buffer + offset, &subType, sizeof(subType));
    offset += sizeof(subType);
    memcpy(buffer + offset, &headerSpecific, sizeof(headerSpecific));
		offset += sizeof(headerSpecific);
    memcpy(buffer + offset, &version, sizeof(version));
}

// Deserialize the AVTPDU from a buffer
void Avtpdu::deserialize(const uint8_t* buffer)
{
    // Ensure that the buffer contains enough data to deserialize
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're reading from
    size_t offset = 0;

    // Copy fields from the buffer in the correct order
    memcpy(&cd, buffer + offset, sizeof(cd));
    offset += sizeof(cd);
    memcpy(&subType, buffer + offset, sizeof(subType));
    offset += sizeof(subType);
    memcpy(&headerSpecific, buffer + offset, sizeof(headerSpecific));
		offset += sizeof(headerSpecific);
    memcpy(&version, buffer + offset, sizeof(version));
}

// Copy method to create a deep copy of the current AVTPDU instance
Avtpdu* Avtpdu::copy() const
{
    // Create a new Adpdu object and copy the current object's data into it
    Avtpdu* newAvtpdu = new Avtpdu();
    memcpy(newAvtpdu, this, sizeof(Avtpdu));
    return newAvtpdu;
}

// Entry point for creating a new AVTPDU instance
Avtpdu* Avtpdu::createRawAvtpdu() noexcept
{
    return new Avtpdu(); // Manual memory management in ESP-IDF (use with care)
}

// Destroy method to delete the current AVTPDU instance
void Avtpdu::destroy() noexcept
{
    delete this; // Free manually allocated memory
}

/***********************************************************/
/* AVTPDU control header class definition                  */
/***********************************************************/

// Constructor to initialize default AVTPDU control fields
AvtpduControl::AvtpduControl() noexcept
{
    // Initialize all fields to zero or default values
    controlData = 0;
    status = 0;
    controlDataLength = 0;
    streamID = 0;
}

// Destructor
AvtpduControl::~AvtpduControl() noexcept = default;

// Serialize the AVTPDU control fields into a buffer for transmission
void AvtpduControl::serialize(uint8_t* buffer) const noexcept
{
    // Ensure that the buffer is large enough to hold the AVTPDU control
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're writing
    size_t offset = 0;

    // Reserved fields
    uint32_t reserved = 0;

    // Copy fields into the buffer in the correct order
    memcpy(buffer + offset, &controlData, sizeof(controlData));
    offset += sizeof(controlData);
    memcpy(buffer + offset, &status, sizeof(status));
    offset += sizeof(status);
    memcpy(buffer + offset, &controlDataLength, sizeof(controlDataLength));
		offset += sizeof(controlDataLength);
    memcpy(buffer + offset, &streamID, sizeof(streamID));
}

// Deserialize the AVTPDU control from a buffer
void AvtpduControl::deserialize(const uint8_t* buffer)
{
    // Ensure that the buffer contains enough data to deserialize
    assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're reading from
    size_t offset = 0;

    // Copy fields from the buffer in the correct order
    memcpy(&controlData, buffer + offset, sizeof(controlData));
    offset += sizeof(controlData);
    memcpy(&status, buffer + offset, sizeof(status));
    offset += sizeof(status);
    memcpy(&controlDataLength, buffer + offset, sizeof(controlDataLength));
		offset += sizeof(controlDataLength);
    memcpy(&streamID, buffer + offset, sizeof(streamID));
}

// Copy method to create a deep copy of the current AVTPDU control instance
AvtpduControl* AvtpduControl::copy() const
{
    // Create a new Adpdu object and copy the current object's data into it
    AvtpduControl* newAvtpduControl = new AvtpduControl();
    memcpy(newAvtpduControl, this, sizeof(AvtpduControl));
    return newAvtpduControl;
}

// Entry point for creating a new AVTPDU control instance
AvtpduControl* AvtpduControl::createRawAvtpduControl() noexcept
{
    return new AvtpduControl(); // Manual memory management in ESP-IDF (use with care)
}

// Destroy method to delete the current AVTPDU control instance
void AvtpduControl::destroy() noexcept
{
    delete this; // Free manually allocated memory
}