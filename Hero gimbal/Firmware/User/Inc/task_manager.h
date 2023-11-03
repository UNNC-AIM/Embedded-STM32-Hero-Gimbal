//
// Created by ssyzz7 on 2/17/2023.
//

#ifndef STM32_PROJ_TASK_MANAGER_H
#define STM32_PROJ_TASK_MANAGER_H

#include <cmsis_os2.h>
#include "Stewart_task.h"
#include "launch_task.h"
#include "speed.h"
namespace Tasks {
//    void Stewart_control_loop();
//    void launch_control_loop();
    void usb_transmit_loop();
    void speed_control_task();
}

#endif //STM32_PROJ_TASK_MANAGER_H
