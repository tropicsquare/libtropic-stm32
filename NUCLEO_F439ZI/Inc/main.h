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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "stdio.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USART_DBG/UARTx instance used and associated
   resources */

/* Definition for USART_DBG */
#define USART_DBG                           USART3
#define USART_DBG_CLK_ENABLE()              __HAL_RCC_USART3_CLK_ENABLE();
#define USART_DBG_TX_AF                     GPIO_AF7_USART3
#define USART_DBG_RX_AF                     GPIO_AF7_USART3
#define USART_DBG_FORCE_RESET()             __HAL_RCC_USART3_FORCE_RESET()
#define USART_DBG_RELEASE_RESET()           __HAL_RCC_USART3_RELEASE_RESET()

#if UART_TESTING_RIG
  /* Used by libtropic automating testing rig, USART is wired out on PB_9 and PB_10 pins */
  #define USART_DBG_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
  #define USART_DBG_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
  #define USART_DBG_TX_PIN                    GPIO_PIN_10
  #define USART_DBG_TX_GPIO_PORT              GPIOB
  #define USART_DBG_RX_PIN                    GPIO_PIN_11
  #define USART_DBG_RX_GPIO_PORT              GPIOB
#else
  /* Standard Nucleo's way, debug USART is wired out over st-link */
  #define USART_DBG_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
  #define USART_DBG_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
  #define USART_DBG_TX_PIN                    GPIO_PIN_8
  #define USART_DBG_TX_GPIO_PORT              GPIOD
  #define USART_DBG_RX_PIN                    GPIO_PIN_9
  #define USART_DBG_RX_GPIO_PORT              GPIOD
#endif

/* Definition for SPIx */
#define LT_SPI_CS_BANK   GPIOD
#define LT_SPI_CS_PIN    GPIO_PIN_14
#define LT_SPI_INSTANCE  SPI1
#define LT_SPI_CS_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_NSS_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()
#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_SCK_AF                      GPIO_AF5_SPI1
#define SPIx_MISO_PIN                    GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT              GPIOA
#define SPIx_MISO_AF                     GPIO_AF5_SPI1
#define SPIx_MOSI_PIN                    GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_MOSI_AF                     GPIO_AF5_SPI1

#if USE_INT_PIN
/* Following GPIO is used to check on INT pin for READY signal during communication */
#define LT_INT_BANK   GPIOF
#define LT_INT_PIN    GPIO_PIN_15
#define LT_INT_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()
#define LT_INT_GPIO_PORT              GPIOF
#endif

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */
