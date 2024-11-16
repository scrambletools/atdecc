#ifndef COMPONENTS_ATDECC_INCLUDE_UTILS_HPP_
#define COMPONENTS_ATDECC_INCLUDE_UTILS_HPP_

#pragma once

#include <string>
#include "serialization.hpp"

// Useful when doing things like printing frames
typedef enum {
	FrameTypeAdpdu = 0,
	FrameTypeAecpdu = 1,
	FrameTypeAcmpdu = 2
} FrameType;

const char* getFrameTypeName(FrameType type);
void binprintf(int v);
void printFrame(FrameType type, SerBuffer& frame);

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

/** Class to easily manipulate an enum that represents a bitfield (strongly typed alternative to traits). */
template<typename EnumType, typename = std::enable_if_t<std::is_enum<EnumType>::value>>
class EnumBitfield2 final
{
public:
	using value_type = EnumType;
	using underlying_value_type = std::underlying_type_t<value_type>;
	static constexpr size_t value_size = sizeof(underlying_value_type) * 8;

	/** Iterator allowing quick enumeration of all the bits that are set in the bitfield */
	class iterator final
	{
		using self_name = iterator;
		static constexpr auto EndBit = value_size;

	public:
		using value_type = EnumType;
		using underlying_value_type = std::underlying_type_t<value_type>;
		using difference_type = size_t;
		using iterator_category = std::forward_iterator_tag;
		using reference = value_type&;
		using const_reference = value_type const&;
		using pointer = value_type*;
		using const_pointer = value_type const*;

		constexpr iterator(underlying_value_type const value, std::uint8_t const currentBitPosition) noexcept
			: _value(value)
			, _currentBitPosition(currentBitPosition)
		{
			findNextBitSet();
		}
		// Pre-increment operator
		constexpr self_name& operator++() noexcept
		{
			++_currentBitPosition;
			findNextBitSet();
			return *this;
		}
		// Post-increment operator
		constexpr self_name operator++(int) noexcept
		{
			auto tmp(*this);
			operator++();
			return tmp;
		}
		// Addition operator
		constexpr self_name operator+(size_t const count) const noexcept
		{
			auto tmp(*this);
			tmp.operator+=(count);
			return tmp;
		}
		// Addition assignment operator
		constexpr self_name& operator+=(size_t const count) noexcept
		{
			for (auto c = 0u; c < count; ++c)
			{
				if (_currentBitPosition == EndBit)
				{
					break;
				}
				++_currentBitPosition;
				findNextBitSet();
			}
			return *this;
		}
		constexpr value_type operator*() const noexcept
		{
			return static_cast<value_type>(_currentValue);
		}
		constexpr bool operator==(self_name const& other) const noexcept
		{
			return _currentBitPosition == other._currentBitPosition;
		}
		constexpr bool operator!=(self_name const& other) const noexcept
		{
			return !operator==(other);
		}

	private:
		constexpr void updateCurrentValue() noexcept
		{
			// Make a mask for current bit
			auto mask = pow(underlying_value_type(2), _currentBitPosition);

			// Extract the current bit
			_currentValue = _value & mask;
		}
		constexpr void findNextBitSet() noexcept
		{
			while (_currentBitPosition < EndBit)
			{
				updateCurrentValue();
				if (_currentValue != static_cast<underlying_value_type>(0))
				{
					break;
				}
				++_currentBitPosition;
			}
		}

		underlying_value_type const _value{ static_cast<underlying_value_type>(0) }; /** All possible Values this iterator can point to */
		std::uint8_t _currentBitPosition{ 0u }; /** Bit position the iterator is currently pointing to */
		underlying_value_type _currentValue{ static_cast<underlying_value_type>(0) }; /** Value the iterator is currently pointing to */
	};

	/** Construct a bitfield using individual bits passed as variadic parameters. If passed value is not valid (not exactly one bit set), this leads to undefined behavior. */
	template<typename... Values>
	constexpr explicit EnumBitfield2(value_type const value, Values const... values) noexcept
		: _value(to_integral(value))
	{
		checkInvalidValue(value);
		(checkInvalidValue(values), ...);
		((_value |= to_integral(values)), ...);
	}

	/** Assigns the entire underlying bitfield with the passed value */
	void assign(underlying_value_type const value) noexcept
	{
		_value = value;
	}

	/** Returns true if the specified flag is set in the bitfield */
	constexpr bool test(value_type const flag) const noexcept
	{
		return (_value & to_integral(flag)) != static_cast<underlying_value_type>(0);
	}

	/** Sets the specified flag. If passed value is not valid (not exactly one bit set), this leads to undefined behavior. */
	EnumBitfield2& set(value_type const flag) noexcept
	{
		checkInvalidValue(flag);
		_value |= to_integral(flag);
		return *this;
	}

	/** Clears the specified flag. If passed value is not valid (not exactly one bit set), this leads to undefined behavior. */
	EnumBitfield2& reset(value_type const flag) noexcept
	{
		checkInvalidValue(flag);
		_value &= ~to_integral(flag);
		return *this;
	}

