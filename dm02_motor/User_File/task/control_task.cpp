#include "control_task.h"
#include "math_support.h"
#include "motor_task.h"
#include "remote_control.h"
#include "tim.h"

/**
 * @brief 1ms的回调函数
 * @return void
 */
 
void Task1ms_Callback(){
	motor_task();

	
}

/**
 * @brief 500us的回调函数
 * @return void
 */
void Task500us_Callback (){

}


void Task25ns_Callback (){

}

/**
 * @brief 控制任务的初始化
 * @return void
 */
void control_task_init(){
	remote_control_init();
	motor_task_init();
    HAL_TIM_Base_Start_IT(&htim7);
    HAL_TIM_Base_Start_IT(&htim8);
}


/**
 * @brief 控制任务
 * @return void
 */
void control_task (){
	
	

}


/**
 * @brief 定时器的中断回调函数
 * @param htim   定时器TIM_HandleTypeDef的指针
 * @return void
 */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7)
    {
        Task500us_Callback();    //500ns的定时
    }
    else if (htim->Instance == TIM8)
    {
        Task1ms_Callback();   //1ms的定时
    }

}


