#ifndef COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMPAYLOADSIZES_HPP_
#define COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMPAYLOADSIZES_HPP_

#pragma once

#include "protocolAecpdu.hpp"
#include "protocolAemAecpdu.hpp"

/** ACQUIRE_ENTITY Command and Response - Clause 7.4.1.1 */
constexpr size_t AECP_AEM_ACQUIRE_ENTITY_COMMAND_PAYLOAD_SIZE = 16u;
constexpr size_t AECP_AEM_ACQUIRE_ENTITY_RESPONSE_PAYLOAD_SIZE = 16u;

/** LOCK_ENTITY Command and Response - Clause 7.4.2.1 */
constexpr size_t AECP_AEM_LOCK_ENTITY_COMMAND_PAYLOAD_SIZE = 16u;
constexpr size_t AECP_AEM_LOCK_ENTITY_RESPONSE_PAYLOAD_SIZE = 16u;

/** ENTITY_AVAILABLE Command and Response - Clause 7.4.3.1 */
constexpr size_t AECP_AEM_ENTITY_AVAILABLE_COMMAND_PAYLOAD_SIZE = 0u;
constexpr size_t AECP_AEM_ENTITY_AVAILABLE_RESPONSE_PAYLOAD_SIZE = 0u;

/** CONTROLLER_AVAILABLE Command and Response - Clause 7.4.4.1 */
constexpr size_t AECP_AEM_CONTROLLER_AVAILABLE_COMMAND_PAYLOAD_SIZE = 0u;
constexpr size_t AECP_AEM_CONTROLLER_AVAILABLE_RESPONSE_PAYLOAD_SIZE = 0u;

/** READ_DESCRIPTOR Command - Clause 7.4.5.1 */
constexpr size_t AECP_AEM_READ_DESCRIPTOR_COMMAND_PAYLOAD_SIZE = 8u;

/** READ_DESCRIPTOR Response - Clause 7.4.5.2 */
constexpr size_t AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = 8u;
constexpr size_t AECP_AEM_READ_ENTITY_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 308u;
constexpr size_t AECP_AEM_READ_CONFIGURATION_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 70u;
constexpr size_t AECP_AEM_READ_AUDIO_UNIT_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 140u;
constexpr size_t AECP_AEM_READ_STREAM_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 128u;
constexpr size_t AECP_AEM_READ_JACK_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 74u;
constexpr size_t AECP_AEM_READ_AVB_INTERFACE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 94u;
constexpr size_t AECP_AEM_READ_CLOCK_SOURCE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 82u;
constexpr size_t AECP_AEM_READ_MEMORY_OBJECT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 96u;
constexpr size_t AECP_AEM_READ_LOCALE_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 68u;
constexpr size_t AECP_AEM_READ_STRINGS_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 448u;
constexpr size_t AECP_AEM_READ_STREAM_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 16u;
constexpr size_t AECP_AEM_READ_EXTERNAL_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 20u;
constexpr size_t AECP_AEM_READ_INTERNAL_PORT_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 20u;
constexpr size_t AECP_AEM_READ_AUDIO_CLUSTER_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 83u;
constexpr size_t AECP_AEM_READ_AUDIO_MAP_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 4u;
constexpr size_t AECP_AEM_READ_CONTROL_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 100u;
constexpr size_t AECP_AEM_READ_SIGNAL_SELECTOR_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 92u;
constexpr size_t AECP_AEM_READ_MIXER_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 84u;
constexpr size_t AECP_AEM_READ_MATRIX_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 98u;
constexpr size_t AECP_AEM_READ_MATRIX_SIGNAL_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 4u;
constexpr size_t AECP_AEM_READ_SIGNAL_SPLITTER_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 88u;
constexpr size_t AECP_AEM_READ_SIGNAL_COMBINER_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 84u;
constexpr size_t AECP_AEM_READ_SIGNAL_DEMULTIPLEXER_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 88u;
constexpr size_t AECP_AEM_READ_SIGNAL_MULTIPLEXER_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 84u;
constexpr size_t AECP_AEM_READ_SIGNAL_TRANSCODER_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 88u;
constexpr size_t AECP_AEM_READ_CLOCK_DOMAIN_DESCRIPTOR_RESPONSE_PAYLOAD_MIN_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 72u;
constexpr size_t AECP_AEM_READ_CONTROL_BLOCK_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE = AECP_AEM_READ_COMMON_DESCRIPTOR_RESPONSE_PAYLOAD_SIZE + 72u;

