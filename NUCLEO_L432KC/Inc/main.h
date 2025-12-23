/**
 ******************************************************************************
 * @file    SPI/SPI_FullDuplex_ComPolling/Inc/main.h
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
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_32.h"

#define USART_DBG USART2
#define USART_DBG_CLK_ENABLE() __HAL_RCC_USART2_CLK_ENABLE();
#define USART_DBG_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define USART_DBG_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define USART_DBG_FORCE_RESET() __HAL_RCC_USART2_FORCE_RESET()
#define USART_DBG_RELEASE_RESET() __HAL_RCC_USART2_RELEASE_RESET()

#define USART_DBG_TX_PIN GPIO_PIN_2
#define USART_DBG_TX_GPIO_PORT GPIOA
#define USART_DBG_TX_AF GPIO_AF7_USART2
#define USART_DBG_RX_PIN GPIO_PIN_3
#define USART_DBG_RX_GPIO_PORT GPIOA
#define USART_DBG_RX_AF GPIO_AF7_USART2

#define USARTx_RCC_CONFIG(__USARTxCLKSource__) __HAL_RCC_USART2_CONFIG(__USARTxCLKSource__)
#define RCC_USARTxCLKSOURCE_HSI RCC_USART2CLKSOURCE_HSI

// #define USARTx_RCC_CONFIG(__USARTxCLKSource__)   __HAL_RCC_USART1_CONFIG(__USARTxCLKSource__)
// #define RCC_USARTxCLKSOURCE_HSI                  RCC_USART2CLKSOURCE_HSI
///* Definition for USART_DBG clock resources */
// #define USART_DBG                           USART1
// #define USART_DBG_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
// #define USART_DBG_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
// #define USART_DBG_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
//
// #define USART_DBG_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
// #define USART_DBG_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()
//
// #define USART_DBG_TX_PIN                    GPIO_PIN_9
// #define USART_DBG_TX_GPIO_PORT              GPIOA
// #define USART_DBG_TX_AF                     GPIO_AF7_USART1
// #define USART_DBG_RX_PIN                    GPIO_PIN_10
// #define USART_DBG_RX_GPIO_PORT              GPIOA
// #define USART_DBG_RX_AF                     GPIO_AF7_USART1
//
///* Power mode related macros */
// #define USARTx_RCC_CONFIG(__USARTxCLKSource__)   __HAL_RCC_USART1_CONFIG(__USARTxCLKSource__)
// #define RCC_USARTxCLKSOURCE_HSI                  RCC_USART1CLKSOURCE_HSI

/* Definition for GPIO chip select clock resources */
#define LT_SPI_CS_CLK_ENABLE() __HAL_RCC_SPI1_CLK_ENABLE()

/* Definition for GPIO chip select pins */
#define LT_SPI_CS_BANK GPIOA
#define LT_SPI_CS_PIN GPIO_PIN_4

/* Definition for SPIx clock resources */
#define LT_SPI_INSTANCE SPI1
#define SPIx SPI1
#define SPIx_CLK_ENABLE() __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define SPIx_FORCE_RESET() __HAL_RCC_SPI1_FORCE_RESET()
#define SPIx_RELEASE_RESET() __HAL_RCC_SPI1_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPIx_SCK_PIN GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT GPIOA
#define SPIx_SCK_AF GPIO_AF5_SPI1
#define SPIx_MISO_PIN GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT GPIOA
#define SPIx_MISO_AF GPIO_AF5_SPI1
#define SPIx_MOSI_PIN GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT GPIOA
#define SPIx_MOSI_AF GPIO_AF5_SPI1

/* Exported variables ------------------------------------------------------- */
extern RNG_HandleTypeDef RNGHandle;

#endif /* __MAIN_H */
