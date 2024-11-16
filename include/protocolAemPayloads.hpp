#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMPAYLOADS_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMPAYLOADS_HPP_

#pragma once

#include "memoryBuffer.hpp"
#include "serialization.hpp"
#include "protocolAemPayloadSizes.hpp"
#include "entity.hpp"
#include "entityModel.hpp"
#include "protocolDefines.hpp"
#include <cstdint>
#include <tuple>
#include "esp_log.h"

// Logging tag
//static const char* TAG = "AEM_PAYLOADS";

/** ACQUIRE_ENTITY Command - Clause 7.4.1.1 */
Serializer<AECP_AEM_ACQUIRE_ENTITY_COMMAND_PAYLOAD_SIZE> serializeAcquireEntityCommand(AemAcquireEntityFlags const flags, UniqueIdentifier const ownerID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<AemAcquireEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeAcquireEntityCommand(const AemAecpdu::Payload& payload);

/** ACQUIRE_ENTITY Response - Clause 7.4.1.1 */
Serializer<AECP_AEM_ACQUIRE_ENTITY_RESPONSE_PAYLOAD_SIZE> serializeAcquireEntityResponse(AemAcquireEntityFlags const flags, UniqueIdentifier const ownerID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<AemAcquireEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeAcquireEntityResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** LOCK_ENTITY Command - Clause 7.4.2.1 */
Serializer<AECP_AEM_LOCK_ENTITY_COMMAND_PAYLOAD_SIZE> serializeLockEntityCommand(AemLockEntityFlags const flags, UniqueIdentifier const lockedID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<AemLockEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeLockEntityCommand(const AemAecpdu::Payload& payload);

/** LOCK_ENTITY Response - Clause 7.4.2.1 */
Serializer<AECP_AEM_LOCK_ENTITY_RESPONSE_PAYLOAD_SIZE> serializeLockEntityResponse(AemLockEntityFlags const flags, UniqueIdentifier const lockedID, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<AemLockEntityFlags, UniqueIdentifier, DescriptorType, DescriptorIndex> deserializeLockEntityResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** ENTITY_AVAILABLE Command - Clause 7.4.3.1 */
// No payload

/** ENTITY_AVAILABLE Response - Clause 7.4.3.1 */
// No payload

/** CONTROLLER_AVAILABLE Command - Clause 7.4.4.1 */
// No payload

/** CONTROLLER_AVAILABLE Response - Clause 7.4.4.1 */
// No payload

/** READ_DESCRIPTOR Command - Clause 7.4.5.1 */
Serializer<AECP_AEM_READ_DESCRIPTOR_COMMAND_PAYLOAD_SIZE> serializeReadDescriptorCommand(ConfigurationIndex const configurationIndex, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<ConfigurationIndex, DescriptorType, DescriptorIndex> deserializeReadDescriptorCommand(const AemAecpdu::Payload& payload);

/** READ_DESCRIPTOR Response - Clause 7.4.5.2 */
Serializer<AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH> serializeReadDescriptorCommonResponse(ConfigurationIndex const configurationIndex, DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
void serializeReadEntityDescriptorResponse(Serializer<AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH>& ser, const EntityDescriptor& entityDescriptor);
void serializeReadConfigurationDescriptorResponse(Serializer<AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH>& ser, const ConfigurationDescriptor& configurationDescriptor);
std::tuple<size_t, ConfigurationIndex, DescriptorType, DescriptorIndex> deserializeReadDescriptorCommonResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);
EntityDescriptor deserializeReadEntityDescriptorResponse(const AemAecpdu::Payload& payload, size_t const commonSize, AecpStatus const status);
ConfigurationDescriptor deserializeReadConfigurationDescriptorResponse(const AemAecpdu::Payload& payload, size_t const commonSize, AecpStatus const status);

/** WRITE_DESCRIPTOR Command - Clause 7.4.6.1 */
// To be implemented

/** WRITE_DESCRIPTOR Response - Clause 7.4.6.1 */
// To be implemented

/** SET_CONFIGURATION Command - Clause 7.4.7.1 */
Serializer<AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE> serializeSetConfigurationCommand(ConfigurationIndex const configurationIndex);
std::tuple<ConfigurationIndex> deserializeSetConfigurationCommand(const AemAecpdu::Payload& payload);

/** SET_CONFIGURATION Response - Clause 7.4.7.1 */
Serializer<AECP_AEM_SET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE> serializeSetConfigurationResponse(ConfigurationIndex const configurationIndex);
std::tuple<ConfigurationIndex> deserializeSetConfigurationResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_CONFIGURATION Command - Clause 7.4.8.1 */
// No payload

/** GET_CONFIGURATION Response - Clause 7.4.8.2 */
Serializer<AECP_AEM_GET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE> serializeGetConfigurationResponse(ConfigurationIndex const configurationIndex);
std::tuple<ConfigurationIndex> deserializeGetConfigurationResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** SET_STREAM_FORMAT Command - Clause 7.4.9.1 */
Serializer<AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE> serializeSetStreamFormatCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamFormat const streamFormat);
std::tuple<DescriptorType, DescriptorIndex, StreamFormat> deserializeSetStreamFormatCommand(const AemAecpdu::Payload& payload);

/** SET_STREAM_FORMAT Response - Clause 7.4.9.1 */
Serializer<AECP_AEM_SET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE> serializeSetStreamFormatResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamFormat const streamFormat);
std::tuple<DescriptorType, DescriptorIndex, StreamFormat> deserializeSetStreamFormatResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_STREAM_FORMAT Command - Clause 7.4.10.1 */
Serializer<AECP_AEM_GET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE> serializeGetStreamFormatCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeGetStreamFormatCommand(const AemAecpdu::Payload& payload);

/** GET_STREAM_FORMAT Response - Clause 7.4.10.2 */
Serializer<AECP_AEM_GET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE> serializeGetStreamFormatResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, StreamFormat const streamFormat);
std::tuple<DescriptorType, DescriptorIndex, StreamFormat> deserializeGetStreamFormatResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** SET_STREAM_INFO Command - Clause 7.4.15.1 */
Serializer<AECP_AEM_SET_STREAM_INFO_COMMAND_PAYLOAD_SIZE> serializeSetStreamInfoCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const StreamInfo& streamInfo);
std::tuple<DescriptorType, DescriptorIndex, StreamInfo> deserializeSetStreamInfoCommand(const AemAecpdu::Payload& payload);

/** SET_STREAM_INFO Response - Clause 7.4.15.1 */
Serializer<AECP_AEM_SET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE> serializeSetStreamInfoResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const StreamInfo& streamInfo);
std::tuple<DescriptorType, DescriptorIndex, StreamInfo> deserializeSetStreamInfoResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_STREAM_INFO Command - Clause 7.4.16.1 */
Serializer<AECP_AEM_GET_STREAM_INFO_COMMAND_PAYLOAD_SIZE> serializeGetStreamInfoCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeGetStreamInfoCommand(const AemAecpdu::Payload& payload);

