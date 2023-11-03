//
// Created by ssyzz7 on 2/17/2023.
//
#include <map>
#include <can.h>
#include "rm_motors.h"
#include "math.h"

#define getMotorInfo(ptr, data)     \
{\
(ptr)->pre_encoder=(ptr)->encoder_val;\
(ptr)->encoder_val=(uint16_t)(data)[0]<<8| (uint16_t)(data)[1]; \
(ptr)->delta = (ptr)->encoder_val - (ptr)->pre_encoder;  \
(ptr)->rotate_speed=(uint16_t)(data)[2]<<8 | (uint16_t)(data)[3];\
(ptr)->tq_current=(uint16_t)(data)[4] << 8 | (uint16_t)rxData[5];\
}

static std::map<uint8_t , RMMotor::FeedbackDataType> motorData[2];

namespace RMMotor {
    FeedbackDataType* get_data(CAN_HandleTypeDef *hcan, MotorType motorType, uint8_t motorID) {
        if (hcan->Instance == CAN1) {
            return &motorData[0][motorID - 1];
        } else {
            return &motorData[1][motorID - 1];
        }
    }


    void set_control(uint32_t canID, CAN_HandleTypeDef *hcan, const int16_t* controlValue, uint8_t totalNumOfMotor) {
        CAN_TxHeaderTypeDef txHeader;
        uint8_t txData[8];
        uint32_t txMailBox;

        txHeader.StdId = canID;
        txHeader.DLC = 2 * totalNumOfMotor;
        txHeader.IDE = CAN_ID_STD;
        txHeader.RTR = CAN_RTR_DATA;

        for (uint8_t i = 0; i < totalNumOfMotor; i++) {
            txData[2 * i] = (uint8_t)(controlValue[i] >> 8);
            txData[2 * i + 1] = (uint8_t)(controlValue[i]);
        }

        HAL_CAN_AddTxMessage(hcan, &txHeader, txData, &txMailBox);
    }

}

extern "C" void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef rxHeader;
    uint8_t rxData[8];

    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);

    if (hcan->Instance == CAN1) {
        if (rxHeader.StdId > 0x200 && rxHeader.StdId < 0x20C) {
            uint8_t motorID = rxHeader.StdId - 0x200;
            getMotorInfo(&motorData[0][motorID - 1], rxData);

            if(motorData[0][motorID - 1].rotate_speed < -100)
            {
                if(motorData[0][motorID - 1].delta > 0)
                    motorData[0][motorID - 1].turns = motorData[0][motorID - 1].turns - 1;
            }else if(motorData[0][motorID - 1].rotate_speed > 100)
            {
                if(motorData[0][motorID - 1].delta < 0)
                    motorData[0][motorID - 1].turns = motorData[0][motorID - 1].turns + 1;
            }
        }
    }
    else if (hcan->Instance == CAN2) {
        if (rxHeader.StdId > 0x200 && rxHeader.StdId < 0x20C) {
            uint8_t motorID = rxHeader.StdId - 0x200;
            getMotorInfo(&motorData[1][motorID - 1], rxData);

            if(motorData[1][motorID - 1].rotate_speed < -100)
            {
                if(motorData[1][motorID - 1].delta > 0)
                    motorData[1][motorID - 1].turns = motorData[1][motorID - 1].turns - 1;
            }else if(motorData[1][motorID - 1].rotate_speed > 100)
            {
                if(motorData[1][motorID - 1].delta < 0)
                    motorData[1][motorID - 1].turns = motorData[1][motorID - 1].turns + 1;
            }
        }
    }
}
