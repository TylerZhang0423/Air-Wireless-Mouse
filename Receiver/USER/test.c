#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 	 	 
#include "key.h"	 	 
#include "exti.h"	 	 
#include "wdg.h" 	 
#include "timer.h"		 	 		 	 
#include "lcd.h"	
#include "rtc.h"	 	 
#include "wkup.h"	
#include "adc.h" 	 
#include "dac.h" 	 
#include "dma.h" 	 
#include "24cxx.h" 	 
#include "flash.h" 	 	 
#include "can.h" 	  	 
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
						  
//计算x1,x2的绝对值
u32 usb_abs(u32 x1,u32 x2)
{
	if(x1>x2)return x1-x2;
	else return x2-x1;
}
//设置USB 连接/断线
//enable:0,断开
//       1,允许连接	   
void usb_port_set(u8 enable)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//退出断电模式
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // 断电模式
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;	    		  
	}
}  	  	  
int main(void)
{	
	u8 key;
	u8 i=0;
	short X,Y;		 //发送到电脑端的坐标值
	u8 usbstatus=0;	
	u8 tmp_buf[5];	 //发射寄存器
	u8 keysta;		//[0]:0,左键松开;1,左键按下;
					//[1]:0,右键松开;1,右键按下
					//[2]:0,中键松开;1,中键按下		
   	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,9600); //串口1初始化  	  
	LCD_Init();			//初始化液晶 
	LED_Init();         //LED初始化	 
	KEY_Init();			//按键初始化
	NRF24L01_Init();		//24l01初始化	
	IIC_Init();             //I2C初始化
	POINT_COLOR=BLUE;		//设置字体为蓝色
	LCD_ShowString(30,110,200,16,16,"This is receiver");
	while(NRF24L01_Check())
	{
		LCD_ShowString(30,130,200,16,16,"NRF24L01 error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"NRF24L01 is ready"); 
	NRF24L01_RX_Mode();      //NRF24L01设置为接收模式
  	delay_ms(1800);
 	usb_port_set(0); 	//USB先断开
	delay_ms(300);
   	usb_port_set(1);	//USB再次连接
	//USB配置
 	USB_Interrupts_Config();    
 	Set_USBClock();   
 	USB_Init();
	LCD_ShowString(30,150,200,16,16,"USB is ready"); 
	while(1)
	{	
		if(usbstatus!=bDeviceState)//USB连接状态发生了改变.
		{
			usbstatus=bDeviceState;//记录新的状态
			if(usbstatus==CONFIGURED)
			{
				LCD_ShowString(30,170,200,16,16,"USB Connected    ");//提示USB连接成功
				LED1=0;//DS1亮
			}else
			{
				LCD_ShowString(30,170,200,16,16,"USB disConnected ");//提示USB断开
				LED1=1;//DS1灭
			}
		}			
		if(NRF24L01_RxPacket(tmp_buf)==0)
		{
			key=KEY_Scan(1);//支持连按
			X=(s16)((tmp_buf[1]<<8)|tmp_buf[2]);
			Y=(s16)((tmp_buf[3]<<8)|tmp_buf[4]);
			if(key==KEY_LEFT)keysta|=0X01;					//发送鼠标左键    
			if(key==KEY_RIGHT)keysta|=0X02;					//发送鼠标右键
			Joystick_Send(keysta,X,Y,0);	 				//发送给电脑
			LCD_ShowString(30,210,200,16,16,"X:");LCD_ShowNum(120,210,X,10,16);
			LCD_ShowString(30,230,200,16,16,"Y:");LCD_ShowNum(120,230,Y,10,16);
		}
		if(bDeviceState==CONFIGURED)LED1=0;          //当USB配置成功了，LED1亮，否则，灭
		else LED1=1;    
		i++;
		if(i==200)
		{
			i=0;
			LED0=!LED0;
		}
	}	   										    			    
}


