/** SET_CONFIGURATION Command and Response - Clause 7.4.7.1 */
constexpr size_t AECP_AEM_SET_CONFIGURATION_COMMAND_PAYLOAD_SIZE = 4u;
constexpr size_t AECP_AEM_SET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE = 4u;

/** GET_CONFIGURATION Command - Clause 7.4.8.1 */
constexpr size_t AECP_AEM_GET_CONFIGURATION_COMMAND_PAYLOAD_SIZE = 0u;

/** GET_CONFIGURATION Response - Clause 7.4.8.2 */
constexpr size_t AECP_AEM_GET_CONFIGURATION_RESPONSE_PAYLOAD_SIZE = 4u;

/** SET_STREAM_FORMAT Command and Response - Clause 7.4.9.1 */
constexpr size_t AECP_AEM_SET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE = 12u;
constexpr size_t AECP_AEM_SET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE = 12u;

/** GET_STREAM_FORMAT Command - Clause 7.4.10.1 */
constexpr size_t AECP_AEM_GET_STREAM_FORMAT_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_STREAM_FORMAT Response - Clause 7.4.10.2 */
constexpr size_t AECP_AEM_GET_STREAM_FORMAT_RESPONSE_PAYLOAD_SIZE = 12u;

/** SET_STREAM_INFO Command and Response - Clause 7.4.15.1 */
constexpr size_t AECP_AEM_SET_STREAM_INFO_COMMAND_PAYLOAD_SIZE = 48u;
constexpr size_t AECP_AEM_SET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE = 48u;

/** GET_STREAM_INFO Command - Clause 7.4.16.1 */
constexpr size_t AECP_AEM_GET_STREAM_INFO_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_STREAM_INFO Response - Clause 7.4.16.2 */
constexpr size_t AECP_AEM_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE = 48u;

/** GET_STREAM_INFO Response - Milan Clause 7.3.10 */
constexpr size_t AECP_AEM_MILAN_GET_STREAM_INFO_RESPONSE_PAYLOAD_SIZE = 56u;

/** SET_NAME Command and Response - Clause 7.4.17.1 */
constexpr size_t AECP_AEM_SET_NAME_COMMAND_PAYLOAD_SIZE = 72u;
constexpr size_t AECP_AEM_SET_NAME_RESPONSE_PAYLOAD_SIZE = 72u;

/** GET_NAME Command - Clause 7.4.18.1 */
constexpr size_t AECP_AEM_GET_NAME_COMMAND_PAYLOAD_SIZE = 8u;

/** GET_NAME Response - Clause 7.4.18.2 */
constexpr size_t AECP_AEM_GET_NAME_RESPONSE_PAYLOAD_SIZE = 72u;

/** SET_ASSOCIATION_ID Command and Response - Clause 7.4.19.1 */
constexpr size_t AECP_AEM_SET_ASSOCIATION_ID_COMMAND_PAYLOAD_SIZE = 8u;
constexpr size_t AECP_AEM_SET_ASSOCIATION_ID_RESPONSE_PAYLOAD_SIZE = 8u;

/** GET_ASSOCIATION_ID Command - Clause 7.4.20.1 */
constexpr size_t AECP_AEM_GET_ASSOCIATION_ID_COMMAND_PAYLOAD_SIZE = 0u;

/** GET_ASSOCIATION_ID Response - Clause 7.4.20.2 */
constexpr size_t AECP_AEM_GET_ASSOCIATION_ID_RESPONSE_PAYLOAD_SIZE = 8u;

/** SET_SAMPLING_RATE Command and Response - Clause 7.4.21.1 */
constexpr size_t AECP_AEM_SET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE = 8u;
constexpr size_t AECP_AEM_SET_SAMPLING_RATE_RESPONSE_PAYLOAD_SIZE = 8u;

