#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLDEFINES_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLDEFINES_HPP_

#pragma once

#include <stdint.h>
#include <string>

/* Global protocol defines */
constexpr uint16_t ETHERNET_MAX_FRAME_SIZE =  1522;
constexpr uint16_t AVTP_ETHER_TYPE =        0x22f0;
constexpr uint16_t AVTP_MAX_PAYLOAD_LENGTH =  1486;
constexpr uint8_t AVTP_VERSION =              0x00;
constexpr uint8_t AVTP_SUBTYPE_ADP =          0xfa; // 0x7a?
constexpr uint8_t AVTP_SUBTYPE_AECP =         0xfb; // 0x7b?
constexpr uint8_t AVTP_SUBTYPE_ACMP =         0xfc; // 0x7c?
constexpr uint8_t AVTP_SUBTYPE_MAAP =         0xfe; // 0x7e?
constexpr uint8_t AVTP_SUBTYPE_EXPERIMENTAL = 0xff; // 0x7f?

// Enum class for ADP message types
enum class AdpMessageType : uint8_t {
    ENTITY_AVAILABLE = 0x00,
    ENTITY_DEPARTING = 0x01,
    ENTITY_DISCOVER =  0x02
};

/** AECP Message Type - Based on the specification */
enum class AecpMessageType : uint8_t
{
    AEM_COMMAND =             0x00,
    AEM_RESPONSE =            0x01,
    ADDRESS_ACCESS_COMMAND =  0x02,
    ADDRESS_ACCESS_RESPONSE = 0x03,
    AVC_COMMAND =             0x04,
    AVC_RESPONSE =            0x05,
    VENDOR_UNIQUE_COMMAND =   0x06,
    VENDOR_UNIQUE_RESPONSE =  0x07,
    HDCP_AEM_COMMAND =        0x08,
    HDCP_AEM_RESPONSE =       0x09,
    EXTENDED_COMMAND =        0x0e,
    EXTENDED_RESPONSE =       0x0f,
    UNKNOWN =                 0xff
};

/** AECP Status Types - Clause 7.4 */
enum class AecpStatus : uint8_t
{
    SUCCESS =                 0,
    NOT_IMPLEMENTED =         1,
    NO_SUCH_DESCRIPTOR =      2,
    ENTITY_LOCKED =           3,
    ENTITY_ACQUIRED =         4,
    NOT_AUTHENTICATED =       5,
    AUTHENTICATION_DISABLED = 6,
    BAD_ARGUMENTS =           7,
    NO_RESOURCES =            8,
    IN_PROGRESS =             9,
    ENTITY_MISBEHAVING =     10,
    NOT_SUPPORTED =          11,
    STREAM_IS_RUNNING =      12,
    UNKNOWN =              0xff
};

using MacAddress = std::array<uint8_t, 6>;

/** AECP SequenceID - Clause 9.2.1.1.10 */
using AecpSequenceID = uint16_t;

