#include "entity.hpp"
#include "esp_log.h"

#define LOG_TAG "Entity"

Entity::Entity(CommonInformation const& commonInformation, InterfaceInformation const& interfacesInformation)
    : _commonInformation(commonInformation), _interfaceInformation(interfacesInformation)
{
}

Entity::CommonInformation const& Entity::getCommonInformation() const noexcept
{
    return _commonInformation;
}

Entity::CommonInformation& Entity::getCommonInformation() noexcept
{
    return _commonInformation;
}

Entity::InterfaceInformation const& Entity::getInterfacesInformation() const noexcept
{
    return _interfaceInformation;
}

Entity::InterfaceInformation& Entity::getInterfacesInformation() noexcept
{
    return _interfaceInformation;
}

UniqueIdentifier Entity::getEntityID() const noexcept
{
    return _commonInformation.entityID;
}

void Entity::getMacAddress(MacAddress& getMacAddress) const noexcept
{

    memcpy(getMacAddress.data(), _interfaceInformation.macAddress.data(), getMacAddress.size());
}

void Entity::setEntityCapabilities(EntityCapabilities const entityCapabilities) noexcept
{
    _commonInformation.entityCapabilities = entityCapabilities;
}

void Entity::setAssociationID(std::optional<UniqueIdentifier> const associationID) noexcept
{
    _commonInformation.associationID = associationID;
}

void Entity::setValidTime(uint8_t const validTime, std::optional<AvbInterfaceIndex> const interfaceIndex)
{
    uint8_t minValidTime = 1;
    uint8_t maxValidTime = 31;
    auto value = std::min(maxValidTime, std::max(minValidTime, validTime));

    if (interfaceIndex)
    {
        getInterfaceInformation(*interfaceIndex).validTime = value;
    }
}

UniqueIdentifier Entity::generateEID(MacAddress const& macAddress, uint16_t const progID)
{
    UniqueIdentifier::value_type eid{ 0u };
    if (sizeof(macAddress)/sizeof(macAddress[0]) != 6)
    {
        ESP_LOGE("Entity", "Invalid MAC address size.");
        return UniqueIdentifier{}; // Return an invalid UniqueIdentifier
    }

    eid += macAddress[0];
    eid <<= 8;
    eid += macAddress[1];
    eid <<= 8;
    eid += macAddress[2];

    eid <<= 8;
    eid += macAddress[3];
    eid <<= 8;
    eid += macAddress[4];
    eid <<= 8;
    eid += macAddress[5];

	eid <<= 16;
    eid += progID;

    return UniqueIdentifier{ eid };
}