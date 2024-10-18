#ifndef COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREE_HPP_
#define COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREE_HPP_

#pragma once

#include "entityModelTreeDynamic.hpp"
#include "entityModelTreeStatic.hpp"

#include <map>
#include <set>

struct AudioUnitNodeModels
{
    AudioUnitNodeStaticModel staticModel{};
    AudioUnitNodeDynamicModel dynamicModel{};
};

struct StreamInputNodeModels
{
    StreamNodeStaticModel staticModel{};
    StreamInputNodeDynamicModel dynamicModel{};
};

struct StreamOutputNodeModels
{
    StreamNodeStaticModel staticModel{};
    StreamOutputNodeDynamicModel dynamicModel{};
};

struct AvbInterfaceNodeModels
{
    AvbInterfaceNodeStaticModel staticModel{};
    AvbInterfaceNodeDynamicModel dynamicModel{};
};

struct ClockSourceNodeModels
{
    ClockSourceNodeStaticModel staticModel{};
    ClockSourceNodeDynamicModel dynamicModel{};
};

struct MemoryObjectNodeModels
{
    MemoryObjectNodeStaticModel staticModel{};
    MemoryObjectNodeDynamicModel dynamicModel{};
};

struct LocaleNodeModels
{
    LocaleNodeStaticModel staticModel{};
};

struct StringsNodeModels
{
    StringsNodeStaticModel staticModel{};
};

struct StreamPortNodeModels
{
    StreamPortNodeStaticModel staticModel{};
    StreamPortNodeDynamicModel dynamicModel{};
};

struct AudioClusterNodeModels
{
    AudioClusterNodeStaticModel staticModel{};
    AudioClusterNodeDynamicModel dynamicModel{};
};

struct AudioMapNodeModels
{
    AudioMapNodeStaticModel staticModel{};
};

struct ControlNodeModels
{
    ControlNodeStaticModel staticModel{};
    ControlNodeDynamicModel dynamicModel{};
};

struct ClockDomainNodeModels
{
    ClockDomainNodeStaticModel staticModel{};
    ClockDomainNodeDynamicModel dynamicModel{};
};

struct ConfigurationTree
{
    // Children
    std::map<AudioUnitIndex, AudioUnitNodeModels> audioUnitModels{};
    std::map<StreamIndex, StreamInputNodeModels> streamInputModels{};
    std::map<StreamIndex, StreamOutputNodeModels> streamOutputModels{};
    std::map<AvbInterfaceIndex, AvbInterfaceNodeModels> avbInterfaceModels{};
    std::map<ClockSourceIndex, ClockSourceNodeModels> clockSourceModels{};
    std::map<MemoryObjectIndex, MemoryObjectNodeModels> memoryObjectModels{};
    std::map<LocaleIndex, LocaleNodeModels> localeModels{};
    std::map<StringsIndex, StringsNodeModels> stringsModels{};
    std::map<StreamPortIndex, StreamPortNodeModels> streamPortInputModels{};
    std::map<StreamPortIndex, StreamPortNodeModels> streamPortOutputModels{};
    std::map<ClusterIndex, AudioClusterNodeModels> audioClusterModels{};
    std::map<MapIndex, AudioMapNodeModels> audioMapModels{};
    std::map<ControlIndex, ControlNodeModels> controlModels{};
    std::map<ClockDomainIndex, ClockDomainNodeModels> clockDomainModels{};

    // AEM Static info
    ConfigurationNodeStaticModel staticModel;

    // AEM Dynamic info
    ConfigurationNodeDynamicModel dynamicModel;
};

struct EntityTree
{
    using ConfigurationTrees = std::map<ConfigurationIndex, ConfigurationTree>;

    // Children
    ConfigurationTrees configurationTrees{};

    // AEM Static info
    EntityNodeStaticModel staticModel;

    // AEM Dynamic info
    EntityNodeDynamicModel dynamicModel;
};




#endif /* COMPONENTS_ATDECC_INCLUDE_ENTITYMODELTREE_HPP_ */
