#include "main.h"

/* Private variables ---------------------------------------------------------*/
// Menggunakan keyword `static` membatasi scope hanya di file ini (CERT: DCL00-C)
static ADC_HandleTypeDef hadc1;
static TIM_HandleTypeDef htim2;
static UART_HandleTypeDef huart2;
static DMA_HandleTypeDef hdma_usart2_rx;

/* Function Prototypes -------------------------------------------------------*/
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init(); // Inisialisasi hardware abstraction layer (CERT: DCL00-C - harus dipanggil sebelum HAL function lain)
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();

  // Cek return value dan validasi kesalahan (CERT: ERR33-C)
  if (HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler(); // CERT: MSC00-C (fail-safe handler digunakan)
  }

  for (;;)
  {
    // Inisialisasi eksplisit variabel (CERT: DCL04-C)
    uint32_t adc_value = 0U;
    uint32_t pulse = 0U;
    const uint32_t adc_threshold = 500U; // Ambang batas tekanan minimal agar servo bergerak

    // Selalu validasi hasil start ADC (CERT: ERR33-C)
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
      Error_Handler();
    }

    // Validasi keberhasilan konversi sebelum mengambil nilai (CERT: ERR33-C)
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
    {
      adc_value = HAL_ADC_GetValue(&hadc1);

      if (adc_value >= adc_threshold)
      {
        // Tekanan mencukupi, hitung nilai PWM secara aman (CERT: INT32-C)
        pulse = 1000U + ((adc_value * 1000U) / 4095U);
      }
      else
      {
        // Tekanan belum cukup, set ke posisi netral agar servo tidak bergerak
        pulse = 1500U;
      }

      __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
    }

    // Pastikan ADC dihentikan secara eksplisit (CERT: MSC00-C)
    if (HAL_ADC_Stop(&hadc1) != HAL_OK)
    {
      Error_Handler();
    }

    HAL_Delay(50U); // Delay eksplisit dengan konstan unsigned (CERT: CON37-C)
  }
}

/**
  * @brief System Clock Configuration
  */
static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0}; // Inisialisasi struct default (CERT: DCL00-C)
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  // Nilai PLL dan oscillator eksplisit sesuai konfigurasi (CERT: MSC30-C)
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(); // Validasi error konfigurasi (CERT: ERR33-C)
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;

  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_TIM2_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83U;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19999U;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 1500U;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200U;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_DMA_Init(void)
{
  __HAL_RCC_DMA1_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0U, 0U); // Nilai prioritas eksplisit
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Konfigurasi GPIO tambahan bisa ditambahkan di sini */
}

// Fungsi error handler dengan infinite loop sesuai prinsip fail-safe (CERT: MSC00-C)
void Error_Handler(void)
{
  __disable_irq(); // Mencegah interrupt untuk kondisi kritis
  for (;;) { }     // Loop aman tanpa keluar, digunakan untuk error fatal
}
