#ifndef COMPONENTS_ATDECC_INCLUDE_UNIQUEIDENTIFIER_HPP_
#define COMPONENTS_ATDECC_INCLUDE_UNIQUEIDENTIFIER_HPP_

#pragma once

#include <stdint.h>  // Use standard C headers for compatibility
#include <functional>
#include <type_traits>

// UniqueIdentifier class definition
class UniqueIdentifier final
{
public:
    using value_type = uint64_t;

    /** Default constructor. */
    constexpr UniqueIdentifier() noexcept
        : _eui(UninitializedIdentifierValue)
    {
    }

    /** Constructor to create a UniqueIdentifier from the underlying value. */
    explicit constexpr UniqueIdentifier(value_type const eui) noexcept
        : _eui(eui)
    {
    }

    /** Setter to change the underlying value. */
    void setValue(value_type const eui) noexcept
    {
        _eui = eui;
    }

    /** Getter to retrieve the underlying value. */
    constexpr value_type getValue() const noexcept
    {
        return _eui;
    }

    /** Returns the VendorID as a OUI-24 (by default) or OUI-64 if using uint64_t type. */
   template<typename Type = uint32_t>
	constexpr typename std::enable_if<std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value, Type>::type
	getVendorID() const noexcept
	{
	    if (std::is_same<Type, uint32_t>::value)
	    {
	        return static_cast<Type>((_eui >> 40) & 0x0000000000FFFFFF);
	    }
	    else if (std::is_same<Type, uint64_t>::value)
	    {
	        return static_cast<Type>((_eui >> 28) & 0x0000000FFFFFFFFF);
	    }
	}

    /** Returns the Vendor-specific value after the OUI part. */
	template<typename Type = uint64_t>
	constexpr typename std::enable_if<std::is_same<Type, uint32_t>::value || std::is_same<Type, uint64_t>::value, Type>::type
	getVendorValue() const noexcept
	{
	    if (std::is_same<Type, uint64_t>::value)
	    {
	        return static_cast<Type>(_eui) & 0x000000FFFFFFFFFF;
	    }
	    else if (std::is_same<Type, uint32_t>::value)
	    {
	        return static_cast<Type>(_eui) & 0x0FFFFFFF;
	    }
	}

    /** Returns true if the UniqueIdentifier is a Group (Multicast/Broadcast) identifier. */
    constexpr bool isGroupIdentifier() const noexcept
    {
        return isValid() && ((_eui & 0x0100000000000000) == 0x0100000000000000);
    }

    /** Returns true if the UniqueIdentifier is locally administered. */
    constexpr bool isLocalIdentifier() const noexcept
    {
        return isValid() && ((_eui & 0x0200000000000000) == 0x0200000000000000);
    }

    /** Returns true if the UniqueIdentifier is valid. */
    constexpr bool isValid() const noexcept
    {
        return _eui != NullIdentifierValue && _eui != UninitializedIdentifierValue;
    }

    /** Operator to get the underlying value. */
    constexpr operator value_type() const noexcept
    {
        return getValue();
    }

    /** Operator to check validity. */
    explicit constexpr operator bool() const noexcept
    {
        return isValid();
    }

    /** Equality operator. */
    constexpr friend bool operator==(UniqueIdentifier const& lhs, UniqueIdentifier const& rhs) noexcept
    {
        return (!lhs.isValid() && !rhs.isValid()) || lhs._eui == rhs._eui;
    }

    /** Inequality operator. */
    constexpr friend bool operator!=(UniqueIdentifier const& lhs, UniqueIdentifier const& rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }

    /** Less-than operator. */
    constexpr friend bool operator<(UniqueIdentifier const& lhs, UniqueIdentifier const& rhs) noexcept
    {
        return lhs._eui < rhs._eui;
    }

    /** Static helper to create a Null UniqueIdentifier. */
    static UniqueIdentifier getNullUniqueIdentifier() noexcept
    {
        return UniqueIdentifier{ NullIdentifierValue };
    }

    /** Static helper to create an Uninitialized UniqueIdentifier. */
    static UniqueIdentifier getUninitializedUniqueIdentifier() noexcept
    {
        return UniqueIdentifier{ UninitializedIdentifierValue };
    }

    /** Hash functor for use with std::hash */
    struct hash
    {
        std::size_t operator()(UniqueIdentifier const& uid) const
        {
            return std::hash<value_type>()(uid._eui);
        }
    };

    // Defaulted compiler auto-generated methods
    UniqueIdentifier(UniqueIdentifier&&) = default;
    UniqueIdentifier(UniqueIdentifier const&) = default;
    UniqueIdentifier& operator=(UniqueIdentifier const&) = default;
    UniqueIdentifier& operator=(UniqueIdentifier&&) = default;

private:
    static constexpr value_type NullIdentifierValue = 0x0000000000000000;
    static constexpr value_type UninitializedIdentifierValue = 0xFFFFFFFFFFFFFFFF;
    value_type _eui{ UninitializedIdentifierValue };
};

#endif /* COMPONENTS_ATDECC_INCLUDE_UNIQUEIDENTIFIER_HPP_ */