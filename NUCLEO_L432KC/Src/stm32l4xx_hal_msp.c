/**
 ******************************************************************************
 * @file    SPI/SPI_FullDuplex_ComPolling/Src/stm32l4xx_hal_msp.c
 * @author  MCD Application Team
 * @brief   HAL MSP module.
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

/** @addtogroup STM32L4xx_HAL_Examples
 * @{
 */

/** @defgroup SPI_FullDuplex_AdvComPolling
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
 * @{
 */

/**
 * @brief UART MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO and NVIC configuration to their default state
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    USART_DBG_FORCE_RESET();
    USART_DBG_RELEASE_RESET();
    HAL_GPIO_DeInit(USART_DBG_TX_GPIO_PORT, USART_DBG_TX_PIN);
    HAL_GPIO_DeInit(USART_DBG_RX_GPIO_PORT, USART_DBG_RX_PIN);
}

/**
 * @brief SPI MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 *           - Peripheral's GPIO Configuration
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable peripherals and GPIO Clocks
    // Enable GPIO TX/RX clock
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    SPIx_CLK_ENABLE();

    // Configure peripheral GPIO
    // SPI SCK GPIO pin configuration
    GPIO_InitStruct.Pin = SPIx_SCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = SPIx_SCK_AF;
    HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    // SPI MISO GPIO pin configuration
    GPIO_InitStruct.Pin = SPIx_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIx_MISO_AF;
    HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    // SPI MOSI GPIO pin configuration
    GPIO_InitStruct.Pin = SPIx_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIx_MOSI_AF;
    HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief SPI MSP De-Initialization
 *        This function frees the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 *          - Revert GPIO configuration to its default state
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPIx) {
        // Reset peripherals
        SPIx_FORCE_RESET();
        SPIx_RELEASE_RESET();

        // Disable peripherals and GPIO Clocks
        // Configure SPI SCK as alternate function
        HAL_GPIO_DeInit(SPIx_SCK_GPIO_PORT, SPIx_SCK_PIN);
        // Configure SPI MISO as alternate function
        HAL_GPIO_DeInit(SPIx_MISO_GPIO_PORT, SPIx_MISO_PIN);
        // Configure SPI MOSI as alternate function
        HAL_GPIO_DeInit(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_PIN);
    }
}

/**
 * @brief RNG MSP Initialization
 *        This function inicializes hardware resources used in this example:
 *          - Enable the Peripheral's clock
 * @param hrng: RNG handle pointer
 * @retval None
 */
void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
    __HAL_RCC_HSI48_ENABLE();
    // RNG Peripheral clock enable
    __HAL_RCC_RNG_CLK_ENABLE();
    __HAL_RCC_RNG_CONFIG(RCC_RNGCLKSOURCE_HSI48);
}

/**
 * @brief RNG MSP De-Initialization
 *        This function freeze the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 * @param hrng: RNG handle pointer
 * @retval None
 */
void HAL_RNG_MspDeInit(RNG_HandleTypeDef *hrng)
{
    // Enable RNG reset state
    __HAL_RCC_RNG_FORCE_RESET();

    // Release RNG from reset state
    __HAL_RCC_RNG_RELEASE_RESET();
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
