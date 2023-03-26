#include "main.h"
#include "stm32f4xx_hal.h"
#include "achuankou.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_uart.h"

#define duankou1 USART1
#define duankou &huart1

 extern UART_HandleTypeDef huart1;
 volatile uint8_t jieshou1zongchangdu = 30,jieshou1[30];
 volatile uint32_t jieshou1changdu,jieshou1tim=0;


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
	if(jieshou1changdu == 0)//如果没有接收到数据，就将错误溢出设置成0
	{
		jieshou1tim = 0;
	}
	if(jieshou1tim > 200)
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