/** AEM Command Type - Clause 7.4 */
enum class AemCommandType : uint16_t
{
    ACQUIRE_ENTITY =        0x0000,
    LOCK_ENTITY =           0x0001,
    ENTITY_AVAILABLE =      0x0002,
    CONTROLLER_AVAILABLE =  0x0003,
    READ_DESCRIPTOR =       0x0004,
    WRITE_DESCRIPTOR =      0x0005,
    SET_CONFIGURATION =     0x0006,
    GET_CONFIGURATION =     0x0007,
    SET_STREAM_FORMAT =     0x0008,
    GET_STREAM_FORMAT =     0x0009,
    SET_VIDEO_FORMAT =      0x000a,
    GET_VIDEO_FORMAT =      0x000b,
    SET_SENSOR_FORMAT =     0x000c,
    GET_SENSOR_FORMAT =     0x000d,
    SET_STREAM_INFO =       0x000e,
    GET_STREAM_INFO =       0x000f,
    SET_NAME =              0x0010,
    GET_NAME =              0x0011,
    SET_ASSOCIATION_ID =    0x0012,
    GET_ASSOCIATION_ID =    0x0013,
    SET_SAMPLING_RATE =     0x0014,
    GET_SAMPLING_RATE =     0x0015,
    SET_CLOCK_SOURCE =      0x0016,
    GET_CLOCK_SOURCE =      0x0017,
    SET_CONTROL =           0x0018,
    GET_CONTROL =           0x0019,
    INCREMENT_CONTROL =     0x001a,
    DECREMENT_CONTROL =     0x001b,
    SET_SIGNAL_SELECTOR =   0x001c,
    GET_SIGNAL_SELECTOR =   0x001d,
    SET_MIXER =             0x001e,
    GET_MIXER =             0x001f,
    SET_MATRIX =            0x0020,
    GET_MATRIX =            0x0021,
    START_STREAMING =       0x0022,
    STOP_STREAMING =        0x0023,
    REGISTER_UNSOLICITED_NOTIFICATION =   0x0024,
    DEREGISTER_UNSOLICITED_NOTIFICATION = 0x0025,
    IDENTIFY_NOTIFICATION = 0x0026,
    GET_AVB_INFO =          0x0027,
    GET_AS_PATH =           0x0028,
    GET_COUNTERS =          0x0029,
    REBOOT =                0x002a,
    GET_AUDIO_MAP =         0x002b,
    ADD_AUDIO_MAPPINGS =    0x002c,
    REMOVE_AUDIO_MAPPINGS = 0x002d,
    GET_VIDEO_MAP =         0x002e,
    ADD_VIDEO_MAPPINGS =    0x002f,
    REMOVE_VIDEO_MAPPINGS = 0x0030,
    GET_SENSOR_MAP =        0x0031,
    ADD_SENSOR_MAPPINGS =   0x0032,
    REMOVE_SENSOR_MAPPINGS = 0x0033,
    START_OPERATION =        0x0034,
    ABORT_OPERATION =        0x0035,
    OPERATION_STATUS =       0x0036,
    AUTH_ADD_KEY =           0x0037,
    AUTH_DELETE_KEY =        0x0038,
    AUTH_GET_KEY_LIST =      0x0039,
    AUTH_GET_KEY =           0x003a,
    AUTH_ADD_KEY_TO_CHAIN =  0x003b,
    AUTH_DELETE_KEY_FROM_CHAIN = 0x003c,
    AUTH_GET_KEYCHAIN_LIST =     0x003d,
    AUTH_GET_IDENTITY =          0x003e,
    AUTH_ADD_TOKEN =             0x003f,
    AUTH_DELETE_TOKEN =          0x0040,
    AUTHENTICATE =               0x0041,
    DEAUTHENTICATE =             0x0042,
    ENABLE_TRANSPORT_SECURITY =  0x0043,
    DISABLE_TRANSPORT_SECURITY = 0x0044,
    ENABLE_STREAM_ENCRYPTION =   0x0045,
    DISABLE_STREAM_ENCRYPTION =  0x0046,
    SET_MEMORY_OBJECT_LENGTH =   0x0047,
    GET_MEMORY_OBJECT_LENGTH =   0x0048,
    SET_STREAM_BACKUP =          0x0049,
    GET_STREAM_BACKUP =          0x004a,
    EXPANSION =                  0x7fff,
    INVALID_COMMAND_TYPE =       0xffff
};

