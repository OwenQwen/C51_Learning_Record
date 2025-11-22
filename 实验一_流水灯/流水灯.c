#include <REGX52.H>
#include <INTRINS.H>

void Delay500ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	i = 4;
	j = 129;
	k = 119;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}




#define LED_PORT P0



// 定义LED模式数组
unsigned char led_patterns[] = {
    0xF8, // 1111 1000 - LED0,1,2亮
    0xF1, // 1111 0001 - LED1,2,3亮  
    0xE3, // 1110 0011 - LED2,3,4亮
    0xC7, // 1100 0111 - LED3,4,5亮
    0x8F, // 1000 1111 - LED4,5,6亮
    0x1F, // 0001 1111 - LED5,6,7亮
    0x3F, // 0011 1111 - LED6,7亮（末端特殊情况）
    0x7F  // 0111 1111 - LED7亮（末端特殊情况）
};

void main()
{	
    unsigned char i;
    unsigned char direction = 0; // 0:向右移动, 1:向左移动
    
    while(1) 
	{
        if(direction == 0) 
			{
            // 向右移动
            for(i = 0; i < 6; i++) 
				{ // 正常移动范围是0-5
                LED_PORT = led_patterns[i];
               Delay500ms();
				}
            // 到达右端，改变方向
            direction = 1;
			} 
			else 
			{
            // 向左移动
            for(i = 5; i > 0; i--) 
				{ // 从5递减到1
                LED_PORT = led_patterns[i];
				Delay500ms();
				// 到达左端，改变方向
				direction = 0;
				}
			}
	}
}
