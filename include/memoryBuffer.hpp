/*
 * memoryBuffer.hpp
 *
 *  Created on: Oct 1, 2024
 *      Author: Dulini
 */

#ifndef COMPONENTS_ATDECC_INCLUDE_MEMORYBUFFER_HPP_
#define COMPONENTS_ATDECC_INCLUDE_MEMORYBUFFER_HPP_

#pragma once

#include <cstdint>  // uint8_t
#include <vector>   // vector
#include <string>   // string
#include <cstring>  // memmove / memcpy
#include <cstdlib>  // free / realloc
#include <algorithm>  // min
#include <esp_log.h>

/**
 * @brief Lightweight and efficient vector-like container.
 * @details A vector-like container that handles a "byte" resizable array
 *          which can be resized without forcing data initialization.
 *          All the methods of this class have the same meaning and
 *          specification than std::vector, with the addition of a
 *          `set_size` method to change the "used bytes" size
 *          of the array without default initializing it.
 */
class MemoryBuffer
{
public:
    using value_type = std::uint8_t;

    static_assert(sizeof(value_type) == sizeof(std::uint8_t), "MemoryBuffer::value_type should be of 'char' size, or all the 'assign' methods don't make sense");

    /* ************************************************************************** */
    /* Life cycle                                                                 */

    /** Default constructor */
    constexpr MemoryBuffer() noexcept {}

    /** Constructor from a std::vector */
    template<typename T>
    explicit MemoryBuffer(const std::vector<T>& vec)
        : MemoryBuffer(vec.data(), vec.size() * sizeof(T))
    {
        static_assert(sizeof(T) == sizeof(value_type), "vector::value_type should have the same size as MemoryBuffer::value_type");
    }

    /** Constructor from a std::string */
    MemoryBuffer(const std::string& str)
        : MemoryBuffer(str.data(), str.size() * sizeof(std::string::value_type))
    {
    }

    /** Constructor from a raw buffer */
    MemoryBuffer(const void* ptr, size_t bytes)
    {
        assign(ptr, bytes);
    }

    /** Copy constructor */
    MemoryBuffer(const MemoryBuffer& buffer)
    {
        *this = buffer;
    }

    /** Move constructor */
    MemoryBuffer(MemoryBuffer&& buffer) noexcept
        : _data(buffer._data)
        , _capacity(buffer._capacity)
        , _size(buffer._size)
    {
        buffer._data = nullptr;
        buffer._capacity = 0;
        buffer._size = 0;
    }

    /** Copy assignment */
    MemoryBuffer& operator=(const MemoryBuffer& buffer)
    {
        if (this != &buffer)
        {
            if (!buffer.empty())
            {
                if (!reserve(buffer.size())) {
                    ESP_LOGE("BUFFER", "Memory allocation failed during copy assignment");
                    return *this;
                }
                std::memcpy(_data, buffer.data(), buffer.size());
            }
            set_size(buffer.size());
        }
        return *this;
    }

    /** Move assignment */
    MemoryBuffer& operator=(MemoryBuffer&& buffer) noexcept
    {
        if (this != &buffer)
        {
            if (_data != nullptr)
            {
                std::free(_data);
            }
            _data = buffer._data;
            _capacity = buffer._capacity;
            _size = buffer._size;

            buffer._data = nullptr;
            buffer._capacity = 0;
            buffer._size = 0;
        }
        return *this;
    }

    /** Destructor */
    ~MemoryBuffer() noexcept
    {
        clear();
        deallocate_buffer();
    }

    /* ************************************************************************** */
    /* Comparison operators                                                       */
    bool operator==(const MemoryBuffer& other) const noexcept
    {
        if (_size != other._size)
        {
            return false;
        }

        if (_data == other._data)
        {
            return true;
        }

        return std::memcmp(_data, other._data, _size) == 0;
    }

    bool operator!=(const MemoryBuffer& other) const noexcept
    {
        return !(*this == other);
    }

    /* ************************************************************************** */
    /* Writers                                                                   */

    /** Assign content from a std::string */
    void assign(const std::string& str)
    {
        assign(str.data(), str.size() * sizeof(std::string::value_type));
    }

    /** Assign content from a std::vector */
    template<typename T>
    void assign(const std::vector<T>& vec)
    {
        static_assert(sizeof(T) == sizeof(value_type), "vector::value_type should have the same size as MemoryBuffer::value_type");
        assign(vec.data(), vec.size() * sizeof(T));
    }

