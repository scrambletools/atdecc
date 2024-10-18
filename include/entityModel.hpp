#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYMODEL_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYMODEL_HPP_

#pragma once

#include "entityEnums.hpp"
#include "uniqueIdentifier.hpp"
#include "protocolDefines.hpp"
#include "entityModelTypes.hpp"

#include <cstdint>
#include <cstring>
#include <unordered_map>
#include "esp_log.h"

// Logging tag for entity model
#define ATDECC_MAX_FORMATS 10
#define ATDECC_MAX_SAMPLING_RATES 10
#define ATDECC_MAX_DESCRIPTOR_TYPES 15

using MacAddress = uint8_t[6];

/** ENTITY Descriptor - Clause 7.2.1 */
struct EntityDescriptor
{
	UniqueIdentifier entityID{};
	UniqueIdentifier entityModelID{};
	EntityCapabilities entityCapabilities{};
	std::uint16_t talkerStreamSources{ 0u };
	TalkerCapabilities talkerCapabilities{};
	std::uint16_t listenerStreamSinks{ 0u };
	ListenerCapabilities listenerCapabilities{};
	ControllerCapabilities controllerCapabilities{};
	std::uint32_t availableIndex{ 0u };
	UniqueIdentifier associationID{};
	AtdeccFixedString entityName{};
	LocalizedStringReference vendorNameString{};
	LocalizedStringReference modelNameString{};
	AtdeccFixedString firmwareVersion{};
	AtdeccFixedString groupName{};
	AtdeccFixedString serialNumber{};
	std::uint16_t configurationsCount{ 0u };
	std::uint16_t currentConfiguration{ 0u };
};

struct EnumClassHash
{
    // Template to hash enum classes
    template<typename T>
    uint32_t operator()(T t) const
    {
        // Cast the enum class to its underlying type and return the hash value
        return static_cast<uint32_t>(t);
    }
};

/** CONFIGURATION Descriptor - Clause 7.2.2 */
struct ConfigurationDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	std::unordered_map<DescriptorType, std::uint16_t, EnumClassHash> descriptorCounts{};
};

/** AUDIO_UNIT Descriptor - Clause 7.2.3 */
struct AudioUnitDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	ClockDomainIndex clockDomainIndex{ 0u };
	std::uint16_t numberOfStreamInputPorts{ 0u };
	StreamPortIndex baseStreamInputPort{ StreamPortIndex(0u) };
	std::uint16_t numberOfStreamOutputPorts{ 0u };
	StreamPortIndex baseStreamOutputPort{ StreamPortIndex(0u) };
	std::uint16_t numberOfExternalInputPorts{ 0u };
	ExternalPortIndex baseExternalInputPort{ ExternalPortIndex(0u) };
	std::uint16_t numberOfExternalOutputPorts{ 0u };
	ExternalPortIndex baseExternalOutputPort{ ExternalPortIndex(0u) };
	std::uint16_t numberOfInternalInputPorts{ 0u };
	InternalPortIndex baseInternalInputPort{ InternalPortIndex(0u) };
	std::uint16_t numberOfInternalOutputPorts{ 0u };
	InternalPortIndex baseInternalOutputPort{ InternalPortIndex(0u) };
	std::uint16_t numberOfControls{ 0u };
	ControlIndex baseControl{ ControlIndex(0u) };
	std::uint16_t numberOfSignalSelectors{ 0u };
	SignalSelectorIndex baseSignalSelector{ SignalSelectorIndex(0u) };
	std::uint16_t numberOfMixers{ 0u };
	MixerIndex baseMixer{ MixerIndex(0u) };
	std::uint16_t numberOfMatrices{ 0u };
	MatrixIndex baseMatrix{ MatrixIndex(0u) };
	std::uint16_t numberOfSplitters{ 0u };
	SignalSplitterIndex baseSplitter{ SignalSplitterIndex(0u) };
	std::uint16_t numberOfCombiners{ 0u };
	SignalCombinerIndex baseCombiner{ SignalCombinerIndex(0u) };
	std::uint16_t numberOfDemultiplexers{ 0u };
	SignalDemultiplexerIndex baseDemultiplexer{ SignalDemultiplexerIndex(0u) };
	std::uint16_t numberOfMultiplexers{ 0u };
	SignalMultiplexerIndex baseMultiplexer{ SignalMultiplexerIndex(0u) };
	std::uint16_t numberOfTranscoders{ 0u };
	SignalTranscoderIndex baseTranscoder{ SignalTranscoderIndex(0u) };
	std::uint16_t numberOfControlBlocks{ 0u };
	ControlBlockIndex baseControlBlock{ ControlBlockIndex(0u) };
	SamplingRate currentSamplingRate{};
	SamplingRate samplingRates[ATDECC_MAX_SAMPLING_RATES]{};
};