/** GET_STREAM_INFO Response - Clause 7.4.16.2 */
Serializer<AECP_AEM_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE> serializeGetStreamInfoResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const StreamInfo& streamInfo);
std::tuple<DescriptorType, DescriptorIndex, StreamInfo> deserializeGetStreamInfoResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** SET_NAME Command - Clause 7.4.17.1 */
Serializer<AECP_AEM_SET_NAME_COMMAND_PAYLOAD_SIZE> serializeSetNameCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex, const AtdeccFixedString& name);
std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex, AtdeccFixedString> deserializeSetNameCommand(const AemAecpdu::Payload& payload);

/** SET_NAME Response - Clause 7.4.17.1 */
Serializer<AECP_AEM_SET_NAME_RESPONSE_PAYLOAD_SIZE> serializeSetNameResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex, const AtdeccFixedString& name);
std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex, AtdeccFixedString> deserializeSetNameResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_NAME Command - Clause 7.4.18.1 */
Serializer<AECP_AEM_GET_NAME_COMMAND_PAYLOAD_SIZE> serializeGetNameCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex);
std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex> deserializeGetNameCommand(const AemAecpdu::Payload& payload);

/** GET_NAME Response - Clause 7.4.18.2 */
Serializer<AECP_AEM_GET_NAME_RESPONSE_PAYLOAD_SIZE> serializeGetNameResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, std::uint16_t const nameIndex, ConfigurationIndex const configurationIndex, const AtdeccFixedString& name);
std::tuple<DescriptorType, DescriptorIndex, std::uint16_t, ConfigurationIndex, AtdeccFixedString> deserializeGetNameResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** SET_ASSOCIATION_ID Command - Clause 7.4.19.1 */
Serializer<AECP_AEM_SET_ASSOCIATION_ID_COMMAND_PAYLOAD_SIZE> serializeSetAssociationIDCommand(UniqueIdentifier const associationID);
std::tuple<UniqueIdentifier> deserializeSetAssociationIDCommand(const AemAecpdu::Payload& payload);

