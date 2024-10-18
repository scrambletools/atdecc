#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREECOMMON_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREECOMMON_HPP_

#pragma once

#include "entityModel.hpp"
#include <set>
#include <array>
#include <unordered_map>
#include <vector>

struct StreamInputConnectionInfo
{
    enum class State
    {
        NotConnected = 0,
        FastConnecting = 1,
        Connected = 2,
    };
    StreamIdentification talkerStream{}; // Only valid if state != State::NotConnected
    State state{ State::NotConnected };
};

inline bool operator==(StreamInputConnectionInfo const& lhs, StreamInputConnectionInfo const& rhs) noexcept
{
    if (lhs.state == rhs.state)
    {
        // Only compare talkerStream field if State is not NotConnected
        return (lhs.state == StreamInputConnectionInfo::State::NotConnected) || (lhs.talkerStream == rhs.talkerStream);
    }
    return false;
}

constexpr bool operator!=(StreamInputConnectionInfo const& lhs, StreamInputConnectionInfo const& rhs) noexcept
{
    return !(lhs == rhs);
}

/** A subset of StreamInfo */
struct StreamDynamicInfo
{
    bool isClassB{ false };           // Indicates that the Stream is Class B instead of Class A (default 0 is class A).
    bool hasSavedState{ false };      // Connection has saved ACMP state.
    bool doesSupportEncrypted{ false };  // Indicates that the Stream supports streaming with encrypted PDUs.
    bool arePdusEncrypted{ false };      // Indicates that the Stream is using encrypted PDUs.
    bool hasTalkerFailed{ false };       // Indicates that the Listener has registered an SRP Talker Failed attribute for the Stream.
    StreamInfoFlags _streamInfoFlags{};  // LEGACY FIELD - Last received StreamInfoFlags
    UniqueIdentifier streamID{};
    std::uint32_t msrpAccumulatedLatency{};
    //std::optional<la::networkInterface::MacAddress> streamDestMac{};
    MsrpFailureCode msrpFailureCode{};
    BridgeIdentifier msrpFailureBridgeID{};
    std::uint16_t streamVlanID{};
};

constexpr bool operator==(StreamDynamicInfo const& lhs, StreamDynamicInfo const& rhs) noexcept
{
    return (lhs.isClassB == rhs.isClassB) && (lhs.hasSavedState == rhs.hasSavedState) && (lhs.doesSupportEncrypted == rhs.doesSupportEncrypted) && (lhs.arePdusEncrypted == rhs.arePdusEncrypted) && (lhs.hasTalkerFailed == rhs.hasTalkerFailed) && (lhs._streamInfoFlags.getValue() == rhs._streamInfoFlags.getValue()) && (lhs.streamID == rhs.streamID) && (lhs.msrpAccumulatedLatency == rhs.msrpAccumulatedLatency) && (lhs.msrpFailureCode == rhs.msrpFailureCode) && (lhs.msrpFailureBridgeID == rhs.msrpFailureBridgeID) && (lhs.streamVlanID == rhs.streamVlanID);
}

constexpr bool operator!=(StreamDynamicInfo const& lhs, StreamDynamicInfo const& rhs) noexcept
{
    return !(lhs == rhs);
}

/** A subset of AvbInfo */
struct AvbInterfaceInfo
{
    std::uint32_t propagationDelay{ 0u };
    AvbInfoFlags flags{};
    MsrpMappings mappings{};
};

constexpr bool operator==(AvbInterfaceInfo const& lhs, AvbInterfaceInfo const& rhs) noexcept
{
    return (lhs.propagationDelay == rhs.propagationDelay) && (lhs.flags.getValue() == rhs.flags.getValue()) && (lhs.mappings == rhs.mappings);
}

constexpr bool operator!=(AvbInterfaceInfo const& lhs, AvbInterfaceInfo const& rhs) noexcept
{
    return !(lhs == rhs);
}

using StreamConnections = std::set<StreamIdentification>;
using StreamFormats = std::set<StreamFormat>;
#ifdef ENABLE_ATDECC_FEATURE_REDUNDANCY
using RedundantStreams = std::set<StreamIndex>;
#endif // ENABLE_ATDECC_FEATURE_REDUNDANCY
using SamplingRates = std::set<SamplingRate>;
using AtdeccFixedStrings = std::array<AtdeccFixedString, 7>;
using ClockSources = std::vector<ClockSourceIndex>;
using DescriptorCounts = std::unordered_map<DescriptorType, std::uint16_t, EnumClassHash>;

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREECOMMON_HPP_ */
