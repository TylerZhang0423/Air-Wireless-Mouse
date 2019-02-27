#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//ADC 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2015/1/14
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20150114
//1,在Adc_Init函数增加了内部温度测量的初始化参数。
//2,新增short Get_Temprate(void)函数，用于获取内部温度值
//V1.2 20150114
//1,新增Adc3_Init函数,用于初始化ADC3
//2,新增Get_Adc3函数,用于读取ADC3的某个通道	
////////////////////////////////////////////////////////////////////////////////// 
								  
#define ADC_CH1 		1  			//通道1(连接在PA1)	 	    
#define ADC_CH_TEMP  	16 			//温度传感器通道
	   									   
void Adc_Init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道10次采样的平均值 	
short Get_Temprate(void);			//获取内部温度传感器温度值
void Adc3_Init(void); 				//ADC3初始化
u16  Get_Adc3(u8 ch); 				//获得ADC3某个通道值  
#endif 















