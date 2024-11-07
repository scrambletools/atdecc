/*
 * entityModelTypes.hpp
 *
 *  Created on: Sep 28, 2024
 *      Author: Dulini
 */

#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTYPES_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTYPES_HPP_

#include <string>
#pragma once

#include <stdint.h>  // Standard integer types
#include <stdio.h>   // For printf (replacing std::ostream usage)
#include <string.h>  // For memcpy, strcmp
#include <vector>
#include <type_traits>
#include <utility>   // for std::make_pair
#include <esp_log.h> // for ESP_LOGE
#include "uniqueIdentifier.hpp"

template <typename T>
struct decay
{
    using type = typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

template <typename T>
using decay_t = typename decay<T>::type;

// Replace all DescriptorIndex types with simpler uint16_t
typedef uint16_t DescriptorIndex;
#define INVALID_DESCRIPTOR_INDEX 0xFFFF

using ConfigurationIndex = uint16_t;
using DescriptorIndex = uint16_t;
using AudioUnitIndex = DescriptorIndex;
using StreamIndex = DescriptorIndex;
using JackIndex = DescriptorIndex;
using AvbInterfaceIndex = DescriptorIndex;
using ClockSourceIndex = DescriptorIndex;
using MemoryObjectIndex = DescriptorIndex;
using LocaleIndex = DescriptorIndex;
using StringsIndex = DescriptorIndex;
using StreamPortIndex = DescriptorIndex;
using ExternalPortIndex = DescriptorIndex;
using InternalPortIndex = DescriptorIndex;
using ClusterIndex = DescriptorIndex;
using MapIndex = DescriptorIndex;
using ControlIndex = DescriptorIndex;
using SignalSelectorIndex = DescriptorIndex;
using MixerIndex = DescriptorIndex;
using MatrixIndex = DescriptorIndex;
using SignalSplitterIndex = DescriptorIndex;
using SignalCombinerIndex = DescriptorIndex;
using SignalDemultiplexerIndex = DescriptorIndex;
using SignalMultiplexerIndex = DescriptorIndex;
using SignalTranscoderIndex = DescriptorIndex;
using ClockDomainIndex = DescriptorIndex;
using ControlBlockIndex = DescriptorIndex;
using DescriptorCounterValidFlag = uint32_t; /** Counters valid flag - Clause 7.4.42 */
using DescriptorCounter = uint32_t; /** Counter - Clause 7.4.42 */
using OperationID = uint16_t; /** OperationID for OPERATIONS returned by an entity to a controller - Clause 7.4.53 */
using BridgeIdentifier = uint64_t;

// Simplified Descriptor Type Enum - Clause 7.2
enum class DescriptorType : uint16_t
{
	Entity =              0x0000,
	Configuration =       0x0001,
	AudioUnit =           0x0002,
	VideoUnit =           0x0003,
	SensorUnit =          0x0004,
	StreamInput =         0x0005,
	StreamOutput =        0x0006,
	JackInput =           0x0007,
	JackOutput =          0x0008,
	AvbInterface =        0x0009,
	ClockSource =         0x000a,
	MemoryObject =        0x000b,
	Locale =              0x000c,
	Strings =             0x000d,
	StreamPortInput =     0x000e,
	StreamPortOutput =    0x000f,
	ExternalPortInput =   0x0010,
	ExternalPortOutput =  0x0011,
	InternalPortInput =   0x0012,
	InternalPortOutput =  0x0013,
	AudioCluster =        0x0014,
	VideoCluster =        0x0015,
	SensorCluster =       0x0016,
	AudioMap =            0x0017,
	VideoMap =            0x0018,
	SensorMap =           0x0019,
	Control =             0x001a,
	SignalSelector =      0x001b,
	Mixer =               0x001c,
	Matrix =              0x001d,
	MatrixSignal =        0x001e,
	SignalSplitter =      0x001f,
	SignalCombiner =      0x0020,
	SignalDemultiplexer = 0x0021,
	SignalMultiplexer =   0x0022,
	SignalTranscoder =    0x0023,
	ClockDomain =         0x0024,
	ControlBlock =        0x0025,
	/* 0026 to fffe reserved for future use */
	Invalid =             0xffff
};

constexpr bool operator!(DescriptorType const lhs)
{
    return lhs == DescriptorType::Invalid;
}

class AtdeccFixedString final
{
public:
    static constexpr size_t MaxLength = 64;
    using value_type = char;

    /** Default constructor */
    AtdeccFixedString() noexcept
    {
        clear();
    }

    /** Constructor from a std::string */
    AtdeccFixedString(std::string const& str) noexcept
    {
        assign(str);
    }

    /** Constructor from a raw buffer */
    AtdeccFixedString(void const* const ptr, size_t const size) noexcept
    {
        assign(ptr, size);
    }

    /** Assign from a std::string */
    void assign(std::string const& str) noexcept
    {
        assign(str.c_str(), str.size());
    }

    /** Assign from a raw buffer */
    void assign(void const* const ptr, size_t const size) noexcept
    {
        auto const copySize = std::min(size, MaxLength);

        // Copy buffer content
        memcpy(_buffer, ptr, copySize);

        // Fill remaining bytes with '\0'
        if (copySize < MaxLength)
        {
            memset(_buffer + copySize, 0, MaxLength - copySize);
        }
    }

    /** Clears the string */
    void clear() noexcept
    {
        memset(_buffer, 0, MaxLength);
    }

    /** Returns the raw buffer */
    value_type* data() noexcept
    {
        return _buffer;
    }

