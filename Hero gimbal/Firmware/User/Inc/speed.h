#ifndef STM32_PROJ_SPEED_H
#define STM32_PROJ_SPEED_H

#include "typedefs.h"
#include "dbus_uart.h"
#include "pid.h"
#include "rm_motors.h"
#include "task_manager.h"

class stewart_speed{
	private :
    RMMotor::FeedbackDataType motorInfo[6];
    PID speedControl[6];
    int16_t cmdCurrent[6];
    int16_t set_speed[6];
	public :
    void pid_init();
    void update_motor_info();
    void currentcontrol();
    void set_value(int8_t motorID);
    void control_loop();
};


#endif
