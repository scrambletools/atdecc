#ifndef COMPONENTS_ATDECC_INCLUDE_ENDIAN_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENDIAN_HPP_

#pragma once

#include <climits>
#include <cstdint>

enum class Endianness
{
	Unknown,
	LittleEndian,
	BigEndian,
	NetworkEndian = BigEndian,
	HostEndian = LittleEndian,
	InvertHostEndian = BigEndian,
};

namespace detail
{
template<typename T>
T swapBytes(T const& u)
{
	static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

	union
	{
		T u{};
		unsigned char u8[sizeof(T)];
	} source, dest;

	source.u = u;

	for (size_t k = 0; k < sizeof(T); k++)
		dest.u8[k] = source.u8[sizeof(T) - k - 1];

	return dest.u;
}

template<Endianness from, Endianness to, typename T>
struct endianSwap
{
	inline T operator()(T const& u)
	{
		return swapBytes<T>(u);
	}
};

// Specializations when attempting to swap to the same endianess
template<typename T>
struct endianSwap<Endianness::LittleEndian, Endianness::LittleEndian, T>
{
	inline T operator()(T const& value)
	{
		return value;
	}
};
template<typename T>
struct endianSwap<Endianness::BigEndian, Endianness::BigEndian, T>
{
	inline T operator()(T const& value)
	{
		return value;
	}
};

} // namespace detail

template<Endianness from, Endianness to, typename T>
inline T endianSwap(T const& u)
{
	static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8, "Unsupported value size");
	//static_assert(std::is_arithmetic<T>::value, "Only supporting arithmetic types");

	return detail::endianSwap<from, to, T>()(u);
}

#define ATDECC_PACK_TYPE(x, y) endianSwap<Endianness::HostEndian, Endianness::NetworkEndian, y>(x)
#define ATDECC_PACK_WORD(x) endianSwap<Endianness::HostEndian, Endianness::NetworkEndian, std::uint16_t>(x)
#define ATDECC_PACK_DWORD(x) endianSwap<Endianness::HostEndian, Endianness::NetworkEndian, std::uint32_t>(x)
#define ATDECC_PACK_QWORD(x) endianSwap<Endianness::HostEndian, Endianness::NetworkEndian, std::uint64_t>(x)

#define ATDECC_UNPACK_TYPE(x, y) endianSwap<Endianness::NetworkEndian, Endianness::HostEndian, y>(x)
#define ATDECC_UNPACK_WORD(x) endianSwap<Endianness::NetworkEndian, Endianness::HostEndian, std::uint16_t>(x)
#define ATDECC_UNPACK_DWORD(x) endianSwap<Endianness::NetworkEndian, Endianness::HostEndian, std::uint32_t>(x)
#define ATDECC_UNPACK_QWORD(x) endianSwap<Endianness::NetworkEndian, Endianness::HostEndian, std::uint64_t>(x)

#endif /* COMPONENTS_ATDECC_INCLUDE_ENDIAN_HPP_ */