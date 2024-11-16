#include "protocolAcmpdu.hpp"
#include <cstring> // For memcpy

// Multicast MAC Address for ACMPDU
const MacAddress Acmpdu::Multicast_Mac_Address = { 0x91, 0xe0, 0xf0, 0x01, 0x00, 0x00 };

// Factory method to create a new Acmpdu UniquePointer
UniquePointer<Acmpdu> Acmpdu::create() noexcept {
    return UniquePointer<Acmpdu>(createRawAcmpdu(), [](Acmpdu* self) { self->destroy(); });
}

// Methods to create different types of ACMPDU messages
Acmpdu Acmpdu::createConnectTxCommand() {
    Acmpdu acmpdu;
    acmpdu.setMessageType(AcmpMessageType::CONNECT_TX_COMMAND);
    return acmpdu;
}

Acmpdu Acmpdu::createDisconnectTxCommand() {
    Acmpdu acmpdu;
    acmpdu.setMessageType(AcmpMessageType::DISCONNECT_TX_COMMAND);
    return acmpdu;
}

Acmpdu Acmpdu::createGetTxStateCommand() {
    Acmpdu acmpdu;
    acmpdu.setMessageType(AcmpMessageType::GET_TX_STATE_COMMAND);
    return acmpdu;
}

Acmpdu Acmpdu::createConnectRxCommand() {
    Acmpdu acmpdu;
    acmpdu.setMessageType(AcmpMessageType::CONNECT_RX_COMMAND);
    return acmpdu;
}

Acmpdu Acmpdu::createDisconnectRxCommand() {
    Acmpdu acmpdu;
    acmpdu.setMessageType(AcmpMessageType::DISCONNECT_RX_COMMAND);
    return acmpdu;
}

Acmpdu Acmpdu::createGetRxStateCommand() {
    Acmpdu acmpdu;
    acmpdu.setMessageType(AcmpMessageType::GET_RX_STATE_COMMAND);
    return acmpdu;
}

Acmpdu Acmpdu::createGetTxConnectionCommand() {
    Acmpdu acmpdu;
    acmpdu.setMessageType(AcmpMessageType::GET_TX_CONNECTION_COMMAND);
    return acmpdu;
}

// Constructor
Acmpdu::Acmpdu() {
    std::memset(_streamDestAddress.data(), 0, _streamDestAddress.size());
}

// Destructor
Acmpdu::~Acmpdu() = default;

// Setters
void Acmpdu::setMessageType(AcmpMessageType messageType) {
    _messageType = messageType;
}

void Acmpdu::setStatus(AcmpStatus status) {
    _status = status;
}

void Acmpdu::setControllerEntityID(UniqueIdentifier controllerEntityID) {
    _controllerEntityID = controllerEntityID;
}

void Acmpdu::setTalkerEntityID(UniqueIdentifier talkerEntityID) {
    _talkerEntityID = talkerEntityID;
}

void Acmpdu::setListenerEntityID(UniqueIdentifier listenerEntityID) {
    _listenerEntityID = listenerEntityID;
}

void Acmpdu::setTalkerUniqueID(AcmpUniqueID talkerUniqueID) {
    _talkerUniqueID = talkerUniqueID;
}

void Acmpdu::setListenerUniqueID(AcmpUniqueID listenerUniqueID) {
    _listenerUniqueID = listenerUniqueID;
}

void Acmpdu::setStreamDestAddress(const MacAddress streamDestAddress) {
    std::memcpy(_streamDestAddress.data(), streamDestAddress.data(), _streamDestAddress.size());
}

void Acmpdu::setConnectionCount(uint16_t connectionCount) {
    _connectionCount = connectionCount;
}

void Acmpdu::setSequenceID(AcmpSequenceID sequenceID) {
    _sequenceID = sequenceID;
}

void Acmpdu::setFlags(uint16_t flags) {
    _flags = flags;
}

void Acmpdu::setStreamVlanID(uint16_t streamVlanID) {
    _streamVlanID = streamVlanID;
}

// Getters
AcmpMessageType Acmpdu::getMessageType() const {
    return _messageType;
}

AcmpStatus Acmpdu::getStatus() const {
    return _status;
}

UniqueIdentifier Acmpdu::getControllerEntityID() const {
    return _controllerEntityID;
}

UniqueIdentifier Acmpdu::getTalkerEntityID() const {
    return _talkerEntityID;
}

UniqueIdentifier Acmpdu::getListenerEntityID() const {
    return _listenerEntityID;
}

