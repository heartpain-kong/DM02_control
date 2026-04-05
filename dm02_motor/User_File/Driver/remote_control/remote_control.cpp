#include "remote_control.h"
#include "string.h"

extern UART_HandleTypeDef huart5;

Remote_Info_Typedef remote_ctrl={
	
    .online_cnt = 0xFAU,
	.rc_lost = true,
};

void Remote_Message_Moniter(Remote_Info_Typedef  *__remote_ctrl);
void SBUS_TO_RC(volatile const uint8_t *sbus_buf, Remote_Info_Typedef  *__remote_ctrl);
/**
 * @brief 遥控器接收回调函数
 * @param huart  串口编号
 * @param Buffer  接收数据
 * @return void
 */
void SBUS_TO_RC_Handle(UART_HandleTypeDef *Header, uint8_t *Buffer){
    SBUS_TO_RC(Buffer,&remote_ctrl );

}

/**
 * @brief 遥控器接收初始化
 * @return void
 */
void remote_control_init(){
    USART_init(&huart5,SBUS_TO_RC_Handle);
}

void SBUS_TO_RC(volatile const uint8_t *sbus_buf, Remote_Info_Typedef  *__remote_ctrl)
{
    if (sbus_buf == NULL || __remote_ctrl == NULL) return;

    __remote_ctrl->rc.ch[0] = (  sbus_buf[0]       | (sbus_buf[1] << 8 ) ) & 0x07ff;                            //!< Channel 0
    __remote_ctrl->rc.ch[1] = ( (sbus_buf[1] >> 3) | (sbus_buf[2] << 5 ) ) & 0x07ff;                            //!< Channel 1
    __remote_ctrl->rc.ch[2] = ( (sbus_buf[2] >> 6) | (sbus_buf[3] << 2 ) | (sbus_buf[4] << 10) ) & 0x07ff;      //!< Channel 2
    __remote_ctrl->rc.ch[3] = ( (sbus_buf[4] >> 1) | (sbus_buf[5] << 7 ) ) & 0x07ff;                            //!< Channel 3
    __remote_ctrl->rc.ch[4] = (  sbus_buf[16] 	   | (sbus_buf[17] << 8) ) & 0x07ff;                 			      //!< Channel 4

    __remote_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                  //!< Switch left
    __remote_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;             //!< Switch right

    __remote_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X axis
    __remote_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y axis
    __remote_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z axis
    __remote_ctrl->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press ?
    __remote_ctrl->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press ?
    __remote_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                    //!< KeyBoard value             //NULL

    __remote_ctrl->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    __remote_ctrl->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    __remote_ctrl->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    __remote_ctrl->rc.ch[3] -= RC_CH_VALUE_OFFSET;
    __remote_ctrl->rc.ch[4] -= RC_CH_VALUE_OFFSET;

	__remote_ctrl->online_cnt = 0xFAU;
		
	__remote_ctrl->rc_lost = false;
}

void Remote_Message_Moniter(Remote_Info_Typedef  *__remote_ctrl)
{
 
  if(__remote_ctrl->online_cnt <= 0x32U)
  {
    memset(__remote_ctrl,0,sizeof(Remote_Info_Typedef));

		__remote_ctrl->rc_lost = true;
		
  }
  else if(__remote_ctrl->online_cnt > 0)
  {
    __remote_ctrl->online_cnt--;
  }
}
