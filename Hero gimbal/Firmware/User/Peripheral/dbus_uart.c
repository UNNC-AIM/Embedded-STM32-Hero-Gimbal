//
// Created by hari on 23-3-3.
//
#include "dbus_uart.h"
#include "main.h"

extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart3_rx;

static uint8_t rx_buffer[2][18];
static RC_Ctl_t RC_CtrlData;

void rc_init() {
    SET_BIT(huart3.Instance->CR3, USART_CR3_DMAR);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    __HAL_DMA_DISABLE(&hdma_usart3_rx);
    while(hdma_usart3_rx.Instance->CR & DMA_SxCR_EN)
    {
        __HAL_DMA_DISABLE(&hdma_usart3_rx);
    }
    hdma_usart3_rx.Instance->PAR = (uint32_t) & (USART3->DR);
    hdma_usart3_rx.Instance->M0AR = (uint32_t)(rx_buffer[0]);
    hdma_usart3_rx.Instance->M1AR = (uint32_t)(rx_buffer[1]);
    hdma_usart3_rx.Instance->NDTR = 36;
    SET_BIT(hdma_usart3_rx.Instance->CR, DMA_SxCR_DBM);
    __HAL_DMA_ENABLE(&hdma_usart3_rx);
}

void remote_data_process(uint8_t *pData) {
    if (pData == NULL) {
        return;
    }

    RC_CtrlData.rc.ch0 = ((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;
    RC_CtrlData.rc.last_ch1 = RC_CtrlData.rc.ch1;
    RC_CtrlData.rc.ch1 = (((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5))
                         & 0x07FF;
    RC_CtrlData.rc.ch2 = (((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |
                          ((int16_t)pData[4] << 10)) & 0x07FF;
    RC_CtrlData.rc.ch3 = (((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) &
                         0x07FF;

    RC_CtrlData.rc.s1 = ((pData[5] >> 4) & 0x000C) >> 2;
    RC_CtrlData.rc.last_s2=RC_CtrlData.rc.s2;
    RC_CtrlData.rc.s2 = ((pData[5] >> 4) & 0x0003);
    RC_CtrlData.mouse.x = ((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
    RC_CtrlData.mouse.y = ((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
    RC_CtrlData.mouse.z = ((int16_t)pData[10]) | ((int16_t)pData[11] << 8);
    RC_CtrlData.mouse.press_l = pData[12];
    RC_CtrlData.mouse.press_r = pData[13];
    RC_CtrlData.key.v = ((int16_t)pData[14]) | ((int16_t)pData[15] << 8);
    RC_CtrlData.rc.rotor = ((int16_t)pData[16]) | ((int16_t)pData[17] << 8);
}

void rc_receive_from_dma() {
    if(huart3.Instance->SR & UART_FLAG_RXNE) {
        __HAL_UART_CLEAR_PEFLAG(&huart3);
    }
    else if(USART3->SR & UART_FLAG_IDLE) {
        static uint16_t this_time_rx_len = 0;

        __HAL_UART_CLEAR_PEFLAG(&huart3);

        if ((hdma_usart3_rx.Instance->CR & DMA_SxCR_CT) == RESET) {
            __HAL_DMA_DISABLE(&hdma_usart3_rx);
            this_time_rx_len = 36u - hdma_usart3_rx.Instance->NDTR;
            hdma_usart3_rx.Instance->NDTR = 36u;
            hdma_usart3_rx.Instance->CR |= DMA_SxCR_CT;
            __HAL_DMA_ENABLE(&hdma_usart3_rx);

            if(this_time_rx_len == 18u) {
                //Add function here
                remote_data_process(rx_buffer[0]);
            }
        }
        else {
            __HAL_DMA_DISABLE(&hdma_usart3_rx);
            this_time_rx_len = 36u - hdma_usart3_rx.Instance->NDTR;
            hdma_usart3_rx.Instance->NDTR = 36u;
            DMA1_Stream1->CR &= ~(DMA_SxCR_CT);
            __HAL_DMA_ENABLE(&hdma_usart3_rx);

            if(this_time_rx_len == 18u) {
                remote_data_process(rx_buffer[1]);
            }
        }
    }
}

RC_Ctl_t* get_rc_data() {
    return &RC_CtrlData;
}

void rc_prevent_failure() {
    if(get_rc_data()->rc.ch0 < 364 || get_rc_data()->rc.ch0 > 1684) {
        get_rc_data()->rc.ch0 = 1024;
    }
    if(get_rc_data()->rc.ch1 < 364 || get_rc_data()->rc.ch1 > 1684) {
        get_rc_data()->rc.ch1 = 1024;
    }
    if(get_rc_data()->rc.ch2 < 364 || get_rc_data()->rc.ch2 > 1684) {
        get_rc_data()->rc.ch2 = 1024;
    }
    if(get_rc_data()->rc.ch3 < 364 || get_rc_data()->rc.ch3 > 1684) {
        get_rc_data()->rc.ch3 = 1024;
    }
    if(get_rc_data()->rc.rotor < 364 || get_rc_data()->rc.rotor > 1684) {
        get_rc_data()->rc.rotor = 1024;
    }
    if(get_rc_data()->rc.s1 < 1 || get_rc_data()->rc.s1 > 3) {
        get_rc_data()->rc.s1 = 1;
    }
    if(get_rc_data()->rc.s2 < 1 || get_rc_data()->rc.s2 > 3) {
        get_rc_data()->rc.s2 = 1;
    }
}