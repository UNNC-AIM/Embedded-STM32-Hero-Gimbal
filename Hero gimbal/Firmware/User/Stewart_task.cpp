#include "rm_motors.h"
#include "dbus_uart.h"
#include "Stewart_task.h"
#include "math.h"
#include "usb_ros_msg.h"

float rxlength[6];
//float rxVelocity[6];
float leng1,leng2,leng3,leng4,leng5,leng6;

void Stewart::transmit_cmdCurrent(){
    RMMotor::set_control(0x200, &hcan1, &cmdCurrent[0], 4);
    RMMotor::set_control(0x1ff, &hcan1, &cmdCurrent[4], 2);

}
void Stewart::control_loop(){
    remote_control_stewart();
    self_protect();
    //f(get_rc_data()->rc.ch1 != 1024) {
        for(int8_t i = 0; i < 6; i++) {
            set_value(i + 1);
            pid_control(i + 1);
        }
    //}

}


void Stewart::update_motor_info()
{
    motorInfo[0] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 1);
    motorInfo[1] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 2);
    motorInfo[2] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 3);
    motorInfo[3] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 4);
    motorInfo[4] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 5);
    motorInfo[5] = *RMMotor::get_data(&hcan1, RMMotor::M3508, 6);
}
void Stewart::pid_init()
{
//    speedControl[0].init(30.0f, 5.0f, 3.0f, 2000, 1200);
//    speedControl[1].init(30.0f, 5.0f, 3.0f, 2000, 1700);
//    speedControl[2].init(30.0f, 5.0f, 3.0f, 2000, 1800);
//    speedControl[3].init(30.0f, 5.0f, 3.0f, 2000, 1800);
//    speedControl[4].init(30.0f, 5.0f, 3.0f, 2000, 1800);
//    speedControl[5].init(30.0f, 5.0f, 3.0f, 2000, 1800);

    speedControl[0].init(120.0f, 5.0f, 1.0f, 3000, 1800);
    speedControl[1].init(120.0f, 5.0f, 1.0f, 3500, 1700);
    speedControl[2].init(120.0f, 5.0f, 1.0f, 2000, 1800);
    speedControl[3].init(120.0f, 5.0f, 1.0f, 2000, 1800);
    speedControl[4].init(150.0f, 5.0f, 1.0f, 3000, 1800);
    speedControl[5].init(120.0f, 5.0f, 1.0f, 3500, 1800);

    positionControl_speed[0].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_speed[1].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_speed[2].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_speed[3].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_speed[4].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_speed[5].init(1.0f, 0.0f, 0.0f, 800, 600);

    positionControl_position[0].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_position[1].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_position[2].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_position[3].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_position[4].init(1.0f, 0.0f, 0.0f, 800, 600);
    positionControl_position[5].init(1.0f, 0.0f, 0.0f, 800, 600);
}



void Stewart::get_init_encoder(int8_t motorID) {
        init_encoder[motorID - 1] = RMMotor::get_data(&hcan1, RMMotor::M3508, motorID)->encoder_val;
}

void Stewart::set_value(int8_t motorID) {

    if (set_distance[motorID - 1] < -8192) {
        set_turns[motorID - 1] = (set_distance[motorID - 1] + init_encoder[motorID - 1]) / 8192 - 1;
        set_encoder[motorID - 1] = set_distance[motorID - 1] + init_encoder[motorID - 1] - 8192 * set_turns[motorID - 1];
    } else if (set_distance[motorID - 1] > 8192) {
        set_turns[motorID - 1] = (set_distance[motorID - 1] - 8192 + init_encoder[motorID - 1]) / 8192 + 1;
        set_encoder[motorID - 1] = set_distance[motorID - 1] + init_encoder[motorID - 1] - 8192 * set_turns[motorID - 1];
    }
    else
        remote_control_stewart();

}