/** SET_ASSOCIATION_ID Response - Clause 7.4.19.1 */
Serializer<AECP_AEM_SET_ASSOCIATION_ID_RESPONSE_PAYLOAD_SIZE> serializeSetAssociationIDResponse(UniqueIdentifier const associationID);
std::tuple<UniqueIdentifier> deserializeSetAssociationIDResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** SET_SAMPLING_RATE Command - Clause 7.4.21.1 */
Serializer<AECP_AEM_SET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE> serializeSetSamplingRateCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, SamplingRate const samplingRate);
std::tuple<DescriptorType, DescriptorIndex, SamplingRate> deserializeSetSamplingRateCommand(const AemAecpdu::Payload& payload);

/** SET_SAMPLING_RATE Response - Clause 7.4.21.1 */
Serializer<AECP_AEM_SET_SAMPLING_RATE_RESPONSE_PAYLOAD_SIZE> serializeSetSamplingRateResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, SamplingRate const samplingRate);
std::tuple<DescriptorType, DescriptorIndex, SamplingRate> deserializeSetSamplingRateResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_SAMPLING_RATE Command - Clause 7.4.22.1 */
Serializer<AECP_AEM_GET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE> serializeGetSamplingRateCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeGetSamplingRateCommand(const AemAecpdu::Payload& payload);

/** GET_SAMPLING_RATE Response - Clause 7.4.22.2 */
Serializer<AECP_AEM_GET_SAMPLING_RATE_RESPONSE_PAYLOAD_SIZE> serializeGetSamplingRateResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, SamplingRate const samplingRate);
std::tuple<DescriptorType, DescriptorIndex, SamplingRate> deserializeGetSamplingRateResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** SET_CLOCK_SOURCE Command - Clause 7.4.23.1 */
Serializer<AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE> serializeSetClockSourceCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ClockSourceIndex const clockSourceIndex);
std::tuple<DescriptorType, DescriptorIndex, ClockSourceIndex> deserializeSetClockSourceCommand(const AemAecpdu::Payload& payload);

/** SET_CLOCK_SOURCE Response - Clause 7.4.23.1 */
Serializer<AECP_AEM_SET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE> serializeSetClockSourceResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ClockSourceIndex const clockSourceIndex);
std::tuple<DescriptorType, DescriptorIndex, ClockSourceIndex> deserializeSetClockSourceResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_CLOCK_SOURCE Command - Clause 7.4.24.1 */
Serializer<AECP_AEM_GET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE> serializeGetClockSourceCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeGetClockSourceCommand(const AemAecpdu::Payload& payload);

/** GET_CLOCK_SOURCE Response - Clause 7.4.24.2 */
Serializer<AECP_AEM_GET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE> serializeGetClockSourceResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, ClockSourceIndex const clockSourceIndex);
std::tuple<DescriptorType, DescriptorIndex, ClockSourceIndex> deserializeGetClockSourceResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** SET_CONTROL Command - Clause 7.4.25.1 */
Serializer<AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE> serializeSetControlCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const ControlValues& controlValues);
std::tuple<DescriptorType, DescriptorIndex, MemoryBuffer> deserializeSetControlCommand(const AemAecpdu::Payload& payload);

/** SET_CONTROL Response - Clause 7.4.25.1 */
Serializer<AECP_AEM_SET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE> serializeSetControlResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const ControlValues& controlValues);
std::tuple<DescriptorType, DescriptorIndex, MemoryBuffer> deserializeSetControlResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_CONTROL Command - Clause 7.4.26.1 */
Serializer<AECP_AEM_GET_CONTROL_COMMAND_PAYLOAD_SIZE> serializeGetControlCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeGetControlCommand(const AemAecpdu::Payload& payload);

/** GET_CONTROL Response - Clause 7.4.26.2 */
Serializer<AECP_AEM_GET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetControlResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const ControlValues& controlValues);
std::tuple<DescriptorType, DescriptorIndex, MemoryBuffer> deserializeGetControlResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** START_STREAMING Command - Clause 7.4.35.1 */
Serializer<AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE> serializeStartStreamingCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeStartStreamingCommand(const AemAecpdu::Payload& payload);

/** START_STREAMING Response - Clause 7.4.35.1 */
Serializer<AECP_AEM_START_STREAMING_RESPONSE_PAYLOAD_SIZE> serializeStartStreamingResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeStartStreamingResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** STOP_STREAMING Command - Clause 7.4.36.1 */
Serializer<AECP_AEM_STOP_STREAMING_COMMAND_PAYLOAD_SIZE> serializeStopStreamingCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeStopStreamingCommand(const AemAecpdu::Payload& payload);

