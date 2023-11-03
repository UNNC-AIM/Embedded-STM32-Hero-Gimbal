//
// Created by hari on 23-3-3.
//
#include "Twist.h"
#include "dbus_uart.h"

static geometry_msgs::Twist remoteCtlMsg;

geometry_msgs::Twist* get_rc_ctl_msg() {
    rc_prevent_failure();

//    remoteCtlMsg.linear.x = ((fp32)(get_rc_data()->rc.ch3) - 1024.0f) / 1320.0f * 4.0f;
//    remoteCtlMsg.linear.y = ((fp32)(get_rc_data()->rc.ch2) - 1024.0f) / 1320.0f * 4.0f;
//    remoteCtlMsg.angular.z = ((fp32)(get_rc_data()->rc.ch0) - 1024.0f) / 1320.0f * 4.0f;
	remoteCtlMsg.linear.x = 1.0f;
	remoteCtlMsg.linear.z = 10.0f;
	remoteCtlMsg.angular.z = 1.0f;

    return &remoteCtlMsg;
}