void Stewart::pid_control(int8_t motorID)
{
    if(motorInfo[motorID - 1].turns > set_turns[motorID - 1])
    {
        cmdCurrent[motorID - 1] = speedControl[motorID-1].calc(-800, motorInfo[motorID - 1].rotate_speed);
        local_flag[motorID - 1] = 2;
    }
    else if(motorInfo[motorID - 1].turns < set_turns[motorID - 1])
    {
        cmdCurrent[motorID - 1] = speedControl[motorID-1].calc(800, motorInfo[motorID - 1].rotate_speed);
        local_flag[motorID - 1] = 3;
    }
    else
    {
        local_flag[motorID - 1] = 4;
        set_speed[motorID - 1] = positionControl_position[motorID - 1].calc(set_encoder[motorID - 1], motorInfo[motorID - 1].encoder_val);
        cmdCurrent[motorID - 1] = positionControl_speed[motorID - 1].calc(set_speed[motorID - 1], motorInfo[motorID - 1].rotate_speed);
    }
}


void Stewart::remote_control_stewart() {
            set_distance[0] = rxlength[0]*8192;
            set_distance[1] = rxlength[1]*8192;
            set_distance[2] = rxlength[2]*8192;
            set_distance[3] = rxlength[3]*8192;
            set_distance[4] = rxlength[4]*8192;
            set_distance[5] = rxlength[5]*8192;

//            leng1 = rxlength[0];
//            leng2 = rxlength[1];
//            leng3 = rxlength[2];
//            leng4 = rxlength[3];
//            leng5 = rxlength[4];
//            leng6 = rxlength[5];


//    		set_distance[0] = -50*8192;
//    		set_distance[1] = -50*8192;
//    		set_distance[2] = -50*8192;
//			set_distance[3] = -50*8192;
//			set_distance[4] = -50*8192;
//			set_distance[5] = -50*8192;


}

void Stewart::self_protect(){

        if (motorInfo[0].turns > 125) set_turns[0] = 125;
        if (motorInfo[1].turns > 125) set_turns[1] = 125;
        if (motorInfo[2].turns > 125) set_turns[2] = 125;
        if (motorInfo[3].turns > 125) set_turns[3] = 125;
        if (motorInfo[4].turns > 125) set_turns[4] = 125;
        if (motorInfo[5].turns > 125) set_turns[5] = 125;

//    else{
//        if (motorInfo[0].turns < -15) set_turns[0] = -15;
//        if (motorInfo[1].turns < -15) set_turns[1] = -15;
//        if (motorInfo[2].turns < -80.9) set_turns[2] = -80.9;
//        if (motorInfo[3].turns < -106) set_turns[3] = -106;
//        if (motorInfo[4].turns < -106) set_turns[4] = -106;
//        if (motorInfo[5].turns < -80.9) set_turns[5] = -80.9;
//    }
}

void Stewart::init_condition(){
//    if (state == 0) {
//        for (uint8_t i = 0; i < 6; i++) {
//            if (abs(motorInfo[i].tq_current) < 2100) {
//                cmdCurrent[i] = speedControl[i].calc(800, motorInfo[i].rotate_speed);
//                local_flag[i] = 10;
//            }
//            else {
//                cmdCurrent[3] = 0;
//                if(cmdCurrent [3] = cmdCurrent[4] == 0)
//                    state = 1;
//            }
//        }
//
//    }










    //    if(state == NOTREADY) {
//        for (uint8_t i = 0; i < 6; i++) {
//            update_motor_info();
//            cmdCurrent[i] = speedControl[i].calc(-1200, motorInfo[i].rotate_speed);
//            transmit_cmdCurrent();
//
//        }
//        if (abs(motorInfo[4].tq_current) <-2100){
//            cmdCurrent[4] = 0;
//        }
//        if (cmdCurrent[0] == cmdCurrent[1] == cmdCurrent[2]== cmdCurrent[3] == cmdCurrent[4] == cmdCurrent[5] == 0)
//            state= GETREADY;
    //  }
//    if(state==GETREADY){

}





