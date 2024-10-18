#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLDEFINES_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLDEFINES_HPP_

#pragma once

#include <stdint.h>
#include <string>

/* Global protocol defines */
constexpr uint16_t ETHERNET_MAX_FRAME_SIZE = 1522;
constexpr uint16_t AVTP_ETHER_TYPE = 0x22f0;
constexpr uint16_t AVTP_MAX_PAYLOAD_LENGTH = 1500;
constexpr uint8_t AVTP_VERSION = 0x00;
constexpr uint8_t AVTP_SUBTYPE_ADP = 0x7a;
constexpr uint8_t AVTP_SUBTYPE_AECP = 0x7b;
constexpr uint8_t AVTP_SUBTYPE_ACMP = 0x7c;
constexpr uint8_t AVTP_SUBTYPE_MAAP = 0x7e;
constexpr uint8_t AVTP_SUBTYPE_EXPERIMENTAL = 0x7f;

// Enum class for ADP message types
enum class AdpMessageType : uint8_t {
    ENTITY_AVAILABLE = 0x00,
    ENTITY_DEPARTING = 0x01,
    ENTITY_DISCOVER = 0x02
};

/** AECP Message Type - Based on the specification */
enum class AecpMessageType : uint8_t
{
    AEM_COMMAND = 0,
    AEM_RESPONSE = 1,
    ADDRESS_ACCESS_COMMAND = 2,
    ADDRESS_ACCESS_RESPONSE = 3,
    AVC_COMMAND = 4,
    AVC_RESPONSE = 5,
    VENDOR_UNIQUE_COMMAND = 6,
    VENDOR_UNIQUE_RESPONSE = 7,
    HDCP_AEM_COMMAND = 8,
    HDCP_AEM_RESPONSE = 9,
    EXTENDED_COMMAND = 10,
    EXTENDED_RESPONSE = 11,
    UNKNOWN = 0xFF
};

/** AECP Status Types - Based on the specification */
enum class AecpStatus : uint8_t
{
    SUCCESS = 0,
    NOT_IMPLEMENTED = 1,
    NO_SUCH_DESCRIPTOR = 2,
    IN_PROGRESS = 3,
    ENTITY_MISBEHAVING = 4,
    NOT_AUTHENTICATED = 5,
    AUTHENTICATION_DISABLED = 6,
    BAD_ARGUMENTS = 7,
    RESERVED = 8,
    NO_RESOURCES = 9,
    LOGICAL_MISBEHAVING = 10,
    ENTITY_NOT_CONNECTED = 11,
    UNEXPECTED_SUBCODE = 12,
    GENERIC_NOT_AVAILABLE = 13,
    UNSUPPORTED_COMMAND = 14,
    RESPONSE_TOO_BIG = 15,
    UNKNOWN = 0xFF
};

/** AECP SequenceID - Clause 9.2.1.1.10 */
using AecpSequenceID = uint16_t;

