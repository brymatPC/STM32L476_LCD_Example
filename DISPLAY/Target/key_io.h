/**
  ******************************************************************************
  * File Name          : Target/key_io.h
  * Description        : This file provides code for the exported APIs
  *                      of the Joystick Driver instances.
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
#ifndef __KEY_IO_H__
#define __KEY_IO_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "key_conf.h"

/** @addtogroup DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup Joystick_Driver Joystick Driver
  * @brief    Joystick Driver API.
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/** @defgroup Joystick_Exported_Constants Joystick Exported Constants
  * @brief    Joystick Drivers Constants.
  * @{
  */
/** @addtogroup BSP_Joystick_Error_codes BSP Joystick Error codes
  * @brief    Returned Error code after calls to Joystick Driver APIs.
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

/** @addtogroup Joystick_Keys Joystick Keys
  * @brief    Joystick buttons values
  * @{
  */
enum
{
  BSP_KEY_INVALID                                 /*!< Returned value when an invalid or an unsupported button is pressed */
, BSP_KEY_CENTER                                  /*!< Returned value when Key Center is pressed */
, BSP_KEY_UP                                      /*!< Returned value when Key Up is pressed */
, BSP_KEY_DOWN                                    /*!< Returned value when Key Down is pressed */
, BSP_KEY_LEFT                                    /*!< Returned value when Key Left is pressed */
, BSP_KEY_RIGHT                                   /*!< Returned value when Key Right is pressed */
};
/**
  * @}
  */

/** @addtogroup Joystick_Orientation Joystick Orientation
  * @brief    Joystick buttons Orientation
  * @{
  */
enum
{
  KEY_ORIENTATION_PORTRAIT                        /*!< \hideinitializer Portrait orientation choice of Joystick Buttons               */
, KEY_ORIENTATION_PORTRAIT_ROT180                 /*!< \hideinitializer Portrait rotated 180° orientation choice of Joystick Buttons  */
, KEY_ORIENTATION_LANDSCAPE                       /*!< \hideinitializer Landscape orientation choice of Joystick Buttons              */
, KEY_ORIENTATION_LANDSCAPE_ROT180                /*!< \hideinitializer Landscape rotated 180° orientation choice of Joystick Buttons */
};
/**
  * @}
  */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
int32_t BSP_KEY_Init(uint32_t Instance, uint8_t Orientation);
int32_t BSP_KEY_DeInit(uint32_t Instance);
int32_t BSP_KEY_SetOrientation(uint32_t Instance, uint32_t Orientation);
int32_t BSP_KEY_GetState(uint32_t Instance, uint8_t *pState);
/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* __KEY_IO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
