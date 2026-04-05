//
// Created by 20159 on 2026/4/1.
//

#ifndef UART_H
#define UART_H

#include "usart.h"
#include "stm32h7xx_hal.h"

#define USART1_recv_Data_N  1
#define SBUS_RX_BUF_NUM		18u
#define USART7_recv_Data_N  1
#define USART10_recv_Data_N 1
//宇树485发送数组大小
#define RS485_Send_Data_N   17
//宇树485接收数组大小
#define RS485_recv_Data_N   16


//定义串口回调函数
typedef void (*USART_Callback)(UART_HandleTypeDef *Header, uint8_t *Buffer);


void USART_RX485_init(UART_HandleTypeDef *huart,USART_Callback back);

void USART_init(UART_HandleTypeDef *huart,USART_Callback back);

uint8_t UART_Transmit_Data(UART_HandleTypeDef *huart, uint8_t *Data,uint8_t len);



#endif //DM02_UART_H