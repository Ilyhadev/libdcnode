/*
 * Copyright (C) 2025 Ilia Kliantsevich <iliawork112005gmail.com>
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef VIBRATION_H_
#define VIBRATION_H_

#include <stdint.h>
#include <stdbool.h>
#include "libdcnode/dronecan.h"
#include "libdcnode/serialization_internal.h"

/**
 * @brief uavcan.mpu.Vibration
 * @note Mpu vibration measurements based on FFT
 */
#define VIBRATION_ID                  20105
#define VIBRATION_SIGNATURE           0xCB6C085C090925A8ULL // I used function signature.compute_signature from dronecan.dsdl
#define VIBRATION_MESSAGE_SIZE        7  // 1 + 2 + 2 + 2 = 7 bytes
#define VIBRATION                     UAVCAN_EXPAND(VIBRATION)

typedef struct {
    uint8_t id;                         // uint8
    float vibration_metrics;            // float16
    float dominant_frequency;           // float16  
    float dominant_snr;                 // float16
} Mpu_vibration;

#ifdef __cplusplus
extern "C" {
#endif

static inline int8_t dronecan_mpu_vibration_serialize(
    const Mpu_vibration* const obj,
    uint8_t* const buffer,
    size_t* const inout_buffer_size_bytes)
{
    if ((obj == NULL) || (buffer == NULL) || (inout_buffer_size_bytes == NULL)) {
        return -2;
    }

    const size_t capacity_bytes = *inout_buffer_size_bytes;
    if (capacity_bytes < VIBRATION_MESSAGE_SIZE) {
        return -3;
    }

    canardEncodeScalar(buffer, 0, 8, &obj->id);
    canardEncodeFloat16(buffer, 8, obj->vibration_metrics);
    canardEncodeFloat16(buffer, 24, obj->dominant_frequency);
    canardEncodeFloat16(buffer, 40, obj->dominant_snr);

    return 0;
}

static inline int8_t dronecan_mpu_vibration_publish(
    const Mpu_vibration* const obj,
    uint8_t* inout_transfer_id)
{
    uint8_t buffer[VIBRATION_MESSAGE_SIZE];
    size_t inout_buffer_size = VIBRATION_MESSAGE_SIZE;
    dronecan_mpu_vibration_serialize(obj, buffer, &inout_buffer_size);
    uavcanPublish(VIBRATION_SIGNATURE,
                  VIBRATION_ID,
                  inout_transfer_id,
                  CANARD_TRANSFER_PRIORITY_MEDIUM,
                  buffer,
                  VIBRATION_MESSAGE_SIZE);

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif  // VIBRATION_H_