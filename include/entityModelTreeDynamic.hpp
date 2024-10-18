#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREEDYNAMIC_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREEDYNAMIC_HPP_

#pragma once

#include "entityModelTreeCommon.hpp"

#include <unordered_map>
#include <cstdint>
#include <map>
#include <optional>

using EntityCounters = std::map<EntityCounterValidFlag, DescriptorCounter>;
using AvbInterfaceCounters = std::map<AvbInterfaceCounterValidFlag, DescriptorCounter>;
using ClockDomainCounters = std::map<ClockDomainCounterValidFlag, DescriptorCounter>;
using StreamInputCounters = std::map<StreamInputCounterValidFlag, DescriptorCounter>;
using StreamOutputCounters = std::map<StreamOutputCounterValidFlag, DescriptorCounter>;

struct AudioUnitNodeDynamicModel
{
    AtdeccFixedString objectName{};
    SamplingRate currentSamplingRate{};
};

struct StreamNodeDynamicModel
{
    AtdeccFixedString objectName{};
    StreamFormat streamFormat{};
    bool isStreamRunning{};
    StreamDynamicInfo streamDynamicInfo{};
};

struct StreamInputNodeDynamicModel : public StreamNodeDynamicModel
{
    StreamInputConnectionInfo connectionInfo{};
    StreamInputCounters counters{};
};

struct StreamOutputNodeDynamicModel : public StreamNodeDynamicModel
{
    StreamConnections connections{};
    StreamOutputCounters counters{};
};

struct AvbInterfaceNodeDynamicModel
{
    AtdeccFixedString objectName{};
    UniqueIdentifier gptpGrandmasterID{};
    std::uint8_t gptpDomainNumber{ 0u };
    AvbInterfaceInfo avbInterfaceInfo{};
    AsPath asPath{};
    AvbInterfaceCounters counters{};
};

struct ClockSourceNodeDynamicModel
{
    AtdeccFixedString objectName{};
    ClockSourceFlags clockSourceFlags{};
    UniqueIdentifier clockSourceIdentifier{};
};

struct MemoryObjectNodeDynamicModel
{
    AtdeccFixedString objectName{};
    std::uint64_t length{ 0u };
};

// Optional locale and string dynamic models have been commented out as per original

struct StreamPortNodeDynamicModel
{
    AudioMappings dynamicAudioMap{};
};

struct AudioClusterNodeDynamicModel
{
    AtdeccFixedString objectName{};
};

struct ControlNodeDynamicModel
{
    AtdeccFixedString objectName{};
    ControlValues values{};
};

struct ClockDomainNodeDynamicModel
{
    AtdeccFixedString objectName{};
    ClockSourceIndex clockSourceIndex{ ClockSourceIndex(0u) };
    ClockDomainCounters counters{};
};

struct ConfigurationNodeDynamicModel
{
    AtdeccFixedString objectName{};
    bool isActiveConfiguration{ false };

    // Internal variables
    StringsIndex selectedLocaleBaseIndex{ StringsIndex{ 0u } };  /** Base StringIndex for the selected locale */
    StringsIndex selectedLocaleCountIndexes{ StringsIndex{ 0u } };  /** Count StringIndexes for the selected locale */
    std::unordered_map<StringsIndex, AtdeccFixedString> localizedStrings{};  /** Aggregated copy of all loaded localized strings */
};

struct EntityNodeDynamicModel
{
    AtdeccFixedString entityName{};
    AtdeccFixedString groupName{};
    AtdeccFixedString firmwareVersion{};
    AtdeccFixedString serialNumber{};
    std::uint16_t currentConfiguration{ 0u };
    EntityCounters counters{};
};

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREEDYNAMIC_HPP_ */
