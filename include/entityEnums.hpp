/*
 * entityEnums.hpp
 *
 *  Created on: Sep 28, 2024
 *      Author: Dulini
 */

#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYENUMS_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYENUMS_HPP_

#pragma once

#include <stdint.h>  // Standard integer types
#include <string>    // For string type
#include "entityModelTypes.hpp"

// Define bitfield traits as C-style enums
typedef enum {
    TestBitfieldEnumTrait_None = 0u,
    TestBitfieldEnumTrait_Implemented = 1u << 0, // 1
    TestBitfieldEnumTrait_Supported = 1u << 1,   // 2
    TestBitfieldEnumTrait_NotSupported = 1u << 2, // 4
    TestBitfieldEnumTrait_Other = 1u << 3        // 8
} TestBitfieldEnumTrait;

typedef enum {
    TestEnum_None = 0,
    TestEnum_First = 1,
    TestEnum_Other = 4,
} TestEnum;

// Hash map entry structure for bitfield to string mapping
typedef struct {
    uint8_t bitfield;
    const char *name;
} BitfieldToStringMapEntry;

// Bitfield to string map
static const BitfieldToStringMapEntry bitfieldToStringMap[] = {
    { TestBitfieldEnumTrait_None, "None" },
    { TestBitfieldEnumTrait_Implemented, "Implemented" },
    { TestBitfieldEnumTrait_Other, "Other" }
};

// Utility class for Enum Bitfield operations
template <typename EnumType>
class EnumBitfield
{
public:
    EnumBitfield() : value(0) {}
    EnumBitfield(EnumType flag) : value(static_cast<uint32_t>(flag)) {}

    // Set a flag
    void setFlag(EnumType flag)
    {
        value |= static_cast<uint32_t>(flag);
    }

    // Check if a flag is set
    bool hasFlag(EnumType flag) const
    {
        return (value & static_cast<uint32_t>(flag)) != 0;
    }

    // Remove a flag
    void removeFlag(EnumType flag)
    {
        value &= ~static_cast<uint32_t>(flag);
    }

    // Get the raw value
    uint32_t getValue() const
    {
        return value;
    }

private:
    uint32_t value;
};

// ADP Entity Capabilities - Clause 6.2.1.10
enum class EntityCapability : uint32_t
{
    None = 0u,
    EfuMode = 1u << 0,            // Entity Firmware Upgrade mode is enabled on the ATDECC Entity.
    AddressAccessSupported = 1u << 1,    // Supports receiving the ADDRESS_ACCESS commands.
    GatewayEntity = 1u << 2,      // Serves as a gateway to a device on another type of media.
    AemSupported = 1u << 3,       // Supports receiving the ATDECC Entity Model (AEM) AECP commands.
    LegacyAvc = 1u << 4,          // Supports using IEEE Std 1394 AV/C protocol.
    AssociationIDSupported = 1u << 5,     // Supports the use of the association_id field.
    AssociationIDValid = 1u << 6, // The association_id field contains a valid value.
    VendorUniqueSupported = 1u << 7,      // Supports receiving the AEM VENDOR_UNIQUE commands.
    ClassASupported = 1u << 8,    // Supports sending and/or receiving Class A Streams.
    ClassBSupported = 1u << 9,    // Supports sending and/or receiving Class B Streams.
    GptpSupported = 1u << 10,     // Implements IEEE Std 802.1AS-2011.
    AemAuthenticationSupported = 1u << 11,    // Supports using AEM Authentication.
    AemAuthenticationRequired = 1u << 12,     // Requires the use of AEM Authentication.
    AemPersistentAcquireSupported = 1u << 13, // Supports the use of the PERSISTENT flag.
    AemIdentifyControlIndexValid = 1u << 14,  // Contains a valid index of an AEM CONTROL descriptor.
    AemInterfaceIndexValid = 1u << 15,        // Contains a valid index of an AEM AVB_INTERFACE descriptor.
    GeneralControllerIgnore = 1u << 16,       // General purpose ATDECC Controllers ignore this ATDECC Entity.
    EntityNotReady = 1u << 17     // The ATDECC Entity is not ready to be enumerated or connected.
};
using EntityCapabilities = EnumBitfield<EntityCapability>;

