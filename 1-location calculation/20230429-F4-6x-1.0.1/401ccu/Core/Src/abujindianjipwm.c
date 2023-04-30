/*
 * abujindianjipwm.c
 *
 *  Created on: 2023年3月23日
 *      Author: HuaWei
 *
 *      使用方法：
 *      设置一个定时器的pwm输出
 *      一般定时器频率为32000000赫兹
 *      在下面宏定义中设置陪我们宽度 看情况设置
 *
 *      需要使用时开启定时器中断 HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
 		HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
		在中断中调用pwmzhongduankongzhi()函数
 *      然后直接写入预定位置，电机会直接加速启动，可以在任意时刻设置任意位置，
 *      电机会自动控制加减速并达到目标位置
 *      不需要使用时可以关闭定时器节省CPU，HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
 *      每次开始使用时最好根据自己电机的情况刷新一遍速度控制表格，jiasuleibiaojisuan(32000000,500,10000,31004);
 *      然后如果对电机转速有需要，可以限制最大速度，其中S2B函数是将速度转化成列表中对应的值
 *      dianji[0].zuidasudu = s2b(32000000,20000);//设置最大速度
 *      最后直接将目标位置写入就ok
  	  	dianji[0].yudingweizhi = 200000;//设置位置

  	  	改方法最高频率大概为200khz，正常使用时不建议将频率超过40khz
  	  	主要时加速表使用了ram，而ram的内存有限，如果使用动态加速表，只能有限的资源，当加速度不是很大时ram会不够用
  	  	所以无法达到非常高的速度
  	  	解决方法是可以使用一张rom的表来存放加减速表格。
  	  	如果对位置精度要求不高的场合，可以使用动态速度控制。实时计算当前需要的速度。总之40k的速度对于一般场合够用了。

寄存器说明：
地址0： 定时器最大频率，默认32000000，不需要改变。
地址1：输出脉冲的最小频率，默认500步每秒，不需要改变。如果太小会导致定时器溢出。
地址2：输出脉冲的最大频率，默认32K步每秒，F4的芯片一般不得大于96K，否则计数器来不及导致脉冲数量错误。
地址3：电机加速度，默认64K步每秒，太大导致电机跟步上，ram太小导致达不到所需的速度。（如果使用主循环速度计算器无需考虑ram太小的问题
地址4：电机绝对位置，向该地址写入数据，电机会达到该指定位置
地址5：写入位置之后是否立即启动电机，0立即启动。1等待该位置2时启动。当该位为1时，写入地址4之后电机不会启动，要再向此位置写入2才会启动，然后该位会自动置1。
地址6：当写入地址4或者5之后，是否返回数据，0表示返回，1表示不返回


jiasuleibiaojisuan(32000000,500,100000,3004);//先计算一遍加速表
//
    dianji[0].zuidasudu = 2000;//设置�?????大�?�度
    chuankoufasong(219,dianji[0].zuidasudu);
    dianji[0].yudingweizhi = 200000;//设置位置

void TIM1_UP_TIM10_IRQHandler(void)
{
	shishisuduyunsuan();
  HAL_TIM_IRQHandler(&htim1);

}
void TIM1_BRK_TIM9_IRQHandler(void)
{
	pwmzhongduankongzhi();
  HAL_TIM_IRQHandler(&htim9);
}
-----------------------------------------------------------------------
//外部计算调用此函数
void shishisuduyunsuan(void);
-------------------------------------------------------------------------
void EXTI9_5_IRQHandler(voids)
{
	if(dianji[0].shijifangxiang == 1)
	{
		dianji[0].jiluweizhi  ++;
	}
	if(dianji[0].shijifangxiang == 0)
	{
		dianji[0].jiluweizhi  --;
	}
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
}

 */
#include "main.h"
#include "math.h"
#include "stm32f4xx_hal.h"
#include "abujindianjipwm.h"