    /** Returns the raw buffer (const) (might not be NULL terminated) */
    value_type const* data() const noexcept
    {
        return _buffer;
    }

    /** Returns the (fixed) size of the buffer */
    constexpr size_t size() const noexcept
    {
        return MaxLength;
    }

    /** Returns true if the buffer contains only '\0' */
    bool empty() const noexcept
    {
        return _buffer[0] == '\0';
    }

    /** Direct access operator */
    value_type& operator[](size_t const pos)
    {
        if (pos >= MaxLength)
        {
            ESP_LOGE("AtdeccFixedString", "Index out of range in AtdeccFixedString::operator[]");
            return _buffer[MaxLength - 1]; // Return the last element as a fallback
        }

        return _buffer[pos];
    }

    /** Direct access const operator */
    value_type const& operator[](size_t const pos) const
    {
        if (pos >= MaxLength)
        {
            ESP_LOGE("AtdeccFixedString", "Index out of range in AtdeccFixedString::operator[]");
            return _buffer[MaxLength - 1]; // Return the last element as a fallback
        }

        return _buffer[pos];
    }

    /** operator== */
    bool operator==(AtdeccFixedString const& afs) const noexcept
    {
        return memcmp(_buffer, afs._buffer, MaxLength) == 0;
    }

    /** operator!= */
    bool operator!=(AtdeccFixedString const& afs) const noexcept
    {
        return !(*this == afs);
    }

    /** operator== overload for a std::string */
    bool operator==(std::string const& str) const noexcept
    {
        return this->str() == str;
    }

    /** operator!= overload for a std::string */
    bool operator!=(std::string const& str) const noexcept
    {
        return !(*this == str);
    }

    /** Returns this AtdeccFixedString as a std::string */
    std::string str() const noexcept
    {
        return std::string(_buffer, strnlen(_buffer, MaxLength));
    }

    /** Returns this AtdeccFixedString as a std::string */
    operator std::string() const noexcept
    {
        return str();
    }

private:
    value_type _buffer[MaxLength];
};

// SamplingRate class definition
class SamplingRate final
{
public:
	
    using value_type = uint32_t;

    /** Default constructor. */
    constexpr SamplingRate() noexcept {}

    /** Constructor to create a SamplingRate from the underlying value. */
    explicit constexpr SamplingRate(value_type const value) noexcept
        : _value(value)
    {
    }

    /** Constructor to create a SamplingRate from pull and baseFrequency values. */
    constexpr SamplingRate(uint8_t const pull, uint32_t const baseFrequency) noexcept
        : _value((pull << 29) + (baseFrequency & 0x1FFFFFFF))
    {
    }

    /** Setter to change the underlying value. */
    void setValue(value_type value) noexcept
    {
        _value = value;
    }

    /** Getter to retrieve the underlying value. */
    constexpr value_type getValue() const noexcept
    {
        return _value;
    }

    /** Gets the Nominal Sample Rate value. */
    double getNominalSampleRate() const noexcept
    {
        auto const [pull, frequency] = getPullBaseFrequency();
        switch (pull)
        {
            case 0:
                return frequency;
            case 1:
                return frequency * 1.0 / 1.001;
            case 2:
                return frequency * 1.001;
            case 3:
                return frequency * 24.0 / 25.0;
            case 4:
                return frequency * 25.0 / 24.0;
            default: // 5 to 7 reserved for future use
                ESP_LOGE("SamplingRate", "Unknown pull value");
                return frequency;
        }
    }

    /** Getter to retrieve the pull and baseFrequency values from this SamplingRate. */
    constexpr std::pair<uint8_t, uint32_t> getPullBaseFrequency() const noexcept
    {
        return std::make_pair(static_cast<uint8_t>(_value >> 29), static_cast<uint32_t>(_value & 0x1FFFFFFF));
    }

    /** Getter to retrieve the pull value from this SamplingRate. */
    constexpr uint8_t getPull() const noexcept
    {
        return static_cast<uint8_t>(_value >> 29);
    }

    /** Getter to retrieve the baseFrequency value from this SamplingRate. */
    constexpr uint32_t getBaseFrequency() const noexcept
    {
        return static_cast<uint32_t>(_value & 0x1FFFFFFF);
    }

    /** True if the SamplingRate contains a valid underlying value, false otherwise. */
    constexpr bool isValid() const noexcept
    {
        // Clause 7.3.1.2 says base_frequency ranges from 1 to 536'870'911, so we can use 0 to detect invalid value.
        return (_value & 0x1FFFFFFF) != 0;
    }

    /** Underlying value operator (equivalent to getValue()). */
    constexpr operator value_type() const noexcept
    {
        return getValue();
    }

    /** Underlying value validity bool operator (equivalent to isValid()). */
    explicit constexpr operator bool() const noexcept
    {
        return isValid();
    }

    /** Equality operator. Returns true if the underlying values are equal (Any 2 invalid SamplingRate are considered equal, since they are both invalid). */
    constexpr friend bool operator==(SamplingRate const& lhs, SamplingRate const& rhs) noexcept
    {
        return (!lhs.isValid() && !rhs.isValid()) || lhs._value == rhs._value;
    }

    /** Non equality operator. */
    constexpr friend bool operator!=(SamplingRate const& lhs, SamplingRate const& rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }

    /** operator< */
    constexpr friend bool operator<(SamplingRate const& lhs, SamplingRate const& rhs) noexcept
    {
        return lhs._value < rhs._value;
    }

    /** Static helper method to create a Null SamplingRate (isValid() returns false). */
    static SamplingRate getNullSamplingRate() noexcept
    {
        return SamplingRate{ NullSamplingRate };
    }

    /** Hash functor to be used for std::hash */
    struct hash
    {
        std::size_t operator()(SamplingRate const& ref) const
        {
            return std::hash<value_type>()(ref._value);
        }
    };

    // Defaulted compiler auto-generated methods
    SamplingRate(SamplingRate&&) = default;
    SamplingRate(SamplingRate const&) = default;
    SamplingRate& operator=(SamplingRate const&) = default;
    SamplingRate& operator=(SamplingRate&&) = default;

private:
    static constexpr value_type NullSamplingRate = 0u;
    value_type _value{ NullSamplingRate };
};

class StreamFormat final
{
public:
    using value_type = uint64_t;

    /** Default constructor. */
    constexpr StreamFormat() noexcept : _value(NullStreamFormat) {}

    /** Constructor to create a StreamFormat from the underlying value. */
    explicit StreamFormat(value_type const value) noexcept
        : _value(value)
    {
    }

    /** Setter to change the underlying value. */
    void setValue(value_type const value) noexcept
    {
        _value = value;
    }

    /** Getter to retrieve the underlying value. */
    constexpr value_type getValue() const noexcept
    {
        return _value;
    }

    /** True if the StreamFormat contains a valid underlying value, false otherwise. */
    constexpr bool isValid() const noexcept
    {
        return _value != NullStreamFormat;
    }

    /** Underlying value operator (equivalent to getValue()). */
    constexpr operator value_type() const noexcept
    {
        return getValue();
    }

    /** Underlying value validity bool operator (equivalent to isValid()). */
    explicit constexpr operator bool() const noexcept
    {
        return isValid();
    }

    /** Equality operator. Returns true if the underlying values are equal (Any 2 invalid StreamFormats are considered equal, since they are both invalid). */
    constexpr friend bool operator==(StreamFormat const& lhs, StreamFormat const& rhs) noexcept
    {
        return (!lhs.isValid() && !rhs.isValid()) || lhs._value == rhs._value;
    }

    /** Non equality operator. */
    constexpr friend bool operator!=(StreamFormat const& lhs, StreamFormat const& rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }

    /** operator< */
    constexpr friend bool operator<(StreamFormat const& lhs, StreamFormat const& rhs) noexcept
    {
        return lhs._value < rhs._value;
    }

    /** Static helper method to create a Null StreamFormat (isValid() returns false). */
    static StreamFormat getNullStreamFormat() noexcept
    {
        return StreamFormat{ NullStreamFormat };
    }

    /** Hash functor to be used for std::hash */
    struct hash
    {
        std::size_t operator()(StreamFormat const& ref) const
        {
            return std::hash<value_type>()(ref._value);
        }
    };

    // Defaulted compiler auto-generated methods
    StreamFormat(StreamFormat&&) = default;
    StreamFormat(StreamFormat const&) = default;
    StreamFormat& operator=(StreamFormat const&) = default;
    StreamFormat& operator=(StreamFormat&&) = default;

private:
    static constexpr value_type NullStreamFormat = 0ul;
    value_type _value{ NullStreamFormat };
};

class LocalizedStringReference final
{
public:
    using value_type = uint16_t;

    /** Default constructor. */
    constexpr LocalizedStringReference() noexcept : _value(NullLocalizedStringReference) {}

    /** Constructor to create a LocalizedStringReference from the underlying value. */
    explicit constexpr LocalizedStringReference(value_type const value) noexcept
        : _value(value)
    {
    }

    /** Constructor to create a LocalizedStringReference from offset and index values. */
    LocalizedStringReference(uint16_t const offset, uint8_t const index) noexcept
    {
        setOffsetIndex(offset, index);
    }

    /** Setter to change the underlying value. */
    void setValue(value_type const value) noexcept
    {
        _value = value;
    }

    /** Getter to retrieve the underlying value. */
    constexpr value_type getValue() const noexcept
    {
        return _value;
    }

    /** Getter to retrieve the global offset for this LocalizedStringReference. */
    value_type getGlobalOffset() const
    {
        if (!isValid())
        {
            ESP_LOGE("LocalizedStringReference", "Invalid LocalizedStringReference");
            return 0; // Return an appropriate invalid value or handle error accordingly
        }

        auto const [offset, index] = getOffsetIndex();
        return ((offset * 7u) + index) & 0xFFFF;
    }

    /** Setter to change the offset and index values for this LocalizedStringReference. */
    void setOffsetIndex(uint16_t const offset, uint8_t const index) noexcept
    {
        _value = (offset << 3) + (index & 0x07);
    }

    /** Getter to retrieve the offset and index values from this LocalizedStringReference. */
    constexpr std::pair<uint16_t, uint8_t> getOffsetIndex() const noexcept
    {
        return std::make_pair(static_cast<uint16_t>(_value >> 3), static_cast<uint8_t>(_value & 0x0007));
    }

    /** True if the LocalizedStringReference contains a valid underlying value, false otherwise. */
    constexpr bool isValid() const noexcept
    {
        // Clause 7.3.6 says any index value of 7 is invalid, we just have to check that.
        return (_value & 0x0007) != 0x07;
    }

    /** Underlying value operator (equivalent to getValue()). */
    constexpr operator value_type() const noexcept
    {
        return getValue();
    }

    /** Underlying value validity bool operator (equivalent to isValid()). */
    explicit constexpr operator bool() const noexcept
    {
        return isValid();
    }

    /** Equality operator. Returns true if the underlying values are equal (Any 2 invalid LocalizedStringReferences are considered equal, since they are both invalid). */
    constexpr friend bool operator==(LocalizedStringReference const& lhs, LocalizedStringReference const& rhs) noexcept
    {
        return (!lhs.isValid() && !rhs.isValid()) || lhs._value == rhs._value;
    }

    /** Non equality operator. */
    constexpr friend bool operator!=(LocalizedStringReference const& lhs, LocalizedStringReference const& rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }

    /** operator< */
    constexpr friend bool operator<(LocalizedStringReference const& lhs, LocalizedStringReference const& rhs) noexcept
    {
        return lhs._value < rhs._value;
    }

    /** Static helper method to create a Null LocalizedStringReference (isValid() returns false). */
    static LocalizedStringReference getNullLocalizedStringReference() noexcept
    {
        return LocalizedStringReference{ NullLocalizedStringReference };
    }

    /** Hash functor to be used for std::hash */
    struct hash
    {
        std::size_t operator()(LocalizedStringReference const& ref) const
        {
            return std::hash<value_type>()(ref._value);
        }
    };

    // Defaulted compiler auto-generated methods
    LocalizedStringReference(LocalizedStringReference&&) = default;
    LocalizedStringReference(LocalizedStringReference const&) = default;
    LocalizedStringReference& operator=(LocalizedStringReference const&) = default;
    LocalizedStringReference& operator=(LocalizedStringReference&&) = default;

private:
    static constexpr value_type NullLocalizedStringReference = 0xFFFF;
    value_type _value{ NullLocalizedStringReference };
};

class ControlValueUnit final
{
public:
    using value_type = uint16_t;

    // Unit Enum Definition
    enum class Unit : uint8_t
    {
        // Unitless Quantities
        Unitless = 0x00,
        Count =    0x01,
        Percent =  0x02,
        FStop =    0x03,

        // Time Quantities
        Seconds = 0x08,
        Minutes = 0x09,
        Hours =   0x0a,
        Days =    0x0b,
        Months =  0x0c,
        Years =   0x0d,
        Samples = 0x0e,
        Frames =  0x0f,

        // Frequency Quantities
        Hertz =     0x10,
        Semitones = 0x11,
        Cents =     0x12,
        Octaves =   0x13,
        Fps =       0x14,

        // Distance Quantities
        Metres =    0x18,

        // Temperature Quantities
        Kelvin =    0x20,

        // Mass Quantities
        Grams =     0x28,

        // Voltage Quantities
        Volts =     0x30,
        Dbv =       0x31,
        Dbu =       0x32,

        // Current Quantities
        Amps =      0x38,

        // Power Quantities
        Watts =     0x40,
        Dbm =       0x41,
        Dbw =       0x42,

        // Pressure Quantities
        Pascals =   0x48,

        // Memory Quantities
        Bits =      0x50,
        Bytes =     0x51,
        KibiBytes = 0x52,
        MebiBytes = 0x53,
        GibiBytes = 0x54,
        TebiBytes = 0x55,

        // Bandwidth Quantities
        BitsPerSec =      0x58,
        BytesPerSec =     0x59,
        KibiBytesPerSec = 0x5a,
        MebiBytesPerSec = 0x5b,
        GibiBytesPerSec = 0x5c,
        TebiBytesPerSec = 0x5d,

        // Luminosity Quantities
        Candelas = 0x60,

        // Energy Quantities
        Joules =   0x68,

        // Angle Quantities
        Radians =  0x70,

        // Force Quantities
        Newtons =  0x78,

        // Resistance Quantities
        Ohms =     0x80,

        // Velocity Quantities
        MetresPerSec =  0x88,
        RadiansPerSec = 0x89,

        // Acceleration Quantities
        MetresPerSecSquared =  0x90,
        RadiansPerSecSquared = 0x91,

        // Magnetic Flux and Fields Quantities
        Teslas =        0x98,
        Webers =        0x99,
        AmpsPerMetre =  0x9a,

        // Area Quantities
        MetresSquared = 0xa0,

        // Volume Quantities
        MetresCubed =   0xa8,
        Litres =        0xa9,

        // Level and Loudness Quantities
        Db =       0xb0,
        DbPeak =   0xb1,
        DbRms =    0xb2,
        Dbfs =     0xb3,
        DbfsPeak = 0xb4,
        DbfsRms =  0xb5,
        Dbtp =     0xb6,
        DbSplA =   0xb7,
        DbZ =      0xb8,
        DbSplC =   0xb9,
        DbSpl =    0xba,
        Lu =       0xbb,
        Lufs =     0xbc,
        DbA =      0xbd,
    };

    /** Default constructor. */
    constexpr ControlValueUnit() noexcept : _value(NullControlValueUnit) {}

    /** Constructor to create a ControlValueUnit from the underlying value. */
    explicit constexpr ControlValueUnit(value_type const value) noexcept
        : _value(value)
    {
    }

    /** Constructor to create a ControlValueUnit from multiplier and unit values. */
    ControlValueUnit(int8_t const multiplier, Unit const unit) noexcept
    {
        setMultiplierUnit(multiplier, unit);
    }

    /** Setter to change the underlying value. */
    void setValue(value_type const value) noexcept
    {
        _value = value;
    }

    /** Getter to retrieve the underlying value. */
    constexpr value_type getValue() const noexcept
    {
        return _value;
    }

    /** Getter to retrieve the multiplier from this ControlValueUnit. */
    constexpr int8_t getMultiplier() const noexcept
    {
        return static_cast<int8_t>(_value >> 8);
    }

    /** Getter to retrieve unit from this ControlValueUnit. */
    constexpr Unit getUnit() const noexcept
    {
        return static_cast<Unit>(_value & 0x00FF);
    }

    /** Setter to change the multiplier and unit values for this ControlValueUnit. */
    void setMultiplierUnit(int8_t const multiplier, Unit const unit) noexcept
    {
        _value = (static_cast<uint16_t>(multiplier) << 8) + static_cast<value_type>(unit);
    }

    /** Getter to retrieve the multiplier and unit values from this ControlValueUnit. */
    constexpr std::pair<int8_t, Unit> getMultiplierUnit() const noexcept
    {
        return std::make_pair(getMultiplier(), getUnit());
    }

    /** Underlying value operator (equivalent to getValue()). */
    constexpr operator value_type() const noexcept
    {
        return getValue();
    }

    /** Equality operator. Returns true if the underlying values are equal. */
    constexpr friend bool operator==(ControlValueUnit const& lhs, ControlValueUnit const& rhs) noexcept
    {
        return lhs._value == rhs._value;
    }

    /** Non equality operator. */
    constexpr friend bool operator!=(ControlValueUnit const& lhs, ControlValueUnit const& rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }

    /** operator< */
    constexpr friend bool operator<(ControlValueUnit const& lhs, ControlValueUnit const& rhs) noexcept
    {
        return lhs._value < rhs._value;
    }

    // Defaulted compiler auto-generated methods
    ControlValueUnit(ControlValueUnit&&) = default;
    ControlValueUnit(ControlValueUnit const&) = default;
    ControlValueUnit& operator=(ControlValueUnit const&) = default;
    ControlValueUnit& operator=(ControlValueUnit&&) = default;

private:
    static constexpr value_type NullControlValueUnit = 0u;
    value_type _value{ NullControlValueUnit };
};

/** MemoryObject Type - Clause 7.2.10.1 */
enum class MemoryObjectType : uint16_t
{
	FirmwareImage =     0x0000,
	VendorSpecific =    0x0001,
	CrashDump =         0x0002,
	LogObject =         0x0003,
	AutostartSettings = 0x0004,
	SnapshotSettings =  0x0005,
	SvgManufacturer =   0x0006,
	SvgEntity =         0x0007,
	SvgGeneric =        0x0008,
	PngManufacturer =   0x0009,
	PngEntity =         0x000a,
	PngGeneric =        0x000b,
	DaeManufacturer =   0x000c,
	DaeEntity =         0x000d,
	DaeGeneric =        0x000e,
	/* 000f to ffff reserved for future use */
};

enum class MemoryObjectOperationType : uint16_t
{
	Store =          0x0000,
	StoreAndReboot = 0x0001,
	Read =           0x0002,
	Erase =          0x0003,
	Upload =         0x0004,
	/* 0005 to ffff reserved for future use */
};

enum class AudioClusterFormat : uint8_t
{
	Iec60958 = 0x00,
	Mbla =     0x40,
	Midi =     0x80,
	Smpte =    0x88,
};

class ControlValueType final
{
public:
    using value_type = uint16_t;

    // ControlValueType Enum Definition
    enum class Type : uint16_t
    {
        ControlLinearInt8 =     0x0000,
        ControlLinearUInt8 =    0x0001,
        ControlLinearInt16 =    0x0002,
        ControlLinearUInt16 =   0x0003,
        ControlLinearInt32 =    0x0004,
        ControlLinearUInt32 =   0x0005,
        ControlLinearInt64 =    0x0006,
        ControlLinearUInt64 =   0x0007,
        ControlLinearFloat =    0x0008,
        ControlLinearDouble =   0x0009,
        ControlSelectorInt8 =   0x000a,
        ControlSelectorUInt8 =  0x000b,
        ControlSelectorInt16 =  0x000c,
        ControlSelectorUInt16 = 0x000d,
        ControlSelectorInt32 =  0x000e,
        ControlSelectorUInt32 = 0x000f,
        ControlSelectorInt64 =  0x0010,
        ControlSelectorUInt64 = 0x0011,
        ControlSelectorFloat =  0x0012,
        ControlSelectorDouble = 0x0013,
        ControlSelectorString = 0x0014,
        ControlArrayInt8 =      0x0015,
        ControlArrayUInt8 =     0x0016,
        ControlArrayInt16 =     0x0017,
        ControlArrayUInt16 =    0x0018,
        ControlArrayInt32 =     0x0019,
        ControlArrayUInt32 =    0x001a,
        ControlArrayInt64 =     0x001b,
        ControlArrayUInt64 =    0x001c,
        ControlArrayFloat =     0x001d,
        ControlArrayDouble =    0x001e,
        ControlUtf8 =           0x001f,
        ControlBodePlot =       0x0020,
        ControlSmpteTime =      0x0021,
        ControlSampleRate =     0x0022,
        ControlGptpTime =       0x0023,
        ControlVendor =         0x3ffe,
        Expansion =             0x3fff,
    };

    /** Default constructor. */
    constexpr ControlValueType() noexcept : _value(NullControlValueType) {}

    /** Constructor to create a ControlValueType from the underlying value. */
    explicit constexpr ControlValueType(value_type const value) noexcept
        : _value(value)
    {
    }

    /** Constructor to create a ControlValueType from isReadOnly, isUnknown and type values. */
    ControlValueType(bool const isReadOnly, bool const isUnknown, Type const type) noexcept
    {
        setReadOnlyUnknownType(isReadOnly, isUnknown, type);
    }

    /** Setter to change the underlying value. */
    void setValue(value_type const value) noexcept
    {
        _value = value;
    }

    /** Getter to retrieve the underlying value. */
    constexpr value_type getValue() const noexcept
    {
        return _value;
    }