// ADP Talker Capabilities - Clause 6.2.1.12
enum class TalkerCapability : uint16_t
{
    None = 0u,
    Implemented = 1u << 0,            // Implements an ATDECC Talker.
    OtherSource = 1u << 9,            // Has other Stream sources not covered by the following.
    ControlSource = 1u << 10,         // Has Control Stream sources.
    MediaClockSource = 1u << 11,      // Has Media Clock Stream sources.
    SmpteSource = 1u << 12,           // Has SMPTE time code Stream sources.
    MidiSource = 1u << 13,            // Has MIDI Stream sources.
    AudioSource = 1u << 14,           // Has Audio Stream sources.
    VideoSource = 1u << 15            // Has Video Stream sources.
};
using TalkerCapabilities = EnumBitfield<TalkerCapability>;

// ADP Listener Capabilities - Clause 6.2.1.14
enum class ListenerCapability : uint16_t
{
    None = 0u,
    Implemented = 1u << 0,            // Implements an ATDECC Listener.
    OtherSink = 1u << 9,              // Has other Stream sinks not covered by the following.
    ControlSink = 1u << 10,           // Has Control Stream sinks.
    MediaClockSink = 1u << 11,        // Has Media Clock Stream sinks.
    SmpteSink = 1u << 12,             // Has SMPTE time code Stream sinks.
    MidiSink = 1u << 13,              // Has MIDI Stream sinks.
    AudioSink = 1u << 14,             // Has Audio Stream sinks.
    VideoSink = 1u << 15              // Has Video Stream sinks.
};
using ListenerCapabilities = EnumBitfield<ListenerCapability>;

// ADP Controller Capabilities - Clause 6.2.1.15
enum class ControllerCapability : uint32_t
{
    None = 0u,
    Implemented = 1u << 0           // Implements an ATDECC Controller.
};
using ControllerCapabilities = EnumBitfield<ControllerCapability>;

// ConnectionFlags - Clause 8.2.1.17
enum class ConnectionFlag : uint16_t
{
    None = 0u,
    ClassB = 1u << 0,               // Indicates that the Stream is Class B.
    FastConnect = 1u << 1,          // Fast Connect Mode.
    SavedState = 1u << 2,           // Connection has saved state.
    StreamingWait = 1u << 3,        // The ATDECC Talker does not start streaming until told.
    SupportsEncrypted = 1u << 4,    // Indicates that the Stream supports streaming with encrypted PDUs.
    EncryptedPdu = 1u << 5,         // Indicates that the Stream is using encrypted PDUs.
    TalkerFailed = 1u << 6          // Indicates that the listener has registered an SRP Talker Failed attribute.
};
using ConnectionFlags = EnumBitfield<ConnectionFlag>;

// StreamFlags - Clause 7.2.6.1
enum class StreamFlag : uint16_t
{
    None = 0u,
    ClockSyncSource = 1u << 0,         // Can be used as a clock synchronization source.
    ClassA = 1u << 1,                  // Supports streaming at Class A.
    ClassB = 1u << 2,                  // Supports streaming at Class B.
    SupportsEncrypted = 1u << 3,       // Supports streaming with encrypted PDUs.
    PrimaryBackupSupported = 1u << 4,  // Backup fields are supported.
    PrimaryBackupValid = 1u << 5,      // Backup fields are valid.
    SecondaryBackupSupported = 1u << 6, // Backup fields are supported.
    SecondaryBackupValid = 1u << 7,    // Backup fields are valid.
    TertiaryBackupSupported = 1u << 8, // Backup fields are supported.
    TertiaryBackupValid = 1u << 9      // Backup fields are valid.
};
using StreamFlags = EnumBitfield<StreamFlag>;

/** JackFlags - Clause 7.2.7.1 */
enum class JackFlag : uint16_t
{
	None = 0u,
	ClockSyncSource = 1u << 0, /**< Indicates that the Jack can be used as a clock synchronization source. */
	Captive = 1u << 1, /**< Indicates that the Jack connection is hardwired, cannot be disconnected and may be physically within the device's structure. */
	/* Bits 0 to 13 reserved for future use */
};
using JackFlags = EnumBitfield<JackFlag>;

