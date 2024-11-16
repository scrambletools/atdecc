#include "protocolAemPayloads.hpp"
#include <cstring>
#include "esp_log.h"

static const char *TAG = "AEM-Payloads";

static constexpr auto PAYLOAD_BUFFER_OFFSET = sizeof(uint16_t) + sizeof(uint16_t); // Assuming ConfigurationIndex is a uint16_t

// Function to check response payload without using exceptions
static inline void checkResponsePayload(AemAecpdu::Payload const& payload, uint8_t status, size_t expectedCommandLength, size_t expectedResponseLength)
{
    auto* commandPayload = payload.first;
    auto commandPayloadLength = payload.second;

    // If status is NotImplemented, we expect a reflected message (using Command length)
    if (status == static_cast<uint8_t>(AemCommandStatus::NotImplemented))
    {
        if (commandPayloadLength != expectedCommandLength || (expectedCommandLength > 0 && commandPayload == nullptr)) // Malformed packet
        {
            ESP_LOGI(TAG, "Incorrect payload size for NotImplemented status");
            return;
        }
    }
    else
    {
        // Otherwise, we expect a valid response with all fields
        if (commandPayloadLength < expectedResponseLength || (expectedResponseLength > 0 && commandPayload == nullptr)) // Malformed packet
        {
            ESP_LOGI(TAG, "Incorrect payload size for valid response");
            return;
        }
    }
}

/** ACQUIRE_ENTITY Command - Clause 7.4.1.1 */
Serializer<AECP_AEM_ACQUIRE_ENTITY_COMMAND_PAYLOAD_SIZE> serializeAcquireEntityCommand(AemAcquireEntityFlags const flags, UniqueIdentifier const ownerID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_ACQUIRE_ENTITY_COMMAND_PAYLOAD_SIZE> ser;

    ser << flags;
    ser << ownerID;
    ser << descriptorType << descriptorIndex;

    ESP_LOGI(TAG, "Serialize Acquire Entity: used bytes %d", ser.usedBytes());

    return ser;
}

std::tuple<AemAcquireEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeAcquireEntityCommand(AemAecpdu::Payload const& payload)
{
    auto* commandPayload = payload.first;
    auto commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_ACQUIRE_ENTITY_COMMAND_PAYLOAD_SIZE) // Malformed packet
    {
        ESP_LOGI(TAG, "Malformed packet in AcquireEntity Command");
        return std::make_tuple(AemAcquireEntityFlags::NONE, UniqueIdentifier{}, DescriptorType::Invalid, 0u);
    }

    // Deserialize the payload
    Deserializer des(commandPayload, commandPayloadLength);
    AemAcquireEntityFlags flags{ AemAcquireEntityFlags::NONE };
    UniqueIdentifier ownerID{};
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> flags;
    des >> ownerID;
    des >> descriptorType >> descriptorIndex;

    ESP_LOGI(TAG, "Deserialize Acquire Entity: used bytes %d", des.usedBytes());

    return std::make_tuple(flags, ownerID, descriptorType, descriptorIndex);
}


// Serializer for ACQUIRE_ENTITY Response - Clause 7.4.1.1
Serializer<AECP_AEM_ACQUIRE_ENTITY_RESPONSE_PAYLOAD_SIZE> serializeAcquireEntityResponse(AemAcquireEntityFlags const flags, UniqueIdentifier ownerID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    // Same as ACQUIRE_ENTITY Command
    ESP_LOGI(TAG, "Serializing Acquire Entity Response");
    return serializeAcquireEntityCommand(flags, ownerID, descriptorType, descriptorIndex);
}

std::tuple<AemAcquireEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeAcquireEntityResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
    // Same as ACQUIRE_ENTITY Command
    ESP_LOGI(TAG, "Deserializing Acquire Entity Response");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_ACQUIRE_ENTITY_COMMAND_PAYLOAD_SIZE, AECP_AEM_ACQUIRE_ENTITY_RESPONSE_PAYLOAD_SIZE);
    return deserializeAcquireEntityCommand(payload);
}

// Serializer for LOCK_ENTITY Command - Clause 7.4.2.1
Serializer<AECP_AEM_LOCK_ENTITY_COMMAND_PAYLOAD_SIZE> serializeLockEntityCommand(AemLockEntityFlags flags, UniqueIdentifier lockedID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_LOCK_ENTITY_COMMAND_PAYLOAD_SIZE> ser;

    ser << flags;
    ser << lockedID;
    ser << descriptorType << descriptorIndex;

    ESP_LOGI(TAG, "Serialize Lock Entity Command: used bytes %d", ser.usedBytes());

    return ser;
}

std::tuple<AemLockEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeLockEntityCommand(AemAecpdu::Payload const& payload)
{
    auto* commandPayload = payload.first;
    auto commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_LOCK_ENTITY_COMMAND_PAYLOAD_SIZE) 
    {
        ESP_LOGI(TAG, "Malformed packet in LockEntity Command");
        return std::make_tuple(AemLockEntityFlags::NONE, UniqueIdentifier{}, DescriptorType::Invalid, 0u);
    }

    // Deserialize the payload
    Deserializer des(commandPayload, commandPayloadLength);
    AemLockEntityFlags flags{ AemLockEntityFlags::NONE };
    UniqueIdentifier lockedID{};
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> flags;
    des >> lockedID;
    des >> descriptorType >> descriptorIndex;

    ESP_LOGI(TAG, "Deserialize Lock Entity Command: used bytes %d", des.usedBytes());

    return std::make_tuple(flags, lockedID, descriptorType, descriptorIndex);
}

/** LOCK_ENTITY Response - Clause 7.4.2.1 */
Serializer<AECP_AEM_LOCK_ENTITY_RESPONSE_PAYLOAD_SIZE> serializeLockEntityResponse(AemLockEntityFlags flags, UniqueIdentifier lockedID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    // Same as LOCK_ENTITY Command
    ESP_LOGI(TAG, "Serializing Lock Entity Response");
    return serializeLockEntityCommand(flags, lockedID, descriptorType, descriptorIndex);
}

std::tuple<AemLockEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeLockEntityResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
    // Same as LOCK_ENTITY Command
    ESP_LOGI(TAG, "Deserializing Lock Entity Response");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_LOCK_ENTITY_COMMAND_PAYLOAD_SIZE, AECP_AEM_LOCK_ENTITY_RESPONSE_PAYLOAD_SIZE);
    return deserializeLockEntityCommand(payload);
}

/** READ_DESCRIPTOR Command - Clause 7.4.5.1 */
Serializer<AECP_AEM_READ_DESCRIPTOR_COMMAND_PAYLOAD_SIZE> serializeReadDescriptorCommand(ConfigurationIndex const configurationIndex, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_READ_DESCRIPTOR_COMMAND_PAYLOAD_SIZE> ser;
    std::uint16_t const reserved{ 0u };

    ser << configurationIndex << reserved;
    ser << descriptorType << descriptorIndex;

    ESP_LOGI(TAG, "Serialize Read Descriptor Command: used bytes %d", ser.usedBytes());

    return ser;
}

std::tuple<ConfigurationIndex, DescriptorType, DescriptorIndex> deserializeReadDescriptorCommand(AemAecpdu::Payload const& payload)
{
    auto* commandPayload = payload.first;
    auto commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_DESCRIPTOR_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI(TAG, "Malformed packet in Read Descriptor Command");
        return std::make_tuple(0u, DescriptorType::Invalid, 0u);
    }

    Deserializer des(commandPayload, commandPayloadLength);
    ConfigurationIndex configurationIndex{ 0u };
    std::uint16_t reserved{ 0u };
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> configurationIndex >> reserved;
    des >> descriptorType >> descriptorIndex;

    ESP_LOGI(TAG, "Deserialize Read Descriptor Command: used bytes %d", des.usedBytes());

    return std::make_tuple(configurationIndex, descriptorType, descriptorIndex);
}

/** READ_DESCRIPTOR Response - Clause 7.4.5.2 */
Serializer<AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH> serializeReadDescriptorCommonResponse(ConfigurationIndex const configurationIndex, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH> ser;
    std::uint16_t const reserved{ 0u };

    ser << configurationIndex << reserved;
    ser << descriptorType << descriptorIndex;

    ESP_LOGI(TAG, "Serialize Read Descriptor Common Response");

    return ser;
}

/** Serialize READ_ENTITY_DESCRIPTOR Response */
void serializeReadEntityDescriptorResponse(Serializer<AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH>& ser, const EntityDescriptor& entityDescriptor)
{
    ser << entityDescriptor.entityID << entityDescriptor.entityModelID << entityDescriptor.entityCapabilities;
    ser << entityDescriptor.talkerStreamSources << entityDescriptor.talkerCapabilities;
    ser << entityDescriptor.listenerStreamSinks << entityDescriptor.listenerCapabilities;
    ser << entityDescriptor.controllerCapabilities;
    ser << entityDescriptor.availableIndex;
    ser << entityDescriptor.associationID;
    ser << entityDescriptor.entityName;
    ser << entityDescriptor.vendorNameString << entityDescriptor.modelNameString;
    ser << entityDescriptor.firmwareVersion;
    ser << entityDescriptor.groupName;
    ser << entityDescriptor.serialNumber;
    ser << entityDescriptor.configurationsCount << entityDescriptor.currentConfiguration;

    ESP_LOGI(TAG, "Serialized Entity Descriptor Response");
}

/** Serialize READ_CONFIGURATION_DESCRIPTOR Response */
void serializeReadConfigurationDescriptorResponse(Serializer<AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH>& ser, const ConfigurationDescriptor& configurationDescriptor)
{
    ser << configurationDescriptor.objectName;
    ser << configurationDescriptor.localizedDescription;
    uint16_t descriptorCountsCount = static_cast<uint16_t>(ATDECC_MAX_DESCRIPTOR_TYPES);
    ser << descriptorCountsCount;

    uint16_t descriptorCountsOffset = static_cast<uint16_t>(ser.usedBytes() - PAYLOAD_BUFFER_OFFSET + sizeof(uint16_t));
    ESP_LOGI(TAG, "Descriptor Counts offset: %d", descriptorCountsOffset);
    ser << descriptorCountsOffset;

    /*for (const auto& descriptorCountEntry : configurationDescriptor.descriptorCounts)
	{
	    auto const& descriptorType = descriptorCountEntry.first;
	    auto const& descriptorCount = descriptorCountEntry.second;
	    ser << descriptorType << descriptorCount;
	    ESP_LOGI(TAG, "Serialized Configuration Descriptor Response");
	}*/
	
	ESP_LOGI(TAG, "Serialized Configuration Descriptor Response");
}

/** Deserialize common fields from a READ_DESCRIPTOR Response */
std::tuple<size_t, ConfigurationIndex, DescriptorType, DescriptorIndex> deserializeReadDescriptorCommonResponse(uint8_t status, const AemAecpdu::Payload& payload)
{
    auto* commandPayload = payload.first;
    auto commandPayloadLength = payload.second;

    checkResponsePayload(payload, status, AECP_AEM_READ_DESCRIPTOR_COMMAND_PAYLOAD_SIZE, AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE);

    Deserializer des(commandPayload, commandPayloadLength);
    ConfigurationIndex configurationIndex = 0u;
    uint16_t reserved = 0u;
    DescriptorType descriptorType = DescriptorType::Invalid;
    DescriptorIndex descriptorIndex = 0u;

    des >> configurationIndex >> reserved;
    des >> descriptorType >> descriptorIndex;

    ESP_LOGI(TAG, "Deserialized Read Descriptor Common Response: Used %d bytes", des.usedBytes());

    return std::make_tuple(des.usedBytes(), configurationIndex, descriptorType, descriptorIndex);
}

/** Deserialize READ_ENTITY_DESCRIPTOR Response */
EntityDescriptor deserializeReadEntityDescriptorResponse(const AemAecpdu::Payload& payload, size_t commonSize, uint8_t status)
{
    EntityDescriptor entityDescriptor{};

    if (status == static_cast<uint8_t>(AecpStatus::SUCCESS))
    {
        auto* commandPayload = payload.first;
        auto commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_ENTITY_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI(TAG, "Malformed packet in Read Entity Descriptor Response");
            return entityDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize);

        des >> entityDescriptor.entityID >> entityDescriptor.entityModelID >> entityDescriptor.entityCapabilities;
        des >> entityDescriptor.talkerStreamSources >> entityDescriptor.talkerCapabilities;
        des >> entityDescriptor.listenerStreamSinks >> entityDescriptor.listenerCapabilities;
        des >> entityDescriptor.controllerCapabilities;
        des >> entityDescriptor.availableIndex;
        des >> entityDescriptor.associationID;
        des >> entityDescriptor.entityName;
        des >> entityDescriptor.vendorNameString >> entityDescriptor.modelNameString;
        des >> entityDescriptor.firmwareVersion;
        des >> entityDescriptor.groupName;
        des >> entityDescriptor.serialNumber;
        des >> entityDescriptor.configurationsCount >> entityDescriptor.currentConfiguration;

        ESP_LOGI(TAG, "Deserialized Read Entity Descriptor Response: Used %d bytes", des.usedBytes());

        if (des.remaining() != 0)
        {
            ESP_LOGI(TAG, "Warning: Remaining bytes in buffer for READ_ENTITY_DESCRIPTOR RESPONSE: %d", des.remaining());
        }
    }

    return entityDescriptor;
}

