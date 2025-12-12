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

template <typename MessageType>
struct DronecanPublisherTraits;

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

template <typename MessageType>
class DronecanPublisher {
public:
    DronecanPublisher() = default;

    inline void publish() {
        DronecanPublisherTraits<MessageType>::publish_once(msg, &inout_transfer_id);
        inout_transfer_id++;
    }

    MessageType msg;
private:
    uint8_t inout_transfer_id;
};


template <typename MessageType>
class DronecanPeriodicPublisher : public DronecanPublisher<MessageType> {
public:
    DronecanPeriodicPublisher(float frequency) :
        DronecanPublisher<MessageType>(),
        PUB_PERIOD_MS(static_cast<uint32_t>(1000.0f / std::clamp(frequency, 0.001f, 1000.0f))) {};

    inline void spinOnce() {
        auto crnt_time_ms = platform.getTimeMs();
        if (crnt_time_ms < next_pub_time_ms) {
            return;
        }
        next_pub_time_ms = crnt_time_ms + PUB_PERIOD_MS;

        this->publish();
    }

private:
    const uint32_t PUB_PERIOD_MS;
    uint32_t next_pub_time_ms{500};
};

#endif // LIBDCNODE_PUB_HPP_