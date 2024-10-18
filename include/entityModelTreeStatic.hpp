#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREESTATIC_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREESTATIC_HPP_

#pragma once

#include "entityModelTreeCommon.hpp"
#include <cstdint>
#include <set>
#include <array>
#include <unordered_map>
#include <vector>

struct AudioUnitNodeStaticModel
{
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
    SamplingRates samplingRates{};
};

struct StreamNodeStaticModel
{
    LocalizedStringReference localizedDescription{};
    ClockDomainIndex clockDomainIndex{ ClockDomainIndex(0u) };
    StreamFlags streamFlags{};
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
    StreamFormats formats{};
#ifdef ENABLE_ATDECC_FEATURE_REDUNDANCY
    RedundantStreams redundantStreams{};
#endif // ENABLE_ATDECC_FEATURE_REDUNDANCY
};

struct AvbInterfaceNodeStaticModel
{
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

struct ClockSourceNodeStaticModel
{
    LocalizedStringReference localizedDescription{};
    ClockSourceType clockSourceType{ ClockSourceType::Internal };
    DescriptorType clockSourceLocationType{ DescriptorType::Invalid };
    DescriptorIndex clockSourceLocationIndex{ DescriptorIndex(0u) };
};

struct MemoryObjectNodeStaticModel
{
    LocalizedStringReference localizedDescription{};
    MemoryObjectType memoryObjectType{ MemoryObjectType::FirmwareImage };
    DescriptorType targetDescriptorType{ DescriptorType::Invalid };
    DescriptorIndex targetDescriptorIndex{ DescriptorIndex(0u) };
    std::uint64_t startAddress{ 0u };
    std::uint64_t maximumLength{ 0u };
};

struct LocaleNodeStaticModel
{
    AtdeccFixedString localeID{};
    std::uint16_t numberOfStringDescriptors{ 0u };
    StringsIndex baseStringDescriptorIndex{ StringsIndex(0u) };
};

struct StringsNodeStaticModel
{
    AtdeccFixedStrings strings{};
};

struct StreamPortNodeStaticModel
{
    ClockDomainIndex clockDomainIndex{ ClockDomainIndex(0u) };
    PortFlags portFlags{};
    std::uint16_t numberOfControls{ 0u };
    ControlIndex baseControl{ ControlIndex(0u) };
    std::uint16_t numberOfClusters{ 0u };
    ClusterIndex baseCluster{ ClusterIndex(0u) };
    std::uint16_t numberOfMaps{ 0u };
    MapIndex baseMap{ MapIndex(0u) };
    bool hasDynamicAudioMap{ false };
};

struct AudioClusterNodeStaticModel
{
    LocalizedStringReference localizedDescription{};
    DescriptorType signalType{ DescriptorType::Invalid };
    DescriptorIndex signalIndex{ DescriptorIndex(0u) };
    std::uint16_t signalOutput{ 0u };
    std::uint32_t pathLatency{ 0u };
    std::uint32_t blockLatency{ 0u };
    std::uint16_t channelCount{ 0u };
    AudioClusterFormat format{ AudioClusterFormat::Iec60958 };
};

struct AudioMapNodeStaticModel
{
    AudioMappings mappings{};
};

struct ControlNodeStaticModel
{
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
    ControlValues values{};
};

struct ClockDomainNodeStaticModel
{
    LocalizedStringReference localizedDescription{};
    ClockSources clockSources{};
};

struct ConfigurationNodeStaticModel
{
    LocalizedStringReference localizedDescription{};
    DescriptorCounts descriptorCounts{};
};

struct EntityNodeStaticModel
{
    LocalizedStringReference vendorNameString{};
    LocalizedStringReference modelNameString{};
};

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREESTATIC_HPP_ */