ConfigurationDescriptor deserializeReadConfigurationDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    ConfigurationDescriptor configurationDescriptor{};

    // Clause 7.4.5.2 says we should only unpack common descriptor fields in case status is not Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_CONFIGURATION_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect payload size or malformed packet.");
            return configurationDescriptor; // Return an empty descriptor on failure
        }

        // Check configuration descriptor payload - Clause 7.2.2
        Deserializer des(commandPayload, commandPayloadLength);
        std::uint16_t descriptorCountsCount{ 0u };
        std::uint16_t descriptorCountsOffset{ 0u };
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> configurationDescriptor.objectName;
        des >> configurationDescriptor.localizedDescription;
        des >> descriptorCountsCount >> descriptorCountsOffset;

        // Check descriptor variable size
        constexpr size_t descriptorInfoSize = sizeof(DescriptorType) + sizeof(std::uint16_t);
        auto const descriptorCountsSize = descriptorInfoSize * descriptorCountsCount;
        if (des.remaining() < descriptorCountsSize) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect descriptor size in the payload.");
            return configurationDescriptor; // Return an empty descriptor on failure
        }

        // Compute deserializer offset for descriptor counts
        descriptorCountsOffset += PAYLOAD_BUFFER_OFFSET;

        // Set deserializer position
        if (descriptorCountsOffset < des.usedBytes())
        {
            ESP_LOGI("AEM", "Incorrect descriptor offset.");
            return configurationDescriptor; // Return an empty descriptor on failure
        }
        des.setPosition(descriptorCountsOffset);

        // Unpack descriptor remaining data
        for (auto index = 0u; index < descriptorCountsCount; ++index)
        {
            DescriptorType type;
            std::uint16_t count;

            des >> type >> count;
            
            // Ensure we do not overflow the fixed-size array
            /*if (index < descriptorCountsCount)
            {
                configurationDescriptor.descriptorCounts[index].descriptorType = type;
                configurationDescriptor.descriptorCounts[index].descriptorCount = count;
            }*/
        }
        //configurationDescriptor.descriptorCountsSize = descriptorCountsCount;

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_CONFIGURATION_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return configurationDescriptor;
}

AudioUnitDescriptor deserializeReadAudioUnitDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    AudioUnitDescriptor audioUnitDescriptor{};

    // Clause 7.4.5.2 says we should only unpack common descriptor fields in case status is not Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_AUDIO_UNIT_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect payload size or malformed packet.");
            return audioUnitDescriptor; // Return an empty descriptor on failure
        }

        // Check audio unit descriptor payload - Clause 7.2.3
        Deserializer des(commandPayload, commandPayloadLength);
        std::uint16_t samplingRatesOffset{ 0u };
        std::uint16_t numberOfSamplingRates{ 0u };
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> audioUnitDescriptor.objectName;
        des >> audioUnitDescriptor.localizedDescription >> audioUnitDescriptor.clockDomainIndex;
        des >> audioUnitDescriptor.numberOfStreamInputPorts >> audioUnitDescriptor.baseStreamInputPort;
        des >> audioUnitDescriptor.numberOfStreamOutputPorts >> audioUnitDescriptor.baseStreamOutputPort;
        des >> audioUnitDescriptor.numberOfExternalInputPorts >> audioUnitDescriptor.baseExternalInputPort;
        des >> audioUnitDescriptor.numberOfExternalOutputPorts >> audioUnitDescriptor.baseExternalOutputPort;
        des >> audioUnitDescriptor.numberOfInternalInputPorts >> audioUnitDescriptor.baseInternalInputPort;
        des >> audioUnitDescriptor.numberOfInternalOutputPorts >> audioUnitDescriptor.baseInternalOutputPort;
        des >> audioUnitDescriptor.numberOfControls >> audioUnitDescriptor.baseControl;
        des >> audioUnitDescriptor.numberOfSignalSelectors >> audioUnitDescriptor.baseSignalSelector;
        des >> audioUnitDescriptor.numberOfMixers >> audioUnitDescriptor.baseMixer;
        des >> audioUnitDescriptor.numberOfMatrices >> audioUnitDescriptor.baseMatrix;
        des >> audioUnitDescriptor.numberOfSplitters >> audioUnitDescriptor.baseSplitter;
        des >> audioUnitDescriptor.numberOfCombiners >> audioUnitDescriptor.baseCombiner;
        des >> audioUnitDescriptor.numberOfDemultiplexers >> audioUnitDescriptor.baseDemultiplexer;
        des >> audioUnitDescriptor.numberOfMultiplexers >> audioUnitDescriptor.baseMultiplexer;
        des >> audioUnitDescriptor.numberOfTranscoders >> audioUnitDescriptor.baseTranscoder;
        des >> audioUnitDescriptor.numberOfControlBlocks >> audioUnitDescriptor.baseControlBlock;
        des >> audioUnitDescriptor.currentSamplingRate >> samplingRatesOffset >> numberOfSamplingRates;

        // Check descriptor variable size
        auto const samplingRatesSize = sizeof(SamplingRate) * numberOfSamplingRates;
        if (des.remaining() < samplingRatesSize) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect sampling rates size in the payload.");
            return audioUnitDescriptor; // Return an empty descriptor on failure
        }

        // Compute deserializer offset for sampling rates
        samplingRatesOffset += PAYLOAD_BUFFER_OFFSET;

        // Set deserializer position
        if (samplingRatesOffset < des.usedBytes())
        {
            ESP_LOGI("AEM", "Incorrect sampling rates offset.");
            return audioUnitDescriptor; // Return an empty descriptor on failure
        }
        des.setPosition(samplingRatesOffset);

        // Let's loop over the sampling rates
        for (auto index = 0u; index < numberOfSamplingRates; ++index)
        {
            SamplingRate rate;
            des >> rate;

            if (index < numberOfSamplingRates)
            {
                audioUnitDescriptor.samplingRates[index] = rate; // Replace `insert` with array access
            }
        }

        //audioUnitDescriptor.samplingRatesSize = numberOfSamplingRates; // Set the actual size

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_AUDIO_UNIT_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return audioUnitDescriptor;
}

StreamDescriptor deserializeReadStreamDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    StreamDescriptor streamDescriptor{};

    // Clause 7.4.5.2 says we should only unpack common descriptor fields in case status is not Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_STREAM_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect payload size or malformed packet.");
            return streamDescriptor; // Return empty descriptor on failure
        }

        // Check stream descriptor payload - Clause 7.2.6
        Deserializer des(commandPayload, commandPayloadLength);
        std::uint16_t formatsOffset{ 0u };
        std::uint16_t numberOfFormats{ 0u };
        auto endDescriptorOffset = commandPayloadLength;
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> streamDescriptor.objectName;
        des >> streamDescriptor.localizedDescription >> streamDescriptor.clockDomainIndex >> streamDescriptor.streamFlags;
        des >> streamDescriptor.currentFormat >> formatsOffset >> numberOfFormats;
        des >> streamDescriptor.backupTalkerEntityID_0 >> streamDescriptor.backupTalkerUniqueID_0;
        des >> streamDescriptor.backupTalkerEntityID_1 >> streamDescriptor.backupTalkerUniqueID_1;
        des >> streamDescriptor.backupTalkerEntityID_2 >> streamDescriptor.backupTalkerUniqueID_2;
        des >> streamDescriptor.backedupTalkerEntityID >> streamDescriptor.backedupTalkerUnique;
        des >> streamDescriptor.avbInterfaceIndex >> streamDescriptor.bufferLength;

        // Compute deserializer offset for formats
        formatsOffset += PAYLOAD_BUFFER_OFFSET;

#ifdef ENABLE_ATDECC_FEATURE_REDUNDANCY
        // Check for redundant fields (AVnu Alliance 'Network Redundancy' extension)
        std::uint16_t redundantOffset{ 0u };
        std::uint16_t numberOfRedundantStreams{ 0u };
        auto const remainingBytesBeforeFormats = formatsOffset - des.usedBytes();
        if (remainingBytesBeforeFormats >= (sizeof(redundantOffset) + sizeof(numberOfRedundantStreams)))
        {
            des >> redundantOffset >> numberOfRedundantStreams;
            redundantOffset += PAYLOAD_BUFFER_OFFSET;
            endDescriptorOffset = redundantOffset;
        }
#endif // ENABLE_ATDECC_FEATURE_REDUNDANCY

        auto const staticPartEndOffset = des.usedBytes();

        // Check descriptor variable size
        constexpr size_t formatInfoSize = sizeof(std::uint64_t);
        auto const formatsSize = formatInfoSize * numberOfFormats;
        if (formatsSize > static_cast<decltype(formatsSize)>(endDescriptorOffset - formatsOffset))
        {
            ESP_LOGI("AEM", "Incorrect formats size in the payload.");
            return streamDescriptor; // Return empty descriptor on failure
        }
        if (formatsOffset < staticPartEndOffset)
        {
            ESP_LOGI("AEM", "Formats offset is smaller than the static part.");
            return streamDescriptor; // Return empty descriptor on failure
        }

        // Read formats
        des.setPosition(formatsOffset);
        for (auto index = 0u; index < numberOfFormats; ++index)
        {
            StreamFormat format{};
            des >> format;
            if (index < numberOfFormats)
            {
                streamDescriptor.formats[index] = format; // Replace `insert` with array access
            }
        }

        //streamDescriptor.formatsSize = numberOfFormats; // Set the actual size

#ifdef ENABLE_ATDECC_FEATURE_REDUNDANCY
        // Read redundant streams association
        if (redundantOffset > 0)
        {
            if (redundantOffset < staticPartEndOffset)
            {
                ESP_LOGI("AEM", "Redundant offset is smaller than the static part.");
                return streamDescriptor; // Return empty descriptor on failure
            }

            des.setPosition(redundantOffset);

            for (auto index = 0u; index < numberOfRedundantStreams; ++index)
            {
                StreamIndex redundantStreamIndex;
                des >> redundantStreamIndex;
                if (index < MAX_REDUNDANT_STREAMS)
                {
                    streamDescriptor.redundantStreams[index] = redundantStreamIndex; // Replace `insert` with array access
                }
            }

            streamDescriptor.redundantStreamsSize = numberOfRedundantStreams; // Set the actual size
        }
#endif // ENABLE_ATDECC_FEATURE_REDUNDANCY

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_STREAM_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return streamDescriptor;
}

JackDescriptor deserializeReadJackDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    JackDescriptor jackDescriptor{};

    // Clause 7.4.5.2 says we should only unpack common descriptor fields in case status is not Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_JACK_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect payload size for JackDescriptor.");
            return jackDescriptor; // Return empty descriptor on error
        }

        // Check jack descriptor payload - Clause 7.2.7
        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> jackDescriptor.objectName;
        des >> jackDescriptor.localizedDescription;
        des >> jackDescriptor.jackFlags >> jackDescriptor.jackType;
        des >> jackDescriptor.numberOfControls >> jackDescriptor.baseControl;

        if (des.usedBytes() != AECP_AEM_READ_JACK_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than specified in protocol constant for JackDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_JACK_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return jackDescriptor;
}

AvbInterfaceDescriptor deserializeReadAvbInterfaceDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    AvbInterfaceDescriptor avbInterfaceDescriptor{};

    // Clause 7.4.5.2 says we should only unpack common descriptor fields in case status is not Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_AVB_INTERFACE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect payload size for AvbInterfaceDescriptor.");
            return avbInterfaceDescriptor; // Return empty descriptor on error
        }

        // Check avb interface descriptor payload - Clause 7.2.8
        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> avbInterfaceDescriptor.objectName;
        des >> avbInterfaceDescriptor.localizedDescription;
        //des >> avbInterfaceDescriptor.macAddress;
        des >> avbInterfaceDescriptor.interfaceFlags;
        des >> avbInterfaceDescriptor.clockIdentity;
        des >> avbInterfaceDescriptor.priority1 >> avbInterfaceDescriptor.clockClass;
        des >> avbInterfaceDescriptor.offsetScaledLogVariance >> avbInterfaceDescriptor.clockAccuracy;
        des >> avbInterfaceDescriptor.priority2 >> avbInterfaceDescriptor.domainNumber;
        des >> avbInterfaceDescriptor.logSyncInterval >> avbInterfaceDescriptor.logAnnounceInterval >> avbInterfaceDescriptor.logPDelayInterval;
        des >> avbInterfaceDescriptor.portNumber;

        if (des.usedBytes() != AECP_AEM_READ_AVB_INTERFACE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than specified in protocol constant for AvbInterfaceDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_AVB_INTERFACE_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return avbInterfaceDescriptor;
}

