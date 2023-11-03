//
// Created by hari on 23-3-3.
//

#ifndef STM32_PROJ_DBUS_UART_H
#define STM32_PROJ_DBUS_UART_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)
#define RC_FRAME_LENGTH 18u
/* ----------------------- Data Struct ------------------------------------- */
typedef struct {
    struct {
        uint16_t ch0;
        uint16_t last_ch1;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;
        uint8_t s1;
        uint8_t s2;
        uint8_t last_s2;
        uint16_t rotor;
    } rc;
    struct {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;
    struct {
        uint16_t v;
    } key;
} RC_Ctl_t;

void rc_init();
void remote_data_process(uint8_t *pData);
void rc_receive_from_dma();
void rc_prevent_failure();
RC_Ctl_t* get_rc_data();

#ifdef __cplusplus
};
#endif


#endif //STM32_PROJ_DBUS_UART_H