/** AEM Command Type - Clause 7.4 */
enum class AemCommandType : uint16_t
{
    ACQUIRE_ENTITY = 0,
    LOCK_ENTITY = 1,
    ENTITY_AVAILABLE = 2,
    CONTROLLER_AVAILABLE = 3,
    READ_DESCRIPTOR = 4,
    WRITE_DESCRIPTOR = 5,
    SET_CONFIGURATION = 6,
    GET_CONFIGURATION = 7,
    SET_STREAM_FORMAT = 8,
    GET_STREAM_FORMAT = 9,
    SET_VIDEO_FORMAT = 10,
    GET_VIDEO_FORMAT = 11,
    SET_SENSOR_FORMAT = 12,
    GET_SENSOR_FORMAT = 13,
    SET_STREAM_INFO = 14,
    GET_STREAM_INFO = 15,
    SET_NAME = 16,
    GET_NAME = 17,
    SET_ASSOCIATION_ID = 18,
    GET_ASSOCIATION_ID = 19,
    SET_SAMPLING_RATE = 20,
    GET_SAMPLING_RATE = 21,
    SET_CLOCK_SOURCE = 22,
    GET_CLOCK_SOURCE = 23,
    SET_CONTROL = 24,
    GET_CONTROL = 25,
    INCREMENT_CONTROL = 26,
    DECREMENT_CONTROL = 27,
    SET_SIGNAL_SELECTOR = 28,
    GET_SIGNAL_SELECTOR = 29,
    SET_MIXER = 30,
    GET_MIXER = 31,
    SET_MATRIX = 32,
    GET_MATRIX = 33,
    START_STREAMING = 34,
    STOP_STREAMING = 35,
    REGISTER_UNSOLICITED_NOTIFICATION = 36,
    DEREGISTER_UNSOLICITED_NOTIFICATION = 37,
    IDENTIFY_NOTIFICATION = 38,
    GET_AVB_INFO = 39,
    GET_AS_PATH = 40,
    GET_COUNTERS = 41,
    REBOOT = 42,
    GET_AUDIO_MAP = 43,
    ADD_AUDIO_MAPPINGS = 44,
    REMOVE_AUDIO_MAPPINGS = 45,
    GET_VIDEO_MAP = 46,
    ADD_VIDEO_MAPPINGS = 47,
    REMOVE_VIDEO_MAPPINGS = 48,
    GET_SENSOR_MAP = 49,
    ADD_SENSOR_MAPPINGS = 50,
    REMOVE_SENSOR_MAPPINGS = 51,
    START_OPERATION = 52,
    ABORT_OPERATION = 53,
    OPERATION_STATUS = 54,
    AUTH_ADD_KEY = 55,
    AUTH_DELETE_KEY = 56,
    AUTH_GET_KEY_LIST = 57,
    AUTH_GET_KEY = 58,
    AUTH_ADD_KEY_TO_CHAIN = 59,
    AUTH_DELETE_KEY_FROM_CHAIN = 60,
    AUTH_GET_KEYCHAIN_LIST = 61,
    AUTH_GET_IDENTITY = 62,
    AUTH_ADD_TOKEN = 63,
    AUTH_DELETE_TOKEN = 64,
    AUTHENTICATE = 65,
    DEAUTHENTICATE = 66,
    ENABLE_TRANSPORT_SECURITY = 67,
    DISABLE_TRANSPORT_SECURITY = 68,
    ENABLE_STREAM_ENCRYPTION = 69,
    DISABLE_STREAM_ENCRYPTION = 70,
    SET_MEMORY_OBJECT_LENGTH = 71,
    GET_MEMORY_OBJECT_LENGTH = 72,
    SET_STREAM_BACKUP = 73,
    GET_STREAM_BACKUP = 74,
    EXPANSION = 75,
    INVALID_COMMAND_TYPE = 0xFFFF
};

