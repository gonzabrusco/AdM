/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "asm_func.h"
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

ETH_TxPacketConfig TxConfig;
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

ETH_HandleTypeDef heth;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void PrivilegiosSVC (void)
{
    // Obtiene valor del registro de 32 bits del procesador llamado "control".
    // El registro guarda los siguientes estados:
    // bit 2: Uso de FPU en el contexto actual. Usado=1, no usado=0.
    // bit 1: Mapeo del stack pointer(sp). MSP=0, PSP=1.
    // bit 0: Modo de ejecucion en Thread. Privilegiado=0, No privilegiado=1.
    //        Recordar que este valor solo se usa en modo Thread. Las
    //        interrupciones siempre se ejecutan en modo Handler con total
    //        privilegio.
    uint32_t x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x |= 1;
    // bit 0 a modo No privilegiado.
    __set_CONTROL (x);

    // En este punto se estaria ejecutando en modo No privilegiado.
    // Lectura del registro "control" para confirmar.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    x &= ~1u;
    // Se intenta volver a modo Privilegiado (bit 0, valor 0).
    __set_CONTROL (x);

    // Confirma que esta operacion es ignorada por estar ejecutandose en modo
    // Thread no privilegiado.
    x = __get_CONTROL ();

    // Actividad de debug: Ver registro "control" y valor de variable "x".
    //__BKPT (0);

    // En este punto, ejecutando en modo Thread no privilegiado, la unica forma
    // de volver a modo privilegiado o de realizar cualquier cambio que requiera
    // modo privilegiado, es pidiendo ese servicio a un hipotetico sistema
    // opertivo de tiempo real.
    // Para esto se invoca por software a la interrupcion SVC (Supervisor Call)
    // utilizando la instruccion "svc".
    // No hay intrinsics para realizar esta tarea. Para utilizar la instruccion
    // es necesario implementar una funcion en assembler. Ver el archivo
    // asm_func.S.
    asm_svc ();

    // El sistema operativo (el handler de SVC) deberia haber devuelto el modo
    // de ejecucion de Thread a privilegiado (bit 0 en valor 0).
    x = __get_CONTROL ();

    // Fin del ejemplo de SVC
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  volatile uint32_t Ciclos;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  // Inicializo contador de ciclos
  DWT->CTRL |= 1 << DWT_CTRL_CYCCNTENA_Pos;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  PrivilegiosSVC ();

  const uint32_t Resultado = asm_sum (5, 3);

  uint32_t array32[11] = {1,2,3,4,5,6,7,8,9,10,11};
  uint32_t array32_2[11] = {1,2,3,4,5,6,7,8,9,10,11};

  asm_zeros(array32, 11);
  asm_productoEscalar32(array32_2, array32, 11, 100);

  uint16_t array16[11] = {0};
  uint16_t array16_2[11] = {1,2,3,4,5,6,7,8,9,10,11};
  asm_productoEscalar16(array16_2, array16, 11, 1000);

  uint16_t array12[11] = {0};
  uint16_t array12_2[11] = {1,2,3,4,5,6,7,8,9,10,11};
  asm_productoEscalar12(array12_2, array12, 11, 1000);

  uint16_t arrayVentanaOut[15] = {0};
  uint16_t arrayVentanaIn[15] = {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
  asm_filtroVentana10(arrayVentanaIn, arrayVentanaOut, sizeof(arrayVentanaIn)/sizeof(arrayVentanaIn[0]));

  int16_t arrayPack32to16Out[12] = {0};
  int32_t arrayPack32to16In[12] = {-65541,-65540,-65539,-65538,-65537,-65536,65534,65535,65536,65537,65538,65539};
  asm_pack32to16(arrayPack32to16In, arrayPack32to16Out, sizeof(arrayPack32to16In)/sizeof(arrayPack32to16In[0]));

  int32_t arrayMaxIn[12] = {-10,-2,10,-5,11,2,128,-1256,129,0,2,3};
  uint32_t maxPos = asm_max(arrayMaxIn, sizeof(arrayMaxIn)/sizeof(arrayMaxIn[0]));

  int32_t arraydownsampleMOut[12] = {0};
  int32_t arraydownsampleMIn[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
  asm_downsampleM(arraydownsampleMIn, arraydownsampleMOut, 12, 2);
  memset(arraydownsampleMOut, 0, sizeof(arraydownsampleMOut));
  asm_downsampleM(arraydownsampleMIn, arraydownsampleMOut, 12, 3);
  memset(arraydownsampleMOut, 0, sizeof(arraydownsampleMOut));
  asm_downsampleM(arraydownsampleMIn, arraydownsampleMOut, 12, 4);
  memset(arraydownsampleMOut, 0, sizeof(arraydownsampleMOut));
  asm_downsampleM(arraydownsampleMIn, arraydownsampleMOut, 12, 5);
  memset(arraydownsampleMOut, 0, sizeof(arraydownsampleMOut));
  asm_downsampleM(arraydownsampleMIn, arraydownsampleMOut, 12, 6);
  memset(arraydownsampleMOut, 0, sizeof(arraydownsampleMOut));
  asm_downsampleM(arraydownsampleMIn, arraydownsampleMOut, 12, 7);
  memset(arraydownsampleMOut, 0, sizeof(arraydownsampleMOut));
  asm_downsampleM(arraydownsampleMIn, arraydownsampleMOut, 12, 13);

  uint16_t arrayinvertir1[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
  uint16_t arrayinvertir2[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
  asm_invertir(arrayinvertir1, 12);
  asm_invertir(arrayinvertir2, 13);

  int16_t audioSample[4096];
  // Inicializo el vector de audio
  for(int16_t i = 0; i < 4096; i++) {
	  audioSample[i] = i;
  }

  DWT->CYCCNT = 0;
  asm_simd_eco(audioSample, 4096);
  Ciclos = DWT->CYCCNT;

  // Inicializo el vector de audio
  for(int16_t i = 0; i < 4096; i++) {
    audioSample[i] = i;
  }

  DWT->CYCCNT = 0;
  asm_eco(audioSample, 4096);
  Ciclos = DWT->CYCCNT;

  // Inicializo el vector de audio
  for(int16_t i = 0; i < 4096; i++) {
    audioSample[i] = i;
  }

  DWT->CYCCNT = 0;
  eco(audioSample, 4096);
  Ciclos = DWT->CYCCNT;

  // Inicializo el vector de audio
  for(int16_t i = 0; i < 4096; i++) {
    audioSample[i] = i;
  }

  DWT->CYCCNT = 0;
  eco_intrinsic_simd(audioSample, 4096);
  Ciclos = DWT->CYCCNT;

  // Resultados de medicion funcion eco - DEBUG sin optimizacion
  // SIMD 21013 CICLOS
  // ASM 41808 CICLOS
  // C 173676 CICLOS
  // C Intrinsic SIMD 109341 CICLOS

  // Optimizado O2 Release
  // SIMD 20922
  // ASM 41800
  // C 35379
  // C Instrinsics 24136

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief USB_OTG_FS Initialization Function
  * @param None
  * @retval None
  */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  /* USER CODE BEGIN USB_OTG_FS_Init 0 */

  /* USER CODE END USB_OTG_FS_Init 0 */

  /* USER CODE BEGIN USB_OTG_FS_Init 1 */

  /* USER CODE END USB_OTG_FS_Init 1 */
  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 4;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = ENABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USB_OTG_FS_Init 2 */

  /* USER CODE END USB_OTG_FS_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD1_Pin|LD3_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USB_PowerSwitchOn_GPIO_Port, USB_PowerSwitchOn_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_Btn_Pin */
  GPIO_InitStruct.Pin = USER_Btn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_Btn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD1_Pin LD3_Pin LD2_Pin */
  GPIO_InitStruct.Pin = LD1_Pin|LD3_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = USB_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USB_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_OverCurrent_Pin */
  GPIO_InitStruct.Pin = USB_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_OverCurrent_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* Ejercicio 1: Realizar una función que inicialice un vector con ceros. La función debe tener el siguiente
prototipo. */
void zeros(uint32_t * vector, uint32_t longitud) {
	for(; longitud > 0; longitud--) {
		vector[longitud] = 0;
	}
}

/* Ejercicio 2: Realizar una función que realice el producto de un vector y un escalar (por ejemplo, podría servir
para cambiar el nivel de amplitud de una señal). */
void productoEscalar32(uint32_t * vectorIn, uint32_t * vectorOut, uint32_t longitud, uint32_t escalar) {
	for(; longitud > 0; longitud--) {
		vectorOut[longitud-1] = vectorIn[longitud-1] * escalar;
	}
}
/* Ejercicio 3: Adapte la función del ejercicio 2 para realizar operaciones sobre vectores de 16 bits. */
void productoEscalar16(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar) {
	for(; longitud > 0; longitud--) {
		vectorOut[longitud-1] = vectorIn[longitud-1] * escalar;
	}
}
/* Ejercicio 4: Adapte la función del ejercicio 3 para saturar el resultado del producto a 12 bits. */
void productoEscalar12(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitud, uint16_t escalar) {
	for(; longitud > 0; longitud--) {
		vectorOut[longitud-1] = vectorIn[longitud-1] * escalar;
		if(vectorOut[longitud-1] > 0x0FFF) {
			vectorOut[longitud-1] = 0x0FFF;
		}
	}
}
/* Ejercicio 5: Realice una función que implemente un filtro de ventana móvil de 10 valores sobre un vector de muestras. */
void filtroVentana10(uint16_t * vectorIn, uint16_t * vectorOut, uint32_t longitudVectorIn) {
	uint32_t vectorIterator;
	uint32_t windowsBeginPos;
	uint32_t windowsEndPos;
	uint16_t average;

	vectorIterator = longitudVectorIn;
	while(vectorIterator-- > 0) {
		average = 0;
		windowsEndPos = vectorIterator+6 < longitudVectorIn ? vectorIterator+6 : longitudVectorIn;
		windowsBeginPos = vectorIterator > 4 ? vectorIterator-5 : 0;
		while(windowsEndPos-- > windowsBeginPos) {
			average += vectorIn[windowsEndPos];
		}
		vectorOut[vectorIterator] = average / 11;
	}
}
/* Ejercicio 6: Realizar una función que reciba un vector de números signados de 32 bits y los “empaquete” en otro vector de 16 bits. La función deberá adecuar los valores de entrada a la nueva precisión. */
void pack32to16(int32_t * vectorIn, int16_t *vectorOut, uint32_t longitud) {
	while(longitud-- > 0) {
		vectorOut[longitud] = vectorIn[longitud] >> 16;
	}
}
/* Ejercicio 7: Realizar una función que reciba un vector de números signados de 32 bits y devuelva la posición del máximo del vector. */
int32_t max(int32_t * vectorIn, uint32_t longitud) {
	int32_t max, pos;
	longitud--;
	max = vectorIn[longitud];
	pos = longitud;
	while(longitud-- > 0) {
		if(vectorIn[longitud] > max) {
			max = vectorIn[longitud];
			pos = longitud;
		}
	}
	return pos;
}

/* Ejercicio 8: Realizar una función que reciba un vector de muestras signadas de 32 bits y lo decime descartando una cada N muestras. */
void downsampleM(int32_t * vectorIn, int32_t * vectorOut, uint32_t longitud, uint32_t N) {
	uint32_t i = 0,j = 0;

	while(i < longitud) {
		if(i%N == N-1) { // En la funcion de assembly se utilizo otra estrategia para no usar el operador modulo.
			vectorOut[j++] = vectorIn[i];
		}
		i++;
	}
}

/* Ejercicio 9: Realizar una función que reciba un vector de muestras no signadas de 16 bits e invierta su orden. */
void invertir(uint16_t * vector, uint32_t longitud) {
	uint16_t swap;
	uint32_t endIndex = longitud / 2;
	uint32_t i = 0;

	while(longitud-- > endIndex) {
		swap = vector[longitud];
		vector[longitud] = vector[i];
		vector[i++] = swap;
	}
}

/* Ejercicio 10: Realizar una función que recibe un vector de 4096 valores de 16 bits (signados),
 * que corresponden a muestras de audio tomadas a una tasa de muestreo de 44.100 muestras/s.
 * La función debe introducir un “eco” de la mitad de la amplitud de la muestra original a los 20ms de comenzada la grabación.
 * Nota: El eco consiste en adicionar a la señal original, la propia señal original dividida por dos y atrasada en 20ms.
*/
#define ECO_DELAY 882
void eco(int16_t * vector, uint32_t longitud) {
	while(longitud-- > 882) { // 882 = 20ms a 44100 muestras/s
		vector[longitud] += vector[longitud-ECO_DELAY]/2;
	}
}

#define ECO_DELAY_HALF 441
void eco_intrinsic_simd(int16_t * vector, uint32_t longitud) {
	longitud = longitud/2;
	uint32_t eco;
	uint32_t * vector_32 = vector;
	while(longitud-- > ECO_DELAY_HALF) { // 441 = 20ms a 44100 muestras/s (tomando de a dos muestras)
		eco = __SHADD16(vector_32[longitud-ECO_DELAY_HALF], 0);
		vector_32[longitud] = __QADD16(vector_32[longitud], eco);
	}
}

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