/** STREAM_INPUT and STREAM_OUTPUT Descriptor - Clause 7.2.6 */
struct StreamDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	ClockDomainIndex clockDomainIndex{ ClockDomainIndex(0u) };
	StreamFlags streamFlags{};
	StreamFormat currentFormat{};
	UniqueIdentifier backupTalkerEntityID_0{};
	std::uint16_t backupTalkerUniqueID_0{ 0u };
	UniqueIdentifier backupTalkerEntityID_1{};
	std::uint16_t backupTalkerUniqueID_1{ 0u };
	UniqueIdentifier backupTalkerEntityID_2{};
	std::uint16_t backupTalkerUniqueID_2{ 0u };
	UniqueIdentifier backedupTalkerEntityID{};
	std::uint16_t backedupTalkerUnique{ 0u };
	AvbInterfaceIndex avbInterfaceIndex{ AvbInterfaceIndex(0u) };
	std::uint32_t bufferLength{ 0u };
	StreamFormat formats[ATDECC_MAX_FORMATS]{};
};

/** JACK_INPUT and JACK_OUTPUT Descriptor - Clause 7.2.7 */
struct JackDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	JackFlags jackFlags{};
	JackType jackType{ JackType::Speaker };
	std::uint16_t numberOfControls{ 0u };
	ControlIndex baseControl{ ControlIndex(0u) };
};

/** AVB_INTERFACE Descriptor - Clause 7.2.8 */
struct AvbInterfaceDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	//MacAddress macAddress{};
	AvbInterfaceFlags interfaceFlags{};
	UniqueIdentifier clockIdentity{};
	std::uint8_t priority1{ 0xff };
	std::uint8_t clockClass{ 0xff };
	std::uint16_t offsetScaledLogVariance{ 0x0000 };
	std::uint8_t clockAccuracy{ 0xff };
	std::uint8_t priority2{ 0xff };
	std::uint8_t domainNumber{ 0u };
	std::uint8_t logSyncInterval{ 0u };
	std::uint8_t logAnnounceInterval{ 0u };
	std::uint8_t logPDelayInterval{ 0u };
	std::uint16_t portNumber{ 0x0000 };
};

/** CLOCK_SOURCE Descriptor - Clause 7.2.9 */
struct ClockSourceDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	ClockSourceFlags clockSourceFlags{};
	ClockSourceType clockSourceType{ ClockSourceType::Internal };
	UniqueIdentifier clockSourceIdentifier{};
	DescriptorType clockSourceLocationType{ DescriptorType::Invalid };
	DescriptorIndex clockSourceLocationIndex{ DescriptorIndex(0u) };
};

/** CLOCK_DOMAIN Descriptor - Clause 7.2.32 */
struct ClockDomainDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	ClockSourceIndex clockSourceIndex{ ClockSourceIndex(0u) };
	std::vector<ClockSourceIndex> clockSources{};
};

/** MEMORY_OBJECT Descriptor - Clause 7.2.10 */
struct MemoryObjectDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	MemoryObjectType memoryObjectType{ MemoryObjectType::FirmwareImage };
	DescriptorType targetDescriptorType{ DescriptorType::Invalid };
	DescriptorIndex targetDescriptorIndex{ DescriptorIndex(0u) };
	std::uint64_t startAddress{ 0u };
	std::uint64_t maximumLength{ 0u };
	std::uint64_t length{ 0u };
};

/** LOCALE Descriptor - Clause 7.2.11 */
struct LocaleDescriptor
{
	AtdeccFixedString localeID{};
	std::uint16_t numberOfStringDescriptors{ 0u };
	StringsIndex baseStringDescriptorIndex{ StringsIndex(0u) };
};