/** AvbInterfaceFlags - Clause 7.2.8.1 */
enum class AvbInterfaceFlag : uint16_t
{
	None = 0u,
	GptpGrandmasterSupported = 1u << 0, /**< Indicates that the interface supports the IEEE Std 802.1AS-2011 grandmaster functionality. */
	GptpSupported = 1u << 1, /**< Indicates that the interface supports the IEEE Std 802.1AS-2011 functionality. */
	SrpSupported = 1u << 2, /**< Indicates that the interface supports Clause 35 of IEEE Std 802.1Q-2011, "Stream Reservation Protocol (SRP)" functionality. */
	/* Bits 0 to 12 reserved for future use */
};
using AvbInterfaceFlags = EnumBitfield<AvbInterfaceFlag>;

/** ClockSourceFlags - Clause 7.2.9.1 */
enum class ClockSourceFlag : uint16_t
{
	None = 0u,
	StreamID = 1u << 0, /**< The INPUT_STREAM Clock Source is identified by the stream_id. */
	LocalID = 1u << 1, /**< The INPUT_STREAM Clock Source is identified by its local ID. */
	/* Bits 0 to 13 reserved for future use */
};
using ClockSourceFlags = EnumBitfield<ClockSourceFlag>;

/** PortFlags - Clause 7.2.13.1 */
enum class PortFlag : uint16_t
{
	None = 0u,
	ClockSyncSource = 1u << 0, /**< Indicates that the Port can be used as a clock synchronization source. */
	AsyncSampleRateConv = 1u << 1, /**< Indicates that the Port has an asynchronous sample rate convertor to convert sample rates between another Clock Domain and the Unit's. */
	SyncSampleRateConv = 1u << 2, /**< Indicates that the Port has a synchronous sample rate convertor to convert between sample rates in the same Clock Domain. */
	/* Bits 0 to 12 reserved for future use */
};
using PortFlags = EnumBitfield<PortFlag>;

/** StreamInfo Flags - Clause 7.4.15.1 */
enum class StreamInfoFlag : uint32_t
{
	None = 0u,
	ClassB = 1u << 0, /**< Indicates that the Stream is Class B instead of Class A (default 0 is class A). */
	FastConnect = 1u << 1, /**< Fast Connect Mode, the Stream was connected in Fast Connect Mode or is presently trying to connect in Fast Connect Mode. */
	SavedState = 1u << 2, /**< Connection has saved ACMP state. */
	StreamingWait = 1u << 3, /**< The Stream is presently in STREAMING_WAIT, either it was connected with STREAMING_WAIT flag set or it was stopped with STOP_STREAMING command. */
	SupportsEncrypted = 1u << 4, /**< Indicates that the Stream supports streaming with encrypted PDUs. */
	EncryptedPdu = 1u << 5, /**< Indicates that the Stream is using encrypted PDUs. */
	TalkerFailed = 1u << 6, /**< Indicates that the Listener has registered an SRP Talker Failed attribute for the Stream. */
	/* Bits 5 to 24 reserved for future use */
	StreamVlanIDValid = 1u << 25, /**< Indicates that the stream_vlan_id field is valid. */
	Connected = 1u << 26, /**< The Stream has been connected with ACMP. This may only be set in a response. */
	MsrpFailureValid = 1u << 27, /**< The values in the msrp_failure_code and msrp_failure_bridge_id fields are valid. */
	StreamDestMacValid = 1u << 28, /**< The value in the stream_dest_mac field is valid. */
	MsrpAccLatValid = 1u << 29, /**< The value in the msrp_accumulated_latency field is valid. */
	StreamIDValid = 1u << 30, /**< The value in the stream_id field is valid. */
	StreamFormatValid = 1u << 31, /**< The value in stream_format field is valid and is to be used to change the Stream format if it is a SET_STREAM_INFO command. */
};
using StreamInfoFlags = EnumBitfield<StreamInfoFlag>;

/** StreamInfoEx Flags - Milan Clause 7.3.10 */
enum class StreamInfoFlagEx : uint32_t
{
	None = 0u,
	Registering = 1u << 0, /**< StreamInput: Registering either a matching Talker Advertise or a matching Talker Failed attribute. StreamOutput: Declaring a Talker Advertise or a Talker Failed attribute and registering a matching Listener attribute. */
	/* Bits 0 to 30 reserved for future use */
};
using StreamInfoFlagsEx = EnumBitfield<StreamInfoFlagEx>;

