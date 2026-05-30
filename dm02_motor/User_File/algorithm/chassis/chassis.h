//
// Created by 20159 on 2026/4/1.
//

#ifndef CHASSIS_H
#define CHASSIS_H

#include "struct_typedef.h"


typedef enum{
	CHASSIS_A = 0,
	CHASSIS_B = 1
}chassis_M_angle;

struct chassis_M_wheel
{
    fp32 speed;
    chassis_M_angle wheel;
    fp32 X_Y[2];
};
//创建麦轮控制类
class Class_chassis_M4{
    
    public:

    void Init(const fp32 __wheel1[2],const fp32 __wheel2[2],const fp32 __wheel3[2],const fp32 __wheel4[2],const chassis_M_angle __Angle[4],const fp32 &__R);

    void count(const fp32 __X_Y[2],const fp32 &__W);

    inline fp32 Get_Speed(const uint8_t &__wheel) const;

    protected:
    fp32 x;
    fp32 y;
    fp32 w;
    fp32 r;
    chassis_M_wheel chassis[4];

};

inline fp32 Class_chassis_M4::Get_Speed(const uint8_t &__wheel) const{
    return chassis[__wheel].speed;
}

struct chassis_D_wheel
{
    fp32 speed;
    fp32 Pos;
    fp32 original_Pos;
    fp32 Pos_last;
    fp32 X_Y[2];
};

//创建舵轮控制类
class Class_chassis_D4{
    
    public:
// x y Original
    void Init(const fp32 __wheel1[3],const fp32 __wheel2[3],const fp32 __wheel3[3],const fp32 __wheel4[3],const fp32 &__R);

    void count(const fp32 __X_Y[2],const fp32 &__W,const fp32 __wheel_last_pos[4]);

    inline fp32 Get_Speed(const uint8_t &__wheel) const;

	inline fp32 Get_Angle(const uint8_t &__wheel) const;

    protected:
    fp32 x;
    fp32 y;
    fp32 w;
    fp32 r;
    chassis_D_wheel chassis[4];

};

inline fp32 Class_chassis_D4::Get_Speed(const uint8_t &__wheel) const{
    return chassis[__wheel].speed;
}

inline fp32 Class_chassis_D4::Get_Angle(const uint8_t &__wheel) const{
    return chassis[__wheel].Pos;
}

#endif //DM02_CHASSIS_H