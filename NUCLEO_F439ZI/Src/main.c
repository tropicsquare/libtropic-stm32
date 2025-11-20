/**
 ******************************************************************************
 * @file    UART/UART_Printf/Src/main.c
 * @author  MCD Application Team
 * @brief   This example shows how to retarget the C library printf function
 *          to the UART.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2017 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include <string.h>
#include <inttypes.h>

#include "common.h"
#include "libtropic_examples.h"
#include "libtropic_functional_tests.h"
#include "libtropic_logging.h"
#include "libtropic_port_stm32_nucleo_f439zi.h"
#include "syscalls.h"

#if LT_USE_TREZOR_CRYPTO
#include "libtropic_trezor_crypto.h"
#elif LT_USE_MBEDTLS_V4
#include "libtropic_mbedtls_v4.h"
#include "psa/crypto.h"
#endif

/** @addtogroup STM32F4xx_HAL_Examples
 * @{
 */

/** @addtogroup UART_Printf
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define LOG_OUT(f_, ...) printf(f_, ##__VA_ARGS__)
#define NUM_OF_PING_CMDS 1
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
static void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/* UART handle declaration */
static UART_HandleTypeDef UartHandle;

/* RNG handle declaration */
RNG_HandleTypeDef RNGHandle;

/**
 * @brief   Configures the UART peripheral
 *          Put the USART peripheral in the Asynchronous mode (UART Mode)
 *          UART configured as follows:
 *          - Word Length = 8 Bits (7 data bit + 1 parity bit) :
 *                        BE CAREFUL : Program 7 data bits + 1 parity bit in PC HyperTerminal
 *          - Stop Bit    = One Stop bit
 *          - Parity      = NONE parity
 *          - BaudRate    = 115200 baud
 *          - Hardware flow control disabled (RTS and CTS signals)
 *
 * @return HAL_StatusTypeDef
 */
static HAL_StatusTypeDef DBG_UART_Init(void)
{
    UartHandle.Instance = USART_DBG;
    UartHandle.Init.BaudRate = 115200;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits = UART_STOPBITS_1;
    UartHandle.Init.Parity = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&UartHandle) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  Retargets the C library printf function to the USART.
 * @param  None
 * @retval None
 */
PUTCHAR_PROTOTYPE
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART3 and Loop until the end of transmission */
    HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
    /* STM32F4xx HAL library initialization:
         - Configure the Flash prefetch
         - Systick timer is configured by default as source of time base, but user
           can eventually implement his proper time base source (a general purpose
           timer for example or other time source), keeping in mind that Time base
           duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
           handled in milliseconds basis.
         - Set NVIC Group Priority to 4
         - Low Level Initialization
       */
    HAL_Init();
    /* Configure the system clock to 180 MHz */
    SystemClock_Config();
    /* Enable Random Number Generator clock */
    __HAL_RCC_RNG_CLK_ENABLE();
    /* Initialize BSP Led for LED2 */
    BSP_LED_Init(LED2);

    if (DBG_UART_Init() != HAL_OK) {
        Error_Handler();
    }

    // IMPORTANT: Initialize RNG peripheral.
    // Do not forget to do this in your application, as the 
    // Libtropic HAL uses RNG for entropy source!
    RNGHandle.Instance = RNG;
    if (HAL_RNG_Init(&RNGHandle) != HAL_OK) {
        Error_Handler();
    }

    // libtropic related code BEGIN
    // libtropic related code BEGIN
    // libtropic related code BEGIN
    // libtropic related code BEGIN
    // libtropic related code BEGIN

    ////////////////////////////////////////////////////////////////
    // CRYPTOGRAPHIC FUNCTION PROVIDER INITIALIZATION             //
    //                                                            //
    // In production, this would typically be done only once,     //
    // usually at the start of the application or before          //
    // the first use of cryptographic functions but no later than //
    // the first occurrence of any Libtropic function             //
    ////////////////////////////////////////////////////////////////
#if LT_USE_MBEDTLS_V4
    psa_status_t status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        LT_LOG_ERROR("PSA Crypto initialization failed, status=%" PRId32 " (psa_status_t)", status);
        Error_Handler();
    }
#endif

    ////////////////////////////////////////////////////////////////
    // HANDLE INITIALIZATION                                      //
    //                                                            //
    // Libtropic handle is declared here (on stack) for           //
    // simplicity. In production, you put it on heap if needed.   //
    ////////////////////////////////////////////////////////////////
    lt_handle_t __lt_handle__ = {0};