/** GET_SAMPLING_RATE Command - Clause 7.4.22.1 */
constexpr size_t AECP_AEM_GET_SAMPLING_RATE_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_SAMPLING_RATE Response - Clause 7.4.22.2 */
constexpr size_t AECP_AEM_GET_SAMPLING_RATE_RESPONSE_PAYLOAD_SIZE = 8u;

/** SET_CLOCK_SOURCE Command and Response - Clause 7.4.23.1 */
constexpr size_t AECP_AEM_SET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE = 8u;
constexpr size_t AECP_AEM_SET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE = 8u;

/** GET_CLOCK_SOURCE Command - Clause 7.4.24.1 */
constexpr size_t AECP_AEM_GET_CLOCK_SOURCE_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_CLOCK_SOURCE Response - Clause 7.4.24.2 */
constexpr size_t AECP_AEM_GET_CLOCK_SOURCE_RESPONSE_PAYLOAD_SIZE = 8u;

/** SET_CONTROL Command and Response - Clause 7.4.25.1 */
constexpr size_t AECP_AEM_SET_CONTROL_COMMAND_PAYLOAD_MIN_SIZE = 4u;
constexpr size_t AECP_AEM_SET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE = 4u;

/** GET_CONTROL Command - Clause 7.4.26.1 */
constexpr size_t AECP_AEM_GET_CONTROL_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_CONTROL Response - Clause 7.4.26.2 */
constexpr size_t AECP_AEM_GET_CONTROL_RESPONSE_PAYLOAD_MIN_SIZE = 4u;

/** START_STREAMING Command and Response - Clause 7.4.35.1 */
constexpr size_t AECP_AEM_START_STREAMING_COMMAND_PAYLOAD_SIZE = 4u;
constexpr size_t AECP_AEM_START_STREAMING_RESPONSE_PAYLOAD_SIZE = 4u;

/** STOP_STREAMING Command and Response - Clause 7.4.36.1 */
constexpr size_t AECP_AEM_STOP_STREAMING_COMMAND_PAYLOAD_SIZE = 4u;
constexpr size_t AECP_AEM_STOP_STREAMING_RESPONSE_PAYLOAD_SIZE = 4u;

/** REGISTER_UNSOLICITED_NOTIFICATION Command and Response - Clause 7.4.37.1 */
constexpr size_t AECP_AEM_REGISTER_UNSOLICITED_NOTIFICATION_COMMAND_PAYLOAD_SIZE = 0u;
constexpr size_t AECP_AEM_REGISTER_UNSOLICITED_NOTIFICATION_RESPONSE_PAYLOAD_SIZE = 0u;

/** DEREGISTER_UNSOLICITED_NOTIFICATION Command and Response - Clause 7.4.38.1 */
constexpr size_t AECP_AEM_DEREGISTER_UNSOLICITED_NOTIFICATION_COMMAND_PAYLOAD_SIZE = 0u;
constexpr size_t AECP_AEM_DEREGISTER_UNSOLICITED_NOTIFICATION_RESPONSE_PAYLOAD_SIZE = 0u;

/** GET_AVB_INFO Command - Clause 7.4.40.1 */
constexpr size_t AECP_AEM_GET_AVB_INFO_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_AVB_INFO Response - Clause 7.4.40.2 */
constexpr size_t AECP_AEM_GET_AVB_INFO_RESPONSE_PAYLOAD_MIN_SIZE = 20u;

/** GET_AS_PATH Command - Clause 7.4.41.1 */
constexpr size_t AECP_AEM_GET_AS_PATH_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_AS_PATH Response - Clause 7.4.41.2 */
constexpr size_t AECP_AEM_GET_AS_PATH_RESPONSE_PAYLOAD_MIN_SIZE = 4u;

/** GET_COUNTERS Command - Clause 7.4.42.1 */
constexpr size_t AECP_AEM_GET_COUNTERS_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_COUNTERS Response - Clause 7.4.42.2 */
constexpr size_t AECP_AEM_GET_COUNTERS_RESPONSE_PAYLOAD_SIZE = 136u;

