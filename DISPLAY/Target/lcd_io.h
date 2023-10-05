/**
  ******************************************************************************
  * File Name          : Target/lcd_io.h
  * Description        : This file provides code for the exported APIs
  *                      of the LCD Driver instances.
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
#ifndef __LCD_IO_H__
#define __LCD_IO_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include "lcd_conf.h"
#include "../ili9341/ili9341.h"

/** @addtogroup DISPLAY DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup LCD_Driver LCD Driver
  * @brief    LCD Driver API.
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/** @defgroup LCD_Exported_Constants LCD Exported Constants
  * @brief    LCD Drivers Constants.
  * @{
  */
/** @addtogroup BSP_LCD_Error_codes BSP LCD Error codes
  * @brief    Returned Error code after calls to LCD Driver APIs.
  * @{
  */
#ifndef BSP_ERROR_NONE
#define BSP_ERROR_NONE                      0     /*!< \hideinitializer No error occured */
#define BSP_ERROR_NO_INIT                   -1    /*!< \hideinitializer Initialization function is not defined */
#define BSP_ERROR_WRONG_PARAM               -2    /*!< \hideinitializer Wrong parameters */
#define BSP_ERROR_BUSY                      -3    /*!< \hideinitializer Driver is busy */
#define BSP_ERROR_PERIPH_FAILURE            -4    /*!< \hideinitializer Peripheral error */
#define BSP_ERROR_COMPONENT_FAILURE         -5    /*!< \hideinitializer Componenet failure */
#define BSP_ERROR_UNKNOWN_FAILURE           -6    /*!< \hideinitializer Unknown failure */
#define BSP_ERROR_UNKNOWN_COMPONENT         -7    /*!< \hideinitializer Unknown component */
#define BSP_ERROR_BUS_FAILURE               -8    /*!< \hideinitializer Bus failure */
#define BSP_ERROR_CLOCK_FAILURE             -9    /*!< \hideinitializer Clock failure */
#define BSP_ERROR_MSP_FAILURE               -10   /*!< \hideinitializer MSP failure */
#define BSP_ERROR_FEATURE_NOT_SUPPORTED     -11   /*!< \hideinitializer Feature not supported */
#endif /* BSP_ERROR_NONE */
/**
  * @}
  */

/**
  * @addtogroup LCD_Orientation LCD Orientation
  * @brief      Supported LCD Orientations.
  * @{
  */
#define LCD_ORIENTATION_PORTRAIT            ILI9341_ORIENTATION_PORTRAIT          /*!< \hideinitializer Portrait orientation choice of LCD screen               */
#define LCD_ORIENTATION_PORTRAIT_ROT180     ILI9341_ORIENTATION_PORTRAIT_ROT180   /*!< \hideinitializer Portrait rotated 180° orientation choice of LCD screen  */
#define LCD_ORIENTATION_LANDSCAPE           ILI9341_ORIENTATION_LANDSCAPE         /*!< \hideinitializer Landscape orientation choice of LCD screen              */
#define LCD_ORIENTATION_LANDSCAPE_ROT180    ILI9341_ORIENTATION_LANDSCAPE_ROT180  /*!< \hideinitializer Landscape rotated 180° orientation choice of LCD screen */
/**
  * @}
  */

/**
  * @addtogroup LCD_Cache_lines LCD Cache lines
  * @brief      LCD Cache buffer as configured by the user.
  * @{
  */
#define BUFFER_CACHE_LINES                  24  /*!< \hideinitializer Number of lines defined by the user for the Buffer cache */
/**
  * @}
  */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
int32_t BSP_LCD_Init(uint32_t Instance, uint32_t Orientation);
int32_t BSP_LCD_DeInit(uint32_t Instance);
int32_t BSP_LCD_SetOrientation(uint32_t Instance, uint32_t Orientation);
int32_t BSP_LCD_GetOrientation(uint32_t Instance, uint32_t *pOrientation);
int32_t BSP_LCD_GetXSize(uint32_t Instance, uint32_t *pXSize);
int32_t BSP_LCD_GetYSize(uint32_t Instance, uint32_t *pYSize);
int32_t BSP_LCD_DisplayOn(uint32_t Instance);
int32_t BSP_LCD_DisplayOff(uint32_t Instance);
int32_t BSP_LCD_WriteData(uint32_t Instance, uint8_t *pData, uint32_t Length);
int32_t BSP_LCD_WriteDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Length);
int32_t BSP_LCD_SetDisplayWindow(uint32_t Instance, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
int32_t BSP_LCD_FillRGBRect(uint32_t Instance, uint8_t UseDMA, uint8_t *pData, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height);
uint8_t BSP_LCD_GetTransferStatus(uint32_t Instance);
void    BSP_LCD_WaitForTransferToBeDone(uint32_t Instance);
void    BSP_LCD_SignalTransferDone(uint32_t Instance);
void    BSP_LCD_SignalTearingEffectEvent(uint32_t Instance, uint8_t State, uint16_t Line);

/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* __LCD_IO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