/** STOP_STREAMING Response - Clause 7.4.36.1 */
Serializer<AECP_AEM_STOP_STREAMING_RESPONSE_PAYLOAD_SIZE> serializeStopStreamingResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeStopStreamingResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** REGISTER_UNSOLICITED_NOTIFICATION Command - Clause 7.4.37.1 */
// No payload

/** REGISTER_UNSOLICITED_NOTIFICATION Response - Clause 7.4.37.1 */
// No payload

/** DEREGISTER_UNSOLICITED_NOTIFICATION Command - Clause 7.4.38.1 */
// No payload

/** DEREGISTER_UNSOLICITED_NOTIFICATION Response - Clause 7.4.38.1 */
// No payload

/** GET_AVB_INFO Command - Clause 7.4.40.1 */
Serializer<AECP_AEM_GET_AVB_INFO_COMMAND_PAYLOAD_SIZE> serializeGetAvbInfoCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeGetAvbInfoCommand(const AemAecpdu::Payload& payload);

/** GET_AVB_INFO Response - Clause 7.4.40.2 */
Serializer<AECP_AEM_GET_AVB_INFO_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetAvbInfoResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const AvbInfo& avbInfo);
std::tuple<DescriptorType, DescriptorIndex, AvbInfo> deserializeGetAvbInfoResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_AS_PATH Command - Clause 7.4.41.1 */
Serializer<AECP_AEM_GET_AS_PATH_COMMAND_PAYLOAD_SIZE> serializeGetAsPathCommand(DescriptorIndex const descriptorIndex);
std::tuple<DescriptorIndex> deserializeGetAsPathCommand(const AemAecpdu::Payload& payload);

/** GET_AS_PATH Response - Clause 7.4.41.2 */
Serializer<AECP_AEM_GET_AS_PATH_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetAsPathResponse(DescriptorIndex const descriptorIndex, const AsPath& asPath);
std::tuple<DescriptorIndex, AsPath> deserializeGetAsPathResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_COUNTERS Command - Clause 7.4.42.1 */
Serializer<AECP_AEM_GET_COUNTERS_COMMAND_PAYLOAD_SIZE> serializeGetCountersCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeGetCountersCommand(const AemAecpdu::Payload& payload);

/** GET_COUNTERS Response - Clause 7.4.42.2 */
Serializer<AECP_AEM_GET_COUNTERS_RESPONSE_PAYLOAD_SIZE> serializeGetCountersResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const DescriptorCounterValidFlag validCounters, const DescriptorCounters& counters);
std::tuple<DescriptorType, DescriptorIndex, DescriptorCounterValidFlag, DescriptorCounters> deserializeGetCountersResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** REBOOT Command - Clause 7.4.43.1 */
Serializer<AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE> serializeRebootCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeRebootCommand(const AemAecpdu::Payload& payload);

/** REBOOT Response - Clause 7.4.43.1 */
Serializer<AECP_AEM_REBOOT_RESPONSE_PAYLOAD_SIZE> serializeRebootResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex);
std::tuple<DescriptorType, DescriptorIndex> deserializeRebootResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_AUDIO_MAP Command - Clause 7.4.44.1 */
Serializer<AECP_AEM_GET_AUDIO_MAP_COMMAND_PAYLOAD_SIZE> serializeGetAudioMapCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, MapIndex const mapIndex);
std::tuple<DescriptorType, DescriptorIndex, MapIndex> deserializeGetAudioMapCommand(const AemAecpdu::Payload& payload);

/** GET_AUDIO_MAP Response - Clause 7.4.44.2 */
Serializer<AECP_AEM_GET_AUDIO_MAP_RESPONSE_PAYLOAD_MIN_SIZE> serializeGetAudioMapResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, MapIndex const mapIndex, MapIndex const numberOfMaps, const AudioMapping& mappings);
std::tuple<DescriptorType, DescriptorIndex, MapIndex, MapIndex, AudioMapping> deserializeGetAudioMapResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** ADD_AUDIO_MAPPINGS Command - Clause 7.4.45.1 */
Serializer<AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE> serializeAddAudioMappingsCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const AudioMapping& mappings);
std::tuple<DescriptorType, DescriptorIndex, AudioMappings> deserializeAddAudioMappingsCommand(const AemAecpdu::Payload& payload);

/** ADD_AUDIO_MAPPINGS Response - Clause 7.4.45.1 */
Serializer<AECP_AEM_ADD_AUDIO_MAPPINGS_RESPONSE_PAYLOAD_MIN_SIZE> serializeAddAudioMappingsResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const AudioMapping& mappings);
std::tuple<DescriptorType, DescriptorIndex, AudioMappings> deserializeAddAudioMappingsResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** REMOVE_AUDIO_MAPPINGS Command - Clause 7.4.46.1 */
Serializer<AECP_AEM_REMOVE_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE> serializeRemoveAudioMappingsCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const AudioMapping& mappings);
std::tuple<DescriptorType, DescriptorIndex, AudioMappings> deserializeRemoveAudioMappingsCommand(const AemAecpdu::Payload& payload);

