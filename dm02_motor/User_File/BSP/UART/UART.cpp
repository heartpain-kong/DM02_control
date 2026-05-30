//
// Created by 20159 on 2026/4/1.
//

#include "UART.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart7;
extern UART_HandleTypeDef huart10;

USART_Callback Usart1_Callback_Function;
USART_Callback Usart2_Callback_Function;
USART_Callback Usart3_Callback_Function;
USART_Callback Usart5_Callback_Function;
USART_Callback Usart7_Callback_Function;
USART_Callback Usart10_Callback_Function;

 bool SBUS_RX_init_bl;

//把数据放在0x24000000之后因为DMA原因
__attribute__((section (".AXI_SRAM"))) static uint8_t Usart1_recv_Data[USART1_recv_Data_N];
__attribute__((section (".AXI_SRAM"))) static uint8_t Usart2_recv_Data[RS485_recv_Data_N];
__attribute__((section (".AXI_SRAM"))) static uint8_t Usart3_recv_Data[RS485_recv_Data_N];
__attribute__((section (".AXI_SRAM")))  uint8_t Usart5_recv_Data[SBUS_RX_BUF_NUM];
__attribute__((section (".AXI_SRAM"))) static uint8_t Usart7_recv_Data[USART7_recv_Data_N];
__attribute__((section (".AXI_SRAM"))) static uint8_t Usart10_recv_Data[USART10_recv_Data_N];

__attribute__((section (".AXI_SRAM"))) static uint8_t SBUS_RX_Data[SBUS_RX_BUF_NUM]={
	0x00,0x04,0x20,0x00,0x01,0x58,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04
};
/**
 * @brief 串口的初始化
 * @param huart  串口编号
 * @param back 回调函数传参
 * @return void
 */
void USART_RX485_init(UART_HandleTypeDef *huart,USART_Callback back){
	HAL_RS485Ex_Init(huart,UART_DE_POLARITY_HIGH,2,2);
    if(huart==&huart2){
        Usart2_Callback_Function = back;
    }else if(huart==&huart3){
        Usart3_Callback_Function = back;
    }
    if(huart==&huart2){
        HAL_UART_Receive_DMA(huart,Usart2_recv_Data, RS485_recv_Data_N);
    }else if(huart==&huart3){
        HAL_UART_Receive_DMA(huart,Usart3_recv_Data, RS485_recv_Data_N);
    }

}

/**
 * @brief 串口的初始化
 * @param huart  串口编号
 * @param back 回调函数传参
 * @return void
 */
void USART_init(UART_HandleTypeDef *huart,USART_Callback back){
    if(huart==&huart1){
        Usart1_Callback_Function = back;
    }else if(huart==&huart2){
        Usart2_Callback_Function = back;
    }else if(huart==&huart3){
        Usart3_Callback_Function = back;
    }else if(huart==&huart10){
        Usart10_Callback_Function = back;
    }else if(huart==&huart7){
        Usart7_Callback_Function = back;
    }else if(huart==&huart5){
        Usart5_Callback_Function = back;
    }
    if(huart==&huart1){
        HAL_UART_Receive_DMA(huart,Usart1_recv_Data, USART1_recv_Data_N);
    }else if(huart==&huart2){
        HAL_UART_Receive_DMA(huart,Usart2_recv_Data, RS485_recv_Data_N);
    }else if(huart==&huart3){
        HAL_UART_Receive_DMA(huart,Usart3_recv_Data, RS485_recv_Data_N);
    }else if(huart==&huart5){
		SBUS_RX_init_bl = 1;
        HAL_UART_Receive_DMA(huart,Usart5_recv_Data, SBUS_RX_BUF_NUM);
    }else if(huart==&huart7){
        HAL_UART_Receive_DMA(huart,Usart7_recv_Data, USART7_recv_Data_N);
    }else if(huart==&huart10){
        HAL_UART_Receive_DMA(huart,Usart10_recv_Data, USART10_recv_Data_N);
    }
}



/**
 * @brief 串口的发送
 * @param huart  串口编号
 * @param *Data 发送数据数组指针
 * @return void
 */
uint8_t UART_Transmit_Data(UART_HandleTypeDef *huart, uint8_t *Data,uint8_t len)
{
    __attribute__((section (".AXI_SRAM")))  static uint8_t Usart_send_Data[RS485_Send_Data_N];
    for(uint8_t i=0;i<len;++i)Usart_send_Data[i] = Data[i];

    return (HAL_UART_Transmit_DMA(huart, Usart_send_Data, len));
}

/**
 * @brief 串口接收回调函数
 * @param huart  串口编号
 * @return void
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if (huart == &huart2)
    {

        if (Usart2_Callback_Function != nullptr)
        {
            Usart2_Callback_Function(huart, Usart2_recv_Data);
		
        }
        
    }
    else if (huart == &huart3)
    {
        if (Usart3_Callback_Function != nullptr)
        {
            Usart3_Callback_Function(huart, Usart3_recv_Data);
        }
    }
    else if (huart == &huart1)
    {
        if (Usart1_Callback_Function != nullptr)
        {
            Usart1_Callback_Function(huart, Usart1_recv_Data);
        }
    }
    else if (huart == &huart5)
    {
        if (Usart5_Callback_Function != nullptr)
        {
			if(SBUS_RX_init_bl){
				for(uint8_t i=0;i<SBUS_RX_BUF_NUM;i++){
					if(Usart5_recv_Data[i]!=SBUS_RX_Data[i]&&i!=5){
						Usart5_recv_Data[i]=0;
						return;
					}
				}
				SBUS_RX_init_bl=0;
			}
            Usart5_Callback_Function(huart, Usart5_recv_Data);
        }
    }
    else if (huart == &huart7)
    {
        if (Usart7_Callback_Function != nullptr)
        {
            Usart7_Callback_Function(huart, Usart7_recv_Data);
        }
    }
    else if (huart == &huart10)
    {
        if (Usart10_Callback_Function != nullptr)
        {
            Usart10_Callback_Function(huart, Usart3_recv_Data);
        }
    }
}

