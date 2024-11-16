
#include "protocolAvtpdu.hpp"
#include <cstring> // for memcpy
#include <cassert> // for assert
#include "esp_log.h"

/***********************************************************/
/* Ethernet header class definition                        */
/***********************************************************/

// Serialize the Ethernet fields into a buffer for transmission
void EtherLayer2::serialize(SerBuffer& buffer) const
{
    // Ensure that the buffer is large enough to hold the EtherHeader
    //assert(buffer != nullptr);

    // Offset to keep track of where in the buffer we're writing
    //size_t offset = 0;

    // Copy fields into the buffer in the correct order
    buffer.packBuffer(destAddress.data(), destAddress.size());
	buffer.packBuffer(srcAddress.data(), srcAddress.size());
	buffer << etherType;
}

// Deserialize the EtherHeader from a buffer
void EtherLayer2::deserialize(const uint8_t* buffer)
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
// EtherLayer2* EtherLayer2::copy() const
// {
//     // Create a new EtherHeader object and copy the current object's data into it
//     EtherLayer2* newEtherLayer2 = new EtherLayer2();
//     memcpy(newEtherLayer2, this, sizeof(EtherLayer2));
//     return newEtherLayer2;
// }

// Entry point for creating a new EtherHeader instance
// EtherLayer2* EtherLayer2::createRawEtherLayer2() noexcept
// {
//     return new EtherLayer2(); // Manual memory management in ESP-IDF (use with care)
// }

// Destroy method to delete the current EtherHeader instance
// void EtherLayer2::destroy() noexcept
// {
//     delete this; // Free manually allocated memory
// }

/**************************************************/
/* AVTPDU class definition                        */
/**************************************************/

// Constructor to initialize default AVTPDU fields
Avtpdu::Avtpdu() noexcept
{
    // set ethertype for avtp
    setEtherType(AVTP_ETHER_TYPE);
}

/***********************************************************/
/* AVTPDU control header class definition                  */
/***********************************************************/

// Constructor to initialize default AVTPDU control fields
AvtpduControl::AvtpduControl() noexcept
{
    // set cd and version at construction
    Avtpdu::setCD(true);
	Avtpdu::setVersion(AVTP_VERSION);
}

// Serialize the AVTPDU control fields into a buffer for transmission
void AvtpduControl::serialize(SerBuffer& buffer) const noexcept
{
    // Copy fields into the buffer in the correct order
    buffer << static_cast<std::uint8_t>(((cd << 7) & 0x80) | (subType & 0x7f));
	buffer << static_cast<std::uint8_t>(((headerSpecific << 7) & 0x80) | ((version << 4) & 0x70) | (controlData & 0x0f));
	buffer << static_cast<std::uint16_t>(((status << 11) & 0xf800) | (controlDataLength & 0x7ff));
	buffer << streamID;
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