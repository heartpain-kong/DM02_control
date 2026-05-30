//
// Created by 20159 on 2026/4/1.
//

#include "chassis.h"
#include "math_support.h"

static fp32 chassis_M_math_sin_cos[2] = {0.707106,-0.707106};	//45 135
static fp32 chassis_M_math_angle[2] = {PI/4,3*PI/4};	//45 135

/**
 * @brief 麦轮控制的初始化
 * @param __wheel1 第一个轮子坐标系
 * @param __wheel2 第二个轮子坐标系
 * @param __wheel3 第三个轮子坐标系
 * @param __wheel4 第四个轮子坐标系
 * @param chassis_M_angle 麦轮是A还B 4个对应4个轮子1-4
 * @param __R 轮子半径
 * @return void
 */
void Class_chassis_M4::Init
    (const fp32 __wheel1[2],const fp32 __wheel2[2],const fp32 __wheel3[2],const fp32 __wheel4[2],const chassis_M_angle __Angle[4],const fp32 &__R){
    r = __R;
    chassis[0].X_Y[0] = __wheel1[0];
    chassis[0].X_Y[1] = __wheel1[1];
    chassis[0].wheel = __Angle[0];
    chassis[1].X_Y[0] = __wheel2[0];
    chassis[1].X_Y[1] = __wheel2[1];
    chassis[1].wheel = __Angle[1];
    chassis[2].X_Y[0] = __wheel3[0];
    chassis[2].X_Y[1] = __wheel3[1];
    chassis[2].wheel = __Angle[2];
    chassis[3].X_Y[0] = __wheel4[0];
    chassis[3].X_Y[1] = __wheel4[1];
    chassis[3].wheel = __Angle[3];    

}

void Class_chassis_M4::count(const fp32 __X_Y[2],const fp32 &__W){
    static fp32 V_Y,V_X,math_M_angle,M_V_mold;
    for(uint8_t i =0 ; i<4; ++i){
		//这一步就是世界坐标系转化成电机坐标系
		V_Y = __X_Y[1]+chassis[i].X_Y[1]*__W;
		V_X = __X_Y[0]+chassis[i].X_Y[0]*__W;
		
		//计算V的模值
		arm_sqrt_f32(V_Y *V_Y  + V_X*V_X , &M_V_mold);
		
		//计算角度用的是arccos 取值范围为[0,pi]
		math_M_angle=acos(V_X/M_V_mold);
		
		//映射到[0,2*pi]上
		if(V_Y<0.0f) math_M_angle=PI2-math_M_angle;		
		
		//计算向量的夹角
		math_M_angle -= chassis_M_math_angle[chassis[i].wheel];
		
		//通过映射公式为speed = |V|x cos<V,speed> 计算出速度
		chassis[i].speed = M_V_mold*arm_cos_f32(math_M_angle)*19.0/r;
	}
	
}

/**
 * @brief 舵轮控制的初始化
 * @param __wheel1 第一个轮子坐标系
 * @param __wheel2 第二个轮子坐标系
 * @param __wheel3 第三个轮子坐标系
 * @param __wheel4 第四个轮子坐标系
 * @param __R 轮子半径
 * @return void
 */
void Class_chassis_D4::Init
    (const fp32 __wheel1[3],const fp32 __wheel2[3],const fp32 __wheel3[3],const fp32 __wheel4[3],const fp32 &__R){

    chassis[0].X_Y[0] = __wheel1[0];
    chassis[0].X_Y[1] = __wheel1[1];
    chassis[0].original_Pos = __wheel1[2];
    chassis[1].X_Y[0] = __wheel2[0];
    chassis[1].X_Y[1] = __wheel2[1];
    chassis[1].original_Pos = __wheel2[2];
    chassis[2].X_Y[0] = __wheel3[0];
    chassis[2].X_Y[1] = __wheel3[1];
    chassis[2].original_Pos = __wheel3[2];
    chassis[3].X_Y[0] = __wheel4[0];
    chassis[3].X_Y[1] = __wheel4[1];
    chassis[3].original_Pos = __wheel4[2];
    r = __R;
}

void Class_chassis_D4::count(const fp32 __X_Y[2],const fp32 &__W,const fp32 __wheel_last_pos[4]){
    static fp32 V_Y,V_X,speed_in,angle_set_ref;

	for(uint8_t i =0 ; i<4; ++i){

		V_Y =__X_Y[0]+__W*chassis[i].X_Y[1];
		V_X =__X_Y[1]+__W*chassis[i].X_Y[0];
		
	
		arm_sqrt_f32(V_Y *V_Y  + V_X*V_X,&speed_in);
		
		
		chassis[i].Pos = acos(V_X/speed_in);
		
		
		if(V_Y<0)chassis[i].Pos=PI2-chassis[i].Pos;
		else if(V_Y==0&&V_X==0) chassis[i].Pos=chassis[i].Pos_last;
		
		chassis[i].Pos_last = __wheel_last_pos[i] - chassis[i].original_Pos;
        if(chassis[i].Pos_last < 0.0f)  chassis[i].Pos_last+=PI2;

        angle_set_ref = chassis_math_angle_set_ref(chassis[i].Pos ,chassis[i].Pos_last);
	    if(angle_set_ref > PI/2 ){
	    	chassis[i].Pos -= PI;
	    	chassis[i].Pos = chassis[i].Pos <0.0 ? PI2+chassis[i].Pos : chassis[i].Pos; 
	    	chassis[i].speed = -speed_in*19/0.061;
	    }else{
	    	if(angle_set_ref>=PI/8)chassis[i].speed=0;
	    	else chassis[i].speed = speed_in*19/r;
    	}        
		
	}    

}