ClockSourceDescriptor deserializeReadClockSourceDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    ClockSourceDescriptor clockSourceDescriptor{};

    // Clause 7.4.5.2 says we should only unpack common descriptor fields in case status is not Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_CLOCK_SOURCE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect payload size for ClockSourceDescriptor.");
            return clockSourceDescriptor; // Return empty descriptor on error
        }

        // Check clock source descriptor payload - Clause 7.2.9
        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> clockSourceDescriptor.objectName;
        des >> clockSourceDescriptor.localizedDescription;
        des >> clockSourceDescriptor.clockSourceFlags >> clockSourceDescriptor.clockSourceType;
        des >> clockSourceDescriptor.clockSourceIdentifier;
        des >> clockSourceDescriptor.clockSourceLocationType >> clockSourceDescriptor.clockSourceLocationIndex;

        if (des.usedBytes() != AECP_AEM_READ_CLOCK_SOURCE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than specified in protocol constant for ClockSourceDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_CLOCK_SOURCE_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return clockSourceDescriptor;
}

MemoryObjectDescriptor deserializeReadMemoryObjectDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    MemoryObjectDescriptor memoryObjectDescriptor{};

    // Clause 7.4.5.2 says we should only unpack common descriptor fields in case status is not Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_MEMORY_OBJECT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE) // Malformed packet
        {
            ESP_LOGI("AEM", "Incorrect payload size for MemoryObjectDescriptor.");
            return memoryObjectDescriptor; // Return empty descriptor on error
        }

        // Check memory object descriptor payload - Clause 7.2.10
        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> memoryObjectDescriptor.objectName;
        des >> memoryObjectDescriptor.localizedDescription;
        des >> memoryObjectDescriptor.memoryObjectType;
        des >> memoryObjectDescriptor.targetDescriptorType >> memoryObjectDescriptor.targetDescriptorIndex;
        des >> memoryObjectDescriptor.startAddress >> memoryObjectDescriptor.maximumLength >> memoryObjectDescriptor.length;

        // TODO: Unpack the new field added in the corrigendum document (but preserve compatibility with older devices if the field is not present!)

        if (des.usedBytes() != AECP_AEM_READ_MEMORY_OBJECT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than specified in protocol constant for MemoryObjectDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_MEMORY_OBJECT_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return memoryObjectDescriptor;
}

LocaleDescriptor deserializeReadLocaleDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    LocaleDescriptor localeDescriptor{};

    // Unpack only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_LOCALE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE) 
        {
            ESP_LOGI("AEM", "Incorrect payload size for LocaleDescriptor.");
            return localeDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> localeDescriptor.localeID;
        des >> localeDescriptor.numberOfStringDescriptors >> localeDescriptor.baseStringDescriptorIndex;

        if (des.usedBytes() != AECP_AEM_READ_LOCALE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than expected in LocaleDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in LocaleDescriptor response: %zu", des.remaining());
        }
    }

    return localeDescriptor;
}

StringsDescriptor deserializeReadStringsDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    StringsDescriptor stringsDescriptor{};

    // Unpack only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_STRINGS_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE) 
        {
            ESP_LOGI("AEM", "Incorrect payload size for StringsDescriptor.");
            return stringsDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header

        for (auto strIndex = 0u; strIndex < 7; ++strIndex)
        {
            des >> stringsDescriptor.strings[strIndex];
        }

        if (des.usedBytes() != AECP_AEM_READ_STRINGS_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than expected in StringsDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in StringsDescriptor response: %zu", des.remaining());
        }
    }

    return stringsDescriptor;
}

StreamPortDescriptor deserializeReadStreamPortDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    StreamPortDescriptor streamPortDescriptor{};

    // Unpack only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_STREAM_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE) 
        {
            ESP_LOGI("AEM", "Incorrect payload size for StreamPortDescriptor.");
            return streamPortDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> streamPortDescriptor.clockDomainIndex >> streamPortDescriptor.portFlags;
        des >> streamPortDescriptor.numberOfControls >> streamPortDescriptor.baseControl;
        des >> streamPortDescriptor.numberOfClusters >> streamPortDescriptor.baseCluster;
        des >> streamPortDescriptor.numberOfMaps >> streamPortDescriptor.baseMap;

        if (des.usedBytes() != AECP_AEM_READ_STREAM_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than expected in StreamPortDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in StreamPortDescriptor response: %zu", des.remaining());
        }
    }

    return streamPortDescriptor;
}

ExternalPortDescriptor deserializeReadExternalPortDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    ExternalPortDescriptor externalPortDescriptor{};

    // Unpack only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_EXTERNAL_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Incorrect payload size for ExternalPortDescriptor.");
            return externalPortDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> externalPortDescriptor.clockDomainIndex >> externalPortDescriptor.portFlags;
        des >> externalPortDescriptor.numberOfControls >> externalPortDescriptor.baseControl;
        des >> externalPortDescriptor.signalType >> externalPortDescriptor.signalIndex >> externalPortDescriptor.signalOutput;
        des >> externalPortDescriptor.blockLatency >> externalPortDescriptor.jackIndex;

        if (des.usedBytes() != AECP_AEM_READ_EXTERNAL_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than expected in ExternalPortDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in ExternalPortDescriptor response: %zu", des.remaining());
        }
    }

    return externalPortDescriptor;
}

InternalPortDescriptor deserializeReadInternalPortDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    InternalPortDescriptor internalPortDescriptor{};

    // Unpack only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_INTERNAL_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Incorrect payload size for InternalPortDescriptor.");
            return internalPortDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> internalPortDescriptor.clockDomainIndex >> internalPortDescriptor.portFlags;
        des >> internalPortDescriptor.numberOfControls >> internalPortDescriptor.baseControl;
        des >> internalPortDescriptor.signalType >> internalPortDescriptor.signalIndex >> internalPortDescriptor.signalOutput;
        des >> internalPortDescriptor.blockLatency >> internalPortDescriptor.internalIndex;

        if (des.usedBytes() != AECP_AEM_READ_INTERNAL_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than expected in InternalPortDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in InternalPortDescriptor response: %zu", des.remaining());
        }
    }

    return internalPortDescriptor;
}

AudioClusterDescriptor deserializeReadAudioClusterDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    AudioClusterDescriptor audioClusterDescriptor{};

    // Unpack only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_AUDIO_CLUSTER_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Incorrect payload size for AudioClusterDescriptor.");
            return audioClusterDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> audioClusterDescriptor.objectName;
        des >> audioClusterDescriptor.localizedDescription;
        des >> audioClusterDescriptor.signalType >> audioClusterDescriptor.signalIndex >> audioClusterDescriptor.signalOutput;
        des >> audioClusterDescriptor.pathLatency >> audioClusterDescriptor.blockLatency;
        des >> audioClusterDescriptor.channelCount >> audioClusterDescriptor.format;

        if (des.usedBytes() != AECP_AEM_READ_AUDIO_CLUSTER_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used more bytes than expected in AudioClusterDescriptor.");
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in AudioClusterDescriptor response: %zu", des.remaining());
        }
    }

    return audioClusterDescriptor;
}

AudioMapDescriptor deserializeReadAudioMapDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    AudioMapDescriptor audioMapDescriptor{};

    // Unpack only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_AUDIO_MAP_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE)
        {
            ESP_LOGI("AEM", "Incorrect payload size for AudioMapDescriptor.");
            return audioMapDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        std::uint16_t mappingsOffset{ 0u };
        std::uint16_t numberOfMappings{ 0u };
        des.setPosition(commonSize); // Skip already unpacked common header
        des >> mappingsOffset >> numberOfMappings;

        // Check descriptor variable size
        auto const mappingsSize = AudioMapping::size() * numberOfMappings;
        if (des.remaining() < mappingsSize)
        {
            ESP_LOGI("AEM", "Malformed packet, remaining bytes less than expected for mappings.");
            return audioMapDescriptor;
        }

        // Compute deserializer offset for mappings
        mappingsOffset += PAYLOAD_BUFFER_OFFSET;

        // Set deserializer position
        if (mappingsOffset < des.usedBytes())
        {
            ESP_LOGI("AEM", "Incorrect offset for mappings.");
            return audioMapDescriptor;
        }
        des.setPosition(mappingsOffset);

        // Unpack the mappings
        for (auto index = 0u; index < numberOfMappings; ++index)
        {
            AudioMapping mapping;
            des >> mapping.streamIndex >> mapping.streamChannel >> mapping.clusterOffset >> mapping.clusterChannel;
            //audioMapDescriptor.mappings.push_back(mapping);
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_AUDIO_MAP_DESCRIPTOR response: %zu", des.remaining());
        }
    }

    return audioMapDescriptor;
}

ControlDescriptor deserializeReadControlDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    //static auto s_Dispatch = std::unordered_map<ControlValueType::Type, std::function<std::tuple<ControlValues, ControlValues>(Deserializer&, std::uint16_t)>>{};

    /*if (s_Dispatch.empty())
    {
        // Create the dispatch table
        createUnpackFullControlValuesDispatchTable(s_Dispatch);
    }*/

    ControlDescriptor controlDescriptor{};

    // Unpack common descriptor fields only if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_CONTROL_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE)
        {
            ESP_LOGI("AEM", "Incorrect payload size for ControlDescriptor.");
            return controlDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        std::uint16_t numberOfValues{ 0u };
        std::uint16_t valuesOffset{ 0u };

        des.setPosition(commonSize); // Skip already unpacked common header
        des >> controlDescriptor.objectName >> controlDescriptor.localizedDescription;
        des >> controlDescriptor.blockLatency >> controlDescriptor.controlLatency >> controlDescriptor.controlDomain;
        des >> controlDescriptor.controlValueType >> controlDescriptor.controlType >> controlDescriptor.resetTime;
        des >> valuesOffset >> numberOfValues;
        des >> controlDescriptor.signalType >> controlDescriptor.signalIndex >> controlDescriptor.signalOutput;

        // Compute deserializer offset for values (Clause 7.2.22 states values_offset is from the base of the descriptor)
        valuesOffset += PAYLOAD_BUFFER_OFFSET;

        // Set deserializer position
        if (valuesOffset < des.usedBytes())
        {
            ESP_LOGI("AEM", "Invalid values offset for ControlDescriptor.");
            return controlDescriptor;
        }
        des.setPosition(valuesOffset);

        // Unpack Control Values based on ControlValueType
        auto valueType = controlDescriptor.controlValueType.getType();
        /*if (auto const& it = s_Dispatch.find(valueType); it != s_Dispatch.end())
        {
            auto [valuesStatic, valuesDynamic] = it->second(des, numberOfValues);
            controlDescriptor.valuesStatic = std::move(valuesStatic);
            controlDescriptor.valuesDynamic = std::move(valuesDynamic);
        }
        else
        {
            ESP_LOGI("AEM", "Unsupported ControlValueType for ControlDescriptor response: %d", (int)valueType);
            return controlDescriptor;
        }*/

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_CONTROL_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return controlDescriptor;
}

ClockDomainDescriptor deserializeReadClockDomainDescriptorResponse(AemAecpdu::Payload const& payload, size_t const commonSize, AecpStatus const status)
{
    ClockDomainDescriptor clockDomainDescriptor{};

    // Only unpack common descriptor fields if status is Success
    if (status == AecpStatus::SUCCESS)
    {
        auto* const commandPayload = payload.first;
        auto const commandPayloadLength = payload.second;

        if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_READ_CLOCK_DOMAIN_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE)
        {
            ESP_LOGI("AEM", "Incorrect payload size for ClockDomainDescriptor.");
            return clockDomainDescriptor;
        }

        Deserializer des(commandPayload, commandPayloadLength);
        std::uint16_t clockSourcesOffset{ 0u };
        std::uint16_t numberOfClockSources{ 0u };

        des.setPosition(commonSize); // Skip already unpacked common header
        des >> clockDomainDescriptor.objectName;
        des >> clockDomainDescriptor.localizedDescription;
        des >> clockDomainDescriptor.clockSourceIndex;
        des >> clockSourcesOffset >> numberOfClockSources;

        // Check descriptor variable size
        auto const clockSourcesSize = sizeof(ClockSourceIndex) * numberOfClockSources;
        if (des.remaining() < clockSourcesSize)
        {
            ESP_LOGI("AEM", "Malformed packet: ClockSources size mismatch.");
            return clockDomainDescriptor;
        }

        // Compute deserializer offset for clock sources (add the PayloadBufferOffset)
        clockSourcesOffset += PAYLOAD_BUFFER_OFFSET;

        // Set deserializer position
        if (clockSourcesOffset < des.usedBytes())
        {
            ESP_LOGI("AEM", "Invalid clock sources offset for ClockDomainDescriptor.");
            return clockDomainDescriptor;
        }
        des.setPosition(clockSourcesOffset);

        // Loop over the clock sources
        for (auto index = 0u; index < numberOfClockSources; ++index)
        {
            ClockSourceIndex clockSourceIndex;
            des >> clockSourceIndex;
            clockDomainDescriptor.clockSources.push_back(clockSourceIndex);
        }

        if (des.remaining() != 0)
        {
            ESP_LOGI("AEM", "Warning: Remaining bytes in buffer for READ_CLOCK_DOMAIN_DESCRIPTOR RESPONSE: %zu", des.remaining());
        }
    }

    return clockDomainDescriptor;
}

