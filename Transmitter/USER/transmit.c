#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"  
#include "mpu6050.h"
#include "usmart.h"   
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "mpuiic.h"
#include "24cxx.h"
#include "spi.h"
#include "adc.h"
#include "dac.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "24l01.h"
#include "myiic.h"
#include "lcd.h"


int main(void)
{
	short gx,gy,gz;
	u8 tmp_buf[5];	 //发射寄存器
 	signed short X,Y;		 //发送到电脑端的坐标值	
	Stm32_Clock_Init(9);	//系统时钟设置
	uart_init(72,115200);	//串口初始化为115200
	delay_init(72);	   	 	//延时初始化 
	LED_Init();		  		//初始化与LED连接的硬件接口
 	LCD_Init();				//初始化LCD
 	POINT_COLOR=RED;		//设置字体为红色 
	KEY_Init();             //按键初始化
	MPU_Init();				//MPU6050初始化
	NRF24L01_Init();		//24l01初始化
	IIC_Init();             //I2C初始化
	LCD_ShowString(30,90,200,16,16,"This is transmit");
	LCD_ShowString(30,110,200,16,16,"MPU6050 is ready");
	while(NRF24L01_Check())
	{
		LCD_ShowString(30,130,200,16,16,"NRF24L01 error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	LCD_ShowString(30,130,200,16,16,"NRF24L01 is ready"); 
	NRF24L01_TX_Mode();      //设置为发送端
	while(1)
	{
		MPU_Get_Gyroscope(&gx,&gy,&gz);
		if(gx*gx>1500)
			Y=gx/15;
		else
			Y=0;

		if(gz*gz>1500)
			X=-gz/15;
		else
			X=0;
		tmp_buf[1]=(u8)((X>>8)&0x00ff);
		tmp_buf[2]=(u8)(X&0x00ff);
		tmp_buf[3]=(u8)((Y>>8)&0x00ff);
		tmp_buf[4]=(u8)(Y&0x00ff);
		NRF24L01_TxPacket(tmp_buf);
		LCD_ShowString(30,150,200,16,16,"X:");LCD_ShowNum(120,150,X,10,16);
		LCD_ShowString(30,170,200,16,16,"Y:");LCD_ShowNum(120,170,Y,10,16);
	}
	
	
}
