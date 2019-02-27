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
						  
//����x1,x2�ľ���ֵ
u32 usb_abs(u32 x1,u32 x2)
{
	if(x1>x2)return x1-x2;
	else return x2-x1;
}
//����USB ����/����
//enable:0,�Ͽ�
//       1,��������	   
void usb_port_set(u8 enable)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));//�˳��ϵ�ģʽ
	else
	{	  
		_SetCNTR(_GetCNTR()|(1<<1));  // �ϵ�ģʽ
		GPIOA->CRH&=0XFFF00FFF;
		GPIOA->CRH|=0X00033000;
		PAout(12)=0;	    		  
	}
}  	  	  
int main(void)
{	
	u8 key;
	u8 i=0;
	short X,Y;		 //���͵����Զ˵�����ֵ
	u8 usbstatus=0;	
	u8 tmp_buf[5];	 //����Ĵ���
	u8 keysta;		//[0]:0,����ɿ�;1,�������;
					//[1]:0,�Ҽ��ɿ�;1,�Ҽ�����
					//[2]:0,�м��ɿ�;1,�м�����		
   	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);		//��ʱ��ʼ��
	uart_init(72,9600); //����1��ʼ��  	  
	LCD_Init();			//��ʼ��Һ�� 
	LED_Init();         //LED��ʼ��	 
	KEY_Init();			//������ʼ��
	NRF24L01_Init();		//24l01��ʼ��	
	IIC_Init();             //I2C��ʼ��
	POINT_COLOR=BLUE;		//��������Ϊ��ɫ
	LCD_ShowString(30,110,200,16,16,"This is receiver");
	while(NRF24L01_Check())
	{
		LCD_ShowString(30,130,200,16,16,"NRF24L01 error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"NRF24L01 is ready"); 
	NRF24L01_RX_Mode();      //NRF24L01����Ϊ����ģʽ
  	delay_ms(1800);
 	usb_port_set(0); 	//USB�ȶϿ�
	delay_ms(300);
   	usb_port_set(1);	//USB�ٴ�����
	//USB����
 	USB_Interrupts_Config();    
 	Set_USBClock();   
 	USB_Init();
	LCD_ShowString(30,150,200,16,16,"USB is ready"); 
	while(1)
	{	
		if(usbstatus!=bDeviceState)//USB����״̬�����˸ı�.
		{
			usbstatus=bDeviceState;//��¼�µ�״̬
			if(usbstatus==CONFIGURED)
			{
				LCD_ShowString(30,170,200,16,16,"USB Connected    ");//��ʾUSB���ӳɹ�
				LED1=0;//DS1��
			}else
			{
				LCD_ShowString(30,170,200,16,16,"USB disConnected ");//��ʾUSB�Ͽ�
				LED1=1;//DS1��
			}
		}			
		if(NRF24L01_RxPacket(tmp_buf)==0)
		{
			key=KEY_Scan(1);//֧������
			X=(s16)((tmp_buf[1]<<8)|tmp_buf[2]);
			Y=(s16)((tmp_buf[3]<<8)|tmp_buf[4]);
			if(key==KEY_LEFT)keysta|=0X01;					//����������    
			if(key==KEY_RIGHT)keysta|=0X02;					//��������Ҽ�
			Joystick_Send(keysta,X,Y,0);	 				//���͸�����
			LCD_ShowString(30,210,200,16,16,"X:");LCD_ShowNum(120,210,X,10,16);
			LCD_ShowString(30,230,200,16,16,"Y:");LCD_ShowNum(120,230,Y,10,16);
		}
		if(bDeviceState==CONFIGURED)LED1=0;          //��USB���óɹ��ˣ�LED1����������
		else LED1=1;    
		i++;
		if(i==200)
		{
			i=0;
			LED0=!LED0;
		}
	}	   										    			    
}


