/** Status code returned by all AEM (AECP) command methods. */
	enum class AemCommandStatus : uint16_t
	{
		// ATDECC Protocol Error Codes
		Success = 0, /**< The ATDECC Entity successfully performed the command and has valid results. */
		NotImplemented = 1, /**< The ATDECC Entity does not support the command type. */
		NoSuchDescriptor = 2, /**< A descriptor with the descriptor_type and descriptor_index specified does not exist. */
		LockedByOther = 3, /**< The ATDECC Entity has been locked by another ATDECC Controller. */
		AcquiredByOther = 4, /**< The ATDECC Entity has been acquired by another ATDECC Controller. */
		NotAuthenticated = 5, /**< The ATDECC Controller is not authenticated with the ATDECC Entity. */
		AuthenticationDisabled = 6, /**< The ATDECC Controller is trying to use an authentication command when authentication isn't enable on the ATDECC Entity. */
		BadArguments = 7, /**< One or more of the values in the fields of the frame were deemed to be bad by the ATDECC Entity (unsupported, incorrect combination, etc.). */
		NoResources = 8, /**< The ATDECC Entity cannot complete the command because it does not have the resources to support it. */
		InProgress = 9, /**< The ATDECC Entity is processing the command and will send a second response at a later time with the result of the command. */
		EntityMisbehaving = 10, /**< The ATDECC Entity generated an internal error while trying to process the command. */
		NotSupported = 11, /**< The command is implemented but the target of the command is not supported. For example trying to set the value of a read - only Control. */
		StreamIsRunning = 12, /**< The Stream is currently streaming and the command is one which cannot be executed on an Active Stream. */
		// Library Error Codes
		BaseProtocolViolation = 991, /**< The entity sent a message that violates the base protocol */
		PartialImplementation = 992, /**< The library does not fully implement this command, please report this */
		Busy = 993, /**< The library is busy, try again later */
		NetworkError = 995, /**< Network error */
		ProtocolError = 996, /**< Failed to unpack the message due to an error in the protocol */
		TimedOut = 997, /**< The command timed out */
		UnknownEntity = 998, /**< The entity has not been detected on the network */
		InternalError = 999, /**< Internal library error, please report this */
	};

	/** Status code returned by all AA (AECP) command methods. */
	enum class AaCommandStatus : uint16_t
	{
		// ATDECC Protocol Error Codes
		Success = 0, /**< The ATDECC Entity successfully performed the command and has valid results. */
		NotImplemented = 1, /**< The ATDECC Entity does not support the command type. */
		AddressTooLow = 2, /**< The value in the address field is below the start of the memory map. */
		AddressTooHigh = 3, /**< The value in the address field is above the end of the memory map. */
		AddressInvalid = 4, /**< The value in the address field is within the memory map but is part of an invalid region. */
		TlvInvalid = 5, /**< One or more of the TLVs were invalid. No TLVs have been processed. */
		DataInvalid = 6, /**< The data for writing is invalid .*/
		Unsupported = 7, /**< A requested action was unsupported. Typically used when an unknown EXECUTE was encountered or if EXECUTE is not supported. */
		// Library Error Codes
		BaseProtocolViolation = 991, /**< The entity sent a message that violates the base protocol */
		PartialImplementation = 992, /**< The library does not fully implement this command, please report this */
		Busy = 993, /**< The library is busy, try again later */
		Aborted = 994, /**< Request aborted */
		NetworkError = 995, /**< Network error */
		ProtocolError = 996, /**< Failed to unpack the message due to an error in the protocol */
		TimedOut = 997, /**< The command timed out */
		UnknownEntity = 998, /**< The entity has not been detected on the network */
		InternalError = 999, /**< Internal library error, please report this */
	};

	/** Status code returned by all MVU (AECP) command methods. */
	enum class MvuCommandStatus : uint16_t
	{
		// Milan Vendor Unique Protocol Error Codes
		Success = 0,
		NotImplemented = 1,
		BadArguments = 2,
		// Library Error Codes
		BaseProtocolViolation = 991, /**< The entity sent a message that violates the base protocol */
		PartialImplementation = 992, /**< The library does not fully implement this command, please report this */
		Busy = 993, /**< The library is busy, try again later */
		NetworkError = 995,
		ProtocolError = 996,
		TimedOut = 997,
		UnknownEntity = 998,
		InternalError = 999,
	};

	/** Status code returned by all ACMP control methods. */
	enum class ControlStatus : uint16_t
	{
		// ATDECC Protocol Error Codes
		Success = 0,
		ListenerUnknownID = 1, /**< Listener does not have the specified unique identifier. */
		TalkerUnknownID = 2, /**< Talker does not have the specified unique identifier. */
		TalkerDestMacFail = 3, /**< Talker could not allocate a destination MAC for the Stream. */
		TalkerNoStreamIndex = 4, /**< Talker does not have an available Stream index for the Stream. */
		TalkerNoBandwidth = 5, /**< Talker could not allocate bandwidth for the Stream. */
		TalkerExclusive = 6, /**< Talker already has an established Stream and only supports one Listener. */
		ListenerTalkerTimeout = 7, /**< Listener had timeout for all retries when trying to send command to Talker. */
		ListenerExclusive = 8, /**< The ATDECC Listener already has an established connection to a Stream. */
		StateUnavailable = 9, /**< Could not get the state from the ATDECC Entity. */
		NotConnected = 10, /**< Trying to disconnect when not connected or not connected to the ATDECC Talker specified. */
		NoSuchConnection = 11, /**< Trying to obtain connection info for an ATDECC Talker connection which does not exist. */
		CouldNotSendMessage = 12, /**< The ATDECC Listener failed to send the message to the ATDECC Talker. */
		TalkerMisbehaving = 13, /**< Talker was unable to complete the command because an internal error occurred. */
		ListenerMisbehaving = 14, /**< Listener was unable to complete the command because an internal error occurred. */
		// Reserved
		ControllerNotAuthorized = 16, /**< The ATDECC Controller with the specified Entity ID is not authorized to change Stream connections. */
		IncompatibleRequest = 17, /**< The ATDECC Listener is trying to connect to an ATDECC Talker that is already streaming with a different traffic class, etc. or does not support the requested traffic class. */
		// Reserved
		NotSupported = 31, /**< The command is not supported. */
		// Library Error Codes
		BaseProtocolViolation = 991, /**< The entity sent a message that violates the base protocol */
		NetworkError = 995, /**< A network error occured. */
		ProtocolError = 996, /**< A protocol error occured. */
		TimedOut = 997, /**< Command timed out. */
		UnknownEntity = 998, /**< Entity is unknown. */
		InternalError = 999, /**< Internal library error. */
	};

	/** EntityAdvertise dirty flag */
	enum class AdvertiseFlag : uint8_t
	{
		None = 0,
		EntityCapabilities = 1u << 0, /**< EntityCapabilities field changed */
		AssociationID = 1u << 1, /**< AssociationID field changed */
		ValidTime = 1u << 2, /**< ValidTime field changed */
		GptpGrandmasterID = 1u << 3, /**< gPTP GrandmasterID field changed */
		GptpDomainNumber = 1u << 4, /**< gPTP DomainNumber field changed */
	};

