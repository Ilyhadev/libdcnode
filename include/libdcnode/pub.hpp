<<<<<<< HEAD
#ifndef LIBDCNODE_PUB_HPP_
#define LIBDCNODE_PUB_HPP_

// Hack
// It's ok to keep some undefs for now, but maybe there is a better solution;
// This hack won't be necessary after removing legacy serialization
#undef UAVCAN_EQUIPMENT_ACTUATOR_COMMAND_SIGNATURE
#undef UAVCAN_EQUIPMENT_ACTUATOR_STATUS_SIGNATURE
#undef UAVCAN_EQUIPMENT_AHRS_SOLUTION_SIGNATURE
#undef UAVCAN_EQUIPMENT_ESC_RAWCOMMAND_SIGNATURE
#undef UAVCAN_EQUIPMENT_HARDPOINT_COMMAND_SIGNATURE
#undef UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_SIGNATURE
#undef UAVCAN_EQUIPMENT_INDICATION_BEEPCOMMAND_SIGNATURE

#include <stdint.h>
#include <algorithm>
#include "libdcnode/dronecan.h"
#include "serialization/uavcan/include/dronecan_msgs.h"

// extern PlatformApi platform;
=======
/*
 * Copyright (C) 2025 Dmitry Ponomarev <ponomarevda96@gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef LIBDCNODE_PUB_HPP_
#define LIBDCNODE_PUB_HPP_

#include <cstdint>
#include <algorithm>
#include <type_traits>
#include <utility>
#include "libdcnode/dronecan.h"
#include "dronecan_msgs.h"

// Max encoded message size (bytes) for publisher stack buffer.
// Override via -DLIBDCNODE_MAX_PUB_MESSAGE_SIZE=... if your DSDL set requires more.
#ifndef LIBDCNODE_MAX_PUB_MESSAGE_SIZE
#define LIBDCNODE_MAX_PUB_MESSAGE_SIZE 250U
#endif

// Initial delay before first publish after boot/reset (ms).
// Intended to reduce CAN bus flooding during rapid reboot loops (e.g., watchdog resets).
// Override via -DLIBDCNODE_INITIAL_PUB_DELAY_MS=...
#ifndef LIBDCNODE_INITIAL_PUB_DELAY_MS
#define LIBDCNODE_INITIAL_PUB_DELAY_MS 500U
#endif

// @todo we should not make it global; retrieve it from the library itself later
extern PlatformApi platform;

namespace libdcnode {
>>>>>>> upstream/pr-dsdlgeneration

template <typename MessageType>
struct DronecanPublisherTraits;

<<<<<<< HEAD
#define PUBLISHER_ENCODE(MessageType) MessageType##_encode
#define PUBLISHER_BROADCAST(buffer, Prefix, inout_transfer_id, size) uavcanPublish(Prefix##_SIGNATURE, Prefix##_ID, inout_transfer_id, CANARD_TRANSFER_PRIORITY_MEDIUM, buffer, size)
#define DEFINE_PUBLISHER_TRAITS(MessageType, MessagePrefix) \
template <> \
struct DronecanPublisherTraits<MessageType> { \
    static inline int8_t publish_once(MessageType* msg, uint8_t* inout_transfer_id) { \
        uint8_t buffer[512]; \
        uint32_t bytes_needed = PUBLISHER_ENCODE(MessageType)(msg, buffer); \
        if (bytes_needed == 0) return 0; \
        auto can_frames = PUBLISHER_BROADCAST(buffer, MessagePrefix, inout_transfer_id, bytes_needed);\
        return can_frames;\
    } \
};


// DEFINE_PUBLISHER_TRAITS(uavcan_equipment_power_CircuitStatus, UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS)
// DEFINE_PUBLISHER_TRAITS(uavcan_equipment_power_BatteryInfo, UAVCAN_EQUIPMENT_POWER_BATTERYINFO)
=======
#define LIBDCNODE_PUB_ENCODE(MessageType) MessageType##_encode
#define LIBDCNODE_PUB_BROADCAST(buffer, Prefix, inout_transfer_id, size) uavcanPublish(Prefix##_SIGNATURE, Prefix##_ID, inout_transfer_id, CANARD_TRANSFER_PRIORITY_MEDIUM, buffer, size)
#define LIBDCNODE_DEFINE_PUB_TRAITS(MessageType, MessagePrefix) \
template <> \
struct DronecanPublisherTraits<MessageType> { \
    using EncodeRet = decltype(LIBDCNODE_PUB_ENCODE(MessageType)(std::declval<MessageType*>(), std::declval<std::uint8_t*>())); \
    static_assert(std::is_same_v<EncodeRet, std::uint32_t>, "*_encode() must return uint32_t exactly."); \
    static inline int16_t publish_once(MessageType* msg, uint8_t* inout_transfer_id) { \
        uint8_t buffer[LIBDCNODE_MAX_PUB_MESSAGE_SIZE]; \
        auto bytes_needed = LIBDCNODE_PUB_ENCODE(MessageType)(msg, buffer); \
        if (bytes_needed == 0U || bytes_needed > LIBDCNODE_MAX_PUB_MESSAGE_SIZE) return (int16_t)0; \
        return LIBDCNODE_PUB_BROADCAST(buffer, MessagePrefix, inout_transfer_id, bytes_needed);\
    } \
};

LIBDCNODE_DEFINE_PUB_TRAITS(::uavcan_equipment_power_CircuitStatus, UAVCAN_EQUIPMENT_POWER_CIRCUITSTATUS)
LIBDCNODE_DEFINE_PUB_TRAITS(::uavcan_equipment_power_BatteryInfo, UAVCAN_EQUIPMENT_POWER_BATTERYINFO)

>>>>>>> upstream/pr-dsdlgeneration

template <typename MessageType>
class DronecanPublisher {
public:
    DronecanPublisher() = default;

    inline void publish() {
<<<<<<< HEAD
        DronecanPublisherTraits<MessageType>::publish_once(msg, &inout_transfer_id);
        inout_transfer_id++;
    }

    MessageType msg;
private:
    uint8_t inout_transfer_id;
=======
        DronecanPublisherTraits<MessageType>::publish_once(&msg, &inout_transfer_id);
        inout_transfer_id++;
    }

    MessageType msg{};
private:
    uint8_t inout_transfer_id{0};
>>>>>>> upstream/pr-dsdlgeneration
};


template <typename MessageType>
class DronecanPeriodicPublisher : public DronecanPublisher<MessageType> {
public:
<<<<<<< HEAD
    DronecanPeriodicPublisher(float frequency) :
        DronecanPublisher<MessageType>(),
        PUB_PERIOD_MS(static_cast<uint32_t>(1000.0f / std::clamp(frequency, 0.001f, 1000.0f))) {};

    inline void spinOnce() {
        auto crnt_time_ms = platform.getTimeMs();
        if (crnt_time_ms < next_pub_time_ms) {
            return;
        }
        next_pub_time_ms = crnt_time_ms + PUB_PERIOD_MS;
=======
    explicit DronecanPeriodicPublisher(float frequency) :
        DronecanPublisher<MessageType>(),
        _pub_period_ms(static_cast<uint32_t>(1000.0f / std::clamp(frequency, 0.001f, 1000.0f))) {}

    inline void spinOnce() {
        auto crnt_time_ms = platform.getTimeMs();
        if (crnt_time_ms < _next_pub_time_ms) {
            return;
        }
        _next_pub_time_ms = crnt_time_ms + _pub_period_ms;
>>>>>>> upstream/pr-dsdlgeneration

        this->publish();
    }

private:
<<<<<<< HEAD
    const uint32_t PUB_PERIOD_MS;
    uint32_t next_pub_time_ms{500};
};

#endif // LIBDCNODE_PUB_HPP_
=======
    const uint32_t _pub_period_ms;
    uint32_t _next_pub_time_ms{LIBDCNODE_INITIAL_PUB_DELAY_MS};
};

}  // namespace libdcnode

// Undef internal macros
#undef LIBDCNODE_PUB_ENCODE
#undef LIBDCNODE_PUB_BROADCAST
#undef LIBDCNODE_DEFINE_PUB_TRAITS

#endif  // LIBDCNODE_PUB_HPP_
>>>>>>> upstream/pr-dsdlgeneration