/** SET_CONFIGURATION Command - Clause 7.4.7.1 */
Serializer<AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE> serializeSetConfigurationCommand(ConfigurationIndex const configurationIndex)
{
    Serializer<AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE> ser;
    std::uint16_t const reserved{0u};

    ser << reserved << configurationIndex;

    ESP_LOGI("AEM", "SET_CONFIGURATION Command serialized with ConfigurationIndex: %u", configurationIndex);

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for SET_CONFIGURATION Command.");
    }

    return ser;
}

std::tuple<ConfigurationIndex> deserializeSetConfigurationCommand(const AemAecpdu::Payload& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Malformed SET_CONFIGURATION Command payload.");
        return std::make_tuple(0);
    }

    Deserializer des(commandPayload, commandPayloadLength);
    std::uint16_t reserved{0u};
    ConfigurationIndex configurationIndex{0u};

    des >> reserved >> configurationIndex;

    if (des.usedBytes() != AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for SET_CONFIGURATION Command deserialization.");
    }

    return std::make_tuple(configurationIndex);
}

/** SET_CONFIGURATION Response - Clause 7.4.7.1 */
Serializer<AECP_AEM_SET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE> serializeSetConfigurationResponse(ConfigurationIndex const configurationIndex)
{
    // Same as SET_CONFIGURATION Command
    ESP_LOGI("AEM", "Serializing SET_CONFIGURATION Response with ConfigurationIndex: %u", configurationIndex);
    return serializeSetConfigurationCommand(configurationIndex);
}

std::tuple<ConfigurationIndex> deserializeSetConfigurationResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
    ESP_LOGI("AEM", "Deserializing SET_CONFIGURATION Response.");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE, AECP_AEM_SET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetConfigurationCommand(payload);
}

/** GET_CONFIGURATION Command - Clause 7.4.8.1 */
// No payload

/** GET_CONFIGURATION Response - Clause 7.4.8.2 */
Serializer<AECP_AEM_GET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE> serializeGetConfigurationResponse(ConfigurationIndex const configurationIndex)
{
    ESP_LOGI("AEM", "Serializing GET_CONFIGURATION Response with ConfigurationIndex: %u", configurationIndex);
    return serializeSetConfigurationCommand(configurationIndex);
}

std::tuple<ConfigurationIndex> deserializeGetConfigurationResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
    ESP_LOGI("AEM", "Deserializing GET_CONFIGURATION Response.");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetConfigurationCommand(payload);
}

/** SET_STREAM_FORMAT Command - Clause 7.4.9.1 */
Serializer<AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE> serializeSetStreamFormatCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamFormat const streamFormat)
{
    Serializer<AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;
    ser << streamFormat;

    ESP_LOGI("AEM", "SET_STREAM_FORMAT Command serialized for DescriptorType: %hu, DescriptorIndex: %u, StreamFormat: %llu", (uint16_t)descriptorType, descriptorIndex, streamFormat.getValue());

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for SET_STREAM_FORMAT Command.");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, StreamFormat> deserializeSetStreamFormatCommand(AemAecpdu::Payload const& payload)
{
	auto* const commandPayload = payload.first;
	auto const commandPayloadLength = payload.second;

	if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE) // Malformed packet
		ESP_LOGI("AEM", "Incorrect Payload Size");

	// Check payload
	Deserializer des(commandPayload, commandPayloadLength);
	DescriptorType descriptorType{ DescriptorType::Invalid };
	DescriptorIndex descriptorIndex{ 0u };
	StreamFormat streamFormat{};

	des >> descriptorType >> descriptorIndex;
	des >> streamFormat;
	
	if (des.usedBytes() != AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_STREAM_FORMAT Command deserialization.");
    }

	return std::make_tuple(descriptorType, descriptorIndex, streamFormat);
}


/** GET_STREAM_FORMAT Command - Clause 7.4.10.1 */
Serializer<AECP_AEM_GET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE> serializeGetStreamFormatCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_GET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    ESP_LOGI("AEM", "GET_STREAM_FORMAT Command serialized for DescriptorType: %hu, DescriptorIndex: %u", (uint16_t)descriptorType, descriptorIndex);

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_STREAM_FORMAT Command.");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeGetStreamFormatCommand(const AemAecpdu::Payload& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Malformed GET_STREAM_FORMAT Command payload.");
        return std::make_tuple(DescriptorType::Invalid, 0);
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{DescriptorType::Invalid};
    DescriptorIndex descriptorIndex{0};

    des >> descriptorType >> descriptorIndex;

    if (des.usedBytes() != AECP_AEM_GET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_STREAM_FORMAT Command deserialization.");
    }

    return std::make_tuple(descriptorType, descriptorIndex);
}

Serializer<AECP_AEM_SET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE> serializeSetStreamFormatResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamFormat const streamFormat)
{
	// Same as SET_STREAM_FORMAT Command
	static_assert(AECP_AEM_SET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE == AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE, "SET_STREAM_FORMAT Response no longer the same as SET_STREAM_FORMAT Command");
	return serializeSetStreamFormatCommand(descriptorType, descriptorIndex, streamFormat);
}

std::tuple<DescriptorType, DescriptorIndex, StreamFormat> deserializeSetStreamFormatResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
	// Same as SET_STREAM_FORMAT Command
	static_assert(AECP_AEM_SET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE == AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE, "SET_STREAM_FORMAT Response no longer the same as SET_STREAM_FORMAT Command");
	checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE, AECP_AEM_SET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE);
	return deserializeSetStreamFormatCommand(payload);
}

/** GET_STREAM_FORMAT Response - Clause 7.4.10.2 */
Serializer<AECP_AEM_GET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE> serializeGetStreamFormatResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamFormat const streamFormat)
{
    // Same as SET_STREAM_FORMAT Command
    ESP_LOGI("AEM", "Serializing GET_STREAM_FORMAT Response for DescriptorType: %hu, DescriptorIndex: %u, StreamFormat: %llu", (uint16_t)descriptorType, descriptorIndex, streamFormat.getValue());
    return serializeSetStreamFormatCommand(descriptorType, descriptorIndex, streamFormat);
}

std::tuple<DescriptorType, DescriptorIndex, StreamFormat> deserializeGetStreamFormatResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
    ESP_LOGI("AEM", "Deserializing GET_STREAM_FORMAT Response.");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetStreamFormatCommand(payload);
}

/** SET_STREAM_INFO Command - Clause 7.4.15.1 */
Serializer<AECP_AEM_SET_STREAM_INFO_COMMAND_PAYLOAD_SIZE> serializeSetStreamInfoCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const StreamInfo& streamInfo)
{
    Serializer<AECP_AEM_SET_STREAM_INFO_COMMAND_PAYLOAD_SIZE> ser;
    std::uint8_t const reserved{0u};
    std::uint16_t const reserved2{0u};

    ser << descriptorType << descriptorIndex;
    ser << streamInfo.streamInfoFlags;
    ser << streamInfo.streamFormat;
    ser << streamInfo.streamID;
    ser << streamInfo.msrpAccumulatedLatency;
    ser.packBuffer(streamInfo.streamDestMac.data(), streamInfo.streamDestMac.size());
    ser << streamInfo.msrpFailureCode;
    ser << reserved;
    ser << streamInfo.msrpFailureBridgeID;
    ser << streamInfo.streamVlanID;
    ser << reserved2;

    ESP_LOGI("AEM", "SET_STREAM_INFO Command serialized for DescriptorType: %hu, DescriptorIndex: %u", (uint16_t)descriptorType, descriptorIndex);

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for SET_STREAM_INFO Command.");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, StreamInfo> deserializeSetStreamInfoCommand(const AemAecpdu::Payload& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_STREAM_INFO_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Malformed SET_STREAM_INFO Command payload.");
        return std::make_tuple(DescriptorType::Invalid, 0, StreamInfo{});
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{DescriptorType::Invalid};
    DescriptorIndex descriptorIndex{0};
    StreamInfo streamInfo{};
    std::uint8_t reserved{0u};
    std::uint16_t reserved2{0u};

    des >> descriptorType >> descriptorIndex;
    des >> streamInfo.streamInfoFlags;
    des >> streamInfo.streamFormat;
    des >> streamInfo.streamID;
    des >> streamInfo.msrpAccumulatedLatency;
    //des.unpackBuffer(streamInfo.streamDestMac.data(), streamInfo.streamDestMac.size());
    des >> streamInfo.msrpFailureCode;
    des >> reserved;
    des >> streamInfo.msrpFailureBridgeID;
    des >> streamInfo.streamVlanID;
    des >> reserved2;

    if (des.usedBytes() != AECP_AEM_SET_STREAM_INFO_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for SET_STREAM_INFO Command deserialization.");
    }

    return std::make_tuple(descriptorType, descriptorIndex, streamInfo);
}

/** SET_STREAM_INFO Response - Clause 7.4.15.1 */
Serializer<AECP_AEM_SET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE> serializeSetStreamInfoResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamInfo const& streamInfo)
{
    // Same as SET_STREAM_INFO Command
    ESP_LOGI("AEM", "Serializing SET_STREAM_INFO Response for DescriptorType: %hu, DescriptorIndex: %u", (uint16_t)descriptorType, descriptorIndex);
    return serializeSetStreamInfoCommand(descriptorType, descriptorIndex, streamInfo);
}

std::tuple<DescriptorType, DescriptorIndex, StreamInfo> deserializeSetStreamInfoResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
    // Same as SET_STREAM_INFO Command
    ESP_LOGI("AEM", "Deserializing SET_STREAM_INFO Response.");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_STREAM_INFO_COMMAND_PAYLOAD_SIZE, AECP_AEM_SET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetStreamInfoCommand(payload);
}

/** GET_STREAM_INFO Command - Clause 7.4.16.1 */
Serializer<AECP_AEM_GET_STREAM_INFO_COMMAND_PAYLOAD_SIZE> serializeGetStreamInfoCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_GET_STREAM_INFO_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    ESP_LOGI("AEM", "GET_STREAM_INFO Command serialized for DescriptorType: %hu, DescriptorIndex: %u", (uint16_t)descriptorType, descriptorIndex);

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_STREAM_INFO Command.");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeGetStreamInfoCommand(const AemAecpdu::Payload& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_STREAM_INFO_COMMAND_PAYLOAD_SIZE) // Malformed packet
    {
        ESP_LOGI("AEM", "Malformed GET_STREAM_INFO Command payload.");
        return std::make_tuple(DescriptorType::Invalid, 0);
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{DescriptorType::Invalid};
    DescriptorIndex descriptorIndex{0};

    des >> descriptorType >> descriptorIndex;

    if (des.usedBytes() != AECP_AEM_GET_STREAM_INFO_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_STREAM_INFO Command deserialization.");
    }

    return std::make_tuple(descriptorType, descriptorIndex);
}

/** GET_STREAM_INFO Response - Clause 7.4.16.2 */
Serializer<AECP_AEM_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE> serializeGetStreamInfoResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamInfo const& streamInfo)
{
    Serializer<AECP_AEM_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE> ser;
    std::uint8_t const reserved{0u};
    std::uint16_t const reserved2{0u};

    ser << descriptorType << descriptorIndex;
    ser << streamInfo.streamInfoFlags;
    ser << streamInfo.streamFormat;
    ser << streamInfo.streamID;
    ser << streamInfo.msrpAccumulatedLatency;
    //ser.packBuffer(streamInfo.streamDestMac.data(), streamInfo.streamDestMac.size());
    ser << streamInfo.msrpFailureCode;
    ser << reserved;
    ser << streamInfo.msrpFailureBridgeID;
    ser << streamInfo.streamVlanID;
    ser << reserved2;

    /*if (streamInfo.streamInfoFlagsEx && streamInfo.probingStatus && streamInfo.acmpStatus)
    {
        std::uint8_t reserved3 = 0u;
        std::uint16_t reserved4 = 0u;

        ser << *streamInfo.streamInfoFlagsEx
            << static_cast<std::uint8_t>((to_integral(*streamInfo.probingStatus) << 5) & 0xe0) | ((*streamInfo.acmpStatus).getValue() & 0x1f))
            << reserved3 << reserved4;

        if (ser.usedBytes() != AECP_AEM_MILAN_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for MILAN GET_STREAM_INFO Response serialization.");
        }
    }
    else
    {*/
        if (ser.usedBytes() != AECP_AEM_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_STREAM_INFO Response serialization.");
        }
    //}

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, StreamInfo> deserializeGetStreamInfoResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_GET_STREAM_INFO_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE);

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{DescriptorType::Invalid};
    DescriptorIndex descriptorIndex{0};
    StreamInfo streamInfo{};
    std::uint8_t reserved{0u};
    std::uint16_t reserved2{0u};

    des >> descriptorType >> descriptorIndex;
    des >> streamInfo.streamInfoFlags;
    des >> streamInfo.streamFormat;
    des >> streamInfo.streamID;
    des >> streamInfo.msrpAccumulatedLatency;
    //des.unpackBuffer(streamInfo.streamDestMac.data(), streamInfo.streamDestMac.size());
    des >> streamInfo.msrpFailureCode;
    des >> reserved;
    des >> streamInfo.msrpFailureBridgeID;
    des >> streamInfo.streamVlanID;
    des >> reserved2;

    if (commandPayloadLength >= AECP_AEM_MILAN_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE)
    {
        StreamInfoFlagsEx streamInfoFlagsEx{};
        std::uint8_t probing_acmp_status{0u};
        std::uint8_t reserved3{0u};
        std::uint16_t reserved4{0u};

        des >> streamInfoFlagsEx >> probing_acmp_status >> reserved3 >> reserved4;

        /*streamInfo.streamInfoFlagsEx = streamInfoFlagsEx;
        streamInfo.probingStatus = static_cast<ProbingStatus>((probing_acmp_status & 0xe0) >> 5);
        streamInfo.acmpStatus = static_cast<AcmpStatus>(probing_acmp_status & 0x1f);*/

        if (des.usedBytes() != AECP_AEM_MILAN_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for MILAN GET_STREAM_INFO Response deserialization.");
        }
    }
    else
    {
        if (des.usedBytes() != AECP_AEM_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE)
        {
            ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_STREAM_INFO Response deserialization.");
        }
    }

    return std::make_tuple(descriptorType, descriptorIndex, streamInfo);
}