/** AvbInfo Flags - Clause 7.4.40.2 */
enum class AvbInfoFlag : uint8_t
{
	None = 0u,
	AsCapable = 1u << 0, /**< The IEEE Std 802.1AS-2011 variable asCapable is set on this interface. */
	GptpEnabled = 1u << 1, /**< Indicates that the interface has the IEEE Std 802.1AS-2011 functionality enabled. */
	SrpEnabled = 1u << 2, /**< Indicates that the interface has the IEEE Std 802.1Q-2011 Clause 35, "Stream Reservation Protocol (SRP)" functionality enabled. */
	/* Bits 0 to 4 reserved for future use */
};
using AvbInfoFlags = EnumBitfield<AvbInfoFlag>;

/* ENTITY Counters - Clause 7.4.42.2.1 */
enum class EntityCounterValidFlag : DescriptorCounterValidFlag
{
	None = 0u,
	EntitySpecific8 = 1u << 24, /**< Entity Specific counter 8. */
	EntitySpecific7 = 1u << 25, /**< Entity Specific counter 7. */
	EntitySpecific6 = 1u << 26, /**< Entity Specific counter 6. */
	EntitySpecific5 = 1u << 27, /**< Entity Specific counter 5. */
	EntitySpecific4 = 1u << 28, /**< Entity Specific counter 4. */
	EntitySpecific3 = 1u << 29, /**< Entity Specific counter 3. */
	EntitySpecific2 = 1u << 30, /**< Entity Specific counter 2. */
	EntitySpecific1 = 1u << 31, /**< Entity Specific counter 1. */
};
using EntityCounterValidFlags = EnumBitfield<EntityCounterValidFlag>;

/* AVB_INTERFACE Counters - Clause 7.4.42.2.2 */
enum class AvbInterfaceCounterValidFlag : DescriptorCounterValidFlag
{
	None = 0u,
	LinkUp = 1u << 0, /**< Total number of network link up events. */
	LinkDown = 1u << 1, /**< Total number of network link down events. */
	FramesTx = 1u << 2, /**< Total number of network frames sent. */
	FramesRx = 1u << 3, /**< Total number of network frames received. */
	RxCrcError = 1u << 4, /**< Total number of network frames received with an incorrect CRC. */
	GptpGmChanged = 1u << 5, /**< gPTP grandmaster change count. */
	EntitySpecific8 = 1u << 24, /**< Entity Specific counter 8. */
	EntitySpecific7 = 1u << 25, /**< Entity Specific counter 7. */
	EntitySpecific6 = 1u << 26, /**< Entity Specific counter 6. */
	EntitySpecific5 = 1u << 27, /**< Entity Specific counter 5. */
	EntitySpecific4 = 1u << 28, /**< Entity Specific counter 4. */
	EntitySpecific3 = 1u << 29, /**< Entity Specific counter 3. */
	EntitySpecific2 = 1u << 30, /**< Entity Specific counter 2. */
	EntitySpecific1 = 1u << 31, /**< Entity Specific counter 1. */
};
using AvbInterfaceCounterValidFlags = EnumBitfield<AvbInterfaceCounterValidFlag>;

/* CLOCK_DOMAIN Counters - Clause 7.4.42.2.3 */
enum class ClockDomainCounterValidFlag : DescriptorCounterValidFlag
{
	None = 0u,
	Locked = 1u << 0, /**< Increments on a clock locking event. */
	Unlocked = 1u << 1, /**< Increments on a clock unlocking event. */
	EntitySpecific8 = 1u << 24, /**< Entity Specific counter 8. */
	EntitySpecific7 = 1u << 25, /**< Entity Specific counter 7. */
	EntitySpecific6 = 1u << 26, /**< Entity Specific counter 6. */
	EntitySpecific5 = 1u << 27, /**< Entity Specific counter 5. */
	EntitySpecific4 = 1u << 28, /**< Entity Specific counter 4. */
	EntitySpecific3 = 1u << 29, /**< Entity Specific counter 3. */
	EntitySpecific2 = 1u << 30, /**< Entity Specific counter 2. */
	EntitySpecific1 = 1u << 31, /**< Entity Specific counter 1. */
};
using ClockDomainCounterValidFlags = EnumBitfield<ClockDomainCounterValidFlag>;

