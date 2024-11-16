#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMAECPDU_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMAECPDU_HPP_

#pragma once

#include "protocolAecpdu.hpp"
#include <utility>
#include <array>
#include <cstring> // memcpy

/** AEM Aecpdu message */
class AemAecpdu final : public Aecpdu
{
public:
    static constexpr size_t HEADER_LENGTH = 2; /* Unsolicited + CommandType */
    static constexpr size_t MAXIMUM_PAYLOAD_LENGTH_17221 = Aecpdu::MAXIMUM_LENGTH_1722_1 - Aecpdu::HEADER_LENGTH - HEADER_LENGTH;
    static constexpr size_t MAXIMUM_PAYLOAD_BUFFER_LENGTH = Aecpdu::MAXIMUM_LENGTH_BIG_PAYLOADS - Aecpdu::HEADER_LENGTH - HEADER_LENGTH;
    static constexpr size_t MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH = Aecpdu::MAXIMUM_SEND_LENGTH - Aecpdu::HEADER_LENGTH - HEADER_LENGTH;
    static constexpr size_t MAXIMUM_RECV_PAYLOAD_BUFFER_LENGTH = Aecpdu::MAXIMUM_RECV_LENGTH - Aecpdu::HEADER_LENGTH - HEADER_LENGTH;
    static_assert(MAXIMUM_PAYLOAD_BUFFER_LENGTH >= MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH && MAXIMUM_PAYLOAD_BUFFER_LENGTH >= MAXIMUM_RECV_PAYLOAD_BUFFER_LENGTH, "Incoherent constexpr values");

    using Payload = std::pair<const void*, size_t>;

    /**
     * @brief Factory method to create a new AemAecpdu.
     * @details Creates a new AemAecpdu as a unique pointer.
     * @param[in] isResponse True if the AEM message is a response, false if it's a command.
     * @return A new AemAecpdu as a Aecpdu::UniquePointer.
     */
    static UniquePointer create(bool isResponse) noexcept;

    /** Constructor for heap AemAecpdu */
    AemAecpdu(bool isResponse) noexcept;

    /** Destructor */
    ~AemAecpdu() noexcept;

    // Setters
    void setUnsolicited(bool unsolicited) noexcept;
    void setCommandType(AemCommandType commandType) noexcept;
    void setCommandSpecificData(const void* commandSpecificData, size_t commandSpecificDataLength);

    // Getters
    bool getUnsolicited() const noexcept;
    AemCommandType getCommandType() const noexcept;
    Payload getPayload() const noexcept;

    /** Serialize the AEM AECPDU to a buffer */
    void serialize(uint8_t* buffer, size_t length) const;

    /** Deserialize the AEM AECPDU from a buffer */
    void deserialize(const uint8_t* buffer, size_t length);

    /** Construct a Response message to this Command (changing the messageType to Response kind) */
    UniquePointer responseCopy() const;

private:
    /** Entry point for creating a raw AemAecpdu */
    static AemAecpdu* createRawAemAecpdu(bool isResponse) noexcept;

    /** Destroy method for UniquePointer */
    void destroy();

    // Aem header data
    bool _unsolicited{ false };
    AemCommandType _commandType{ AemCommandType::INVALID_COMMAND_TYPE };
    std::array<uint8_t, MAXIMUM_PAYLOAD_BUFFER_LENGTH> _commandSpecificData{};
    size_t _commandSpecificDataLength{ 0u };
};

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMAECPDU_HPP_ */