AcmpUniqueID Acmpdu::getTalkerUniqueID() const {
    return _talkerUniqueID;
}

AcmpUniqueID Acmpdu::getListenerUniqueID() const {
    return _listenerUniqueID;
}

const MacAddress& Acmpdu::getStreamDestAddress() const {
    return _streamDestAddress;
}

uint16_t Acmpdu::getConnectionCount() const {
    return _connectionCount;
}

AcmpSequenceID Acmpdu::getSequenceID() const {
    return _sequenceID;
}

uint16_t Acmpdu::getFlags() const {
    return _flags;
}

uint16_t Acmpdu::getStreamVlanID() const {
    return _streamVlanID;
}

// Serialization
void Acmpdu::serialize(uint8_t* buffer) const {
    if (buffer == nullptr) {
        ESP_LOGE("ACMPDU", "Serialization buffer is null");
        return;
    }

    std::memcpy(buffer, &_controllerEntityID, sizeof(_controllerEntityID));
    buffer += sizeof(_controllerEntityID);

    std::memcpy(buffer, &_talkerEntityID, sizeof(_talkerEntityID));
    buffer += sizeof(_talkerEntityID);

    std::memcpy(buffer, &_listenerEntityID, sizeof(_listenerEntityID));
    buffer += sizeof(_listenerEntityID);

    std::memcpy(buffer, &_talkerUniqueID, sizeof(_talkerUniqueID));
    buffer += sizeof(_talkerUniqueID);

    std::memcpy(buffer, &_listenerUniqueID, sizeof(_listenerUniqueID));
    buffer += sizeof(_listenerUniqueID);

    std::memcpy(buffer, _streamDestAddress.data(), _streamDestAddress.size());
    buffer += sizeof(_streamDestAddress);

    std::memcpy(buffer, &_connectionCount, sizeof(_connectionCount));
    buffer += sizeof(_connectionCount);

    std::memcpy(buffer, &_sequenceID, sizeof(_sequenceID));
    buffer += sizeof(_sequenceID);

    std::memcpy(buffer, &_flags, sizeof(_flags));
    buffer += sizeof(_flags);

    std::memcpy(buffer, &_streamVlanID, sizeof(_streamVlanID));
    buffer += sizeof(_streamVlanID);

    std::uint16_t reserved = 0;
    std::memcpy(buffer, &reserved, sizeof(reserved));
}

// Deserialization
void Acmpdu::deserialize(const uint8_t* buffer, size_t length) {
    if (buffer == nullptr || length < Length) {
        ESP_LOGE("ACMPDU", "Buffer is null or length is insufficient for deserialization.");
    }

    std::memcpy(&_controllerEntityID, buffer, sizeof(_controllerEntityID));
    buffer += sizeof(_controllerEntityID);

    std::memcpy(&_talkerEntityID, buffer, sizeof(_talkerEntityID));
    buffer += sizeof(_talkerEntityID);

    std::memcpy(&_listenerEntityID, buffer, sizeof(_listenerEntityID));
    buffer += sizeof(_listenerEntityID);

    std::memcpy(&_talkerUniqueID, buffer, sizeof(_talkerUniqueID));
    buffer += sizeof(_talkerUniqueID);

    std::memcpy(&_listenerUniqueID, buffer, sizeof(_listenerUniqueID));
    buffer += sizeof(_listenerUniqueID);

    std::memcpy(_streamDestAddress.data(), buffer, _streamDestAddress.size());
    buffer += sizeof(_streamDestAddress);

    std::memcpy(&_connectionCount, buffer, sizeof(_connectionCount));
    buffer += sizeof(_connectionCount);

    std::memcpy(&_sequenceID, buffer, sizeof(_sequenceID));
    buffer += sizeof(_sequenceID);

    std::memcpy(&_flags, buffer, sizeof(_flags));
    buffer += sizeof(_flags);

    std::memcpy(&_streamVlanID, buffer, sizeof(_streamVlanID));
    buffer += sizeof(_streamVlanID);

    std::uint16_t reserved = 0;
    std::memcpy(&reserved, buffer, sizeof(reserved));
}

// Copy method for cloning an Acmpdu
UniquePointer<Acmpdu> Acmpdu::copy() const {
    return UniquePointer<Acmpdu>(new Acmpdu(*this), [](Acmpdu* self) { self->destroy(); });
}

// Create a new Acmpdu object
Acmpdu* Acmpdu::createRawAcmpdu() noexcept {
    return new Acmpdu();
}

// Destroy an Acmpdu object
void Acmpdu::destroy() noexcept {
    delete this;
}