/** AEM Acquire Entity Flags - Clause 7.4.1.1 */
enum class AemAcquireEntityFlags : uint32_t
{
	NONE = 0,
	PERSISTENT = 1,
	RELEASE = 2
};

/** AEM Lock Entity Flags - Clause 7.4.2.1 */
enum class AemLockEntityFlags : uint32_t
{
	NONE = 0,
	UNLOCK = 1
};

/** Address Access Mode - Clause 9.2.1.3.3 */
enum class AaMode : uint8_t
{
    READ = 0,
    WRITE = 1,
    EXECUTE = 2
};

/** ACMP Message Type - Clause 8.2.1.5 */
enum class AcmpMessageType : uint8_t
{
    CONNECT_TX_COMMAND = 0,
    CONNECT_TX_RESPONSE = 1,
    DISCONNECT_TX_COMMAND = 2,
    DISCONNECT_TX_RESPONSE = 3,
    GET_TX_STATE_COMMAND = 4,
    GET_TX_STATE_RESPONSE = 5,
    CONNECT_RX_COMMAND = 6,
    CONNECT_RX_RESPONSE = 7,
    DISCONNECT_RX_COMMAND = 8,
    DISCONNECT_RX_RESPONSE = 9,
    GET_RX_STATE_COMMAND = 10,
    GET_RX_STATE_RESPONSE = 11,
    GET_TX_CONNECTION_COMMAND = 12,
    GET_TX_CONNECTION_RESPONSE = 13
};

/** ACMP Status - Clause 8.2.1.6 */
enum class AcmpStatus : uint8_t
{
    SUCCESS = 0,
    LISTENER_UNKNOWN_ID = 1,
    TALKER_UNKNOWN_ID = 2,
    TALKER_DEST_MAC_FAIL = 3,
    TALKER_NO_STREAM_INDEX = 4,
    TALKER_NO_BANDWIDTH = 5,
    TALKER_EXCLUSIVE = 6,
    LISTENER_TALKER_TIMEOUT = 7,
    LISTENER_EXCLUSIVE = 8,
    STATE_UNAVAILABLE = 9,
    NOT_CONNECTED = 10,
    NO_SUCH_CONNECTION = 11,
    COULD_NOT_SEND_MESSAGE = 12,
    TALKER_MISBEHAVING = 13,
    LISTENER_MISBEHAVING = 14,
    CONTROLLER_NOT_AUTHORIZED = 15,
    INCOMPATIBLE_REQUEST = 16,
    NOT_SUPPORTED = 17
};

/** ACMP UniqueID - Clause 8.2.1.12 and 8.2.1.13 */
using AcmpUniqueID = uint16_t;

/** ACMP SequenceID - Clause 8.2.1.16 */
using AcmpSequenceID = uint16_t;

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLDEFINES_HPP_ */