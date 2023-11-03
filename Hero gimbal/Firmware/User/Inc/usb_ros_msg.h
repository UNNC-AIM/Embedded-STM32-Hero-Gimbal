
// Created by ssyzz7 on 2/17/2023.
//

#ifndef STM32_PROJ_USB_ROS_MSG_H
#define STM32_PROJ_USB_ROS_MSG_H

#include "Twist.h"
#include "Velocity.h"
//float rxlength[6];

unsigned char getCrc8(unsigned char *ptr, unsigned short len);
#endif //STM32_PROJ_USB_ROS_MSG_H