    /** Getter to retrieve the readOnly bit from this ControlValueType. */
    constexpr bool isReadOnly() const noexcept
    {
        return ((_value >> 15) & 0x1) == 1;
    }

    /** Getter to retrieve the isUnknown bit from this ControlValueType. */
    constexpr bool isUnknown() const noexcept
    {
        return ((_value >> 14) & 0x1) == 1;
    }

    /** Getter to retrieve Type from this ControlValueType. */
    constexpr Type getType() const noexcept
    {
        return static_cast<Type>(_value & 0x3FFF);
    }

    /** Setter to change the isReadOnly, isUnknown, and type values. */
    void setReadOnlyUnknownType(bool const isReadOnly, bool const isUnknown, Type const type) noexcept
    {
        _value = ((isReadOnly & 0x1) << 15) + ((isUnknown & 0x1) << 14) + (static_cast<value_type>(type) & 0x3FFF);
    }

    /** Underlying value operator (equivalent to getValue()). */
    constexpr operator value_type() const noexcept
    {
        return getValue();
    }

    /** Equality operator. Returns true if the underlying values are equal. */
    constexpr friend bool operator==(ControlValueType const& lhs, ControlValueType const& rhs) noexcept
    {
        return lhs._value == rhs._value;
    }

    /** Non equality operator. */
    constexpr friend bool operator!=(ControlValueType const& lhs, ControlValueType const& rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }

    /** operator< */
    constexpr friend bool operator<(ControlValueType const& lhs, ControlValueType const& rhs) noexcept
    {
        return lhs._value < rhs._value;
    }

    // Defaulted compiler auto-generated methods
    ControlValueType(ControlValueType&&) = default;
    ControlValueType(ControlValueType const&) = default;
    ControlValueType& operator=(ControlValueType const&) = default;
    ControlValueType& operator=(ControlValueType&&) = default;

private:
    static constexpr value_type NullControlValueType = 1u << 14;
    value_type _value{ NullControlValueType };
};

// Replace std::any with a union or void pointer
struct ControlValueHolder {
    void* value;
};

// ControlValues Class Definition
class ControlValues final
{
public:
    /** Traits to handle ValueDetails behavior. */
    template<typename ValueDetailsType>
    struct control_value_details_traits
    {
        static constexpr bool is_value_details = false;
        static constexpr bool is_dynamic = false;
        static constexpr bool static_dynamic_counts_identical = false;
        static constexpr ControlValueType::Type control_value_type = ControlValueType::Type::Expansion; // Default value if none is provided
    };

    /** Default constructor. */
    constexpr ControlValues() noexcept {}

    /** Constructor to initialize ControlValues with a value. */
    template<class ValueDetailsType, typename Traits = control_value_details_traits<decay_t<ValueDetailsType>>>
    explicit ControlValues(ValueDetailsType const& values) noexcept
    {
        static_assert(Traits::is_value_details, "ControlValues::ControlValues, control_value_details_traits::is_value_details trait not defined for requested ValueDetailsType.");
        _isValid = true;
        _type = Traits::control_value_type;
        _areDynamic = Traits::is_dynamic;
        _countMustBeIdentical = Traits::static_dynamic_counts_identical;
        _countValues = values.countValues();
        _values.value = (void*)&values;
    }

    /** Constructor to initialize ControlValues with a moveable value. */
    template<class ValueDetailsType, typename Traits = control_value_details_traits<decay_t<ValueDetailsType>>>
    explicit ControlValues(ValueDetailsType&& values) noexcept
    {
        static_assert(Traits::is_value_details, "ControlValues::ControlValues, control_value_details_traits::is_value_details trait not defined for requested ValueDetailsType.");
        _isValid = true;
        _type = Traits::control_value_type;
        _areDynamic = Traits::is_dynamic;
        _countMustBeIdentical = Traits::static_dynamic_counts_identical;
        _countValues = values.countValues();
        _values.value = (void*)&values;
    }

    /** Get the Control Value Type. */
    constexpr ControlValueType::Type getType() const noexcept
    {
        return _type;
    }

    /** Check if values are Dynamic. */
    constexpr bool areDynamicValues() const noexcept
    {
        return _areDynamic;
    }

    /** Check if the count of Static Values must be identical to the count of Dynamic Values. */
    constexpr bool countMustBeIdentical() const noexcept
    {
        return _countMustBeIdentical;
    }

    /** Get the number of values. */
    constexpr uint16_t size() const noexcept
    {
        return _countValues;
    }

    /** Check if there are no values. */
    constexpr bool empty() const noexcept
    {
        return _countValues == 0;
    }

    /** Check if the ControlValues contains valid values. */
    constexpr bool isValid() const noexcept
    {
        return _isValid;
    }

    /** Validity check operator. */
    explicit constexpr operator bool() const noexcept
    {
        return isValid();
    }

    /** Get the actual values. */
    template<class ValueDetailsType, typename Traits = control_value_details_traits<decay_t<ValueDetailsType>>>
    decay_t<ValueDetailsType> getValues() const
    {
        static_assert(Traits::is_value_details, "ControlValues::getValues, control_value_details_traits::is_value_details trait not defined for requested ValueDetailsType.");
        if (!isValid())
        {
            ESP_LOGE("ControlValues", "ControlValues::getValues, no valid values to get");
            return decay_t<ValueDetailsType>{};
        }
        if (_type != Traits::control_value_type)
        {
            ESP_LOGE("ControlValues", "ControlValues::getValues, incorrect ControlValueType::Type");
            return decay_t<ValueDetailsType>{};
        }
        if (_areDynamic != Traits::is_dynamic)
        {
            ESP_LOGE("ControlValues", "ControlValues::getValues, static/dynamic mismatch");
            return decay_t<ValueDetailsType>{};
        }
        return *static_cast<decay_t<ValueDetailsType>*>(_values.value);
    }

