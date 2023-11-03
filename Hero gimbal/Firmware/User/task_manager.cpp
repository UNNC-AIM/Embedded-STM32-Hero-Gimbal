//
// Created by ssyzz7 on 2/17/2023.
//
#include "task_manager.h"
#include "Stewart_task.h"
#include "launch_task.h"
#include "speed.h"

static Stewart stewartControl;
static launch launchControl;
static stewart_speed speedControl;

namespace Tasks {
    osThreadId_t StewartControlTaskHandle;
    const osThreadAttr_t stewartControlTask_attributes = {
            .name = "stewartControlTask",
            .stack_size = 1024,
            .priority = (osPriority_t) osPriorityRealtime,
    };

    osThreadId_t LaunchControlTaskHandle;
    const osThreadAttr_t launchControlTask_attributes = {
            .name = "gimbalControlTask",
            .stack_size = 1024,
            .priority = (osPriority_t) osPriorityHigh,
    };

    osThreadId_t usbTransmitTaskHandle;
    const osThreadAttr_t usbTransmitTask_attributes = {
            .name = "usbTransmitTask",
            .stack_size = 256,
            .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadId_t speedControlTaskHandle;
    const osThreadAttr_t speedControlTask_attributes = {
            .name = "speedControlTask",
            .stack_size = 1024,
            .priority = (osPriority_t) osPriorityRealtime,
    };


//    osThreadId_t usbTransmitTaskHandle;
//    const osThreadAttr_t usbTransmitTask_attributes = {
//            .name = "usbTransmitTask",
//            .stack_size = 256,
//            .priority = (osPriority_t) osPriorityRealtime,
//    };

    [[noreturn]] void stewart_control_task(void *);

    [[noreturn]] void launch_control_task(void *);

    [[noreturn]] void usb_transmit_task(void *);

    [[noreturn]] void speed_control_task(void *);

    [[noreturn]] void stewart_control_task(void *pvParameters) {
            for(int8_t i=0; i<6; i++) {
                stewartControl.get_init_encoder(i+1);
            }
            stewartControl.pid_init();
            for(;;) {
//                stewartControl.init_condition();
                stewartControl.update_motor_info();
                stewartControl.control_loop();
                stewartControl.transmit_cmdCurrent();
                osDelay(2);
            }
        osThreadTerminate(osThreadGetId());
        }



    [[noreturn]] void launch_control_task(void *pvParameters) {
        for(;;) {
            launchControl.pid_init();
            launchControl.get_init_encoder(RMMotor::get_data(&hcan1,RMMotor::M3508,1)->encoder_val);
            for(;;) {
                launchControl.update_motorInfo();
                rc_prevent_failure();
                launchControl.fric_control();
                launchControl.dial_control();
                launchControl.push_rod();
                launchControl.transmit_current();
                osDelay(2);
            }
        }
        osThreadTerminate(osThreadGetId());
    }

    [[noreturn]] void usb_transmit_task(void *pvParameters) {
        for(;;) {
            usb_transmit_loop();

        }

        osThreadTerminate(osThreadGetId());
    }
    [[noreturn]] void speed_control_task(void *pvParameters) {

        for(;;) {
        	speedControl.pid_init();
        	speedControl.update_motor_info();
        	speedControl.control_loop();
        }

        osThreadTerminate(osThreadGetId());
    }
}

extern "C" void start_task(void *pvParameters) {

//    Tasks::StewartControlTaskHandle = osThreadNew(Tasks::stewart_control_task,
//                                                  nullptr,
//                                                  &Tasks::stewartControlTask_attributes);

//    Tasks::LaunchControlTaskHandle = osThreadNew(Tasks::launch_control_task,
//                                                 nullptr,
//                                                 &Tasks::launchControlTask_attributes);

    Tasks::usbTransmitTaskHandle = osThreadNew(Tasks::usb_transmit_task,
                                               nullptr,
                                               &Tasks::usbTransmitTask_attributes);

    Tasks::speedControlTaskHandle = osThreadNew(Tasks::speed_control_task,
                                               nullptr,
                                               &Tasks::speedControlTask_attributes);

    osThreadTerminate(osThreadGetId());
}