/** Status code returned by all AEM (AECP) command methods. */
	enum class AemCommandStatus : uint16_t
	{
		// ATDECC Protocol Error Codes
		Success =                 0, /**< The ATDECC Entity successfully performed the command and has valid results. */
		NotImplemented =          1, /**< The ATDECC Entity does not support the command type. */
		NoSuchDescriptor =        2, /**< A descriptor with the descriptor_type and descriptor_index specified does not exist. */
		LockedByOther =           3, /**< The ATDECC Entity has been locked by another ATDECC Controller. */
		AcquiredByOther =         4, /**< The ATDECC Entity has been acquired by another ATDECC Controller. */
		NotAuthenticated =        5, /**< The ATDECC Controller is not authenticated with the ATDECC Entity. */
		AuthenticationDisabled =  6, /**< The ATDECC Controller is trying to use an authentication command when authentication isn't enable on the ATDECC Entity. */
		BadArguments =            7, /**< One or more of the values in the fields of the frame were deemed to be bad by the ATDECC Entity (unsupported, incorrect combination, etc.). */
		NoResources =             8, /**< The ATDECC Entity cannot complete the command because it does not have the resources to support it. */
		InProgress =              9, /**< The ATDECC Entity is processing the command and will send a second response at a later time with the result of the command. */
		EntityMisbehaving =      10, /**< The ATDECC Entity generated an internal error while trying to process the command. */
		NotSupported =           11, /**< The command is implemented but the target of the command is not supported. For example trying to set the value of a read - only Control. */
		StreamIsRunning =        12, /**< The Stream is currently streaming and the command is one which cannot be executed on an Active Stream. */
		
        // Library Error Codes
		BaseProtocolViolation = 991, /**< The entity sent a message that violates the base protocol */
		PartialImplementation = 992, /**< The library does not fully implement this command, please report this */
		Busy =                  993, /**< The library is busy, try again later */
		NetworkError =          995, /**< Network error */
		ProtocolError =         996, /**< Failed to unpack the message due to an error in the protocol */
		TimedOut =              997, /**< The command timed out */
		UnknownEntity =         998, /**< The entity has not been detected on the network */
		InternalError =         999, /**< Internal library error, please report this */
	};

	/** Status code returned by all AA (AECP) command methods. */
	enum class AaCommandStatus : uint16_t
	{
		// ATDECC Protocol Error Codes
		Success =        0, /**< The ATDECC Entity successfully performed the command and has valid results. */
		NotImplemented = 1, /**< The ATDECC Entity does not support the command type. */
		AddressTooLow =  2, /**< The value in the address field is below the start of the memory map. */
		AddressTooHigh = 3, /**< The value in the address field is above the end of the memory map. */
		AddressInvalid = 4, /**< The value in the address field is within the memory map but is part of an invalid region. */
		TlvInvalid =     5, /**< One or more of the TLVs were invalid. No TLVs have been processed. */
		DataInvalid =    6, /**< The data for writing is invalid .*/
		Unsupported =    7, /**< A requested action was unsupported. Typically used when an unknown EXECUTE was encountered or if EXECUTE is not supported. */
		
        // Library Error Codes
        BaseProtocolViolation = 991, /**< The entity sent a message that violates the base protocol */
		PartialImplementation = 992, /**< The library does not fully implement this command, please report this */
		Busy =                  993, /**< The library is busy, try again later */
		Aborted =               994, /**< Request aborted */
		NetworkError =          995, /**< Network error */
		ProtocolError =         996, /**< Failed to unpack the message due to an error in the protocol */
		TimedOut =              997, /**< The command timed out */
		UnknownEntity =         998, /**< The entity has not been detected on the network */
		InternalError =         999, /**< Internal library error, please report this */
	};

	/** Status code returned by all MVU (AECP) command methods. */
	enum class MvuCommandStatus : uint16_t
	{
		// Milan Vendor Unique Protocol Error Codes
		Success =        0,
		NotImplemented = 1,
		BadArguments =   2,

		// Library Error Codes
		BaseProtocolViolation = 991, /**< The entity sent a message that violates the base protocol */
		PartialImplementation = 992, /**< The library does not fully implement this command, please report this */
		Busy =                  993, /**< The library is busy, try again later */
		NetworkError =          995,
		ProtocolError =         996,
		TimedOut =              997,
		UnknownEntity =         998,
		InternalError =         999,
	};

	/** Status code returned by all ACMP control methods. */
	enum class ControlStatus : uint16_t // or AcmpStatus
	{
		// ATDECC Protocol Error Codes
		Success =                   0,
		ListenerUnknownID =         1, /**< Listener does not have the specified unique identifier. */
		TalkerUnknownID =           2, /**< Talker does not have the specified unique identifier. */
		TalkerDestMacFail =         3, /**< Talker could not allocate a destination MAC for the Stream. */
		TalkerNoStreamIndex =       4, /**< Talker does not have an available Stream index for the Stream. */
		TalkerNoBandwidth =         5, /**< Talker could not allocate bandwidth for the Stream. */
		TalkerExclusive =           6, /**< Talker already has an established Stream and only supports one Listener. */
		ListenerTalkerTimeout =     7, /**< Listener had timeout for all retries when trying to send command to Talker. */
		ListenerExclusive =         8, /**< The ATDECC Listener already has an established connection to a Stream. */
		StateUnavailable =          9, /**< Could not get the state from the ATDECC Entity. */
		NotConnected =             10, /**< Trying to disconnect when not connected or not connected to the ATDECC Talker specified. */
		NoSuchConnection =         11, /**< Trying to obtain connection info for an ATDECC Talker connection which does not exist. */
		CouldNotSendMessage =      12, /**< The ATDECC Listener failed to send the message to the ATDECC Talker. */
		TalkerMisbehaving =        13, /**< Talker was unable to complete the command because an internal error occurred. */
		ListenerMisbehaving =      14, /**< Listener was unable to complete the command because an internal error occurred. */
		// Reserved
		ControllerNotAuthorized =  16, /**< The ATDECC Controller with the specified Entity ID is not authorized to change Stream connections. */
		IncompatibleRequest =      17, /**< The ATDECC Listener is trying to connect to an ATDECC Talker that is already streaming with a different traffic class, etc. or does not support the requested traffic class. */
		// Reserved
		NotSupported =             31, /**< The command is not supported. */
		
        // Library Error Codes
		BaseProtocolViolation =   991, /**< The entity sent a message that violates the base protocol */
		NetworkError =            995, /**< A network error occured. */
		ProtocolError =           996, /**< A protocol error occured. */
		TimedOut =                997, /**< Command timed out. */
		UnknownEntity =           998, /**< Entity is unknown. */
		InternalError =           999, /**< Internal library error. */
	};

	/** EntityAdvertise dirty flag */
	enum class AdvertiseFlag : uint8_t
	{
		None =               0,
		EntityCapabilities = 1u << 0, /**< EntityCapabilities field changed */
		AssociationID =      1u << 1, /**< AssociationID field changed */
		ValidTime =          1u << 2, /**< ValidTime field changed */
		GptpGrandmasterID =  1u << 3, /**< gPTP GrandmasterID field changed */
		GptpDomainNumber =   1u << 4, /**< gPTP DomainNumber field changed */
	};

