#include <reg52.h>

// LED引脚定义
sbit LED1 = P0^0;
sbit LED2 = P0^1;
sbit LED3 = P0^2;
sbit LED4 = P0^3;
sbit LED5 = P0^4;
sbit LED6 = P0^5;
sbit LED7 = P0^6;
sbit LED8 = P0^7;

// 按键引脚定义 - 修正这里
sbit KEY = P3^5;  // 定义P3.5为按键引脚

// 全局变量声明
unsigned int Cnt_ms = 0;
bit flag0 = 0;
bit flag1 = 0;

unsigned char led1_count = 0;
unsigned char led2_count = 0;
unsigned char led3_count = 0;
unsigned char led4_count = 0;

unsigned int key_count = 0;
unsigned char display_count = 0;

// 防抖变量
bit key_flag = 0;
unsigned int key_debounce = 0;

// 定时器初始化函数
void InitTimer0()
{
    TMOD |= 0x01;    // 设置定时器0为模式1(16位定时器)
    TH0 = 0xFC;      // 定时1ms，11.0592MHz
    TL0 = 0x66;
    ET0 = 1;         // 开启定时器0中断
    TR0 = 1;         // 启动定时器0
}

void InitTimer1()
{
    TMOD |= 0x10;    // 设置定时器1为模式1(16位定时器)
    TH1 = 0xFC;      // 定时1ms，11.0592MHz
    TL1 = 0x66;
    ET1 = 1;         // 开启定时器1中断
    TR1 = 1;         // 启动定时器1
}

// 显示函数 - 在LED5~LED8上显示按键计数对4取余的结果
void DisplayKeyCount()
{
    unsigned char remainder = key_count % 4;
    
    // 清除LED5~LED8
    LED5 = 1;
    LED6 = 1;
    LED7 = 1;
    LED8 = 1;
    
    // 根据余数点亮相应的LED
    switch(remainder)
    {
        case 1:
            LED5 = 0;
            break;
        case 2:
            LED5 = 0;
            LED6 = 0;
            break;
        case 3:
            LED5 = 0;
            LED6 = 0;
            LED7 = 0;
            break;
        case 0:
            break;
        default:
            break;
    }
}

// 按键检测函数（带防抖）- 修正这里
void KeyScan()
{
    if(KEY == 0)  // 检测按键是否按下，使用定义好的KEY
    {
        if(key_flag == 0)
        {
            key_debounce++;
            if(key_debounce >= 20)  // 20ms防抖
            {
                key_flag = 1;
                key_debounce = 0;
                key_count++;        // 按键计数加1
                DisplayKeyCount();  // 更新显示
            }
        }
    }
    else
    {
        key_flag = 0;
        key_debounce = 0;
    }
}

void main()
{
    // 初始化所有LED为熄灭状态
    P1 = 0xFF;
    
    InitTimer0();  // 定时器0初始化
    InitTimer1();  // 定时器1初始化
    EA = 1;        // 开启总中断
    
    while(1)
    {
        if(flag0)  // 判断标志
        {
            // 时间片0的操作 - LED闪烁控制
            flag0 = 0;
            
            // LED1 - 500ms周期 (250ms亮，250ms灭)
            if(Cnt_ms % 500 == 0)
            {
                LED1 = ~LED1;
            }
            
            // LED2 - 1s周期 (500ms亮，500ms灭)
            if(Cnt_ms % 1000 == 0)
            {
                LED2 = ~LED2;
            }
            
            // LED3 - 2s周期 (1s亮，1s灭)
            if(Cnt_ms % 2000 == 0)
            {
                LED3 = ~LED3;
            }
            
            // LED4 - 5s周期 (2.5s亮，2.5s灭)
            if(Cnt_ms % 5000 == 0)
            {
                LED4 = ~LED4;
            }
        }
        else if(flag1)  // 判断标志
        {
            // 时间片1的操作 - 按键检测
            flag1 = 0;
            KeyScan();
        }
    }
}

// Timer0中断服务函数
void ISP_Timer0() interrupt 1
{
    TH0 = 0xFC;  // 重新加载定时值
    TL0 = 0x66;
    
    Cnt_ms++;    // 毫秒计数加1
    
    // 每1ms设置标志
    flag0 = 1;
    
    // 防止计数溢出
    if(Cnt_ms >= 60000)
    {
        Cnt_ms = 0;
    }
}

// Timer1中断服务函数
void ISP_Timer1() interrupt 3
{
    TH1 = 0xFC;  // 重新加载定时值
    TL1 = 0x66;
    
    // 每1ms设置标志
    flag1 = 1;
}
