//
// Created by Administrator on 2025/1/4.
//

#ifndef STEP_MOTOR_STEP_MOTOR_H
#define STEP_MOTOR_STEP_MOTOR_H

#include "retarget.h"
#include "usart.h"
#include "stdio.h"
#include "math.h"

#define CW 0x01
#define CCW 0x00
#define AB_POS 0x01
#define RE_POS 0x00
#define MUL_START 0x01
#define SIN_START 0x00

typedef struct STEP_MOTOR{
    UART_HandleTypeDef *huart;
    uint8_t id;
    double vel;
    double pos;
}Step_Motor;

extern Step_Motor motor1;
extern Step_Motor motor2;
void Step_Motor_Enable(Step_Motor *motor);
void Step_Motor_Disable(Step_Motor *motor);

void Step_Motor_Vel_Control(Step_Motor *motor,uint8_t dir,uint16_t vel,uint8_t acc,uint8_t multi_start);
void Step_Motor_Pos_Contrpl(Step_Motor *motor,uint8_t dir,uint16_t vel,uint8_t acc,int angle,
                            uint8_t pos_mode,uint8_t multi_start);

void Step_Motor_Get_Cur_Pos(Step_Motor *motor);
void Step_Motor_Get_Cur_Vel(Step_Motor *motor);

void Step_Motor_Stop(Step_Motor *motor);

#endif //STEP_MOTOR_STEP_MOTOR_H