	/** Clears all the flags. */
	void clear() noexcept
	{
		_value = {};
	}

	/** Returns true if no bit is set */
	constexpr bool empty() const noexcept
	{
		return _value == static_cast<underlying_value_type>(0);
	}

	/** Returns the size number of bits the bitfield can hold */
	constexpr size_t size() const noexcept
	{
		return value_size;
	}

	/** Returns the number of bits that are set */
	constexpr size_t count() const noexcept
	{
		return countBits(_value);
	}

	/** Returns the underlying value of the bitfield */
	constexpr underlying_value_type value() const noexcept
	{
		return _value;
	}

	/** Comparison operator (equality) */
	constexpr bool operator==(EnumBitfield2 const other) const noexcept
	{
		return other._value == _value;
	}

	/** Comparison operator (difference) */
	constexpr bool operator!=(EnumBitfield2 const other) const noexcept
	{
		return !operator==(other);
	}

	/** OR EQUAL operator (sets the bits that are present in this or the other EnumBitfield, clears all other bits) */
	EnumBitfield2& operator|=(EnumBitfield2 const other) noexcept
	{
		_value |= other._value;
		return *this;
	}

	/** AND EQUAL operator (sets the bits that are present in this and the other EnumBitfield, clears all other bits) */
	EnumBitfield2& operator&=(EnumBitfield2 const other) noexcept
	{
		_value &= other._value;
		return *this;
	}

	/** OR operator (sets the bits that are present in either lhs or rhs, clears all other bits) */
	friend constexpr EnumBitfield2 operator|(EnumBitfield2 const lhs, EnumBitfield2 const rhs) noexcept
	{
		auto result = EnumBitfield2{};
		result._value = lhs._value | rhs._value;
		return result;
	}

	/** AND operator (sets the bits that are present in both lhs and rhs, clears all other bits) */
	friend constexpr EnumBitfield2 operator&(EnumBitfield2 const lhs, EnumBitfield2 const rhs) noexcept
	{
		auto result = EnumBitfield2{};
		result._value = lhs._value & rhs._value;
		return result;
	}

	/** Returns the value at the specified bit set position (only counting bits that are set). Specified position must be inclusively comprised btw 0 and (count() - 1) or an out_of_range exception will be thrown. */
	constexpr value_type at(size_t const setPosition) const
	{
		if (setPosition >= count())
		{
            return ESP_FAIL;
			//throw error("EnumBitfield2::at() out of range");
		}
		return *(begin() + setPosition);
	}

	/** Returns the bit set position for the specified value (only counting bits that are set). Specified value must be set or an out_of_range exception will be thrown. */
	constexpr size_t getBitSetPosition(value_type const value) const
	{
		checkInvalidValue(value);
		if (!test(value))
		{
            return ESP_FAIL;
			//throw error("EnumBitfield2::getBitSetPosition() out of range");
		}
		return getBitPosition(to_integral(value), _value);
	}

	/** Returns the bit position for the specified value. Specified value must has exactly one bit set or an out_of_range exception will be thrown. */
	static constexpr size_t getPosition(value_type const value)
	{
		checkInvalidValue(value);
		return getBitPosition(to_integral(value));
	}

	/** Returns the begin iterator */
	constexpr iterator begin() noexcept
	{
		return iterator(_value, 0);
	}

	/** Returns the begin const iterator */
	constexpr iterator const begin() const noexcept
	{
		return iterator(_value, 0);
	}

	/** Returns the end iterator */
	constexpr iterator end() noexcept
	{
		return iterator(_value, value_size);
	}

	/** Returns the end const iterator */
	constexpr iterator const end() const noexcept
	{
		return iterator(_value, value_size);
	}

	struct Hash
	{
		std::size_t operator()(EnumBitfield2 t) const
		{
			return std::hash<underlying_value_type>()(t._value);
		}
	};

	// Defaulted compiler auto-generated methods
	EnumBitfield2() noexcept = default;
	EnumBitfield2(EnumBitfield2&&) noexcept = default;
	EnumBitfield2(EnumBitfield2 const&) noexcept = default;
	EnumBitfield2& operator=(EnumBitfield2 const&) noexcept = default;
	EnumBitfield2& operator=(EnumBitfield2&&) noexcept = default;

private:
	static constexpr esp_err_t checkInvalidValue([[maybe_unused]] value_type const value)
	{
		if (countBits(to_integral(value)) != 1)
		{
			//throw std::logic_error("Invalid value: not exactly one 1 bit set");
            return ESP_FAIL;
		}
	}
	static constexpr size_t countBits(underlying_value_type const value) noexcept
	{
		return (value == 0u) ? 0u : 1u + countBits(value & (value - 1u));
	}
	static constexpr size_t getBitPosition(underlying_value_type const value, underlying_value_type const setBitValue = static_cast<underlying_value_type>(-1)) noexcept
	{
		return (value == 1u) ? 0u : (setBitValue & 0x1) + getBitPosition(value >> 1, setBitValue >> 1);
	}

	underlying_value_type _value{};
};

#endif /* COMPONENTS_ATDECC_INCLUDE_UTILS_HPP_ */