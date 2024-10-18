#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYADDRESSACCESSTYPES_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYADDRESSACCESSTYPES_HPP_

#pragma once

#include <stdint.h>
#include <vector>
#include <string.h>
#include "esp_log.h"
#include "protocolDefines.hpp"

constexpr size_t MaxTlvLength = (1 << 12);  // Maximum length for TLV memory data (4096 bytes)

/** Type-Length-Value for Address Access */
class Tlv final
{
public:
    using value_type = uint8_t;
    using memory_data_type = std::vector<value_type>;

    /** Default constructor for an invalid TLV. */
    Tlv() noexcept
        : _mode(AaMode::READ), _address(0), _memoryData()
    {
    }

    /** Constructor for a Read mode TLV with specified address and length. */
    Tlv(uint64_t address, size_t length)
        : _mode(AaMode::READ), _address(address), _memoryData()
    {
        if (length == 0)
        {
            ESP_LOGI("TLV", "Length cannot be zero.");
            return;
        }
        if (length > MaxTlvLength)
        {
            ESP_LOGI("TLV", "Length exceeds maximum TLV length.");
            return;
        }
        _memoryData.resize(length, 0);
    }

    /** Constructor for a specified mode, address, and length. */
    Tlv(AaMode mode, uint64_t address, size_t length)
        : _mode(mode), _address(address), _memoryData()
    {
        if (length == 0)
        {
            ESP_LOGI("TLV", "Length cannot be zero.");
            return;
        }
        if (length > MaxTlvLength)
        {
            ESP_LOGI("TLV", "Length exceeds maximum TLV length.");
            return;
        }
        _memoryData.resize(length, 0);
    }

    /** Constructor for Write/Execute mode with address and memory data. */
    Tlv(uint64_t address, AaMode mode, const memory_data_type& memoryData)
        : _mode(mode), _address(address), _memoryData()
    {
        if (memoryData.empty())
        {
            ESP_LOGI("TLV", "Memory data cannot be empty.");
            return;
        }
        if (memoryData.size() > MaxTlvLength)
        {
            ESP_LOGI("TLV", "Memory data length exceeds maximum TLV length.");
            return;
        }
        _memoryData = memoryData;
    }

    /** Constructor for Write/Execute mode with address and move memory data. */
    Tlv(uint64_t address, AaMode mode, memory_data_type&& memoryData)
        : _mode(mode), _address(address), _memoryData(std::move(memoryData))
    {
        if (_memoryData.empty())
        {
            ESP_LOGI("TLV", "Memory data cannot be empty.");
            return;
        }
        if (_memoryData.size() > MaxTlvLength)
        {
            ESP_LOGI("TLV", "Memory data length exceeds maximum TLV length.");
            return;
        }
    }

    /** Constructor for Write/Execute mode with raw buffer data. */
    Tlv(uint64_t address, AaMode mode, const void* ptr, size_t size)
        : _mode(mode), _address(address), _memoryData()
    {
        if (size == 0)
        {
            ESP_LOGI("TLV", "Buffer size cannot be zero.");
            return;
        }
        if (size > MaxTlvLength)
        {
            ESP_LOGI("TLV", "Buffer size exceeds maximum TLV length.");
            return;
        }
        const value_type* copyPtr = static_cast<const value_type*>(ptr);
        _memoryData.assign(copyPtr, copyPtr + size);
    }

    /** Default destructor. */
    ~Tlv() = default;

    // Getter for mode
    AaMode getMode() const noexcept { return _mode; }

    // Getter for address
    uint64_t getAddress() const noexcept { return _address; }

    // Getter for memory data (const)
    const memory_data_type& getMemoryData() const noexcept { return _memoryData; }

    // Getter for memory data (non-const)
    memory_data_type& getMemoryData() noexcept { return _memoryData; }

    // Getter for raw memory data (non-const)
    void* data() noexcept { return _memoryData.data(); }

    // Getter for raw memory data (const)
    const void* data() const noexcept { return _memoryData.data(); }

    // Getter for size of memory data
    size_t size() const noexcept { return _memoryData.size(); }

    // Check if TLV is valid
    bool isValid() const noexcept { return !_memoryData.empty(); }

    // Equality operator
    bool operator==(const Tlv& other) const noexcept
    {
        return _mode == other._mode && _address == other._address && _memoryData == other._memoryData;
    }

    // Inequality operator
    bool operator!=(const Tlv& other) const noexcept { return !(*this == other); }

    // Validity operator
    explicit operator bool() const noexcept { return isValid(); }

    // Move and Copy constructors and assignment operators
    Tlv(Tlv&&) = default;
    Tlv(const Tlv&) = default;
    Tlv& operator=(const Tlv&) = default;
    Tlv& operator=(Tlv&&) = default;

private:
    AaMode _mode;           // TLV mode
    uint64_t _address;      // Memory address
    memory_data_type _memoryData;  // Memory data
};

/** Alias for a vector of TLVs */
using Tlvs = std::vector<Tlv>;

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYADDRESSACCESSTYPES_HPP_ */
