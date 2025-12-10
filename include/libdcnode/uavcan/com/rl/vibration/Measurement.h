
typedef struct dsdl_com_rl_vibration_Measurement VibrationMeasurement_t;

#define COM_RL_VIBRATION_MEASUREMENT_ID        DSDL_COM_RL_VIBRATION_MEASUREMENT_ID
#define COM_RL_VIBRATION_MEASUREMENT_SIGNATURE DSDL_COM_RL_VIBRATION_MEASUREMENT_SIGNATURE
#define COM_RL_VIBRATION_MEASUREMENT_MAX_SIZE  DSDL_COM_RL_VIBRATION_MEASUREMENT_MAX_SIZE

#include "libdcnode/serialization/uavcan/include/dsdl.com.rl.vibration.Measurement.h"
#include "libdcnode/dronecan.h"
#include "libdcnode/serialization_internal.h"

static inline int8_t com_rl_vibration_measurement_serialize(
    const VibrationMeasurement_t* const obj,
    uint8_t* const buffer,
    size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL))
        return -2;
    if (*inout_buffer_size_bytes < COM_RL_VIBRATION_MEASUREMENT_MAX_SIZE)
        return -3;
    
    uint32_t byte_count = dsdl_com_rl_vibration_Measurement_encode(
        (struct dsdl_com_rl_vibration_Measurement*)obj, buffer
#if CANARD_ENABLE_TAO_OPTION
        , true
#endif
    );
    
    *inout_buffer_size_bytes = byte_count;
    return 0;
}


static inline int8_t com_rl_vibration_measurement_deserialize(
    const CanardRxTransfer* transfer,
    VibrationMeasurement_t* msg)
{
    if ((transfer == NULL) || (msg == NULL))
        return -2;
    
    bool decode_failed = dsdl_com_rl_vibration_Measurement_decode(transfer, msg);
    return decode_failed ? -1 : 0;
}

// Publish function
static inline int8_t dronecan_com_rl_vibration_measurement_publish(
    const VibrationMeasurement_t* const obj,
    uint8_t* inout_transfer_id)
{
    if ((obj == NULL) || (inout_transfer_id == NULL))
        return -2;

    uint8_t buffer[COM_RL_VIBRATION_MEASUREMENT_MAX_SIZE];
    size_t buffer_size = COM_RL_VIBRATION_MEASUREMENT_MAX_SIZE;

    int8_t res = com_rl_vibration_measurement_serialize(obj, buffer, &buffer_size);
    if (res != 0)
        return res;

    uavcanPublish(COM_RL_VIBRATION_MEASUREMENT_SIGNATURE,
                  COM_RL_VIBRATION_MEASUREMENT_ID,
                  inout_transfer_id,
                  CANARD_TRANSFER_PRIORITY_MEDIUM,
                  buffer, buffer_size);
    return 0;
}
