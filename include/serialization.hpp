/*
 * serialization.hpp
 *
 *  Created on: Oct 1, 2024
 *      Author: Dulini
 */

#ifndef COMPONENTS_ATDECC_INCLUDE_SERIALIZATION_HPP_
#define COMPONENTS_ATDECC_INCLUDE_SERIALIZATION_HPP_

#pragma once

#include "memoryBuffer.hpp"
#include <esp_log.h>
#include <cstdint>
#include <type_traits>
#include <array>
#include <cstring> // memcpy

#define TAG_S "SERIALIZATION"

/* SERIALIZATION */
template<size_t MaximumSize>
class Serializer
{
public:
    static constexpr size_t maximum_size = MaximumSize;

    /** Initializes a serializer with a default initial size */
    Serializer() = default;

    /** Gets raw pointer to serialized buffer */
    const std::uint8_t* data() const
    {
        return _buffer.data();
    }

    /** Gets size of serialized buffer */
    size_t size() const
    {
        return _pos;
    }

    /** Serializes any arithmetic type (including enums) */
    template<typename T, typename = std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value>>
    Serializer& operator<<(const T& v)
    {
        if (remaining() < sizeof(v))
        {
            ESP_LOGE(TAG_S, "Not enough room to serialize");
            return *this;
        }

        T* const ptr = reinterpret_cast<T*>(_buffer.data() + _pos);
        *ptr = v;  // No endian conversion, assuming native order.

        _pos += sizeof(v);
        return *this;
    }

    /** Serializes a buffer (without changing endianness) */
    Serializer& packBuffer(const void* ptr, size_t size)
    {
        if (remaining() < size)
        {
            ESP_LOGE(TAG_S, "Not enough room to serialize buffer");
            return *this;
        }

        std::memcpy(_buffer.data() + _pos, ptr, size);
        _pos += size;
        return *this;
    }

    size_t remaining() const
    {
        return MaximumSize - _pos;
    }

    constexpr size_t usedBytes() const
    {
        return _pos;
    }

    constexpr size_t capacity() const
    {
        return MaximumSize;
    }

private:
    std::array<std::uint8_t, MaximumSize> _buffer{};
    size_t _pos{ 0u };
};

/* DESERIALIZATION */
class Deserializer
{
public:
    Deserializer(const void* ptr, size_t size) noexcept
        : _ptr(ptr)
        , _size(size)
    {
    }

    Deserializer(const MemoryBuffer& buffer) noexcept
        : _ptr(buffer.data())
        , _size(buffer.size())
    {
    }

    /** Gets raw pointer to deserialization buffer */
    const void* data() const
    {
        return _ptr;
    }

    /** Gets size of deserialization buffer */
    size_t size() const
    {
        return _size;
    }

    /** Unpacks any arithmetic (including enums) */
    template<typename T, typename = std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value>>
    Deserializer& operator>>(T& v)
    {
        if (remaining() < sizeof(v))
        {
            ESP_LOGE(TAG_S, "Not enough data to deserialize");
            return *this;
        }

        const auto* const ptr = static_cast<const std::uint8_t*>(_ptr) + _pos;
        v = *reinterpret_cast<const T*>(ptr);

        _pos += sizeof(v);
        return *this;
    }

    /** Unpacks data to a raw buffer (without changing endianness) */
    void unpackBuffer(void* buffer, size_t size)
    {
        if (remaining() < size)
        {
            ESP_LOGE(TAG_S, "Not enough data to deserialize buffer");
            return;
        }

        std::memcpy(buffer, static_cast<const std::uint8_t*>(_ptr) + _pos, size);
        _pos += size;
    }

    size_t remaining() const
    {
        return _size - _pos;
    }

    size_t usedBytes() const
    {
        return _pos;
    }

    void setPosition(size_t position)
    {
        if (position > _size)
        {
            ESP_LOGE(TAG_S, "Position out of range");
            return;
        }
        _pos = position;
    }

private:
    size_t _pos{ 0 };
    const void* _ptr{ nullptr };
    size_t _size{ 0 };
};

#endif /* COMPONENTS_ATDECC_INCLUDE_SERIALIZATION_HPP_ */
