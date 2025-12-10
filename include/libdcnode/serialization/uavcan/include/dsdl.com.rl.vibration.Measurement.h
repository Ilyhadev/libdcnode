#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <canard.h>


#define DSDL_COM_RL_VIBRATION_MEASUREMENT_MAX_SIZE 7
#define DSDL_COM_RL_VIBRATION_MEASUREMENT_SIGNATURE (0xCB6C085C090925A8ULL)
#define DSDL_COM_RL_VIBRATION_MEASUREMENT_ID 20105

#if defined(__cplusplus) && defined(DRONECAN_CXX_WRAPPERS)
class dsdl_com_rl_vibration_Measurement_cxx_iface;
#endif

struct dsdl_com_rl_vibration_Measurement {
#if defined(__cplusplus) && defined(DRONECAN_CXX_WRAPPERS)
    using cxx_iface = dsdl_com_rl_vibration_Measurement_cxx_iface;
#endif
    uint8_t id;
    float vibration_metrics;
    float dominant_frequency;
    float dominant_snr;
};

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t dsdl_com_rl_vibration_Measurement_encode(struct dsdl_com_rl_vibration_Measurement* msg, uint8_t* buffer
#if CANARD_ENABLE_TAO_OPTION
    , bool tao
#endif
);
bool dsdl_com_rl_vibration_Measurement_decode(const CanardRxTransfer* transfer, struct dsdl_com_rl_vibration_Measurement* msg);

#if defined(CANARD_DSDLC_INTERNAL)
static inline void _dsdl_com_rl_vibration_Measurement_encode(uint8_t* buffer, uint32_t* bit_ofs, struct dsdl_com_rl_vibration_Measurement* msg, bool tao);
static inline bool _dsdl_com_rl_vibration_Measurement_decode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct dsdl_com_rl_vibration_Measurement* msg, bool tao);
void _dsdl_com_rl_vibration_Measurement_encode(uint8_t* buffer, uint32_t* bit_ofs, struct dsdl_com_rl_vibration_Measurement* msg, bool tao) {
    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;

    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->id);
    *bit_ofs += 8;
    {
        uint16_t float16_val = canardConvertNativeFloatToFloat16(msg->vibration_metrics);
        canardEncodeScalar(buffer, *bit_ofs, 16, &float16_val);
    }
    *bit_ofs += 16;
    {
        uint16_t float16_val = canardConvertNativeFloatToFloat16(msg->dominant_frequency);
        canardEncodeScalar(buffer, *bit_ofs, 16, &float16_val);
    }
    *bit_ofs += 16;
    {
        uint16_t float16_val = canardConvertNativeFloatToFloat16(msg->dominant_snr);
        canardEncodeScalar(buffer, *bit_ofs, 16, &float16_val);
    }
    *bit_ofs += 16;
}

/*
 decode dsdl_com_rl_vibration_Measurement, return true on failure, false on success
*/
bool _dsdl_com_rl_vibration_Measurement_decode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct dsdl_com_rl_vibration_Measurement* msg, bool tao) {
    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;
    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->id);
    *bit_ofs += 8;

    {
        uint16_t float16_val;
        canardDecodeScalar(transfer, *bit_ofs, 16, true, &float16_val);
        msg->vibration_metrics = canardConvertFloat16ToNativeFloat(float16_val);
    }
    *bit_ofs += 16;

    {
        uint16_t float16_val;
        canardDecodeScalar(transfer, *bit_ofs, 16, true, &float16_val);
        msg->dominant_frequency = canardConvertFloat16ToNativeFloat(float16_val);
    }
    *bit_ofs += 16;

    {
        uint16_t float16_val;
        canardDecodeScalar(transfer, *bit_ofs, 16, true, &float16_val);
        msg->dominant_snr = canardConvertFloat16ToNativeFloat(float16_val);
    }
    *bit_ofs += 16;

    return false; /* success */
}
#endif
#ifdef CANARD_DSDLC_TEST_BUILD
struct dsdl_com_rl_vibration_Measurement sample_dsdl_com_rl_vibration_Measurement_msg(void);
#endif
#ifdef __cplusplus
} // extern "C"

#ifdef DRONECAN_CXX_WRAPPERS
#include <canard/cxx_wrappers.h>
BROADCAST_MESSAGE_CXX_IFACE(dsdl_com_rl_vibration_Measurement, DSDL_COM_RL_VIBRATION_MEASUREMENT_ID, DSDL_COM_RL_VIBRATION_MEASUREMENT_SIGNATURE, DSDL_COM_RL_VIBRATION_MEASUREMENT_MAX_SIZE);
#endif
#endif
