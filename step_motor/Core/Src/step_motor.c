//
// Created by Administrator on 2025/1/4.
//
#include "step_motor.h"

Step_Motor motor1;
Step_Motor motor2;
void Step_Motor_Enable(Step_Motor *motor){
    if(motor_data_buff==0x6B && motor_data[1]==0xF3)
    {
        if (motor_data[2]==0x02)
        {
            printf("Succeed to Enable\r\n");
        } else if(motor_data[2]==0xE2)
        {
            printf("Fail to Enable\r\n");
        }
    } else
    {
        char cmd[6]={0};
        cmd[0]=motor->id;
        cmd[1]=0xF3;
        cmd[2]=0xAB;
        cmd[3]=0x01;
        cmd[4]=0x00;
        cmd[5]=0x6B;
        HAL_UART_Transmit(motor->huart,cmd,6,100);
    }

}

void Step_Motor_Disable(Step_Motor *motor){
    char cmd[6]={0};
    cmd[0]=motor->id;
    cmd[1]=0xF3;
    cmd[2]=0xAB;
    cmd[3]=0x00;
    cmd[4]=0x00;
    cmd[5]=0x6B;
    HAL_UART_Transmit(motor->huart,cmd,6,100);
//    HAL_Delay(100);
}

void Step_Motor_Vel_Control(Step_Motor *motor,uint8_t dir,uint16_t vel,uint8_t acc,uint8_t multi_start)
{
    char cmd[8]={0};
    cmd[0]=motor->id;
    cmd[1]=0xF6;
    cmd[2]=dir;
    cmd[3]=(vel>>8)&0xFF;
    cmd[4]=vel&0xFF;
    cmd[5]=acc;
    cmd[6]=multi_start;
    cmd[7]=0x6B;
    HAL_UART_Transmit(motor->huart,cmd,8,100);
//    HAL_Delay(100);
}


//命令格式：地址 + 0xFD + 方向 + 速度+ 加速度 + 脉冲数 + 相对/绝对模式标志 + 多机同步标志 + 校验字节
void Step_Motor_Pos_Contrpl(Step_Motor *motor,uint8_t dir,uint16_t vel,uint8_t acc,int angle,
                            uint8_t pos_mode,uint8_t multi_start){
    char cmd[13]={0};
    uint32_t pulses=(int)(angle*3200/360);
    cmd[0]=motor->id;
    cmd[1]=0xFD;
    cmd[2]=dir;
    cmd[3]=(vel>>8)&0xFF;
    cmd[4]=vel&0xFF;
    cmd[5]=acc;
    cmd[6]=(pulses>>24)&0xFF;
    cmd[7]=(pulses>>16)&0xFF;
    cmd[8]=(pulses>>8)&0xFF;
    cmd[9]=pulses&0xFF;
    cmd[10]=pos_mode;
    cmd[11]=multi_start;
    cmd[12]=0x6B;
    HAL_UART_Transmit(motor->huart,cmd,13,100);
//    HAL_Delay(100);
}

//命令格式：地址 + 0x36 + 校验字节
//命令返回：地址 + 0x36 + 符号 + 电机实时位置 + 校验字节
void Step_Motor_Get_Cur_Pos(Step_Motor *motor){
    if (motor_data_buff==0x6B && motor_data[1]==0x36)
    {
        motor->pos=((double)(1-motor_data[2]*2)*(motor_data[3]* pow(16,6)+motor_data[4]* pow(16,4)+
                motor_data[5]*pow(16,2)+motor_data[6]))/65535*360;
        printf("Pos:%lf\r\n",motor->pos);
    } else
    {
        char cmd[3]={0};
        cmd[0]=motor->id;
        cmd[1]=0x36;
        cmd[2]=0x6B;
        HAL_UART_Transmit(motor->huart,cmd,3,100);
    }
}
//命令格式：地址 + 0x35 + 校验字节
//命令返回：地址 + 0x35 + 符号 + 电机实时转速 + 校验字节
void Step_Motor_Get_Cur_Vel(Step_Motor *motor){
    if (motor_data_buff==0x6B && motor_data[1]==0x35)
    {
        motor->vel=(1-motor_data[2]*2)*(motor_data[3]* pow(16,2)+motor_data[4]);
        printf("Vel:%lf rpm\r\n",motor->vel);
    } else
    {
        char cmd[3]={0};
        cmd[0]=motor->id;
        cmd[1]=0x35;
        cmd[2]=0x6B;
        HAL_UART_Transmit(motor->huart,cmd,3,100);
    }
}

void Step_Motor_Stop(Step_Motor *motor){
    char cmd[5]={0};
    cmd[0]=motor->id;
    cmd[1]=0xFE;
    cmd[2]=0x98;
    cmd[3]=0x00;
    cmd[4]=0x6B;
    HAL_UART_Transmit(motor->huart,cmd,5,100);
//    HAL_Delay(100);
}


