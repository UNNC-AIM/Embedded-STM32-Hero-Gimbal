//
// Created by ssyzz7 on 2/17/2023.
//
#include <usbd_cdc_if.h>
#include "usb_ros_msg.h"
#include "rc_msg.h"
#include "task_manager.h"
#include "Stewart_task.h"
#include "speed.h"
#include "Velocity.h"


extern "C" USBD_HandleTypeDef hUsbDeviceFS;

static velocity_msgs::Velocity gimbal_cmd_msg;

extern float rxVelocity[6];

//namespace USBDevice {
//    geometry_msgs::Twist& get_left_cmd() {
//        return left_cmd_vel;
//    }
//    geometry_msgs::Twist& get_right_cmd() {
//        return right_cmd_vel;
//    }
//    geometry_msgs::Twist& get_gimbal_cmd() {
//        return gimbal_cmd_msg;
//    }
//}

velocity_msgs::Velocity velocity_msg;
extern "C" void USB_DataDecode(uint8_t* Buf, uint32_t *Len) {

	if(!USBD_OK){
		velocity_msg.deserialize(Buf);


//    if (twist_msg.angular.x == 0) {
		gimbal_cmd_msg = velocity_msg;

		rxVelocity[0]=gimbal_cmd_msg.length_dot[0];
		rxVelocity[1]=gimbal_cmd_msg.length_dot[1];
		rxVelocity[2]=gimbal_cmd_msg.length_dot[2];
		rxVelocity[3]=gimbal_cmd_msg.length_dot[3];
		rxVelocity[4]=gimbal_cmd_msg.length_dot[4];
		rxVelocity[5]=gimbal_cmd_msg.length_dot[5];
//    }
//    else if (twist_msg.angular.x == 1) {
//        right_cmd_vel = twist_msg;
//    }
//    else if (twist_msg.angular.x == 2) {
//        gimbal_cmd_msg = twist_msg;
//    }
	}
}


void Tasks::usb_transmit_loop() {
    uint8_t buffer[48] = {0};

    get_rc_ctl_msg()->serialize(buffer);
    CDC_Transmit_FS(buffer, 48);

    osDelay(5);
}












//static uint8_t header[2] = {0x55, 0xaa};
//static uint8_t ender[2] = {0x0d, 0x0a};
//static uint16_t distance[6] = {0};



//extern "C" void USB_DataDecode(uint8_t* Buf, uint32_t *Len) {
//
//    uint8_t length;
//
//    while (Buf[1] == header[0] || Buf[2] == header[1] ||Buf[16] == ender[0] || Buf[17] == ender[1]) {
//        length = Buf[3];
//        if (length == 12) {
//            distance[0] = Buf[4] << 8 | Buf[5];
//            distance[1] = Buf[6] << 8 | Buf[7];
//            distance[2] = Buf[8] << 8 | Buf[9];
//            distance[3] = Buf[10] << 8 | Buf[11];
//            distance[4] = Buf[12] << 8 | Buf[13];
//            distance[5] = Buf[14] << 8 | Buf[15];
//        }
//    }
//}


