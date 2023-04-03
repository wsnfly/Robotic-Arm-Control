#include "main.h"
#include "stm32f4xx_hal.h"
#include "achuankou.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_uart.h"

#include "abujindianjipwm.h"
#define duankou1 USART1
#define duankou &huart1

////////////////////////////////
#define SHEBEIDIZHI 1
int64_t jicunqi[256];

 extern UART_HandleTypeDef huart1;
 extern bujinpwm dianji[4];
 uint8_t jieshou1zongchangdu = 30,jieshou1[30];
 volatile uint32_t jieshou1changdu,jieshou1tim=0;
 void duqu(int64_t d)
 {
	 jieshou1[5] = (d>>56) & 0xff;
	 jieshou1[6] = (d>>48) & 0xff;
	 jieshou1[7] = (d>>40) & 0xff;
	 jieshou1[8] = (d>>32) & 0xff;
	 jieshou1[9] = (d>>24) & 0xff;
	 jieshou1[10] = (d>>16) & 0xff;
	 jieshou1[11] = (d>>8) & 0xff;
	 jieshou1[12] = (d) & 0xff;
 }
 void shujuduqu(void)
 {
	 if(jieshou1[3] == 0)//寄存器部分
	 {
		 jieshou1[5] = (jicunqi[ jieshou1[4]]>>56) & 0xff;
		 jieshou1[6] = (jicunqi[jieshou1[4]]>>48) & 0xff;
		 jieshou1[7] = (jicunqi[jieshou1[4]]>>40) & 0xff;
		 jieshou1[8] = (jicunqi[jieshou1[4]]>>32) & 0xff;
		 jieshou1[9] = (jicunqi[jieshou1[4]]>>24) & 0xff;
		 jieshou1[10] = (jicunqi[jieshou1[4]]>>16) & 0xff;
		 jieshou1[11] = (jicunqi[jieshou1[4]]>>8) & 0xff;
		 jieshou1[12] = (jicunqi[jieshou1[4]]) & 0xff;
	 }
	 if(jieshou1[3] == 1)//只读部分
	 {
		 if(jieshou1[4] == 0)//当前位置
		 {
			 duqu(dianji[0].dangqianweizhi);
		 }
		 if(jieshou1[4] == 1)//当前速度
		 {
			 duqu(dianji[0].dangqiansudu);
		 }
		 if(jieshou1[4] == 2)//当前刹车停止位置
		 {
			 duqu(dianji[0].tingzhiweizhi);
		 }
		 if(jieshou1[4] == 3)//定时CNT
		 {
			 duqu(dianji[0].dingshi);
		 }
		 if(jieshou1[4] == 4)//速度控制
		 {
			 duqu(dianji[0].sudukongzhi);
		 }
		 if(jieshou1[4] == 5)//返回外部中断记录的位置
		 {
			 duqu(dianji[0].jiluweizhi);
		 }

	 }


	 HAL_UART_Transmit(duankou, jieshou1 ,15,100);

	 duankou1qingchu();//清除数据
 }
 void shujuxieru(void)
 {
	 if(jieshou1[3] == 0)
	 {
		 jicunqi[jieshou1[4]] = ((int64_t)jieshou1[5]<<56) + ((int64_t)jieshou1[6]<<48) + ((int64_t)jieshou1[7]<<40) +
				 ((int64_t)jieshou1[8]<<32) + ((int64_t)jieshou1[9]<<24) + ((int64_t)jieshou1[10]<<16) + ((int64_t)jieshou1[11]<<8) + jieshou1[12];

	 }
	 HAL_UART_Transmit(duankou, jieshou1 ,15,100);
	 duankou1qingchu();//清除数据
 }

uint32_t duankoupanduan(void)
{
	uint32_t inok = 0;
	uint16_t c;
	uint8_t cl,ch;
	if(jieshou1changdu >= 15)//目前数据包长度为统一15字节-20230331
	{
		 if(jieshou1[0] == 0xcc)//这个时数据包头字节，固定为0xcc
		 {
			 if(jieshou1[1] == SHEBEIDIZHI)//如果对应的设备地址为该设备-20230331
			 {
				 c = modbus_crc16((uint8_t*)&jieshou1[0],13);
				 cl = c&0xff;
				 ch = (c/256)&0xff;
				 if(cl == jieshou1[13] && ch == jieshou1[14])//CRC
				 {
						 if(jieshou1[2] == 3)//如果操作码为读取
						 {
							 shujuduqu();
						 }else if(jieshou1[2] == 6)//如果操作码为写操作
						 {
							 shujuxieru();
						 }else{inok = 3;}//如果操作码错误
				 }else{inok = 4;}//如果crc校验出错
			 }else{inok = 2;}//如果设备号错误
		 }else{inok = 1;}//如果头字节错误

		 duankou1qingchu();//清除数据
	}

	return inok;
}
void duankou1qingchu(void)//端口3清除数据
{
	uint32_t i;
	for(i = 0; i < jieshou1zongchangdu;i ++)
		{
			jieshou1[ i ] = 0;
		}
		jieshou1tim = 0;
		jieshou1changdu = 0;
}

void duankou1xunhuan(void)//端口3循环处理事件
{
	jieshou1tim++;
	if(jieshou1changdu == 0)//如果没有接收到数据，就将错误溢出设置成0
	{
		jieshou1tim = 0;
	}
	if(jieshou1tim > 50)
	{
		duankou1qingchu();
	}

}

/**
* @brief This function handles USART3 global interrupt.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
	uint8_t d;

	if(jieshou1zongchangdu > jieshou1changdu)
		{
			jieshou1[ jieshou1changdu ] = USART1->DR;
			jieshou1tim = 0;
			jieshou1changdu++;

		}
	d = USART1->DR;
	d=d;
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}







uint8_t chuankoufasongzi(uint8_t x)
{
	uint8_t zi[2];

	zi[0] = x;

	HAL_UART_Transmit(duankou, zi, 1, 10);
	return 0;
}


uint8_t shuzidaowenbenwenben[20], shuzidaowenbenlinshi[20];
uint8_t* chuankoufasongshuzi(int32_t shuzi)
{

	int8_t i = 0,u = 0;
	if(shuzi < 0)//先判断是否需要加负号
	{
		shuzi = -shuzi;
		shuzidaowenbenwenben[ i ] = '-';
		i++;
	}

	do
	{
			shuzidaowenbenlinshi[ u ] = (shuzi%10);
			shuzi /= 10;
			u++;
	} while(shuzi > 0);
	u--;
	for(;u>=0;u--)
	{
		shuzidaowenbenwenben[ i ] = shuzidaowenbenlinshi[ u ] + 48;
		i++;
	}


	HAL_UART_Transmit(duankou, shuzidaowenbenwenben ,i,100);

	return shuzidaowenbenwenben;
}

void chuankoufasong(uint8_t tou,uint32_t zi)
{
	  chuankoufasongzi(tou);
	  chuankoufasongshuzi(zi);
	  chuankoufasongzi(200);
}



uint16_t modbus_crc16(uint8_t *data, size_t length)
{
    const uint16_t crc_poly = 0xA001; // Modbus CRC polynomial
    uint16_t crc = 0xFFFF; // Initial value for CRC register

    for (size_t i = 0; i < length; ++i) {
        crc ^= (uint16_t)data[i];
        for (int j = 0; j < 8; ++j) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= crc_poly;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}