/** SET_NAME Command - Clause 7.4.17.1 */
Serializer<AECP_AEM_SET_NAME_COMMAND_PAYLOAD_SIZE> serializeSetNameCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex, AtdeccFixedString const& name)
{
    Serializer<AECP_AEM_SET_NAME_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;
    ser << nameIndex << configurationIndex;
    ser << name;

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for SET_NAME Command serialization.");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex, AtdeccFixedString> deserializeSetNameCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_NAME_COMMAND_PAYLOAD_SIZE) // Malformed packet
    {
        ESP_LOGI("AEM", "Malformed SET_NAME Command payload.");
        return std::make_tuple(DescriptorType::Invalid, 0, 0, 0, AtdeccFixedString{});
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{DescriptorType::Invalid};
    DescriptorIndex descriptorIndex{0};
    std::uint16_t nameIndex{0};
    ConfigurationIndex configurationIndex{0};
    AtdeccFixedString name{};

    des >> descriptorType >> descriptorIndex;
    des >> nameIndex >> configurationIndex >> name;

    if (des.usedBytes() != AECP_AEM_SET_NAME_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Warning: Used more bytes than specified in protocol constant for SET_NAME Command deserialization.");
    }

    return std::make_tuple(descriptorType, descriptorIndex, nameIndex, configurationIndex, name);
}

/** SET_NAME Response - Clause 7.4.17.1 */
Serializer<AECP_AEM_SET_NAME_RESPONSE_PAYLOAD_SIZE> serializeSetNameResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex, AtdeccFixedString const& name)
{
    // Same as SET_NAME Command
    return serializeSetNameCommand(descriptorType, descriptorIndex, nameIndex, configurationIndex, name);
}

std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex, AtdeccFixedString> deserializeSetNameResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_NAME_COMMAND_PAYLOAD_SIZE, AECP_AEM_SET_NAME_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetNameCommand(payload);
}

/** GET_NAME Command - Clause 7.4.18.1 */
Serializer<AECP_AEM_GET_NAME_COMMAND_PAYLOAD_SIZE> serializeGetNameCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex)
{
    Serializer<AECP_AEM_GET_NAME_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;
    ser << nameIndex << configurationIndex;

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "Warning: Used bytes do not match the protocol constant for GET_NAME Command serialization.");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex> deserializeGetNameCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_NAME_COMMAND_PAYLOAD_SIZE) // Malformed packet
    {
        ESP_LOGI("AEM", "Malformed GET_NAME Command payload.");
        return std::make_tuple(DescriptorType::Invalid, 0, 0, 0);
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{DescriptorType::Invalid};
    DescriptorIndex descriptorIndex{0};
    std::uint16_t nameIndex{0};
    ConfigurationIndex configurationIndex{0};

    des >> descriptorType >> descriptorIndex;
    des >> nameIndex >> configurationIndex;

    if (des.usedBytes() != AECP_AEM_GET_NAME_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "Warning: Used more bytes than specified in protocol constant for GET_NAME Command deserialization.");
    }

    return std::make_tuple(descriptorType, descriptorIndex, nameIndex, configurationIndex);
}

/** GET_NAME Response - Clause 7.4.18.2 */
Serializer<AECP_AEM_GET_NAME_RESPONSE_PAYLOAD_SIZE> serializeGetNameResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex, AtdeccFixedString const& name)
{
    // Same as SET_NAME Command
    return serializeSetNameCommand(descriptorType, descriptorIndex, nameIndex, configurationIndex, name);
}

std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex, AtdeccFixedString> deserializeGetNameResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_GET_NAME_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_NAME_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetNameCommand(payload);
}

/** SET_ASSOCIATION_ID Command - Clause 7.4.19.1 */
Serializer<AECP_AEM_SET_ASSOCIATION_ID_COMMAND_PAYLOAD_SIZE> serializeSetAssociationIDCommand(UniqueIdentifier const associationID)
{
    Serializer<AECP_AEM_SET_ASSOCIATION_ID_COMMAND_PAYLOAD_SIZE> ser;

    ser << associationID;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGE("AEM", "Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<UniqueIdentifier> deserializeSetAssociationIDCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_ASSOCIATION_ID_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Malformed packet in deserializeSetAssociationIDCommand");
        return {};
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    UniqueIdentifier associationID{};

    des >> associationID;

    if (des.usedBytes() != AECP_AEM_GET_ASSOCIATION_ID_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Used more bytes than specified in protocol constant");
    }

    return std::make_tuple(associationID);
}

/** SET_ASSOCIATION_ID Response - Clause 7.4.19.1 */
Serializer<AECP_AEM_SET_ASSOCIATION_ID_RESPONSE_PAYLOAD_SIZE> serializeSetAssociationIDResponse(UniqueIdentifier const associationID)
{
    // Same as SET_ASSOCIATION_ID Command
    return serializeSetAssociationIDCommand(associationID);
}

std::tuple<UniqueIdentifier> deserializeSetAssociationIDResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    if (status != AemCommandStatus::Success) {
        ESP_LOGI("AEM", "SET_ASSOCIATION_ID command not successful");
        return {};
    }

    return deserializeSetAssociationIDCommand(payload);
}

/** GET_ASSOCIATION_ID Response - Clause 7.4.20.2 */
Serializer<AECP_AEM_GET_ASSOCIATION_ID_RESPONSE_PAYLOAD_SIZE> serializeGetAssociationIDResponse(UniqueIdentifier const associationID)
{
    // Same as SET_ASSOCIATION_ID Command
    return serializeSetAssociationIDCommand(associationID);
}

std::tuple<UniqueIdentifier> deserializeGetAssociationIDResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    if (status != AemCommandStatus::Success) {
        ESP_LOGI("AEM", "GET_ASSOCIATION_ID command not successful");
        return {};
    }

    return deserializeSetAssociationIDCommand(payload);
}

/** SET_SAMPLING_RATE Command - Clause 7.4.21.1 */
Serializer<AECP_AEM_SET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE> serializeSetSamplingRateCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, SamplingRate const samplingRate)
{
    Serializer<AECP_AEM_SET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;
    ser << samplingRate;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGE("AEM", "Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, SamplingRate> deserializeSetSamplingRateCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Malformed packet in deserializeSetSamplingRateCommand");
        return {};
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };
    SamplingRate samplingRate{};

    des >> descriptorType >> descriptorIndex;
    des >> samplingRate;

    if (des.usedBytes() != AECP_AEM_SET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Used more bytes than specified in protocol constant");
    }

    return std::make_tuple(descriptorType, descriptorIndex, samplingRate);
}

/** SET_SAMPLING_RATE Response - Clause 7.4.21.1 */
Serializer<AECP_AEM_SET_SAMPLING_RATE_RESPONSE_PAYLOAD_SIZE> serializeSetSamplingRateResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, SamplingRate const samplingRate)
{
    // Same as SET_SAMPLING_RATE Command
    return serializeSetSamplingRateCommand(descriptorType, descriptorIndex, samplingRate);
}

std::tuple<DescriptorType, DescriptorIndex, SamplingRate> deserializeSetSamplingRateResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    if (status != AemCommandStatus::Success) {
        ESP_LOGI("AEM", "SET_SAMPLING_RATE command not successful");
        return {};
    }

    return deserializeSetSamplingRateCommand(payload);
}

/** GET_SAMPLING_RATE Command - Clause 7.4.22.1 */
Serializer<AECP_AEM_GET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE> serializeGetSamplingRateCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_GET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGE("AEM", "Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeGetSamplingRateCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Malformed packet in deserializeGetSamplingRateCommand");
        return {};
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> descriptorType >> descriptorIndex;

    if (des.usedBytes() != AECP_AEM_GET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Used more bytes than specified in protocol constant");
    }

    return std::make_tuple(descriptorType, descriptorIndex);
}

/** GET_SAMPLING_RATE Response - Clause 7.4.22.2 */
Serializer<AECP_AEM_GET_SAMPLING_RATE_RESPONSE_PAYLOAD_SIZE> serializeGetSamplingRateResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, SamplingRate const samplingRate)
{
    return serializeSetSamplingRateCommand(descriptorType, descriptorIndex, samplingRate);
}

std::tuple<DescriptorType, DescriptorIndex, SamplingRate> deserializeGetSamplingRateResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_SAMPLING_RATE_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetSamplingRateCommand(payload);
}

/** SET_CLOCK_SOURCE Command - Clause 7.4.23.1 */
Serializer<AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE> serializeSetClockSourceCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ClockSourceIndex const clockSourceIndex)
{
    Serializer<AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE> ser;
    std::uint16_t const reserved{ 0u };

    ser << descriptorType << descriptorIndex;
    ser << clockSourceIndex << reserved;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGE("AEM", "Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, ClockSourceIndex> deserializeSetClockSourceCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Malformed packet in deserializeSetClockSourceCommand");
        return {};
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };
    ClockSourceIndex clockSourceIndex{ 0u };
    std::uint16_t reserved{ 0u };

    des >> descriptorType >> descriptorIndex;
    des >> clockSourceIndex >> reserved;

    if (des.usedBytes() != AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Used more bytes than specified in protocol constant");
    }

    return std::make_tuple(descriptorType, descriptorIndex, clockSourceIndex);
}

/** SET_CLOCK_SOURCE Response - Clause 7.4.23.1 */
Serializer<AECP_AEM_SET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE> serializeSetClockSourceResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ClockSourceIndex const clockSourceIndex)
{
    static_assert(AECP_AEM_SET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE == AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE, "SET_CLOCK_SOURCE Response no longer the same as SET_CLOCK_SOURCE Command");
    return serializeSetClockSourceCommand(descriptorType, descriptorIndex, clockSourceIndex);
}

std::tuple<DescriptorType, DescriptorIndex, ClockSourceIndex> deserializeSetClockSourceResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    static_assert(AECP_AEM_SET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE == AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE, "SET_CLOCK_SOURCE Response no longer the same as SET_CLOCK_SOURCE Command");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE, AECP_AEM_SET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetClockSourceCommand(payload);
}

/** GET_CLOCK_SOURCE Command - Clause 7.4.24.1 */
Serializer<AECP_AEM_GET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE> serializeGetClockSourceCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_GET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGE("AEM", "Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeGetClockSourceCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Malformed packet in deserializeGetClockSourceCommand");
        return {};
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> descriptorType >> descriptorIndex;

    if (des.usedBytes() != AECP_AEM_GET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGE("AEM", "Used more bytes than specified in protocol constant");
    }

    return std::make_tuple(descriptorType, descriptorIndex);
}

/** GET_CLOCK_SOURCE Response - Clause 7.4.24.2 */
Serializer<AECP_AEM_GET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE> serializeGetClockSourceResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ClockSourceIndex const clockSourceIndex)
{
    static_assert(AECP_AEM_GET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE == AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE, "GET_CLOCK_SOURCE Response no longer the same as SET_CLOCK_SOURCE Command");
    return serializeSetClockSourceCommand(descriptorType, descriptorIndex, clockSourceIndex);
}

std::tuple<DescriptorType, DescriptorIndex, ClockSourceIndex> deserializeGetClockSourceResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
    static_assert(AECP_AEM_GET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE == AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE, "GET_CLOCK_SOURCE Response no longer the same as SET_CLOCK_SOURCE Command");
    checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetClockSourceCommand(payload);
}

Serializer<AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE> serializeSetControlCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ControlValues const& controlValues)
{
	//static auto s_Dispatch = std::unordered_map<entity::model::ControlValueType::Type, std::function<void(Serializer<AEM_AECPDU_MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH>&, entity::model::ControlValues const&)>>{};

	/*if (s_Dispatch.empty())
	{
		// Create the dispatch table
		createPackDynamicControlValuesDispatchTable(s_Dispatch);
	}*/

	Serializer<AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE> ser;
	ser << descriptorType << descriptorIndex;

	// Serialize variable data
	/*if (!controlValues.empty())
	{
		auto const valueType = controlValues.getType();
		if (auto const& it = s_Dispatch.find(valueType); it != s_Dispatch.end())
		{
			it->second(ser, controlValues);
		}
		else
		{
			ESP_LOGI("AEM", "serializeSetControlCommand warning: Unsupported ControlValueType: %hu", valueType);

		}
	}*/

	return ser;
}

