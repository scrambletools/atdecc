#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLAECPDU_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLAECPDU_HPP_

#pragma once

#include <stdint.h>
#include <memory>
#include "uniqueIdentifier.hpp" 
#include "protocolDefines.hpp"

/** Aecpdu common header */
class Aecpdu
{
public:
    static constexpr size_t HEADER_LENGTH = 20;  /* Updated AECPDU Header length */
    static constexpr size_t MAXIMUM_LENGTH_1722_1 = 524; /* Maximum payload size as per specification */
#if defined(ALLOW_SEND_BIG_AECP_PAYLOADS) || defined(ALLOW_RECV_BIG_AECP_PAYLOADS)
    static constexpr size_t MAXIMUM_LENGTH_BIG_PAYLOADS = 1500 - 14 - 20; 
#else
    static constexpr size_t MAXIMUM_LENGTH_BIG_PAYLOADS = MAXIMUM_LENGTH_1722_1;
#endif
#if defined(ALLOW_SEND_BIG_AECP_PAYLOADS)
    static constexpr size_t MAXIMUM_SEND_LENGTH = MAXIMUM_LENGTH_BIG_PAYLOADS;
#else
    static constexpr size_t MAXIMUM_SEND_LENGTH = MAXIMUM_LENGTH_1722_1;
#endif
#if defined(ALLOW_RECV_BIG_AECP_PAYLOADS)
    static constexpr size_t MAXIMUM_RECV_LENGTH = MAXIMUM_LENGTH_BIG_PAYLOADS;
#else
    static constexpr size_t MAXIMUM_RECV_LENGTH = MAXIMUM_LENGTH_1722_1;
#endif


    using UniquePointer = std::unique_ptr<Aecpdu, void (*)(Aecpdu*)>;

    /** Default constructor */
    Aecpdu() noexcept;

    /** Virtual destructor */
    virtual ~Aecpdu() = default;

    /** Serialize the AECPDU to a buffer */
    virtual void serialize(uint8_t* buffer, size_t length) const;

    /** Deserialize the AECPDU from a buffer */
    virtual void deserialize(const uint8_t* buffer, size_t length);

    /** Create specific message types */
    static UniquePointer createAemMessage(AecpMessageType messageType);

    /** Destroy method for UniquePointer */
    static void destroy(Aecpdu* aecpdu);

    // Setters
    void setStatus(AecpStatus status) noexcept { _status = status; }
    void setTargetEntityID(UniqueIdentifier targetEntityID) noexcept { _targetEntityID = targetEntityID; }
    void setControllerEntityID(UniqueIdentifier controllerEntityID) noexcept { _controllerEntityID = controllerEntityID; }
    void setSequenceID(uint16_t sequenceID) noexcept { _sequenceID = sequenceID; }
    void setMessageType(AecpMessageType messageType) noexcept { _messageType = messageType; }

    // Getters
    AecpStatus getStatus() const noexcept { return _status; }
    UniqueIdentifier getTargetEntityID() const noexcept { return _targetEntityID; }
    UniqueIdentifier getControllerEntityID() const noexcept { return _controllerEntityID; }
    uint16_t getSequenceID() const noexcept { return _sequenceID; }
    AecpMessageType getMessageType() const noexcept { return _messageType; }
    

private:
    UniqueIdentifier _targetEntityID;   /* Target Entity ID */
    UniqueIdentifier _controllerEntityID; /* Controller Entity ID */
    uint16_t _sequenceID;               /* Sequence ID */
    AecpMessageType _messageType;       /* AECPDU Message Type */
    AecpStatus _status;                 /* AECPDU Status */

};

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLAECPDU_HPP_ */
