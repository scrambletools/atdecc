/*
 * entity.hpp
 *
 *  Created on: Sep 28, 2024
 *      Author: Dulini
 */

#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITY_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITY_HPP_

#include "protocolAcmpdu.hpp"
#include "protocolAcmpdu.hpp"
#pragma once

#include "entityEnums.hpp"
#include "uniqueIdentifier.hpp"
#include <cstdint>
#include <map>
#include <optional>
#include <string>

// Entity Class Definition
class Entity
{
public:
    struct CommonInformation
    {
        UniqueIdentifier entityID;
        UniqueIdentifier entityModelID;
        EntityCapabilities entityCapabilities;
        uint16_t talkerStreamSources;
        TalkerCapabilities talkerCapabilities;
        uint16_t listenerStreamSinks;
        ListenerCapabilities listenerCapabilities;
        ControllerCapabilities controllerCapabilities;
        std::optional<ControlIndex> identifyControlIndex;
        std::optional<UniqueIdentifier> associationID;
    };

    struct InterfaceInformation
    {
        MacAddress macAddress;
        uint8_t validTime;
        uint32_t availableIndex;
        std::optional<UniqueIdentifier> gptpGrandmasterID;
        std::optional<uint8_t> gptpDomainNumber;
    };

    using InterfacesInformation = std::map<AvbInterfaceIndex, InterfaceInformation>;

    Entity(CommonInformation const& commonInformation, InterfaceInformation const& interfacesInformation);
    ~Entity() = default;

    CommonInformation const& getCommonInformation() const noexcept;
    CommonInformation& getCommonInformation() noexcept;
    InterfaceInformation const& getInterfaceInformation(AvbInterfaceIndex const interfaceIndex) const;
    InterfaceInformation& getInterfaceInformation(AvbInterfaceIndex const interfaceIndex);
    InterfaceInformation const& getInterfacesInformation() const noexcept;
    InterfaceInformation& getInterfacesInformation() noexcept;
    void getMacAddress(MacAddress& getMacAddress) const noexcept;
    
    UniqueIdentifier getEntityID() const noexcept;
    UniqueIdentifier getEntityModelID() const noexcept;
    EntityCapabilities getEntityCapabilities() const noexcept;
    uint16_t getTalkerStreamSources() const noexcept;
    TalkerCapabilities getTalkerCapabilities() const noexcept;
    uint16_t getListenerStreamSinks() const noexcept;
    ListenerCapabilities getListenerCapabilities() const noexcept;
    ControllerCapabilities getControllerCapabilities() const noexcept;
    std::optional<ControlIndex> getIdentifyControlIndex() const noexcept;
    std::optional<UniqueIdentifier> getAssociationID() const noexcept;
    void setEntityCapabilities(EntityCapabilities const entityCapabilities) noexcept;
    void setAssociationID(std::optional<UniqueIdentifier> const associationID) noexcept;
    void setValidTime(uint8_t const validTime, std::optional<AvbInterfaceIndex> const interfaceIndex = std::nullopt);
    void setGptpGrandmasterID(UniqueIdentifier const gptpGrandmasterID, AvbInterfaceIndex const interfaceIndex);
    void setGptpDomainNumber(uint8_t const gptpDomainNumber, AvbInterfaceIndex const interfaceIndex);
	static UniqueIdentifier generateEID(MacAddress const& macAddress, uint16_t const progID);
	
private:
    CommonInformation _commonInformation;
    InterfaceInformation _interfaceInformation;
};

#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITY_HPP_ */
