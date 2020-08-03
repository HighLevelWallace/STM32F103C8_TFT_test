//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//  功能描述   : 1.44寸LCD 4接口演示例程(STM32系列)
/******************************************************************************
//本程序适用与STM32F103C8
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES   接PB0
//              DC    接PB1
//              CS    接PA4 
//							BL		接PB10
*******************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "QDTFT_demo.h"
#include "Lcd_Driver.h"
#include "GUI.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_it.h"
#include "led.h"
#include "usart1.h"

int tick_count = 0;



/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* 配置NVIC为优先级组1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    /* 配置中断源：按键1 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI1_IRQn;
    /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  配置 IO为EXTI中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    EXTI_InitTypeDef EXTI_InitStructure;

    /*开启按键GPIO口的时钟*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
                                                                                        
    /* 配置 NVIC 中断*/
    NVIC_Configuration();

    /*--------------------------配置引脚-----------------------------*/
    /* 选择按键用到的GPIO */	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_10;
    /* 配置为浮空输入 */	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	
	
    /* -------------选择EXTI的信号源 -------------*/
    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB,  GPIO_PinSource1); 
    EXTI_InitStructure.EXTI_Line =  EXTI_Line1;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Rising_Falling
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
 
	
 
}



int main(void)
{
	

    GPIO_InitTypeDef GPIO_InitStructure; 
	//u8 prev_a = 0, a = 0, b = 0;
	SystemInit();	//System init.
	delay_init(72);//Delay init.
	NVIC_Configuration();
	EXTI_Config();
	Lcd_Init();
	
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	//LED_GPIO_Config();
	LCD_LED_SET;//通过IO控制背光亮				
	Lcd_Clear(GRAY0);
	USART1_Config();
	while(1)
	{  
		/*a = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
		b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
		if(a && !prev_a)  
		{	 
			tick_count += b ? 1 : -1;
		}*/
		//QDTFT_Test_Demo();	//See the test details in QDTFT_Demo.c	
		//prev_a = a;
		count_display();	
		//UART1Test();
	}
	
}





/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