/** REBOOT Command - Clause 7.4.43.1 */
constexpr size_t AECP_AEM_REBOOT_COMMAND_PAYLOAD_SIZE = 4u;

/** REBOOT Response - Clause 7.4.43.2 */
constexpr size_t AECP_AEM_REBOOT_RESPONSE_PAYLOAD_SIZE = 4u;

/** GET_AUDIO_MAP Command - Clause 7.4.44.1 */
constexpr size_t AECP_AEM_GET_AUDIO_MAP_COMMAND_PAYLOAD_SIZE = 8u;

/** GET_AUDIO_MAP Response - Clause 7.4.44.2 */
constexpr size_t AECP_AEM_GET_AUDIO_MAP_RESPONSE_PAYLOAD_MIN_SIZE = 12u;

/** ADD_AUDIO_MAPPINGS Command - Clause 7.4.45.1 */
constexpr size_t AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE = 8u;

/** ADD_AUDIO_MAPPINGS Response - Clause 7.4.45.1 */
constexpr size_t AECP_AEM_ADD_AUDIO_MAPPINGS_RESPONSE_PAYLOAD_MIN_SIZE = 8u;

/** REMOVE_AUDIO_MAPPINGS Command - Clause 7.4.46.1 */
constexpr size_t AECP_AEM_REMOVE_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE = 8u;

/** REMOVE_AUDIO_MAPPINGS Response - Clause 7.4.46.1 */
constexpr size_t AECP_AEM_REMOVE_AUDIO_MAPPINGS_RESPONSE_PAYLOAD_MIN_SIZE = 8u;

/** START_OPERATION Command - Clause 7.4.53.1 */
constexpr size_t AECP_AEM_START_OPERATION_COMMAND_PAYLOAD_MIN_SIZE = 8u;

/** START_OPERATION Response - Clause 7.4.53.1 */
constexpr size_t AECP_AEM_START_OPERATION_RESPONSE_PAYLOAD_MIN_SIZE = 8u;

/** ABORT_OPERATION Command - Clause 7.4.54.1 */
constexpr size_t AECP_AEM_ABORT_OPERATION_COMMAND_PAYLOAD_SIZE = 8u;

/** ABORT_OPERATION Response - Clause 7.4.54.1 */
constexpr size_t AECP_AEM_ABORT_OPERATION_RESPONSE_PAYLOAD_SIZE = 8u;

/** OPERATION_STATUS Response - Clause 7.4.55.1 */
constexpr size_t AECP_AEM_OPERATION_STATUS_RESPONSE_PAYLOAD_SIZE = 8u;

/** SET_MEMORY_OBJECT_LENGTH Command - Clause 7.4.72.1 */
constexpr size_t AECP_AEM_SET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE = 12u;

/** SET_MEMORY_OBJECT_LENGTH Response - Clause 7.4.72.1 */
constexpr size_t AECP_AEM_SET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE = 12u;

/** GET_MEMORY_OBJECT_LENGTH Command - Clause 7.4.73.1 */
constexpr size_t AECP_AEM_GET_MEMORY_OBJECT_LENGTH_COMMAND_PAYLOAD_SIZE = 4u;

/** GET_MEMORY_OBJECT_LENGTH Response - Clause 7.4.73.2 */
constexpr size_t AECP_AEM_GET_MEMORY_OBJECT_LENGTH_RESPONSE_PAYLOAD_SIZE = 12u;

static_assert(AECP_AEM_ADD_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE == AECP_AEM_REMOVE_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE, "Add and Remove no longer the same size, we should split AECP_AEM_MAX_ADD_REMOVE_AUDIO_MAPPINGS in 2");
constexpr size_t AECP_AEM_MAX_ADD_REMOVE_AUDIO_MAPPINGS = (AemAecpdu::MAXIMUM_SEND_PAYLOAD_BUFFER_LENGTH - AECP_AEM_REMOVE_AUDIO_MAPPINGS_COMMAND_PAYLOAD_MIN_SIZE) / 8;

#endif /* COMPONENTS_ATDECC_INCLUDE_PROTOCOLAEMPAYLOADSIZES_HPP_ */