    // Comparison operators
    template<class ValueDetailsType, typename Traits = control_value_details_traits<decay_t<ValueDetailsType>>>
    inline bool isEqualTo(ControlValues const& other) const
    {
        static_assert(Traits::is_value_details, "ControlValues::isEqualTo, control_value_details_traits::is_value_details trait not defined for requested ValueDetailsType.");
        // Both must have the same valid state
        if (_isValid != other._isValid)
        {
            return false;
        }
        // If both are invalid, they are equal
        if (!_isValid)
        {
            return true;
        }
        // If both are valid, they must have all the same parameters
        if (_type != other._type || _areDynamic != other._areDynamic || _countMustBeIdentical != other._countMustBeIdentical || _countValues != other._countValues)
        {
            return false;
        }
        // Now compare the actual values
        return getValues<ValueDetailsType>() == other.getValues<ValueDetailsType>();
    }

    // Defaulted compiler auto-generated methods
    ControlValues(ControlValues const&) = default;
    ControlValues(ControlValues&&) = default;
    ControlValues& operator=(ControlValues const&) = default;
    ControlValues& operator=(ControlValues&&) = default;

private:
    bool _isValid{ false };
    ControlValueType::Type _type{};
    bool _areDynamic{ false };
    bool _countMustBeIdentical{ false };
    uint16_t _countValues{ 0u };
    ControlValueHolder _values{}; // Use a structure to hold values instead of std::any
};

/** Audio Mapping - Clause 7.2.19.1 */
struct AudioMapping
{
	StreamIndex streamIndex{ StreamIndex(0u) };
	uint16_t streamChannel{ 0u };
	ClusterIndex clusterOffset{ ClusterIndex(0u) };
	uint16_t clusterChannel{ 0u };

	static constexpr size_t size()
	{
		return sizeof(streamIndex) + sizeof(streamChannel) + sizeof(clusterOffset) + sizeof(clusterChannel);
	}
	constexpr friend bool operator==(AudioMapping const& lhs, AudioMapping const& rhs) noexcept
	{
		return (lhs.streamIndex == rhs.streamIndex) && (lhs.streamChannel == rhs.streamChannel) && (lhs.clusterOffset == rhs.clusterOffset) && (lhs.clusterChannel == rhs.clusterChannel);
	}
	constexpr friend bool operator!=(AudioMapping const& lhs, AudioMapping const& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}
};

using AudioMappings = std::vector<AudioMapping>;

// Simplified JackType Enum
enum class JackType : uint16_t
{
    Speaker =           0x0000,
    Headphone =         0x0001,
    AnalogMicrophone =  0x0002,
    Spdif =             0x0003,
    Adat =              0x0004,
    TdIf =              0x0005,
    Madi =              0x0006,
    UnbalancedAnalog =  0x0007,
    BalancedAnalog =    0x0008,
    Digital =           0x0009,
    Midi =              0x000a,
    AesEbu =            0x000b,
    CompositeVideo =    0x000c,
    SVhsVideo =         0x000d,
    ComponentVideo =    0x000e,
    Dvi =               0x000f,
    Hdmi =              0x0010,
    Udi =               0x0011,
    DisplayPort =       0x0012,
    Antenna =           0x0013,
    AnalogTuner =       0x0014,
    Ethernet =          0x0015,
    Wifi =              0x0016,
    Usb =               0x0017,
    Pci =               0x0018,
    PciE =              0x0019,
    Scsi =              0x001a,
    Ata =               0x001b,
    Imager =            0x001c,
    Ir =                0x001d,
    Thunderbolt =       0x001e,
    Sata =              0x001f,
    SmpteLtc =          0x0020,
    DigitalMicrophone = 0x0021,
    AudioMediaClock =   0x0022,
    VideoMediaClock =   0x0023,
    GnssClock =         0x0024,
    Pps =               0x0025,
    Expansion =         0xffff
};

// Structure for MsrpMapping
struct MsrpMapping
{
	uint8_t trafficClass{ 0x00 };
	uint8_t priority{ 0xff };
	uint16_t vlanID{ 0u };

