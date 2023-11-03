//
// Created by 32282 on 2023/5/28.
//

#ifndef STM32_PROJ_LAUNCH_TASK_H
#define STM32_PROJ_LAUNCH_TASK_H

#include "typedefs.h"
#include "pid.h"
#include "rm_motors.h"


class launch{
private:
    RMMotor::FeedbackDataType gimbalmotoInfo[4];
     PID fric1PID;
     PID fric2PID;    // limit 16384
     PID dial_speed_control;
     PID dialPID_speed;
     PID dialPID_position;
     PID pushPID_speed;
     int16_t controlCurrent[4] = {0};
     int32_t set = 0;
     uint8_t flag =0;
     uint8_t door = 0;
     int16_t set_turns;
     int16_t set_encoder;
     int32_t init_encoder;


public:
    void update_motorInfo();
    void pid_init();
    void fric_control();
    void dial_control();
    void push_rod();
    void transmit_current();
    void get_init_encoder(int16_t encoder);
};

#endif //STM32_PROJ_LAUNCH_TASK_H