/** AEM Acquire Entity Flags - Clause 7.4.1.1 */
enum class AemAcquireEntityFlags : uint32_t
{
	NONE =       0x00000000,
	PERSISTENT = 0x00000001,
	RELEASE =    0x80000000
};

/** AEM Lock Entity Flags - Clause 7.4.2.1 */
enum class AemLockEntityFlags : uint32_t
{
	NONE =   0x00000000,
	UNLOCK = 0x00000001
};

/** Address Access Mode - Clause 9.2.1.3.3 */
enum class AaMode : uint8_t
{
    READ =    0x0,
    WRITE =   0x1,
    EXECUTE = 0x2
};

/** ACMP Message Type - Clause 8.2.1.5 */
enum class AcmpMessageType : uint8_t
{
    CONNECT_TX_COMMAND =         0,
    CONNECT_TX_RESPONSE =        1,
    DISCONNECT_TX_COMMAND =      2,
    DISCONNECT_TX_RESPONSE =     3,
    GET_TX_STATE_COMMAND =       4,
    GET_TX_STATE_RESPONSE =      5,
    CONNECT_RX_COMMAND =         6,
    CONNECT_RX_RESPONSE =        7,
    DISCONNECT_RX_COMMAND =      8,
    DISCONNECT_RX_RESPONSE =     9,
    GET_RX_STATE_COMMAND =       10,
    GET_RX_STATE_RESPONSE =      11,
    GET_TX_CONNECTION_COMMAND =  12,
    GET_TX_CONNECTION_RESPONSE = 13
};

/** ACMP Status - Clause 8.2.1.6 */
enum class AcmpStatus : uint8_t
{
    SUCCESS =                   0,
    LISTENER_UNKNOWN_ID =       1,
    TALKER_UNKNOWN_ID =         2,
    TALKER_DEST_MAC_FAIL =      3,
    TALKER_NO_STREAM_INDEX =    4,
    TALKER_NO_BANDWIDTH =       5,
    TALKER_EXCLUSIVE =          6,
    LISTENER_TALKER_TIMEOUT =   7,
    LISTENER_EXCLUSIVE =        8,
    STATE_UNAVAILABLE =         9,
    NOT_CONNECTED =             10,
    NO_SUCH_CONNECTION =        11,
    COULD_NOT_SEND_MESSAGE =    12,
    TALKER_MISBEHAVING =        13,
    LISTENER_MISBEHAVING =      14,
    CONTROLLER_NOT_AUTHORIZED = 16,
    INCOMPATIBLE_REQUEST =      17,
    NOT_SUPPORTED =             31
};

/** ACMP UniqueID - Clause 8.2.1.12 and 8.2.1.13 */
using AcmpUniqueID = uint16_t;

/** ACMP SequenceID - Clause 8.2.1.16 */
using AcmpSequenceID = uint16_t;

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLDEFINES_HPP_ */