std::tuple<DescriptorType, DescriptorIndex, MemoryBuffer> deserializeSetControlCommand(AemAecpdu::Payload const& payload)
{
	auto* const commandPayload = payload.first;
	auto const commandPayloadLength = payload.second;

	if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE)
	{
		ESP_LOGI("AEM", "Malformed packet: Command payload is null or smaller than expected.");
		return std::make_tuple(DescriptorType::Invalid, 0u, MemoryBuffer{});
	}

	// Check payload
	Deserializer des(commandPayload, commandPayloadLength);
	DescriptorType descriptorType{ DescriptorType::Invalid };
	DescriptorIndex descriptorIndex{ 0u };
	MemoryBuffer memoryBuffer{};

	des >> descriptorType >> descriptorIndex;

	// Unpack remaining data
	auto const remaining = des.remaining();
	if (remaining != 0)
	{
		memoryBuffer.set_size(remaining);
		des >> memoryBuffer;
	}

	ESP_LOGI("AEM", "Successfully deserialized Set Control Command: Descriptor Type: %hu, Descriptor Index: %d", (uint16_t)descriptorType, descriptorIndex);

	return std::make_tuple(descriptorType, descriptorIndex, memoryBuffer);
}

/** SET_CONTROL Response - Clause 7.4.25.1 */
Serializer<AECP_AEM_SET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE> serializeSetControlResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ControlValues const& controlValues)
{
	// Same as SET_CONTROL Command
	static_assert(AECP_AEM_SET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE == AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE, "SET_CONTROL Response no longer the same as SET_CONTROL Command");
	return serializeSetControlCommand(descriptorType, descriptorIndex, controlValues);
}

std::tuple<DescriptorType, DescriptorIndex, MemoryBuffer> deserializeSetControlResponse(AemCommandStatus const status, AemAecpdu::Payload const& payload)
{
	// Same as SET_CONTROL Command
	static_assert(AECP_AEM_SET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE == AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE, "SET_CONTROL Response no longer the same as SET_CONTROL Command");
	checkResponsePayload(payload, static_cast<uint8_t>(status), AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE, AECP_AEM_SET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE);
	return deserializeSetControlCommand(payload);
}

/** GET_CONTROL Command - Clause 7.4.26.1 */
Serializer<AECP_AEM_GET_CONTROL_COMMAND_PAYLOAD_SIZE> serializeGetControlCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
	Serializer<AECP_AEM_GET_CONTROL_COMMAND_PAYLOAD_SIZE> ser;

	ser << descriptorType << descriptorIndex;

	if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGI("AEM", "Warning: Used bytes (%d) do not match the protocol constant (%d)", ser.usedBytes(), ser.capacity());
    }
    
	return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeGetControlCommand(AemAecpdu::Payload const& payload)
{
	auto* const commandPayload = payload.first;
	auto const commandPayloadLength = payload.second;

	if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_CONTROL_COMMAND_PAYLOAD_SIZE)
	{
		ESP_LOGI("AEM", "Malformed packet: Command payload is null or smaller than expected.");
		return std::make_tuple(DescriptorType::Invalid, 0u);
	}

	// Check payload
	Deserializer des(commandPayload, commandPayloadLength);
	DescriptorType descriptorType{ DescriptorType::Invalid };
	DescriptorIndex descriptorIndex{ 0u };

	des >> descriptorType >> descriptorIndex;

	ESP_LOGI("AEM", "Successfully deserialized Get Control Command: Descriptor Type: %hu, Descriptor Index: %d", (uint16_t)descriptorType, descriptorIndex);

	return std::make_tuple(descriptorType, descriptorIndex);
}

/** GET_CONTROL Response - Clause 7.4.26.2 */
Serializer<AECP_AEM_GET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetControlResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ControlValues const& controlValues)
{
    // Same as SET_CONTROL Command
    if (AECP_AEM_GET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE != AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE) {
        ESP_LOGI("AEM", "GET_CONTROL Response no longer the same as SET_CONTROL Command");
    }
    return serializeSetControlCommand(descriptorType, descriptorIndex, controlValues);
}

std::tuple<DescriptorType, DescriptorIndex, MemoryBuffer> deserializeGetControlResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    // Same as SET_CONTROL Command
    if (AECP_AEM_GET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE != AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE) {
        ESP_LOGI("AEM", "GET_CONTROL Response no longer the same as SET_CONTROL Command");
    }
    checkResponsePayload(payload, (uint8_t)status, AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE, AECP_AEM_GET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE);
    return deserializeSetControlCommand(payload);
}

/** START_STREAMING Command - Clause 7.4.35.1 */
Serializer<AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE> serializeStartStreamingCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGI("AEM", "Used bytes (%d) do not match the protocol constant (%d) for START_STREAMING Command", ser.usedBytes(), ser.capacity());
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeStartStreamingCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "Malformed packet for START_STREAMING Command");
        return {};
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> descriptorType >> descriptorIndex;

    if (des.usedBytes() != AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "Used more bytes than specified in protocol constant for START_STREAMING Command");
    }

    return std::make_tuple(descriptorType, descriptorIndex);
}

/** START_STREAMING Response - Clause 7.4.35.1 */
Serializer<AECP_AEM_START_STREAMING_RESPONSE_PAYLOAD_SIZE> serializeStartStreamingResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    // Same as START_STREAMING Command
    if (AECP_AEM_START_STREAMING_RESPONSE_PAYLOAD_SIZE != AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "START_STREAMING Response no longer the same as START_STREAMING Command");
    }
    return serializeStartStreamingCommand(descriptorType, descriptorIndex);
}

std::tuple<DescriptorType, DescriptorIndex> deserializeStartStreamingResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    // Same as START_STREAMING Command
    if (AECP_AEM_START_STREAMING_RESPONSE_PAYLOAD_SIZE != AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "START_STREAMING Response no longer the same as START_STREAMING Command");
    }
    checkResponsePayload(payload, (uint8_t)status, AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE, AECP_AEM_START_STREAMING_RESPONSE_PAYLOAD_SIZE);
    return deserializeStartStreamingCommand(payload);
}

/** STOP_STREAMING Command - Clause 7.4.36.1 */
Serializer<AECP_AEM_STOP_STREAMING_COMMAND_PAYLOAD_SIZE> serializeStopStreamingCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    // Same as START_STREAMING Command
    if (AECP_AEM_STOP_STREAMING_COMMAND_PAYLOAD_SIZE != AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "STOP_STREAMING Command no longer the same as START_STREAMING Command");
    }
    return serializeStartStreamingCommand(descriptorType, descriptorIndex);
}

std::tuple<DescriptorType, DescriptorIndex> deserializeStopStreamingCommand(AemAecpdu::Payload const& payload)
{
    // Same as START_STREAMING Command
    if (AECP_AEM_STOP_STREAMING_COMMAND_PAYLOAD_SIZE != AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "STOP_STREAMING Command no longer the same as START_STREAMING Command");
    }
    return deserializeStartStreamingCommand(payload);
}

/** STOP_STREAMING Response - Clause 7.4.36.1 */
Serializer<AECP_AEM_STOP_STREAMING_RESPONSE_PAYLOAD_SIZE> serializeStopStreamingResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    // Same as START_STREAMING Command
    if (AECP_AEM_STOP_STREAMING_RESPONSE_PAYLOAD_SIZE != AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "STOP_STREAMING Response no longer the same as START_STREAMING Command");
    }
    return serializeStartStreamingCommand(descriptorType, descriptorIndex);
}

/** STOP_STREAMING Response - Clause 7.4.36.1 */
std::tuple<DescriptorType, DescriptorIndex> deserializeStopStreamingResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    // Same as START_STREAMING Command
    if (AECP_AEM_STOP_STREAMING_RESPONSE_PAYLOAD_SIZE != AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "STOP_STREAMING Response no longer the same as START_STREAMING Command");
    }
    checkResponsePayload(payload, status, AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE, AECP_AEM_STOP_STREAMING_RESPONSE_PAYLOAD_SIZE);
    return deserializeStartStreamingCommand(payload);
}

/** GET_AVB_INFO Command - Clause 7.4.40.1 */
Serializer<AECP_AEM_GET_AVB_INFO_COMMAND_PAYLOAD_SIZE> serializeGetAvbInfoCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_GET_AVB_INFO_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGI("AEM", "Used bytes (%d) do not match the protocol constant (%d) for GET_AVB_INFO Command", ser.usedBytes(), ser.capacity());
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeGetAvbInfoCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_AVB_INFO_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "Malformed packet for GET_AVB_INFO Command");
        return {};
    }

    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> descriptorType >> descriptorIndex;

    if (des.usedBytes() != AECP_AEM_GET_AVB_INFO_COMMAND_PAYLOAD_SIZE) {
        ESP_LOGI("AEM", "Used more bytes than specified in protocol constant for GET_AVB_INFO Command");
    }

    return std::make_tuple(descriptorType, descriptorIndex);
}

