//
// Created by ssyzz7 on 2/17/2023.
//

#ifndef STM32_PROJ_RM_MOTORS_H
#define STM32_PROJ_RM_MOTORS_H

#include "typedefs.h"
#include <can.h>
#include "pid.h"


namespace RMMotor {
    typedef struct {
        int16_t pre_encoder;
        int16_t encoder_val;
        int16_t delta;
        int16_t turns;
        int16_t rotate_speed;
        int16_t tq_current;
        int8_t temperature;
    } FeedbackDataType;

    typedef enum {
        M2006,
        M3508,
        GM6020
    } MotorType;

    FeedbackDataType* get_data(CAN_HandleTypeDef *hcan, MotorType motorType, uint8_t motorID);
    void set_control(uint32_t canID, CAN_HandleTypeDef *hcan, const int16_t* controlValue, uint8_t totalNumOfMotor);


}



#endif //STM32_PROJ_RM_MOTORS_H
