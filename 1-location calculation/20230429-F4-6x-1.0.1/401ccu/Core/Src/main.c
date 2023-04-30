/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "achuankou.h"
#include "abujindianjipwm.h"
#include "ashuzuchaozuo.h"

uint8_t zi[100],zilen;
__IO uint64_t jici,xitongshijian = 0;
extern uint8_t jieshou1zongchangdu,jieshou1[30],uartdata[10];
extern volatile uint32_t jieshou1changdu,jieshou1tim;
extern bujinpwm dianji[4];
int64_t zhoushijiweizhi[10],zhouyudingweizhi[10],zhouqishiweizhi[10];

//uint8_t uartchar[10],uartlen;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim11;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM11_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */
	xitongshijian++;
	shishisuduyunsuan();
	//duankoupanduan();
	duankou1xunhuan();
  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}
void TIM1_BRK_TIM9_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 0 */

	pwmzhongduankongzhi();
  /* USER CODE END TIM1_BRK_TIM9_IRQn 0 */
  HAL_TIM_IRQHandler(&htim9);
  /* USER CODE BEGIN TIM1_BRK_TIM9_IRQn 1 */

  /* USER CODE END TIM1_BRK_TIM9_IRQn 1 */
}
//--------------------------------------------------------
int64_t jisuanjueduijuli_2x(int64_t a,int64_t b)
{
	return sqrt(a*a + b*b);
}
int64_t jisuanjueduijuli_3x(int64_t a,int64_t b,int64_t c)
{
	return sqrt(a*a + b*b + c*c);
}
int64_t jisuanjueduijuli_6x(int64_t a,int64_t b,int64_t c,int64_t d,int64_t e,int64_t f)
{
	return sqrt(a*a + b*b + c*c + d*d +e*e + f*f);
}
//输出两轴绝对位置
void chabu_2x(int64_t a,int64_t b)
{
	  HAL_TIM_PWM_Stop(&htim9,TIM_CHANNEL_1);
	  dianji[0].dangqianweizhi = 0;
	  zhouqishiweizhi[0] = zhoushijiweizhi[0];
	  zhouqishiweizhi[1] = zhoushijiweizhi[1];
	  zhouyudingweizhi[0] = a;
	  zhouyudingweizhi[1] = b;

	  //dianji[0].zuidasudu = s2b2(500,32000 ,64000);//设置�??????大�?�度
	  chuankoufasong(219,dianji[0].zuidasudu);

	  dianji[0].yudingweizhi = jisuanjueduijuli_2x(zhouqishiweizhi[0] - zhouyudingweizhi[0],zhouqishiweizhi[1] - zhouyudingweizhi[1]);
	  HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_1);
}
void chabu_3x(int64_t a,int64_t b,int64_t c)
{
	  HAL_TIM_PWM_Stop(&htim9,TIM_CHANNEL_1);
	  dianji[0].dangqianweizhi = 0;
	  zhouqishiweizhi[0] = zhoushijiweizhi[0];
	  zhouqishiweizhi[1] = zhoushijiweizhi[1];
	  zhouqishiweizhi[2] = zhoushijiweizhi[2];
	  zhouyudingweizhi[0] = a;
	  zhouyudingweizhi[1] = b;
	  zhouyudingweizhi[2] = c;

	  //dianji[0].zuidasudu = s2b2(500,32000 ,64000);//设置�??????大�?�度
	  chuankoufasong(219,dianji[0].zuidasudu);

	  dianji[0].yudingweizhi = jisuanjueduijuli_3x(zhouqishiweizhi[0] - zhouyudingweizhi[0],zhouqishiweizhi[1] - zhouyudingweizhi[1],zhouqishiweizhi[2] - zhouyudingweizhi[2]);
	  HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_1);
}
void chabu_6x(int64_t a,int64_t b,int64_t c,int64_t d,int64_t e,int64_t f)
{
	  HAL_TIM_PWM_Stop(&htim9,TIM_CHANNEL_1);
	  dianji[0].dangqianweizhi = 0;
	  zhouqishiweizhi[0] = zhoushijiweizhi[0];
	  zhouqishiweizhi[1] = zhoushijiweizhi[1];
	  zhouqishiweizhi[2] = zhoushijiweizhi[2];
	  zhouqishiweizhi[3] = zhoushijiweizhi[3];
	  zhouqishiweizhi[4] = zhoushijiweizhi[4];
	  zhouqishiweizhi[5] = zhoushijiweizhi[5];
	  zhouyudingweizhi[0] = a;
	  zhouyudingweizhi[1] = b;
	  zhouyudingweizhi[2] = c;
	  zhouyudingweizhi[3] = d;
	  zhouyudingweizhi[4] = e;
	  zhouyudingweizhi[5] = f;

	  //dianji[0].zuidasudu = s2b2(500,32000 ,64000);//设置�??????大�?�度
	  chuankoufasong(219,dianji[0].zuidasudu);

	  dianji[0].yudingweizhi = jisuanjueduijuli_6x(zhouqishiweizhi[0] - zhouyudingweizhi[0],zhouqishiweizhi[1] - zhouyudingweizhi[1],zhouqishiweizhi[2] - zhouyudingweizhi[2],zhouqishiweizhi[3] - zhouyudingweizhi[3],zhouqishiweizhi[4] - zhouyudingweizhi[4],zhouqishiweizhi[5] - zhouyudingweizhi[5]);
	  HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_1);
}
void gengxinshijiweizhi_2x(void)
{
	zhoushijiweizhi[0] = zhouqishiweizhi[0]+(zhouyudingweizhi[0] - zhouqishiweizhi[0])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[1] = zhouqishiweizhi[1]+(zhouyudingweizhi[1] - zhouqishiweizhi[1])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
//	zhoushijiweizhi[2] = zhouqishiweizhi[2]+(zhouyudingweizhi[2] - zhouqishiweizhi[2])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
}
void gengxinshijiweizhi_3x(void)
{
	zhoushijiweizhi[0] = zhouqishiweizhi[0]+(zhouyudingweizhi[0] - zhouqishiweizhi[0])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[1] = zhouqishiweizhi[1]+(zhouyudingweizhi[1] - zhouqishiweizhi[1])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[2] = zhouqishiweizhi[2]+(zhouyudingweizhi[2] - zhouqishiweizhi[2])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
}
void gengxinshijiweizhi_6x(void)
{
	zhoushijiweizhi[0] = zhouqishiweizhi[0]+(zhouyudingweizhi[0] - zhouqishiweizhi[0])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[1] = zhouqishiweizhi[1]+(zhouyudingweizhi[1] - zhouqishiweizhi[1])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[2] = zhouqishiweizhi[2]+(zhouyudingweizhi[2] - zhouqishiweizhi[2])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[3] = zhouqishiweizhi[3]+(zhouyudingweizhi[3] - zhouqishiweizhi[3])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[4] = zhouqishiweizhi[4]+(zhouyudingweizhi[4] - zhouqishiweizhi[4])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
	zhoushijiweizhi[5] = zhouqishiweizhi[5]+(zhouyudingweizhi[5] - zhouqishiweizhi[5])*dianji[0].dangqianweizhi/dianji[0].yudingweizhi;
}
//--------------------------------------------------------
void fs(uint8_t tou,double num)
{
	zilen = ftoa(num , (char*)&zi[1] , 0 );
	  zi[0] =tou;
	  zi[zilen+1] = 200;
	  zilen++;
	  CDC_Transmit_FS(zi, zilen+1);
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM9_Init();
  MX_TIM11_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_UART_Receive_IT(&huart1, uartdata, 1);
  //__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_PWM_Start_IT(&htim9, TIM_CHANNEL_1);

  dianji[0].zuidasudu = s2b2(500,8000 ,16000);//设置�??????大�?�度
  zhoushijiweizhi[0] = 0;
  zhoushijiweizhi[1] = 0;
  zhoushijiweizhi[2] = 0;
  zhoushijiweizhi[3] = 0;
  zhoushijiweizhi[4] = 0;
  zhoushijiweizhi[5] = 0;
  chabu_6x(5000,-2000,30000,123000,-88653,9);
  HAL_Delay(1000);
  while (zhoushijiweizhi[1] != zhouyudingweizhi[1])
  {
	  jici++;
	  fs(201,(double)jici);HAL_Delay(10);
	  fs(202,(double)dianji[0].dangqianweizhi);HAL_Delay(10);
	  fs(203,(double)dianji[0].dangqiansudu);HAL_Delay(10);

	  gengxinshijiweizhi_6x();
	  	fs(204,(double)zhoushijiweizhi[0]);HAL_Delay(1);
		fs(205,(double)zhoushijiweizhi[1]);HAL_Delay(1);
		fs(206,(double)zhoushijiweizhi[2]); HAL_Delay(1);
	  	fs(207,(double)zhoushijiweizhi[3]);HAL_Delay(1);
		fs(208,(double)zhoushijiweizhi[4]);HAL_Delay(1);
		fs(209,(double)zhoushijiweizhi[5]); HAL_Delay(1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }

  chabu_6x(100,200,500,1000,50000,-5000);
  HAL_Delay(1000);
  while (1)
  {
	  jici++;
	  fs(201,(double)jici);HAL_Delay(1);
	  fs(202,(double)dianji[0].dangqianweizhi);HAL_Delay(1);
	  fs(203,(double)dianji[0].dangqiansudu);HAL_Delay(1);
	  gengxinshijiweizhi_6x();
	  	fs(204,(double)zhoushijiweizhi[0]);HAL_Delay(1);
		fs(205,(double)zhoushijiweizhi[1]);HAL_Delay(1);
		fs(206,(double)zhoushijiweizhi[2]); HAL_Delay(1);
	  	fs(207,(double)zhoushijiweizhi[3]);HAL_Delay(1);
		fs(208,(double)zhoushijiweizhi[4]);HAL_Delay(1);
		fs(209,(double)zhoushijiweizhi[5]); HAL_Delay(1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 384;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 63;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM9 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 65535;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM9_Init 2 */

  /* USER CODE END TIM9_Init 2 */
  HAL_TIM_MspPostInit(&htim9);

}

/**
  * @brief TIM11 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM11_Init(void)
{

  /* USER CODE BEGIN TIM11_Init 0 */

  /* USER CODE END TIM11_Init 0 */

  /* USER CODE BEGIN TIM11_Init 1 */

  /* USER CODE END TIM11_Init 1 */
  htim11.Instance = TIM11;
  htim11.Init.Prescaler = 63;
  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim11.Init.Period = 65535;
  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM11_Init 2 */

  /* USER CODE END TIM11_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(bu1f_GPIO_Port, bu1f_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : bu1f_Pin */
  GPIO_InitStruct.Pin = bu1f_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(bu1f_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
