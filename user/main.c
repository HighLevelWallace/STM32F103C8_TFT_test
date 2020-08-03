//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//  ��������   : 1.44��LCD 4�ӿ���ʾ����(STM32ϵ��)
/******************************************************************************
//������������STM32F103C8
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB0
//              DC    ��PB1
//              CS    ��PA4 
//							BL		��PB10
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
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    
    /* ����NVICΪ���ȼ���1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    
    /* �����ж�Դ������1 */
    NVIC_InitStructure.NVIC_IRQChannel =  EXTI1_IRQn;
    /* ������ռ���ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* ���������ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* ʹ���ж�ͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  ���� IOΪEXTI�жϿڣ��������ж����ȼ�
  * @param  ��
  * @retval ��
  */
void EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    EXTI_InitTypeDef EXTI_InitStructure;

    /*��������GPIO�ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
                                                                                        
    /* ���� NVIC �ж�*/
    NVIC_Configuration();

    /*--------------------------��������-----------------------------*/
    /* ѡ�񰴼��õ���GPIO */	
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_10;
    /* ����Ϊ�������� */	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

    GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	
	
    /* -------------ѡ��EXTI���ź�Դ -------------*/
    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB,  GPIO_PinSource1); 
    EXTI_InitStructure.EXTI_Line =  EXTI_Line1;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �������ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Rising_Falling
    /* ʹ���ж� */
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
	LCD_LED_SET;//ͨ��IO���Ʊ�����				
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
