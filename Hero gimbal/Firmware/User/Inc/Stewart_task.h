//
// Created by 32282 on 2023/5/31.
//

#ifndef STM32_PROJ_STEWART_TASK_H
#define STM32_PROJ_STEWART_TASK_H
#include "typedefs.h"
#include "dbus_uart.h"
#include "pid.h"
#include "rm_motors.h"
//#include "usb_ros_msg.h"
typedef enum{
    NOTREADY=0,
    GETREADY,
}condition;
class Stewart{
private:
    RMMotor::FeedbackDataType motorInfo[6];
    PID speedControl[6];            //多圈时速度环
    PID positionControl_speed[6];    //最后一圈位置环
    PID positionControl_position[6];
    int16_t cmdCurrent[6];
    int16_t set_encoder[6];
    int16_t set_speed[6];
    fp64 set_distance[6];
    int16_t set_turns[6];
    int8_t local_flag[6];
    int16_t init_encoder[6];
    int8_t state = 0;

public:
    void pid_init();
    void update_motor_info();
    void get_init_encoder(int8_t motorID);
    void set_value(int8_t motorID);
    void pid_control(int8_t motorID);
    void remote_control_stewart();
    void self_protect();
    void transmit_cmdCurrent();
    void init_condition();
    void control_loop();
    };



#endif //STM32_PROJ_STEWART_TASK_H