/** STRINGS Descriptor - Clause 7.2.12 */
struct StringsDescriptor
{
	AtdeccFixedString strings[7]{};
};

/** STREAM_PORT Descriptor - Clause 7.2.13 */
struct StreamPortDescriptor
{
	ClockDomainIndex clockDomainIndex{ ClockDomainIndex(0u) };
	PortFlags portFlags{};
	std::uint16_t numberOfControls{ 0u };
	ControlIndex baseControl{ ControlIndex(0u) };
	std::uint16_t numberOfClusters{ 0u };
	ClusterIndex baseCluster{ ClusterIndex(0u) };
	std::uint16_t numberOfMaps{ 0u };
	MapIndex baseMap{ MapIndex(0u) };
};

/** EXTERNAL_PORT Descriptor - Clause 7.2.14 */
struct ExternalPortDescriptor
{
	ClockDomainIndex clockDomainIndex{ ClockDomainIndex(0u) };
	PortFlags portFlags{};
	std::uint16_t numberOfControls{ 0u };
	ControlIndex baseControl{ ControlIndex(0u) };
	DescriptorType signalType{ DescriptorType::Invalid };
	DescriptorIndex signalIndex{ DescriptorIndex(0u) };
	std::uint16_t signalOutput{ 0u };
	std::uint32_t blockLatency{ 0u };
	JackIndex jackIndex{ JackIndex(0u) };
};

/** INTERNAL_PORT Descriptor - Clause 7.2.15 */
struct InternalPortDescriptor
{
	ClockDomainIndex clockDomainIndex{ ClockDomainIndex(0u) };
	PortFlags portFlags{};
	std::uint16_t numberOfControls{ 0u };
	ControlIndex baseControl{ ControlIndex(0u) };
	DescriptorType signalType{ DescriptorType::Invalid };
	DescriptorIndex signalIndex{ DescriptorIndex(0u) };
	std::uint16_t signalOutput{ 0u };
	std::uint32_t blockLatency{ 0u };
	InternalPortIndex internalIndex{ InternalPortIndex(0u) };
};

/** AUDIO_CLUSTER Descriptor - Clause 7.2.16 */
struct AudioClusterDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	DescriptorType signalType{ DescriptorType::Invalid };
	DescriptorIndex signalIndex{ DescriptorIndex(0u) };
	std::uint16_t signalOutput{ 0u };
	std::uint32_t pathLatency{ 0u };
	std::uint32_t blockLatency{ 0u };
	std::uint16_t channelCount{ 0u };
	AudioClusterFormat format{ AudioClusterFormat::Iec60958 };
};

/** CONTROL Descriptor - Clause 7.2.22 */
struct ControlDescriptor
{
	AtdeccFixedString objectName{};
	LocalizedStringReference localizedDescription{};
	std::uint32_t blockLatency{ 0u };
	std::uint32_t controlLatency{ 0u };
	std::uint16_t controlDomain{ 0u };
	UniqueIdentifier controlType{};
	std::uint32_t resetTime{ 0u };
	DescriptorType signalType{ DescriptorType::Invalid };
	DescriptorIndex signalIndex{ DescriptorIndex(0u) };
	std::uint16_t signalOutput{ 0u };
	ControlValueType controlValueType{};
	ControlValues valuesStatic{};
	ControlValues valuesDynamic{};
};

/** AUDIO_MAP Descriptor - Clause 7.2.19 */
struct AudioMapDescriptor
{
	AudioMapping mappings{};
};

/** GET_STREAM_INFO Dynamic Information - Clause 7.4.16.2 */
struct StreamInfo
{
	StreamInfoFlags streamInfoFlags{};
	StreamFormat streamFormat{};
	UniqueIdentifier streamID{ 0u };
	std::uint32_t msrpAccumulatedLatency{ 0u };
	MacAddress streamDestMac{};
	MsrpFailureCode msrpFailureCode{ MsrpFailureCode::NoFailure };
	BridgeIdentifier msrpFailureBridgeID{ 0u };
	std::uint16_t streamVlanID{ 0u };
	