#if LT_SEPARATE_L3_BUFF
    uint8_t l3_buffer[L3_PACKET_MAX_SIZE] __attribute__((aligned(16))) = {0};
    __lt_handle__.l3.buff = l3_buffer;
    __lt_handle__.l3.buff_len = sizeof(l3_buffer);
#endif

    ////////////////////////////////////////////////////////////////
    // DEVICE MAPPINGS                                            //
    //                                                            //
    // Modify this according to your environment. Usually you     //
    // have to change at least the dev_path, as it is dynamically //
    // assigned by the OS (if you have multiple USB serial        //
    // devices).                                                  //
    ////////////////////////////////////////////////////////////////

    // The device structure has to be zero initialized!
    // STM32 HAL depends on zero init values.
    lt_dev_stm32_nucleo_f439zi_t device = {0};

    device.spi_instance = LT_SPI_INSTANCE;
    device.baudrate_prescaler = SPI_BAUDRATEPRESCALER_16;
    device.spi_cs_gpio_bank = LT_SPI_CS_BANK;
    device.spi_cs_gpio_pin = LT_SPI_CS_PIN;

    // IMPORTANT: Do not forget to initialize RNG peripheral
    // at the beggining  of your application using HAL_RNG_Init()!
    device.rng_handle = &RNGHandle;

#ifdef LT_USE_INT_PIN
    device.int_gpio_bank = LT_INT_BANK;
    device.int_gpio_pin = LT_INT_PIN;
#endif

    __lt_handle__.l2.device = &device;

    ////////////////////////////////////////////////////////////////
    // CRYPTO ABSTRACTION LAYER CONTEXT                           //
    //                                                            //
    // Context for the selected CAL implementation is chosen here //
    // based on the configuration macro. This is only for         //
    // convenient switching between different CALs for demo       //
    // purposes, in production applications you would typically   //
    // stick to a single CAL.                                     //
    ////////////////////////////////////////////////////////////////
#if LT_USE_TREZOR_CRYPTO
    lt_ctx_trezor_crypto_t
#elif LT_USE_MBEDTLS_V4
    lt_ctx_mbedtls_v4_t
#endif
        crypto_ctx;
    __lt_handle__.l3.crypto_ctx = &crypto_ctx;

    ////////////////////////////////////////////////////////////////
    // EXAMPLE OR TEST CODE                                       //
    //                                                            //
    // Depending on the build configuration, either examples or   //
    // tests are executed here.                                   //
    ////////////////////////////////////////////////////////////////
#ifdef LT_BUILD_TESTS
#include "lt_test_registry.c.inc"
#endif

// When examples are being built, special variable containing example return value is defined.
#ifdef LT_BUILD_EXAMPLES
#include "lt_ex_registry.c.inc"
    UNUSED(__lt_ex_return_val__);
#endif

    LT_FINISH_TEST();

    ////////////////////////////////////////////////////////////////
    // CRYPTOGRAPHIC FUNCTION PROVIDER DEINITIALIZATION           //
    //                                                            //
    // In production, this would be done only once, typically     //
    // during termination of the application.                     //
    ////////////////////////////////////////////////////////////////
#if LT_USE_MBEDTLS_V4
    mbedtls_psa_crypto_free();
#endif

    // libtropic related code END
    // libtropic related code END
    // libtropic related code END
    // libtropic related code END
    // libtropic related code END

    // Not strictly necessary, but we deinitialize RNG here to
    // demonstrate proper usage.
    if (HAL_RNG_DeInit(&RNGHandle) != HAL_OK) {
        Error_Handler();
    }

    while (1) {
        BSP_LED_On(LED2);
        HAL_Delay(100);
        BSP_LED_Off(LED2);
        HAL_Delay(500);
    }
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 180000000
 *            HCLK(Hz)                       = 180000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 360
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
       clocked below the maximum system frequency, to update the voltage scaling value
       regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 360;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        /* Initialization Error */
        Error_Handler();
    }

    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        /* Initialization Error */
        Error_Handler();
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    RCC_ClkInitStruct.ClockType
        = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        /* Initialization Error */
        Error_Handler();
    }
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
    /* Turn LED2 on */
    BSP_LED_On(LED2);
    while (1) {
    }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1) {
    }
}
#endif

/**
 * @}
 */

/**
 * @}
 */
