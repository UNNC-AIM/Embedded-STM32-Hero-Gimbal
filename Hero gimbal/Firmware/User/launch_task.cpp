#include "rm_motors.h"
#include "dbus_uart.h"
#include "math.h"
#include "launch_task.h"
#define SHOOTING_SPD 6800


void launch::transmit_current() {
    RMMotor::set_control(0x200, &hcan2, controlCurrent, 4);
}

void launch::get_init_encoder(int16_t encoder){
    init_encoder = encoder;
}
void launch::update_motorInfo(){
    gimbalmotoInfo[0] = *RMMotor::get_data(&hcan2, RMMotor::M3508, 1);
    gimbalmotoInfo[1] = *RMMotor::get_data(&hcan2, RMMotor::M2006, 2);
    gimbalmotoInfo[2] = *RMMotor::get_data(&hcan2, RMMotor::M3508, 3);
    gimbalmotoInfo[3] = *RMMotor::get_data(&hcan2, RMMotor::M3508, 4);

}

    void launch::pid_init()
    {
        pushPID_speed.init(1.0, 0.05, 0.0, 2000.0, 800.0);
        fric1PID.init(1000.0f, 1.0f, 15.0f, 10000.0, 8000.0);
        fric2PID.init(1000.0f, 1.0f, 15.0f, 10000.0, 8000.0);

        dial_speed_control.init(3.0,0.05,0.0,1200,1000);
        //位置环控制
        dialPID_speed.init(1.0f, 0.01f, 0.0f, 1200.0f, 1000.0f);
        dialPID_position.init(1.0f, 0.0f, 0.0f, 1200.0f, 1000.0f);

    }

void launch::dial_control() {
    if (flag == 2) {
        if(gimbalmotoInfo[0].turns > set_turns)
            controlCurrent[0] = dial_speed_control.calc(-800, gimbalmotoInfo[0].rotate_speed);
        else if(gimbalmotoInfo[0].turns < set_turns)
            controlCurrent[0] = dial_speed_control.calc(800, gimbalmotoInfo[0].rotate_speed);
        else{
            fp32 speed = dialPID_position.calc(set_encoder, gimbalmotoInfo[0].encoder_val);
            controlCurrent[0]= dialPID_speed.calc(speed, gimbalmotoInfo[0].rotate_speed);
            if ( (abs(set_encoder- gimbalmotoInfo[0].encoder_val) < 100)) {
                flag = 3;
                controlCurrent[0] = 0;
            }
        }
    }
}

void launch::fric_control() {
    if (get_rc_data()->rc.last_s2 == 1 && get_rc_data()->rc.s2 == 3){
        while(door == 0){
            init_encoder = init_encoder + (8192*19.203/6);
            set_turns = (init_encoder - 8192 ) / 8192 + 1;
            set_encoder= init_encoder - 8192 * set_turns;
            door = 1;
        }
        flag = 1;
    }
    if (flag == 1 || flag == 2 || flag == 3 || flag == 4) {
        controlCurrent[2] = static_cast<int16_t>(fric1PID.calc(SHOOTING_SPD, gimbalmotoInfo[2].rotate_speed));
        controlCurrent[3] = static_cast<int16_t>(fric2PID.calc(-SHOOTING_SPD, gimbalmotoInfo[3].rotate_speed));
    }
    if ((flag == 1) &&
        (abs(gimbalmotoInfo[2].rotate_speed - SHOOTING_SPD) < 70) &&
        (abs(gimbalmotoInfo[3].rotate_speed + SHOOTING_SPD) < 70))   flag = 2;

    if (flag == 5) {
        controlCurrent[1] = 0;
        door = 0;
    }
}

void launch::push_rod() {

    if (flag == 3 && abs(gimbalmotoInfo[2].rotate_speed + gimbalmotoInfo[3].rotate_speed)<10)   //一度432
    {
        controlCurrent[1] = static_cast<int16_t>(pushPID_speed.calc(-11000, gimbalmotoInfo[1].rotate_speed));
        if (gimbalmotoInfo[1].tq_current < -1900) flag = 4;   //105*8192*36
    }
    if (flag == 4) {
        controlCurrent[1] = static_cast<int16_t>(pushPID_speed.calc(11000, gimbalmotoInfo[1].rotate_speed));
        if (gimbalmotoInfo[1].tq_current > 1900) flag = 5;
    }

}