	/*StreamInfoFlagsEx streamInfoFlagsEx{};
	ProbingStatus probingStatus{};
	AcmpStatus acmpStatus{};*/
};

constexpr bool operator==(const StreamInfo& lhs, const StreamInfo& rhs) noexcept
{
	return (lhs.streamInfoFlags.getValue() == rhs.streamInfoFlags.getValue()) &&
	       (lhs.streamFormat == rhs.streamFormat) &&
	       (lhs.streamID == rhs.streamID) &&
	       (lhs.msrpAccumulatedLatency == rhs.msrpAccumulatedLatency) &&
	       //(lhs.streamDestMac == rhs.streamDestMac) &&
	       (lhs.msrpFailureCode == rhs.msrpFailureCode) &&
	       (lhs.msrpFailureBridgeID == rhs.msrpFailureBridgeID) &&
	       (lhs.streamVlanID == rhs.streamVlanID);
}

constexpr bool operator!=(const StreamInfo& lhs, const StreamInfo& rhs) noexcept
{
	return !(lhs == rhs);
}

/** GET_AVB_INFO Dynamic Information - Clause 7.4.40.2 */
struct AvbInfo
{
	UniqueIdentifier gptpGrandmasterID{};
	std::uint32_t propagationDelay{ 0u };
	std::uint8_t gptpDomainNumber{ 0u };
	AvbInfoFlags flags{};
	MsrpMappings mappings{};
};

constexpr bool operator==(const AvbInfo& lhs, const AvbInfo& rhs) noexcept
{
	return (lhs.gptpGrandmasterID == rhs.gptpGrandmasterID) &&
	       (lhs.propagationDelay == rhs.propagationDelay) &&
	       (lhs.gptpDomainNumber == rhs.gptpDomainNumber) &&
	       (lhs.flags.getValue() == rhs.flags.getValue()) &&
	       (lhs.mappings == rhs.mappings);
}

constexpr bool operator!=(const AvbInfo& lhs, const AvbInfo& rhs) noexcept
{
	return !(lhs == rhs);
}

/**
* @brief Make a UniqueIdentifier from vendorID, deviceID and modelID.
* @details Helper method to construct a UniqueIdentifier from vendorID, deviceID and modelID to be used as EntityModelID.
* @param[in] vendorID OUI-24 of the vendor (8 MSBs should be 0, ignored regardless).
* @param[in] deviceID ID of the device (vendor specific).
* @param[in] modelID ID of the model (vendor specific).
* @return Valid UniqueIdentifier that can be used as EntityModelID in ADP messages and EntityDescriptor.
* @note This method is provided as a helper. Packing an EntityModelID that way is NOT mandatory (except for the vendorID).
* @warning This method is intended to be used for an OUI-24, not an OUI-36.
*/
inline UniqueIdentifier makeEntityModelID(std::uint32_t const vendorID, std::uint8_t const deviceID, std::uint32_t const modelID) noexcept
{
	return UniqueIdentifier{ (static_cast<UniqueIdentifier::value_type>(vendorID) << 40) + (static_cast<UniqueIdentifier::value_type>(deviceID) << 32) + static_cast<UniqueIdentifier::value_type>(modelID) };
}

/**
* @brief Split a UniqueIdentifier representing an EntityModelID into vendorID, deviceID and modelID.
* @details Helper method to split a UniqueIdentifier representing an EntityModelID into vendorID, deviceID and modelID.
* @param[in] entityModelID The UniqueIdentifier representing an EntityModelID.
* @return Tuple of vendorID (OUI-24), deviceID and modelID.
* @note This method is provided as a helper. Packing an EntityModelID that way is NOT mandatory (except for the vendorID).
* @warning This method is intended to be used for an OUI-24, not an OUI-36.
*/
inline std::tuple<std::uint32_t, std::uint8_t, std::uint32_t> splitEntityModelID(UniqueIdentifier const entityModelID) noexcept
{
	auto const value = entityModelID.getValue();
	return std::make_tuple(static_cast<std::uint32_t>((value >> 40) & 0x0000000000FFFFFF), static_cast<std::uint8_t>((value >> 32) & 0x00000000000000FF), static_cast<std::uint32_t>(value & 0x00000000FFFFFFFF));
}

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYMODEL_HPP_ */