/** GET_AVB_INFO Response - Clause 7.4.40.2 */
Serializer<AECP_AEM_GET_AVB_INFO_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetAvbInfoResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, AvbInfo const& avbInfo)
{
    Serializer<AECP_AEM_GET_AVB_INFO_RESPONSE_PAYLOAD_MIN_SIZE> ser;

    ser << descriptorType << descriptorIndex;
    ser << avbInfo.gptpGrandmasterID << avbInfo.propagationDelay << avbInfo.gptpDomainNumber << avbInfo.flags << static_cast<std::uint16_t>(avbInfo.mappings.size());

    // Serialize variable data
    for (auto const& mapping : avbInfo.mappings)
    {
        ser << mapping.trafficClass << mapping.priority << mapping.vlanID;
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, AvbInfo> deserializeGetAvbInfoResponse(uint8_t const status, AemAecpdu::Payload const& payload)
{
	auto* const commandPayload = payload.first;
	auto const commandPayloadLength = payload.second;

	checkResponsePayload(payload, status, AECP_AEM_GET_AVB_INFO_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_AVB_INFO_RESPONSE_PAYLOAD_MIN_SIZE);

	// Check payload
	Deserializer des(commandPayload, commandPayloadLength);
	DescriptorType descriptorType{ DescriptorType::Invalid };
	DescriptorIndex descriptorIndex{ 0u };
	AvbInfo avbInfo{};
	auto numberOfMappings = std::uint16_t{ 0u };

	des >> descriptorType >> descriptorIndex;
	des >> avbInfo.gptpGrandmasterID >> avbInfo.propagationDelay >> avbInfo.gptpDomainNumber >> avbInfo.flags >> numberOfMappings;

	// Check variable size
	auto const mappingsSize = MsrpMapping::size() * numberOfMappings;
	if (des.remaining() < mappingsSize)
	{
		ESP_LOGI("AEM", "deserializeGetAvbInfoResponse: Malformed packet, insufficient remaining bytes");
		return std::make_tuple(descriptorType, descriptorIndex, avbInfo); // Return default values on error
	}

	// Unpack remaining data
	MsrpMappings mappings;
	for (auto index = 0u; index < numberOfMappings; ++index)
	{
		MsrpMapping mapping;
		des >> mapping.trafficClass >> mapping.priority >> mapping.vlanID;
		mappings.push_back(mapping);
	}
	avbInfo.mappings = std::move(mappings);

	if (des.remaining() != 0)
	{
		ESP_LOGI("AEM", "deserializeGetAvbInfoResponse: Warning - Remaining bytes in buffer");
	}

	return std::make_tuple(descriptorType, descriptorIndex, avbInfo);
}

/** GET_AS_PATH Command - Clause 7.4.41.1 */
Serializer<AECP_AEM_GET_AS_PATH_COMMAND_PAYLOAD_SIZE> serializeGetAsPathCommand(DescriptorIndex const descriptorIndex)
{
	Serializer<AECP_AEM_GET_AS_PATH_COMMAND_PAYLOAD_SIZE> ser;
	std::uint16_t const reserved{ 0u };

	ser << descriptorIndex << reserved;

	ESP_LOGI("AEM", "serializeGetAsPathCommand: Used bytes: %zu, Expected capacity: %zu", ser.usedBytes(), ser.capacity());

	return ser;
}

std::tuple<DescriptorIndex> deserializeGetAsPathCommand(AemAecpdu::Payload const& payload)
{
	auto* const commandPayload = payload.first;
	auto const commandPayloadLength = payload.second;

	if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_AS_PATH_COMMAND_PAYLOAD_SIZE)
	{
		ESP_LOGI("AEM", "deserializeGetAsPathCommand: Malformed packet");
		return std::make_tuple(DescriptorIndex{0u}); // Return default on error
	}

	// Check payload
	Deserializer des(commandPayload, commandPayloadLength);
	DescriptorIndex descriptorIndex{ 0u };
	std::uint16_t reserved{ 0u };

	des >> descriptorIndex >> reserved;

	ESP_LOGI("AEM", "deserializeGetAsPathCommand: DescriptorIndex: %hu", (uint16_t)descriptorIndex);

	return std::make_tuple(descriptorIndex);
}

/** GET_AS_PATH Response - Clause 7.4.41.2 */
Serializer<AECP_AEM_GET_AS_PATH_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetAsPathResponse(DescriptorIndex const descriptorIndex, AsPath const& asPath)
{
    Serializer<AECP_AEM_GET_AS_PATH_RESPONSE_PAYLOAD_MIN_SIZE> ser;

    ser << descriptorIndex << static_cast<std::uint16_t>(asPath.sequence.size());

    // Serialize variable data
    for (auto const& clockIdentity : asPath.sequence)
    {
        ser << clockIdentity;
    }

    ESP_LOGI("AEM", "serializeGetAsPathResponse: DescriptorIndex: %hu", (uint16_t)descriptorIndex);
    return ser;
}

std::tuple<DescriptorIndex, AsPath> deserializeGetAsPathResponse(uint8_t const status, AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    checkResponsePayload(payload, status, AECP_AEM_GET_AS_PATH_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_AS_PATH_RESPONSE_PAYLOAD_MIN_SIZE);

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorIndex descriptorIndex{ 0u };
    AsPath asPath{};
    auto count = std::uint16_t{ 0u };

    des >> descriptorIndex >> count;

    // Check variable size
    auto const sequenceSize = sizeof(UniqueIdentifier::value_type) * count;
    if (des.remaining() < sequenceSize)
    {
        ESP_LOGI("AEM", "deserializeGetAsPathResponse: Malformed packet, insufficient remaining bytes");
        return std::make_tuple(descriptorIndex, asPath);  // Return default values on error
    }

    // Unpack remaining data
    for (auto index = 0u; index < count; ++index)
    {
        auto clockIdentity = decltype(asPath.sequence)::value_type{};
        des >> clockIdentity;
        asPath.sequence.push_back(clockIdentity);
    }

    if (des.remaining() != 0)
    {
        ESP_LOGI("AEM", "deserializeGetAsPathResponse: Warning - Remaining bytes in buffer");
    }

    ESP_LOGI("AEM", "deserializeGetAsPathResponse: DescriptorIndex: %hu, Count: %hu", (uint16_t)descriptorIndex, count);
    return std::make_tuple(descriptorIndex, asPath);
}

/** GET_COUNTERS Command - Clause 7.4.42.1 */
Serializer<AECP_AEM_GET_COUNTERS_COMMAND_PAYLOAD_SIZE> serializeGetCountersCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_GET_COUNTERS_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    ESP_LOGI("AEM", "serializeGetCountersCommand: DescriptorType: %hu, DescriptorIndex: %hu", (uint16_t)descriptorType, (uint16_t)descriptorIndex);
    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeGetCountersCommand(AemAecpdu::Payload const& payload)
{
	auto* const commandPayload = payload.first;
	auto const commandPayloadLength = payload.second;

	if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_COUNTERS_COMMAND_PAYLOAD_SIZE)
	{
		ESP_LOGI("AEM", "deserializeGetCountersCommand: Malformed packet, payload is nullptr or size mismatch");
		return std::make_tuple(DescriptorType::Invalid, 0u); // Return default values
	}

	// Check payload
	Deserializer des(commandPayload, commandPayloadLength);
	DescriptorType descriptorType{ DescriptorType::Invalid };
	DescriptorIndex descriptorIndex{ 0u };

	des >> descriptorType >> descriptorIndex;

	if (des.usedBytes() != AECP_AEM_GET_COUNTERS_COMMAND_PAYLOAD_SIZE)
	{
		ESP_LOGI("AEM", "deserializeGetCountersCommand: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
	}

	return std::make_tuple(descriptorType, descriptorIndex);
}

/** GET_COUNTERS Response - Clause 7.4.42.2 */
Serializer<AECP_AEM_GET_COUNTERS_RESPONSE_PAYLOAD_SIZE> serializeGetCountersResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, DescriptorCounterValidFlag const validCounters, DescriptorCounters const& counters)
{
	Serializer<AECP_AEM_GET_COUNTERS_RESPONSE_PAYLOAD_SIZE> ser;

	ser << descriptorType << descriptorIndex;
	ser << validCounters;

	// Serialize the counters
	for (auto const counter : counters)
	{
		ser << counter;
	}

	if (ser.usedBytes() != ser.capacity())
	{
		ESP_LOGI("AEM", "serializeGetCountersResponse: Used bytes do not match protocol constant");
	}

	ESP_LOGI("AEM", "serializeGetCountersResponse: DescriptorType: %hu, DescriptorIndex: %hu", (uint16_t)descriptorType, (uint16_t)descriptorIndex);

	return ser;
}

std::tuple<DescriptorType, DescriptorIndex, DescriptorCounterValidFlag, DescriptorCounters> deserializeGetCountersResponse(uint8_t const status, AemAecpdu::Payload const& payload)
{
	auto* const commandPayload = payload.first;
	auto const commandPayloadLength = payload.second;

	checkResponsePayload(payload, status, AECP_AEM_GET_COUNTERS_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_COUNTERS_RESPONSE_PAYLOAD_SIZE);

	// Check payload
	Deserializer des(commandPayload, commandPayloadLength);
	DescriptorType descriptorType{ DescriptorType::Invalid };
	DescriptorIndex descriptorIndex{ 0u };
	DescriptorCounterValidFlag validCounters{ 0u };
	DescriptorCounters counters{};

	des >> descriptorType >> descriptorIndex;
	des >> validCounters;

	// Deserialize the counters
	for (auto& counter : counters)
	{
		des >> counter;
	}

	if (des.usedBytes() != AECP_AEM_GET_COUNTERS_RESPONSE_PAYLOAD_SIZE)
	{
		ESP_LOGI("AEM", "deserializeGetCountersResponse: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
	}

	ESP_LOGI("AEM", "deserializeGetCountersResponse: DescriptorType: %hu, DescriptorIndex: %hu", (uint16_t)descriptorType, (uint16_t)descriptorIndex);

	return std::make_tuple(descriptorType, descriptorIndex, validCounters, counters);
}

/** REBOOT Command - Clause 7.4.43.1 */
Serializer<AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE> serializeRebootCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    Serializer<AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "serializeRebootCommand: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex> deserializeRebootCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeRebootCommand: Malformed packet, payload is nullptr or size mismatch");
        return std::make_tuple(DescriptorType::Invalid, 0u); // Return default values
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };

    des >> descriptorType >> descriptorIndex;

    if (des.usedBytes() != AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeRebootCommand: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    return std::make_tuple(descriptorType, descriptorIndex);
}

/** REBOOT Response - Clause 7.4.43.1 */
Serializer<AECP_AEM_REBOOT_RESPONSE_PAYLOAD_SIZE> serializeRebootResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex)
{
    // Same as REBOOT Command
    static_assert(AECP_AEM_REBOOT_RESPONSE_PAYLOAD_SIZE == AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE, "REBOOT Response no longer the same as REBOOT Command");
    return serializeRebootCommand(descriptorType, descriptorIndex);
}

std::tuple<DescriptorType, DescriptorIndex> deserializeRebootResponse(uint8_t const status, AemAecpdu::Payload const& payload)
{
    // Same as REBOOT Command
    static_assert(AECP_AEM_REBOOT_RESPONSE_PAYLOAD_SIZE == AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE, "REBOOT Response no longer the same as REBOOT Command");

    checkResponsePayload(payload, status, AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE, AECP_AEM_REBOOT_RESPONSE_PAYLOAD_SIZE);

    return deserializeRebootCommand(payload);
}

/** GET_AUDIO_MAP Command - Clause 7.4.44.1 */
Serializer<AECP_AEM_GET_AUDIO_MAP_COMMAND_PAYLOAD_SIZE> serializeGetAudioMapCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, MapIndex const mapIndex)
{
    Serializer<AECP_AEM_GET_AUDIO_MAP_COMMAND_PAYLOAD_SIZE> ser;
    std::uint16_t const reserved{ 0u };

    ser << descriptorType << descriptorIndex;
    ser << mapIndex << reserved;

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "serializeGetAudioMapCommand: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, MapIndex> deserializeGetAudioMapCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_AUDIO_MAP_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeGetAudioMapCommand: Malformed packet, insufficient payload size");
        return std::make_tuple(DescriptorType::Invalid, 0u, 0u);
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };
    MapIndex mapIndex{ 0u };
    std::uint16_t reserved{ 0u };

    des >> descriptorType >> descriptorIndex;
    des >> mapIndex >> reserved;

    if (des.usedBytes() != AECP_AEM_GET_AUDIO_MAP_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeGetAudioMapCommand: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    return std::make_tuple(descriptorType, descriptorIndex, mapIndex);
}

/** GET_AUDIO_MAP Response - Clause 7.4.44.2 */
Serializer<AECP_AEM_GET_AUDIO_MAP_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetAudioMapResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, MapIndex const mapIndex, MapIndex const numberOfMaps, AudioMappings const& mappings)
{
    Serializer<AECP_AEM_GET_AUDIO_MAP_RESPONSE_PAYLOAD_MIN_SIZE> ser;
    std::uint16_t const reserved{ 0u };

    ser << descriptorType << descriptorIndex;
    ser << mapIndex << numberOfMaps << static_cast<std::uint16_t>(mappings.size()) << reserved;

    // Serialize variable data
    for (auto const& mapping : mappings)
    {
        ser << mapping.streamIndex << mapping.streamChannel << mapping.clusterOffset << mapping.clusterChannel;
    }

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "serializeGetAudioMapResponse: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    return ser;
}

/** GET_AUDIO_MAP Response - Clause 7.4.44.2 */
std::tuple<DescriptorType, DescriptorIndex, MapIndex, MapIndex, AudioMappings> deserializeGetAudioMapResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    checkResponsePayload(payload, status, AECP_AEM_GET_AUDIO_MAP_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_AUDIO_MAP_RESPONSE_PAYLOAD_MIN_SIZE);

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };
    MapIndex mapIndex{ 0u };
    MapIndex numberOfMaps{ 0u };
    std::uint16_t numberOfMappings{ 0u };
    std::uint16_t reserved{ 0u };
    AudioMappings mappings{};

    des >> descriptorType >> descriptorIndex;
    des >> mapIndex >> numberOfMaps >> numberOfMappings >> reserved;

    // Check variable size
    auto const mappingsSize = AudioMapping::size() * numberOfMappings;
    if (des.remaining() < mappingsSize)
    {
        ESP_LOGI("AEM", "deserializeGetAudioMapResponse: Malformed packet, insufficient remaining bytes");
        return std::make_tuple(descriptorType, descriptorIndex, mapIndex, numberOfMaps, mappings);
    }

    // Unpack remaining data
    for (auto index = 0u; index < numberOfMappings; ++index)
    {
        AudioMapping mapping;
        des >> mapping.streamIndex >> mapping.streamChannel >> mapping.clusterOffset >> mapping.clusterChannel;
        mappings.push_back(mapping);
    }

    if (des.usedBytes() != (AECP_AEM_GET_AUDIO_MAP_RESPONSE_PAYLOAD_MIN_SIZE + mappingsSize))
    {
        ESP_LOGI("AEM", "deserializeGetAudioMapResponse: Used more bytes than specified, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    if (des.remaining() != 0)
    {
        ESP_LOGI("AEM", "GetAudioMap Response deserialize warning: Remaining bytes in buffer");
    }

    return std::make_tuple(descriptorType, descriptorIndex, mapIndex, numberOfMaps, mappings);
}

/** ADD_AUDIO_MAPPINGS Command - Clause 7.4.45.1 */
Serializer<AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE> serializeAddAudioMappingsCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, AudioMappings const& mappings)
{
    Serializer<AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE> ser;
    std::uint16_t const reserved{ 0u };

    ser << descriptorType << descriptorIndex;
    ser << static_cast<std::uint16_t>(mappings.size()) << reserved;

    // Serialize variable data
    for (auto const& mapping : mappings)
    {
        ser << mapping.streamIndex << mapping.streamChannel << mapping.clusterOffset << mapping.clusterChannel;
    }

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "serializeAddAudioMappingsCommand: Used bytes do not match protocol constant, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, AudioMappings> deserializeAddAudioMappingsCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE) // Malformed packet
    {
        ESP_LOGI("AEM", "deserializeAddAudioMappingsCommand: Malformed packet, null command payload or insufficient length");
        return std::make_tuple(DescriptorType::Invalid, 0, AudioMappings{});
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };
    std::uint16_t numberOfMappings{ 0u };
    std::uint16_t reserved{ 0u };

    des >> descriptorType >> descriptorIndex;
    des >> numberOfMappings >> reserved;

    // Check variable size
    constexpr size_t mapInfoSize = sizeof(AudioMapping::streamIndex) + sizeof(AudioMapping::streamChannel) + sizeof(AudioMapping::clusterOffset) + sizeof(AudioMapping::clusterChannel);
    auto const mappingsSize = mapInfoSize * numberOfMappings;

    if (des.remaining() < mappingsSize) // Malformed packet
    {
        ESP_LOGI("AEM", "deserializeAddAudioMappingsCommand: Malformed packet, insufficient remaining bytes");
        return std::make_tuple(descriptorType, descriptorIndex, AudioMappings{});
    }

    // Unpack remaining data
    AudioMappings mappings;
    for (auto index = 0u; index < numberOfMappings; ++index)
    {
        AudioMapping mapping;
        des >> mapping.streamIndex >> mapping.streamChannel >> mapping.clusterOffset >> mapping.clusterChannel;
        mappings.push_back(mapping);
    }

    if (des.usedBytes() != (AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE + mappingsSize))
    {
        ESP_LOGI("AEM", "deserializeAddAudioMappingsCommand: Used more bytes than expected, DescriptorType: %hu", (uint16_t)descriptorType);
    }

    if (des.remaining() != 0)
    {
        ESP_LOGI("AEM", "AddAudioMap Command deserialize warning: Remaining bytes in buffer");
    }

    return std::make_tuple(descriptorType, descriptorIndex, mappings);
}


