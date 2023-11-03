#include "rm_motors.h"
#include "dbus_uart.h"
#include "speed.h"
#include "math.h"
#include "usb_ros_msg.h"
#include "task_manager.h"

float rxVelocity[6];
int test = 0;

void stewart_speed::currentcontrol(){
    RMMotor::set_control(0x200, &hcan1, &cmdCurrent[0], 4);
    RMMotor::set_control(0x1ff, &hcan1, &cmdCurrent[4], 2);
}

void stewart_speed::pid_init(){
    speedControl[0].init(1.0f, 0.0f, 0.1f, 1000, 1000);
    speedControl[1].init(10.0f, 0.0f, 1.0f, 3500, 1700);
    speedControl[2].init(10.0f, 0.0f, 1.0f, 2000, 1800);
    speedControl[3].init(10.0f, 0.0f, 1.0f, 2000, 1800);
    speedControl[4].init(10.0f, 0.0f, 1.0f, 3000, 1800);
    speedControl[5].init(10.0f, 0.0f, 1.0f, 3500, 1800);
}
void stewart_speed::update_motor_info(){
    motorInfo[0] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 1);
    motorInfo[1] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 2);
    motorInfo[2] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 3);
    motorInfo[3] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 4);
    motorInfo[4] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 5);
    motorInfo[5] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 6);

//	RMMotor::get_data(&hcan1, RMMotor::M3508, 1)->encoder_val
}
void stewart_speed::set_value(int8_t motorID){
    set_speed[motorID-1] = rxVelocity[motorID-1]*8;
	cmdCurrent[motorID - 1] = speedControl[motorID-1].calc(set_speed[motorID-1], motorInfo[motorID - 1].rotate_speed);
	currentcontrol();
}
void stewart_speed::control_loop(){
//	int16_t i;
//	for(i = 1; i<6 ;i++){
//	set_value(i);
//	}
//	speedControl[0].init(1.0f, 0.0f, 0.1f, 1000, 1000);
	update_motor_info();
	pid_init();
	test++;
	set_speed[0] = 500;
	cmdCurrent[0] = speedControl[0].calc(set_speed[0], RMMotor::get_data(&hcan1, RMMotor::M3508, 1)->rotate_speed);
	currentcontrol();
}


