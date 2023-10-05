/**
  ******************************************************************************
  * File Name          : Target/lcd_os.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_OS_H__
#define __LCD_OS_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/** @addtogroup DISPLAY DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup LCD_Driver LCD Driver
  * @brief    LCD Driver API.
  * @{
  */

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  LCD_OS_ERROR_NONE = 0
, LCD_OS_ERROR_WRONG_PARAM
, LCD_OS_ERROR_INIT
, LCD_OS_ERROR_BUSY
, LCD_OS_ERROR_WAIT
, LCD_OS_ERROR_LOCK
, LCD_OS_ERROR_UNLOCK
} LCD_OS_Error_t;

/* USER CODE BEGIN ETD */

/* USER CODE END ETD */

/* Exported constants --------------------------------------------------------*/
/** @defgroup LCD_Exported_Constants LCD Exported Constants
  * @brief    LCD Drivers Constants.
  * @{
  */
#define LCD_OS_NO_WAIT                      ((uint32_t)  0)
#define LCD_OS_WAIT_FOREVER                 ((uint32_t)  0xFFFFFFFFUL)

/**
  * @addtogroup LCD_OS_Busy_Timeout LCD OS Busy Timeout value in Milliseconds
  * @brief      LCD OS Busy Timeout value in Milliseconds, default is 1ms.
  * @{
  */
/* USER CODE BEGIN EC */
#define LCD_OS_TIMEOUT_BUSY                 ((uint32_t)  1) /*!< \hideinitializer LCD OS Busy Timeout value in Milliseconds, default is 1ms */
/* USER CODE END EC */
/**
  * @}
  */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
/**
  * @brief  Initialize the LCD OS ressources.
  * @param  Instance LCD Instance
  * @retval LCD_OS_Error_t
  */
uint8_t LCD_OS_Initialize(uint32_t Instance);

/**
  * @brief  Lock the LCD ressources. Blocks until lock is available.
  * @param  Instance LCD Instance
  * @retval LCD_OS_Error_t
  */
uint8_t LCD_OS_Lock(uint32_t Instance);

/**
  * @brief  Unlock the LCD ressources.
  * @param  Instance LCD Instance
  * @retval LCD_OS_Error_t
  */
uint8_t LCD_OS_Unlock(uint32_t Instance);

/**
  * @brief  Safe unlock the LCD ressources in interrupt context.
  * @param  Instance LCD Instance
  * @retval LCD_OS_Error_t
  */
uint8_t LCD_OS_UnlockFromISR(uint32_t Instance);

/**
  * @brief  Attempt to lock the LCD. If the lock is not available, do
  *         nothing.
  * @param  Instance LCD Instance
  * @param  Timeout timeout in ms
  * @retval LCD_OS_Error_t
  */
uint8_t LCD_OS_TryLock(uint32_t Instance, uint32_t Timeout);

/**
  * @brief  This function checks if the LCD ressources are locked or not.
  * @param  Instance LCD Instance
  * @retval '1' if locked, '0' otherwise
  */
uint8_t LCD_OS_IsLocked(uint32_t Instance);

/**
  * @brief  This function will block until LCD transfer is Done.
  * @param  Instance LCD Instance
  * @retval LCD_OS_Error_t
  */
uint8_t LCD_OS_WaitForTransferToBeDone(uint32_t Instance);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* __LCD_OS_H__ */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