/** REMOVE_AUDIO_MAPPINGS Response - Clause 7.4.46.1 */
Serializer<AECP_AEM_REMOVE_AUDIO_MAPPINGS_RESPONSE_PAYLOAD_MIN_SIZE> serializeRemoveAudioMappingsResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, const AudioMapping& mappings);
std::tuple<DescriptorType, DescriptorIndex, AudioMappings> deserializeRemoveAudioMappingsResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** START_OPERATION Command - Clause 7.4.53.1 */
Serializer<AECP_AEM_START_OPERATION_COMMAND_PAYLOAD_MIN_SIZE> serializeStartOperationCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID, MemoryObjectOperationType const operationType, const MemoryBuffer& memoryBuffer);
std::tuple<DescriptorType, DescriptorIndex, OperationID, MemoryObjectOperationType, MemoryBuffer> deserializeStartOperationCommand(const AemAecpdu::Payload& payload);

/** START_OPERATION Response - Clause 7.4.53.1 */
Serializer<AECP_AEM_START_OPERATION_RESPONSE_PAYLOAD_MIN_SIZE> serializeStartOperationResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID, MemoryObjectOperationType const operationType, const MemoryBuffer& memoryBuffer);
std::tuple<DescriptorType, DescriptorIndex, OperationID, MemoryObjectOperationType, MemoryBuffer> deserializeStartOperationResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** ABORT_OPERATION Command - Clause 7.4.55.1 */
Serializer<AECP_AEM_ABORT_OPERATION_COMMAND_PAYLOAD_SIZE> serializeAbortOperationCommand(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID);
std::tuple<DescriptorType, DescriptorIndex, OperationID> deserializeAbortOperationCommand(const AemAecpdu::Payload& payload);

/** ABORT_OPERATION Response - Clause 7.4.55.1 */
Serializer<AECP_AEM_ABORT_OPERATION_RESPONSE_PAYLOAD_SIZE> serializeAbortOperationResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID);
std::tuple<DescriptorType, DescriptorIndex, OperationID> deserializeAbortOperationResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** OPERATION_STATUS Unsolicited Response - Clause 7.4.55.1 */
Serializer<AECP_AEM_OPERATION_STATUS_RESPONSE_PAYLOAD_SIZE> serializeOperationStatusResponse(DescriptorType const descriptorType, DescriptorIndex const descriptorIndex, OperationID const operationID, std::uint16_t const percentComplete);
std::tuple<DescriptorType, DescriptorIndex, OperationID, std::uint16_t> deserializeOperationStatusResponse(const AemAecpdu::Payload& payload);

/** SET_MEMORY_OBJECT_LENGTH Command - Clause 7.4.72.1 */
Serializer<AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE> serializeSetMemoryObjectLengthCommand(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex, std::uint64_t const length);
std::tuple<ConfigurationIndex, MemoryObjectIndex, std::uint64_t> deserializeSetMemoryObjectLengthCommand(const AemAecpdu::Payload& payload);

/** SET_MEMORY_OBJECT_LENGTH Response - Clause 7.4.72.1 */
Serializer<AECP_AEM_SET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE> serializeSetMemoryObjectLengthResponse(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex, std::uint64_t const length);
std::tuple<ConfigurationIndex, MemoryObjectIndex, std::uint64_t> deserializeSetMemoryObjectLengthResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

/** GET_MEMORY_OBJECT_LENGTH Command - Clause 7.4.73.1 */
Serializer<AECP_AEM_GET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE> serializeGetMemoryObjectLengthCommand(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex);
std::tuple<ConfigurationIndex, MemoryObjectIndex> deserializeGetMemoryObjectLengthCommand(const AemAecpdu::Payload& payload);

/** GET_MEMORY_OBJECT_LENGTH Response - Clause 7.4.73.2 */
Serializer<AECP_AEM_GET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE> serializeGetMemoryObjectLengthResponse(ConfigurationIndex const configurationIndex, MemoryObjectIndex const memoryObjectIndex, std::uint64_t const length);
std::tuple<ConfigurationIndex, MemoryObjectIndex, std::uint64_t> deserializeGetMemoryObjectLengthResponse(AemCommandStatus const status, const AemAecpdu::Payload& payload);

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMPAYLOADS_HPP_ */
