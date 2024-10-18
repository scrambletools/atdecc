#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLACMPDU_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLACMPDU_HPP_

#pragma once

#include <cstdint>            // Standard integer types
#include <cstring>            // For std::memcpy
#include <stdexcept>          // For exceptions
#include <functional>         // For std::function
#include <memory>             // For std::unique_ptr
#include "esp_log.h"          // ESP-IDF logging functions
#include "uniqueIdentifier.hpp"
#include "protocolDefines.hpp"

using AcmpUniqueID = uint16_t;
using AcmpSequenceID = uint16_t;

// Define a simple MAC address type
using MacAddress = uint8_t[6];

// UniquePointer type definition using the provided `UniquePointer` definition
template <typename T>
using UniquePointer = std::unique_ptr<T, std::function<void(T*)>>;

// ACMPDU class definition
class Acmpdu {
public:
    static constexpr size_t Length = 44; // ACMPDU size in bytes
    static const MacAddress Multicast_Mac_Address; // Multicast MAC Address

    // Factory method to create a new Acmpdu UniquePointer
    static UniquePointer<Acmpdu> create() noexcept;

    // Methods to create different types of ACMPDU messages
    static Acmpdu createConnectTxCommand();
    static Acmpdu createDisconnectTxCommand();
    static Acmpdu createGetTxStateCommand();
    static Acmpdu createConnectRxCommand();
    static Acmpdu createDisconnectRxCommand();
    static Acmpdu createGetRxStateCommand();
    static Acmpdu createGetTxConnectionCommand();

    // Constructor
    Acmpdu();

    // Destructor
    ~Acmpdu();

    // Setters
    void setMessageType(AcmpMessageType messageType);
    void setStatus(AcmpStatus status);
    void setControllerEntityID(UniqueIdentifier controllerEntityID);
    void setTalkerEntityID(UniqueIdentifier talkerEntityID);
    void setListenerEntityID(UniqueIdentifier listenerEntityID);
    void setTalkerUniqueID(AcmpUniqueID talkerUniqueID);
    void setListenerUniqueID(AcmpUniqueID listenerUniqueID);
    void setStreamDestAddress(const MacAddress streamDestAddress);
    void setConnectionCount(uint16_t connectionCount);
    void setSequenceID(AcmpSequenceID sequenceID);
    void setFlags(uint16_t flags);
    void setStreamVlanID(uint16_t streamVlanID);

    // Getters
    AcmpMessageType getMessageType() const;
    AcmpStatus getStatus() const;
    UniqueIdentifier getControllerEntityID() const;
    UniqueIdentifier getTalkerEntityID() const;
    UniqueIdentifier getListenerEntityID() const;
    AcmpUniqueID getTalkerUniqueID() const;
    AcmpUniqueID getListenerUniqueID() const;
    const MacAddress& getStreamDestAddress() const;
    uint16_t getConnectionCount() const;
    AcmpSequenceID getSequenceID() const;
    uint16_t getFlags() const;
    uint16_t getStreamVlanID() const;

    // Serialization and Deserialization
    void serialize(uint8_t* buffer) const;
    void deserialize(const uint8_t* buffer, size_t length);

    // Copy method for cloning an Acmpdu
    UniquePointer<Acmpdu> copy() const;

private:
    // ACMPDU data fields
    AcmpMessageType _messageType{AcmpMessageType::CONNECT_TX_COMMAND};  // Default message type
    AcmpStatus _status{AcmpStatus::SUCCESS};  // Default status
    UniqueIdentifier _controllerEntityID{};
    UniqueIdentifier _talkerEntityID{};
    UniqueIdentifier _listenerEntityID{};
    AcmpUniqueID _talkerUniqueID{ 0 };
    AcmpUniqueID _listenerUniqueID{ 0 };
    MacAddress _streamDestAddress{};
    uint16_t _connectionCount{ 0 };
    AcmpSequenceID _sequenceID{ 0 };
    uint16_t _flags{ 0 };
    uint16_t _streamVlanID{ 0 };

    // Private method for initializing a new ACMPDU
    static Acmpdu* createRawAcmpdu() noexcept;
    // Private method for destroying an ACMPDU
    void destroy() noexcept;
};



#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLACMPDU_HPP_ */