    /** Assign content from a raw buffer */
    void assign(const void* ptr, size_t bytes)
    {
        if (!set_size(bytes))
        {
            ESP_LOGE("BUFFER", "Memory allocation failed during assign");
            return;
        }
        std::memcpy(_data, ptr, bytes);
    }

    /** Append content from a std::string */
    void append(const std::string& str)
    {
        append(str.data(), str.size() * sizeof(std::string::value_type));
    }

    /** Append content from a std::vector */
    template<typename T>
    void append(const std::vector<T>& vec)
    {
        append(vec.data(), vec.size() * sizeof(T));
    }

    /** Append content of a simple type (arithmetic or enum) */
    template<typename T, typename = std::enable_if<std::is_arithmetic<T>::value || std::is_enum<T>::value>>
    void append(const T& v)
    {
        append(&v, sizeof(v));
    }

    /** Append content from a raw buffer */
    void append(const void* ptr, size_t bytes)
    {
        size_t offset = _size;
        if (!set_size(_size + bytes))
        {
            ESP_LOGE("BUFFER", "Memory allocation failed during append");
            return;
        }
        std::memcpy(_data + offset, ptr, bytes);
    }

    /* ************************************************************************** */
    /* Data access                                                                */

    /** Returns the raw data */
    value_type* data() noexcept
    {
        return _data;
    }

    /** Returns the raw const data */
    const value_type* data() const noexcept
    {
        return _data;
    }

    /* ************************************************************************** */
    /* Capacity getters                                                           */

    /** Gets the current count of valid elements in the buffer */
    constexpr size_t size() const noexcept
    {
        return _size;
    }

    /** True if the buffer contains no element */
    constexpr bool empty() const noexcept
    {
        return _size == 0;
    }

    /** Gets the current allocated buffer size */
    constexpr size_t capacity() const noexcept
    {
        return _capacity;
    }

    /** True if the buffer has been allocated */
    constexpr bool isValid() const noexcept
    {
        return _data != nullptr;
    }

    explicit constexpr operator bool() const noexcept
    {
        return isValid();
    }

    /* ************************************************************************** */
    /* Capacity modifiers                                                         */

    /** Increase the current allocated buffer size, without changing the count of valid elements */
    bool reserve(size_t new_cap)
    {
        if (new_cap > _capacity)
        {
            auto new_ptr = std::realloc(_data, new_cap);
            if (new_ptr == nullptr)
            {
                ESP_LOGE("BUFFER", "Memory allocation failed during reserve");
                return false;
            }
            _data = static_cast<value_type*>(new_ptr);
            _capacity = new_cap;
        }
        return true;
    }

    /** Shrinks the allocated buffer to best fit the count of valid elements */
    void shrink_to_fit()
    {
        if (_data != nullptr && _size != _capacity)
        {
            if (_size == 0)
            {
                deallocate_buffer();
            }
            else
            {
                auto new_ptr = std::realloc(_data, _size);
                if (new_ptr == nullptr)
                {
                    ESP_LOGE("BUFFER", "Memory allocation failed during shrink_to_fit");
                    return;
                }
                _data = static_cast<value_type*>(new_ptr);
                _capacity = _size;
            }
        }
    }

    /** Removes all the valid elements in the buffer, without deallocating the buffer */
    void clear() noexcept
    {
        set_size(0u);
    }

    /**
     * @brief Changes the used size of the buffer, possibly reallocating it.
     * @param[in] used_size The new used bytes value.
     */
    bool set_size(size_t used_size)
    {
        if (!reserve(used_size))
        {
            return false;
        }
        _size = used_size;
        return true;
    }

    /**
     * @brief Removes bytes from the beginning of the buffer, shifting the remaining.
     * @param[in] consumed_size The bytes to consume.
     */
    void consume_size(size_t consumed_size) noexcept
    {
        if (_data != nullptr && _size != 0 && consumed_size != 0)
        {
            auto to_consume = std::min(consumed_size, _size);
            auto remaining = _size - to_consume;
            if (remaining > 0)  // Remaining data in the buffer that we need to move
            {
                std::memmove(_data, _data + to_consume, remaining);
            }
            _size -= to_consume;
        }
    }

private:
    /** Deallocates the buffer */
    void deallocate_buffer() noexcept
    {
        if (_data != nullptr)
        {
            std::free(_data);
            _data = nullptr;
            _capacity = 0;
        }
    }

    value_type* _data{ nullptr };
    size_t _capacity{ 0u };
    size_t _size{ 0u };
};

#endif /* COMPONENTS_ATDECC_INCLUDE_MEMORYBUFFER_HPP_ */
