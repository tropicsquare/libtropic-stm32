/**
 ******************************************************************************
 * @file    UART/UART_Printf/Inc/main.h
 * @author  MCD Application Team
 * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rng.h"
#include "stm32f4xx_nucleo_144.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USART_DBG/UARTx instance used and associated
   resources */

/** This define controls over which pins will board drive UART.
 * if == 0, debug UART can be read when board is plugged over USB (together with stlink)
 * if == 1, debug uart can be read over pins PB_9 and PB_10 pins used on automatized testing rig
 */
#ifndef LT_TESTING_RIG
#define LT_TESTING_RIG 0
#endif

/* Definition for USART_DBG */
#define USART_DBG USART3
#define USART_DBG_CLK_ENABLE() __HAL_RCC_USART3_CLK_ENABLE();
#define USART_DBG_TX_AF GPIO_AF7_USART3
#define USART_DBG_RX_AF GPIO_AF7_USART3
#define USART_DBG_FORCE_RESET() __HAL_RCC_USART3_FORCE_RESET()
#define USART_DBG_RELEASE_RESET() __HAL_RCC_USART3_RELEASE_RESET()

#if LT_TESTING_RIG
// Used by libtropic automating testing rig, USART is wired out on PB_9 and PB_10 pins
#define USART_DBG_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART_DBG_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define USART_DBG_TX_PIN GPIO_PIN_10
#define USART_DBG_TX_GPIO_PORT GPIOB
#define USART_DBG_RX_PIN GPIO_PIN_11
#define USART_DBG_RX_GPIO_PORT GPIOB
#else
// Standard Nucleo's way, debug USART is wired out over st-link
#define USART_DBG_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART_DBG_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()
#define USART_DBG_TX_PIN GPIO_PIN_8
#define USART_DBG_TX_GPIO_PORT GPIOD
#define USART_DBG_RX_PIN GPIO_PIN_9
#define USART_DBG_RX_GPIO_PORT GPIOD
#endif

/* Definition for GPIO chip select clock resources */
#define LT_SPI_CS_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

/* Definition for GPIO chip select pins */
#define LT_SPI_CS_BANK GPIOD
#define LT_SPI_CS_PIN GPIO_PIN_14
#define LT_SPI_INSTANCE SPI1

/* Definition for SPIx */
#define SPIx SPI1
#define SPIx_CLK_ENABLE() __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_NSS_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_FORCE_RESET() __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET() __HAL_RCC_SPI1_RELEASE_RESET()
#define SPIx_SCK_PIN GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT GPIOA
#define SPIx_SCK_AF GPIO_AF5_SPI1
#define SPIx_MISO_PIN GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT GPIOA
#define SPIx_MISO_AF GPIO_AF5_SPI1
#define SPIx_MOSI_PIN GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT GPIOA
#define SPIx_MOSI_AF GPIO_AF5_SPI1

/* Size of buffer */
#define BUFFERSIZE (COUNTOF(aTxBuffer) - 1)

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__) (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#if LT_USE_INT_PIN
/* Definition for GPIO interrupt pin clock resources
 * Following GPIO is used to check on INT pin for READY signal during communication.
 */
#define LT_INT_BANK GPIOF
#define LT_INT_PIN GPIO_PIN_15
#define LT_INT_GPIO_PORT GPIOF
/* Definition for GPIO interrupt pin clock resources */
#define LT_INT_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Exported variables ------------------------------------------------------- */
extern RNG_HandleTypeDef RNGHandle;

#endif /* __MAIN_H */