/* STREAM_INPUT Counters - Clause 7.4.42.2.4 / Milan Clause 6.8.10 */
enum class StreamInputCounterValidFlag : DescriptorCounterValidFlag
{
	None = 0u,
	MediaLocked = 1u << 0, /**< Increments on a Stream media clock locking. */
	MediaUnlocked = 1u << 1, /**< Increments on a Stream media clock unlocking. */
	StreamReset = 1u << 2, /**< IEEE1722.1-2013 - Increments whenever the Stream playback is reset. */
	StreamInterrupted = 1u << 2, /**< IEEE1722-2016 / Milan - Incremented each time the stream playback is interrupted for any reason other than a Controller Unbind operation. */
	SeqNumMismatch = 1u << 3, /**< Increments when a Stream data AVTPDU is received with a nonsequential sequence_num field. */
	MediaReset = 1u << 4, /**< Increments on a toggle of the mr bit in the Stream data AVTPDU. */
	TimestampUncertain = 1u << 5, /**< Increments on a toggle of the tu bit in the Stream data AVTPDU. */
	TimestampValid = 1u << 6, /**< Increments on receipt of a Stream data AVTPDU with the tv bit set. */
	TimestampNotValid = 1u << 7, /**< Increments on receipt of a Stream data AVTPDU with tv bit cleared. */
	UnsupportedFormat = 1u << 8, /**< Increments on receipt of a Stream data AVTPDU that contains an unsupported media type. */
	LateTimestamp = 1u << 9, /**< Increments on receipt of a Stream data AVTPDU with an avtp_timestamp field that is in the past. */
	EarlyTimestamp = 1u << 10, /**< Increments on receipt of a Stream data AVTPDU with an avtp_timestamp field that is too far in the future to process. */
	FramesRx = 1u << 11, /**< Increments on each Stream data AVTPDU received. */
	FramesTx = 1u << 12, /**< Increments on each Stream data AVTPDU transmitted. */
	EntitySpecific8 = 1u << 24, /**< Entity Specific counter 8. */
	EntitySpecific7 = 1u << 25, /**< Entity Specific counter 7. */
	EntitySpecific6 = 1u << 26, /**< Entity Specific counter 6. */
	EntitySpecific5 = 1u << 27, /**< Entity Specific counter 5. */
	EntitySpecific4 = 1u << 28, /**< Entity Specific counter 4. */
	EntitySpecific3 = 1u << 29, /**< Entity Specific counter 3. */
	EntitySpecific2 = 1u << 30, /**< Entity Specific counter 2. */
	EntitySpecific1 = 1u << 31, /**< Entity Specific counter 1. */
};
using StreamInputCounterValidFlags = EnumBitfield<StreamInputCounterValidFlag>;

/* STREAM_OUTPUT Counters - Milan Clause 6.7.7/7.3.25 */
enum class StreamOutputCounterValidFlag : DescriptorCounterValidFlag
{
	None = 0u,
	StreamStart = 1u << 0, /**< Incremented each time the Talker starts streaming. */
	StreamStop = 1u << 1, /**< Incremented each time the Talker stops streaming. At any time, the PAAD-AE shall ensure that either STREAM_START=STREAM_STOP+1 (in this case, the Talker is currently streaming), or STREAM_START=STREAM_STOP (in this case, the Talker is not currently streaming). */
	MediaReset = 1u << 2, /**< Incremented at the end of every observation interval during which the "mr" bit has been toggled in any of the transmitted Stream Data AVTPDUs. The duration of the observation interval is implementation-specific and shall be less than or equal to 1 second. */
	TimestampUncertain = 1u << 3, /**< Incremented at the end of every observation interval during which the "tu" bit has been set in any of the transmitted Stream Data AVTPDUs. The duration of the observation interval is implementation-specific and shall be less than or equal to 1 second. */
	FramesTx = 1u << 4, /**< Incremented at the end of every observation interval during which at least one Stream Data AVTPDU has been transmitted on this STREAM_OUTPUT. The duration of the observation interval is implementation-specific and shall be less than or equal to 1 second. */
};
using StreamOutputCounterValidFlags = EnumBitfield<StreamOutputCounterValidFlag>;

/** Milan Info Features Flags - Milan Clause 7.4.1 */
enum class MilanInfoFeaturesFlag : uint32_t
{
	None = 0u,
	Redundancy = 1u << 0, /**< The entity supports the milan redundancy scheme. */
};
using MilanInfoFeaturesFlags = EnumBitfield<MilanInfoFeaturesFlag>;

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYENUMS_HPP_ */
