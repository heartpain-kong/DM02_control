#ifndef REMOTE_CONTROL_H
#define REMOTE_CONTROL_H

#include "struct_typedef.h"

#include "UART.h"

#define RC_CH_VALUE_OFFSET		1024U

struct Remote_Info_Typedef
{
    struct
	{
		int16_t ch[5];
		uint8_t s[2];
    } rc;
	struct
    {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    } mouse;
    struct
    {
        uint16_t v;
    } key;

	bool rc_lost;   
	uint8_t online_cnt;   
} ;

extern Remote_Info_Typedef remote_ctrl;

void remote_control_init();


#endif