/** ADD_AUDIO_MAPPINGS Response - Clause 7.4.45.2 */
Serializer<AECP_AEM_ADD_AUDIO_MAPPINGS_RESPONSE_PAYLOAD_MIN_SIZE> serializeAddAudioMappingsResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, AudioMappings const& mappings)
{
    // Same as ADD_AUDIO_MAPPINGS Command
    return serializeAddAudioMappingsCommand(descriptorType, descriptorIndex, mappings);
}

/** REMOVE_AUDIO_MAPPINGS Command - Clause 7.4.46.1 */
Serializer<AECP_AEM_REMOVE_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE> serializeRemoveAudioMappingsCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, AudioMappings const& mappings)
{
    // Same as ADD_AUDIO_MAPPINGS Command
    return serializeAddAudioMappingsCommand(descriptorType, descriptorIndex, mappings);
}

/** REMOVE_AUDIO_MAPPINGS Response - Clause 7.4.46.2 */
Serializer<AECP_AEM_REMOVE_AUDIO_MAPPINGS_RESPONSE_PAYLOAD_MIN_SIZE> serializeRemoveAudioMappingsResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, AudioMappings const& mappings)
{
    // Same as ADD_AUDIO_MAPPINGS Command
    return serializeAddAudioMappingsCommand(descriptorType, descriptorIndex, mappings);
}

/** REMOVE_AUDIO_MAPPINGS Command Deserialization */
std::tuple<DescriptorType, DescriptorIndex, AudioMappings> deserializeRemoveAudioMappingsCommand(AemAecpdu::Payload const& payload)
{
    return deserializeAddAudioMappingsCommand(payload);
}

/** REMOVE_AUDIO_MAPPINGS Response Deserialization */
std::tuple<DescriptorType, DescriptorIndex, AudioMappings> deserializeRemoveAudioMappingsResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    checkResponsePayload(payload, status, AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE, AECP_AEM_REMOVE_AUDIO_MAPPINGS_RESPONSE_PAYLOAD_MIN_SIZE);
    return deserializeAddAudioMappingsCommand(payload);
}

/** ABORT_OPERATION Command - Clause 7.4.55.1 */
Serializer<AECP_AEM_ABORT_OPERATION_COMMAND_PAYLOAD_SIZE> serializeAbortOperationCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID)
{
    Serializer<AECP_AEM_ABORT_OPERATION_COMMAND_PAYLOAD_SIZE> ser;
    std::uint16_t const reserved{ 0u };

    ser << descriptorType << descriptorIndex;
    ser << operationID << reserved;

    if (ser.usedBytes() != ser.capacity()) {
        ESP_LOGI("AEM", "serializeAbortOperationCommand: Used bytes (%zu) do not match the protocol constant (%zu)", ser.usedBytes(), ser.capacity());
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, OperationID, MemoryObjectOperationType, MemoryBuffer> deserializeStartOperationCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_START_OPERATION_COMMAND_PAYLOAD_MIN_SIZE) {
        ESP_LOGI("AEM", "deserializeStartOperationCommand: Malformed packet, insufficient payload size");
        return {};
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType = DescriptorType::Invalid;
    DescriptorIndex descriptorIndex{ 0u };
    OperationID operationID{ 0u };
    MemoryObjectOperationType operationType = MemoryObjectOperationType::Store;
    MemoryBuffer memoryBuffer{};

    des >> descriptorType >> descriptorIndex;
    des >> operationID >> operationType;

    // Unpack remaining data
    auto const remaining = des.remaining();
    if (remaining != 0) {
        memoryBuffer.set_size(remaining);
        des >> memoryBuffer;
    }

    return std::make_tuple(descriptorType, descriptorIndex, operationID, operationType, memoryBuffer);
}

/** START_OPERATION Response - Clause 7.4.53.1 */
Serializer<AECP_AEM_START_OPERATION_RESPONSE_PAYLOAD_MIN_SIZE> serializeStartOperationResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID, MemoryObjectOperationType const operationType, MemoryBuffer const& memoryBuffer)
{
    // Same as START_OPERATION Command
    static_assert(AECP_AEM_START_OPERATION_RESPONSE_PAYLOAD_MIN_SIZE == AECP_AEM_START_OPERATION_COMMAND_PAYLOAD_MIN_SIZE, "START_OPERATION Response no longer the same as START_OPERATION Command");
    return serializeStartOperationCommand(descriptorType, descriptorIndex, operationID, operationType, memoryBuffer);
}

std::tuple<DescriptorType, DescriptorIndex, OperationID, MemoryObjectOperationType, MemoryBuffer> deserializeStartOperationResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    // Same as START_OPERATION Command
    static_assert(AECP_AEM_START_OPERATION_RESPONSE_PAYLOAD_MIN_SIZE == AECP_AEM_START_OPERATION_COMMAND_PAYLOAD_MIN_SIZE, "START_OPERATION Response no longer the same as START_OPERATION Command");

    checkResponsePayload(payload, status, AECP_AEM_START_OPERATION_COMMAND_PAYLOAD_MIN_SIZE, AECP_AEM_START_OPERATION_RESPONSE_PAYLOAD_MIN_SIZE);
    return deserializeStartOperationCommand(payload);
}

std::tuple<DescriptorType, DescriptorIndex, OperationID> deserializeAbortOperationCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_ABORT_OPERATION_RESPONSE_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeAbortOperationCommand: Malformed packet, insufficient payload size");
        return {};
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };
    OperationID operationID{ 0u };
    std::uint16_t reserved{ 0u };

    des >> descriptorType >> descriptorIndex;
    des >> operationID >> reserved;

    if (des.usedBytes() != AECP_AEM_ABORT_OPERATION_RESPONSE_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeAbortOperationCommand: Used bytes do not match the protocol constant");
    }

    return std::make_tuple(descriptorType, descriptorIndex, operationID);
}

/** ABORT_OPERATION Response - Clause 7.4.55.1 */
Serializer<AECP_AEM_ABORT_OPERATION_RESPONSE_PAYLOAD_SIZE> serializeAbortOperationResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID)
{
    // Same as ABORT_OPERATION Command
    ESP_LOGI("AEM", "serializeAbortOperationResponse: Serializing ABORT_OPERATION Response");
    return serializeAbortOperationCommand(descriptorType, descriptorIndex, operationID);
}

std::tuple<DescriptorType, DescriptorIndex, OperationID> deserializeAbortOperationResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    // Same as ABORT_OPERATION Command
    ESP_LOGI("AEM", "deserializeAbortOperationResponse: Deserializing ABORT_OPERATION Response");
    checkResponsePayload(payload, status, AECP_AEM_ABORT_OPERATION_COMMAND_PAYLOAD_SIZE, AECP_AEM_ABORT_OPERATION_RESPONSE_PAYLOAD_SIZE);
    return deserializeAbortOperationCommand(payload);
}

/** OPERATION_STATUS Unsolicited Response - Clause 7.4.55.1 */
Serializer<AECP_AEM_OPERATION_STATUS_RESPONSE_PAYLOAD_SIZE> serializeOperationStatusResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID, std::uint16_t const percentComplete)
{
    Serializer<AECP_AEM_OPERATION_STATUS_RESPONSE_PAYLOAD_SIZE> ser;

    ser << descriptorType << descriptorIndex;
    ser << operationID << percentComplete;

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "serializeOperationStatusResponse: Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<DescriptorType, DescriptorIndex, OperationID, std::uint16_t> deserializeOperationStatusResponse(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_OPERATION_STATUS_RESPONSE_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeOperationStatusResponse: Malformed packet, insufficient payload size");
        return {};
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    DescriptorType descriptorType{ DescriptorType::Invalid };
    DescriptorIndex descriptorIndex{ 0u };
    OperationID operationID{ 0u };
    std::uint16_t percentComplete{ 0u };

    des >> descriptorType >> descriptorIndex;
    des >> operationID >> percentComplete;

    if (des.usedBytes() != AECP_AEM_OPERATION_STATUS_RESPONSE_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeOperationStatusResponse: Used bytes do not match the protocol constant");
    }

    return std::make_tuple(descriptorType, descriptorIndex, operationID, percentComplete);
}

/** SET_MEMORY_OBJECT_LENGTH Command - Clause 7.4.72.1 */
Serializer<AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE> serializeSetMemoryObjectLengthCommand(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex, std::uint64_t const length)
{
    Serializer<AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE> ser;

    ser << memoryObjectIndex << configurationIndex;
    ser << length;

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "serializeSetMemoryObjectLengthCommand: Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<ConfigurationIndex, MemoryObjectIndex, std::uint64_t> deserializeSetMemoryObjectLengthCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeSetMemoryObjectLengthCommand: Malformed packet, insufficient payload size");
        return {};
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    MemoryObjectIndex memoryObjectIndex{0u};
    ConfigurationIndex configurationIndex{0u};
    std::uint64_t length{0u};

    des >> memoryObjectIndex >> configurationIndex;
    des >> length;

    if (des.usedBytes() != AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeSetMemoryObjectLengthCommand: Used bytes do not match the protocol constant");
    }

    return std::make_tuple(configurationIndex, memoryObjectIndex, length);
}

/** SET_MEMORY_OBJECT_LENGTH Response - Clause 7.4.72.1 */
Serializer<AECP_AEM_SET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE> serializeSetMemoryObjectLengthResponse(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex, std::uint64_t const length)
{
    ESP_LOGI("AEM", "serializeSetMemoryObjectLengthResponse: Serializing SET_MEMORY_OBJECT_LENGTH Response");
    return serializeSetMemoryObjectLengthCommand(configurationIndex, memoryObjectIndex, length);
}

std::tuple<ConfigurationIndex, MemoryObjectIndex, std::uint64_t> deserializeSetMemoryObjectLengthResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    ESP_LOGI("AEM", "deserializeSetMemoryObjectLengthResponse: Deserializing SET_MEMORY_OBJECT_LENGTH Response");
    checkResponsePayload(payload, status, AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE, AECP_AEM_SET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetMemoryObjectLengthCommand(payload);
}

/** GET_MEMORY_OBJECT_LENGTH Command - Clause 7.4.73.1 */
Serializer<AECP_AEM_GET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE> serializeGetMemoryObjectLengthCommand(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex)
{
    Serializer<AECP_AEM_GET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE> ser;

    ser << memoryObjectIndex << configurationIndex;

    if (ser.usedBytes() != ser.capacity())
    {
        ESP_LOGI("AEM", "serializeGetMemoryObjectLengthCommand: Used bytes do not match the protocol constant");
    }

    return ser;
}

std::tuple<ConfigurationIndex, MemoryObjectIndex> deserializeGetMemoryObjectLengthCommand(AemAecpdu::Payload const& payload)
{
    auto* const commandPayload = payload.first;
    auto const commandPayloadLength = payload.second;

    if (commandPayload == nullptr || commandPayloadLength < AECP_AEM_GET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeGetMemoryObjectLengthCommand: Malformed packet, insufficient payload size");
        return {};
    }

    // Check payload
    Deserializer des(commandPayload, commandPayloadLength);
    MemoryObjectIndex memoryObjectIndex{0u};
    ConfigurationIndex configurationIndex{0u};

    des >> memoryObjectIndex >> configurationIndex;

    if (des.usedBytes() != AECP_AEM_GET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE)
    {
        ESP_LOGI("AEM", "deserializeGetMemoryObjectLengthCommand: Used bytes do not match the protocol constant");
    }

    return std::make_tuple(configurationIndex, memoryObjectIndex);
}

/** GET_MEMORY_OBJECT_LENGTH Response - Clause 7.4.73.2 */
Serializer<AECP_AEM_GET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE> serializeGetMemoryObjectLengthResponse(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex, std::uint64_t const length)
{
    ESP_LOGI("AEM", "serializeGetMemoryObjectLengthResponse: Serializing GET_MEMORY_OBJECT_LENGTH Response");
    return serializeSetMemoryObjectLengthCommand(configurationIndex, memoryObjectIndex, length);
}

std::tuple<ConfigurationIndex, MemoryObjectIndex, std::uint64_t> deserializeGetMemoryObjectLengthResponse(uint8_t status, AemAecpdu::Payload const& payload)
{
    ESP_LOGI("AEM", "deserializeGetMemoryObjectLengthResponse: Deserializing GET_MEMORY_OBJECT_LENGTH Response");
    checkResponsePayload(payload, status, AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE, AECP_AEM_GET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE);
    return deserializeSetMemoryObjectLengthCommand(payload);
}