#define ccr  100
#define PWMKUANDU TIM9->CCR1 //PWM宽度
#define DINGSHIQIZHI TIM9->CNT//定时器计时值
#define DINGSHIQICHONGZHI TIM9->ARR//定时器重置值
#define FANGXIANGZHENG HAL_GPIO_WritePin(bu1f_GPIO_Port, bu1f_Pin,GPIO_PIN_RESET)//正转控制
#define FANGXIANGFAN HAL_GPIO_WritePin(bu1f_GPIO_Port, bu1f_Pin,GPIO_PIN_SET)//反转控制

uint16_t jiasuliebiao[3006];
bujinpwm dianji[4];

uint16_t dingshi;

void pwmzhongduankongzhi(void)
{

	//如果用的不是pwm方式，首先交换引脚电平给出脉冲�??
	//jjj[0] = TIM4->CNT;

	if(PWMKUANDU > 0 )//判断电机方向，增加绝对位置或者减少绝对位
	{
		if(dianji[0].dangqiansudu > 0)dianji[0].dangqianweizhi++;
		if(dianji[0].dangqiansudu < 0)dianji[0].dangqianweizhi--;
	}

	//判断电机正转还是反转
	if(dianji[0].dangqiansudu > 0)
	{
		dianji[0].yudingsudu = dianji[0].zuidasudu;
		//设置方向引脚输出-20230322
		//--------------------------------------------------------------速度控制�???-20230322
		if(dianji[0].yudingsudu > dianji[0].dangqiansudu)// 如果没有达到�???大度，继续加�???-20230322
		{
			dianji[0].sudukongzhi = 1;
		}else{
				if(dianji[0].yudingsudu == dianji[0].dangqiansudu)
				{
					dianji[0].sudukongzhi = 0;//否则不加�???-20230322
				}else{
					dianji[0].sudukongzhi = -1;//否则不加�???-20230322
				}
			}
		dianji[0].tingzhiweizhi = dianji[0].dangqianweizhi + dianji[0].dangqiansudu;// 计算得到当前停止�??????
		// 判断电机是否�???要减�???-20230322
		if(dianji[0].tingzhiweizhi >= dianji[0].yudingweizhi)
		{
			dianji[0].sudukongzhi = -1;
		}
		dianji[0].dangqiansudu = dianji[0].dangqiansudu + dianji[0].sudukongzhi;
		//DINGSHIQICHONGZHI =jiasuliebiao[ dianji[0].dangqiansudu];// 设置定时器中断时�??????
		//DINGSHIQICHONGZHI = 32000000 / ( sqrt(250000 + 2 * 50000 * dianji[0].dangqiansudu));
		DINGSHIQICHONGZHI = dianji[0].dingshi;
		//---------------------------------------------------------------
		if(PWMKUANDU == 0)
		{
			while(DINGSHIQIZHI <= ccr);
			PWMKUANDU = ccr;
		}
	}
	if(dianji[0].dangqiansudu < 0)
	{
		//设置方向引脚输出-20230322
		dianji[0].yudingsudu = -dianji[0].zuidasudu;
		//--------------------------------------------------------------速度控制�???-20230322
		if(dianji[0].yudingsudu < dianji[0].dangqiansudu)// 如果没有达到�???大度，继续加�???-20230322
		{
			dianji[0].sudukongzhi = -1;
		}else{
			if(dianji[0].yudingsudu == dianji[0].dangqiansudu)
			{
				dianji[0].sudukongzhi = 0;//否则不加�???-20230322
			}else{
				dianji[0].sudukongzhi = 1;//否则不加�???-20230322
			}
		}
		dianji[0].tingzhiweizhi = dianji[0].dangqianweizhi + dianji[0].dangqiansudu;// 计算得到当前停止�??????
		// 判断电机是否�???要减�???-20230322
		if(dianji[0].tingzhiweizhi <= dianji[0].yudingweizhi)
		{
			dianji[0].sudukongzhi = 1;
		}
		dianji[0].dangqiansudu = dianji[0].dangqiansudu + dianji[0].sudukongzhi;
		//DINGSHIQICHONGZHI =jiasuliebiao[ -dianji[0].dangqiansudu];// 设置定时器中断时�??????
		//DINGSHIQICHONGZHI = 32000000 / ( sqrt(250000 + 2 * 50000 * (-dianji[0].dangqiansudu)));
		DINGSHIQICHONGZHI = dianji[0].dingshi;
		//---------------------------------------------------------------
		if(PWMKUANDU == 0)
		{
			while(DINGSHIQIZHI <= ccr);
			PWMKUANDU = ccr;
		}
	}

	if(dianji[0].dangqiansudu == 0)
	{
		PWMKUANDU = 0;
			if(dianji[0].dangqianweizhi < dianji[0].yudingweizhi)
			{
				  dianji[0].yudingsudu = dianji[0].zuidasudu;
				  dianji[0].dangqiansudu = 1;
				  while(DINGSHIQIZHI <= ccr);
				  PWMKUANDU = ccr;
			}
			if(dianji[0].dangqianweizhi > dianji[0].yudingweizhi)
			{
				  dianji[0].yudingsudu = -dianji[0].zuidasudu;
				  dianji[0].dangqiansudu = -1;
				  while(DINGSHIQIZHI <= ccr);
				  PWMKUANDU = ccr;
			}
	}
	if(dianji[0].dangqiansudu > 0)
		{
		dianji[0].shijifangxiang = 1;
			FANGXIANGZHENG;
		}
	if(dianji[0].dangqiansudu < 0)
		{
		dianji[0].shijifangxiang = 0;
			FANGXIANGFAN;
		}
	//jjj[1] = TIM4->CNT;
	//判断下一个脉冲所�?????????要延迟的时间，设置定时器�?????????
	// 如果目标位置和当前位置重合，做出动作
}