	static constexpr size_t size()
	{
		return sizeof(trafficClass) + sizeof(priority) + sizeof(vlanID);
	}
	constexpr friend bool operator==(MsrpMapping const& lhs, MsrpMapping const& rhs) noexcept
	{
		return (lhs.trafficClass == rhs.trafficClass) && (lhs.priority == rhs.priority) && (lhs.vlanID == rhs.vlanID);
	}
	constexpr friend bool operator!=(MsrpMapping const& lhs, MsrpMapping const& rhs) noexcept
	{
		return !operator==(lhs, rhs);
	}
};

using MsrpMappings = std::vector<MsrpMapping>;

/** GET_AS_PATH Dynamic Information - Clause 7.4.41.2 */
using PathSequence = std::vector<UniqueIdentifier>;

/** GET_COUNTERS - Clause 7.4.42.2 */
using DescriptorCounters = std::array<DescriptorCounter, 32>;

// Structure for StreamIdentification
struct StreamIdentification
{
    UniqueIdentifier entityID;
    DescriptorIndex streamIndex;
};

inline bool operator==(const StreamIdentification &lhs, const StreamIdentification &rhs)
{
    return (lhs.entityID.getValue() == rhs.entityID.getValue()) && (lhs.streamIndex == rhs.streamIndex);
}

// Simplified enum class for ProbingStatus
enum ProbingStatus : uint8_t
{
    Disabled =  0x00,    /** The sink is not probing because it is not bound. */
    Passive =   0x01,     /** The sink is probing passively. It waits until the bound talker has been discovered. */
    Active =    0x02,      /** The sink is probing actively. It is querying the stream parameters to the talker. */
    Completed = 0x03,   /** The sink is not probing because it is settled. */
};

// MSRP Failure Code Enum - Simplified
enum MsrpFailureCode : uint8_t
{
    NoFailure =                          0,
    InsufficientBandwidth =              1,
    InsufficientResources =              2,
    InsufficientTrafficClassBandwidth =  3,
    StreamIDInUse =                      4,
    StreamDestinationAddressInUse =      5,
    StreamPreemptedByHigherRank =        6,
    LatencyHasChanged =                  7,
    EgressPortNotAVBCapable =            8,
    UseDifferentDestinationAddress =     9,
    OutOfMSRPResources =                10,
    OutOfMMRPResources =                11,
    CannotStoreDestinationAddress =     12,
    PriorityIsNotAnSRClass =            13,
    MaxFrameSizeTooLarge =              14,
    MaxFanInPortsLimitReached =         15,
    FirstValueChangedForStreamID =      16,
    VlanBlockedOnEgress =               17,
    VlanTaggingDisabledOnEgress =       18,
    SrClassPriorityMismatch =           19,
};

// Function to print AtdeccFixedString
inline void print_atdecc_string(const AtdeccFixedString &str)
{
    //printf("%s", str.c_str());
}

enum class StandardControlType : uint64_t
{
	Enable =           0x90e0f00000000000,
	Identify =         0x90e0f00000000001,
	Mute =             0x90e0f00000000002,
	Invert =           0x90e0f00000000003,
	Gain =             0x90e0f00000000004,
	Attenuate =        0x90e0f00000000005,
	Delay =            0x90e0f00000000006,
	SrcMode =          0x90e0f00000000007,
	Snapshot =         0x90e0f00000000008,
	PowLineFreq =      0x90e0f00000000009,
	PowerStatus =      0x90e0f0000000000a,
	FanStatus =        0x90e0f0000000000b,
	Temperature =      0x90e0f0000000000c,
	Altitude =         0x90e0f0000000000d,
	AbsoluteHumidity = 0x90e0f0000000000e,
	RelativeHumidity = 0x90e0f0000000000f,
	Orientation =      0x90e0f00000000010,
	Velocity =         0x90e0f00000000011,
	Acceleration =     0x90e0f00000000012,
	FilterResponse =   0x90e0f00000000013,
	/* 0x90e0f00000000014 to 0x90e0f0000000ffff reserved for future use */
	Panpot =           0x90e0f00000010000,
	Phantom =          0x90e0f00000010001,
	AudioScale =       0x90e0f00000010002,
	AudioMeters =      0x90e0f00000010003,
	AudioSpectrum =    0x90e0f00000010004,
	/* 0x90e0f00000010005 to 0x90e0f0000001ffff reserved for future use */
	ScanningMode =     0x90e0f00000020000,
	AutoExpMode =      0x90e0f00000020001,
	AutoExpPrio =      0x90e0f00000020002,
	ExpTime =          0x90e0f00000020003,
	Focus =            0x90e0f00000020004,
	FocusAuto =        0x90e0f00000020005,
	Iris =             0x90e0f00000020006,
	Zoom =             0x90e0f00000020007,
	Privacy =          0x90e0f00000020008,
	Backlight =        0x90e0f00000020009,
	Brightness =       0x90e0f0000002000a,
	Contrast =         0x90e0f0000002000b,
	Hue =              0x90e0f0000002000c,
	Saturation =       0x90e0f0000002000d,
	Sharpness =        0x90e0f0000002000e,
	Gamma =            0x90e0f0000002000f,
	WhiteBalTemp =     0x90e0f00000020010,
	WhiteBalTempAuto = 0x90e0f00000020011,
	WhiteBalComp =     0x90e0f00000020012,
	WhiteBalCompAuto = 0x90e0f00000020013,
	DigitalZoom =      0x90e0f00000020014,
	/* 0x90e0f00000020015 to 0x90e0f0000002ffff reserved for future use */
	MediaPlaylist =          0x90e0f00000030000,
	MediaPlaylistName =      0x90e0f00000030001,
	MediaDisk =              0x90e0f00000030002,
	MediaDiskName =          0x90e0f00000030003,
	MediaTrack =             0x90e0f00000030004,
	MediaTrackName =         0x90e0f00000030005,
	MediaSpeed =             0x90e0f00000030006,
	MediaSamplePosition =    0x90e0f00000030007,
	MediaPlaybackTransport = 0x90e0f00000030008,
	MediaRecordTransport =   0x90e0f00000030009,
	/* 0x90e0f0000003000a to 0x90e0f0000003ffff reserved for future use */
	Frequency =              0x90e0f00000040000,
	Modulation =             0x90e0f00000040001,
	Polarization =           0x90e0f00000040002,
	/* 0x90e0f00000040003 to 0x90e0f0ffffffffff reserved for future use */
};

enum class ClockSourceType : uint16_t
{
	Internal =    0x0000,
	External =    0x0001,
	InputStream = 0x0002,
	/* 0003 to fffe reserved for future use */
	Expansion =   0xffff
};

/** GET_AS_PATH Dynamic Information - Clause 7.4.41.2 */
struct AsPath
{
	PathSequence sequence{};
};

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTYPES_HPP_ */
