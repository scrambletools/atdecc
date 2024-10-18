#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLAAAECPDU_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLAAAECPDU_HPP_

#pragma once

#include "protocolAecpdu.hpp"
#include "UniqueIdentifier.hpp"
#include "entityAddressAccessTypes.hpp"
#include "esp_log.h"  // ESP-IDF logging
#include <utility>
#include <vector>

/** AA Aecpdu message */
class AaAecpdu final : public Aecpdu
{
public:
    static constexpr size_t HeaderLength = 2;        /* TlvCount */
    static constexpr size_t TlvHeaderLength = 10;    /* Mode + Length + Address */

    /**
     * @brief Factory method to create a new AaAecpdu.
     * @details Creates a new AaAecpdu as a unique pointer.
     * @param[in] isResponse True if the AA message is a response, false if it's a command.
     * @return A new AaAecpdu as a Aecpdu::UniquePointer.
     */
    static UniquePointer create(bool isResponse) noexcept
    {
        auto deleter = [](Aecpdu* self)
        {
            static_cast<AaAecpdu*>(self)->destroy();
        };
        return UniquePointer(createRawAaAecpdu(isResponse), deleter);
    }

    /** Constructor for heap AaAecpdu */
    AaAecpdu(bool isResponse) noexcept;

    /** Destructor */
    ~AaAecpdu() noexcept override;

    // Setters
    //template<class Tlv, typename = std::enable_if_t<std::is_same<entity::addressAccess::Tlv, std::remove_cv_t<std::remove_reference_t<Tlv>>>::value>>
    void addTlv(Tlv&& tlv) noexcept
    {
        auto const newLength = _tlvDataLength + TlvHeaderLength + tlv.size();

        _tlvDataLength = newLength;
        _tlvData.push_back(std::forward<Tlv>(tlv));

        // Update parent's specific data length field
        // setAecpSpecificDataLength(AaAecpdu::HeaderLength + newLength); // commented because we are not changing the Avtpdu
    }

    // Getters
    const Tlvs& getTlvData() const noexcept
    {
        return _tlvData;
    }

    Tlvs& getTlvData() noexcept
    {
        return _tlvData;
    }

    /** Serialize the AA AECPDU to a buffer */
    void serialize(uint8_t* buffer, size_t length) const override;

    /** Deserialize the AA AECPDU from a buffer */
    void deserialize(const uint8_t* buffer, size_t length) override;

    /**
     * Construct a Response message to this Command.
     * Returns nullptr if the message is not a Command or if no Response is possible for this messageType.
     */
    UniquePointer responseCopy() const ;

    // Defaulted compiler auto-generated methods
    AaAecpdu(AaAecpdu&&) = default;
    AaAecpdu(const AaAecpdu&) = default;
    AaAecpdu& operator=(const AaAecpdu&) = default;
    AaAecpdu& operator=(AaAecpdu&&) = default;

private:
    /** Entry point */
    static AaAecpdu* createRawAaAecpdu(bool isResponse) noexcept;

    /** Destroy method for COM-like interface */
    void destroy() noexcept ;

    // Aa header data
    Tlvs _tlvData{};

    // Private data
    size_t _tlvDataLength{0};
};

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLAAAECPDU_HPP_ */