//参数1 定时器最小分辨率
//参数2 初始速度
//参数3 加速度
//参数4 需要得到的列表成员数
void jiasuleibiaojisuan(uint64_t timc,uint64_t u,uint64_t a ,uint64_t len)
{
	uint64_t i;
	for(i = 0; i < len ; i++)
	{
		jiasuliebiao[ i ] = timc / ( sqrt(u*u + 2 * a * i));
	}
}

//输入目标速度，返回列表中对应的值
//参数1 定时器最大频率
//参数2 目标速度
uint32_t s2b(uint32_t timc,uint32_t s)
{
	uint32_t i = 0;
	if((timc / jiasuliebiao[31000]) < s)
	{
		return 31000;
	}
	while((timc / jiasuliebiao[i]) < s )
	{
		i++;
	}
	return i;
}
//输入目标速度，返回列表中对应的值

//参数1 初始速度
//参数2 加速度
//参数3 目标速度
//返回达到目标速度所需的步数
uint64_t s2b2(uint64_t u,uint64_t a ,uint64_t s)
{
	uint64_t i = 0,x;
	//chuankoufasong(210,s);

	x =(uint64_t) (s * s);
	//chuankoufasong(211,x);
	//chuankoufasong(213,a);
	x = (x - u*u)/2/a;
	//chuankoufasong(212,x);
	return x;
}

//使用外部速度计算器时，周期性调用此函数计算速度
//最小速度的平方加2倍加速度
void shishisuduyunsuan(void)
{
	if(dianji[0].dangqiansudu < 0)
	dianji[0].dingshi = 32000000 / ( sqrt(250000 + 2 * 10000 * (-dianji[0].dangqiansudu)));
	if(dianji[0].dangqiansudu >= 0)
	dianji[0].dingshi = 32000000 / ( sqrt(250000 + 2 * 10000 * (dianji[0].dangqiansudu)));
}



