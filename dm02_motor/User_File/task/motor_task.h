#ifndef MOTOR_TASK_H
#define MOTOR_TASK_H

#ifdef __cplusplus

extern "C"
{
#endif
	
#include "struct_typedef.h"
extern uint8_t motor_lz_init;

void motor_LZ_send_Init(uint8_t bl);
void motor_task_init();
void motor_task();

#ifdef __cplusplus
};

#endif

#endif