/**
  ******************************************************************************
  * File Name          : Target/key_io.c
  * Description        : This file provides code for the configuration
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

/* Includes ------------------------------------------------------------------*/
#include "key_io.h"

/** @addtogroup DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup Joystick_Driver Joystick Driver
  * @brief    Joystick Driver API.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

typedef struct
{
  uint32_t Orientation;
}BSP_KEY_Ctx_t;

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Number of KEY instances */
#define KEY_INSTANCES_NBR                       1U

/* Pressed state GPIO value */
#define KEY_PRESSED_STATE                       GPIO_PIN_RESET

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

#define BSP_KEY_CHECK_PARAMS(Instance)

/* Exported variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

static BSP_KEY_Ctx_t KeyCtx[KEY_INSTANCES_NBR];

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Deprecated functions ------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup Joystick_Exported_Functions Joystick Exported Functions
  * @brief    Joystick Drivers API.
  * @{
  */
/**
  * @brief  Initializes the KEY.
  * @param  Instance:     KEY Instance.
  * @param  Orientation:  KEY Orientation.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_KEY_Init(uint32_t Instance, uint8_t Orientation)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_KEY_CHECK_PARAMS(Instance);

  KeyCtx[Instance].Orientation = Orientation;

  return ret;
}

/**
  * @brief  De-Initializes the KEY resources.
  * @param  Instance:     KEY Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_KEY_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_KEY_CHECK_PARAMS(Instance);

  KeyCtx[Instance].Orientation = 0;

  return ret;
}

/**
  * @brief  Sets the KEY Orientation.
  * @param  Instance:     KEY Instance.
  * @param  Orientation:  New KEY Orientation.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_KEY_SetOrientation(uint32_t Instance, uint32_t Orientation)
{
  int32_t ret = BSP_ERROR_NONE;

  BSP_KEY_CHECK_PARAMS(Instance);

  if(KeyCtx[Instance].Orientation != Orientation)
  {
    KeyCtx[Instance].Orientation = Orientation;
  }

  return ret;
}

/**
  * @brief  Read the Key State.
  * @param  Instance:     KEY Instance.
  * @param  pState:       Pointer to Key State.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_KEY_GetState(uint32_t Instance, uint8_t *pState)
{
  BSP_KEY_CHECK_PARAMS(Instance);

  *pState = BSP_KEY_INVALID;

#if defined(KEY_CENTER_GPIO_PIN)
  if (HAL_GPIO_ReadPin(KEY_CENTER_GPIO_PORT, KEY_CENTER_GPIO_PIN) == KEY_PRESSED_STATE)
    *pState = BSP_KEY_CENTER;
  else
#endif /* KEY_CENTER_GPIO_PIN */
#if defined(KEY_UP_GPIO_PIN)
  if (HAL_GPIO_ReadPin(KEY_UP_GPIO_PORT, KEY_UP_GPIO_PIN) == KEY_PRESSED_STATE)
    *pState =  (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT        ? BSP_KEY_UP :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT_ROT180  ? BSP_KEY_DOWN :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE        ? BSP_KEY_RIGHT :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE_ROT180 ? BSP_KEY_LEFT : BSP_KEY_INVALID))));
  else
#endif /* KEY_UP_GPIO_PIN */
#if defined(KEY_DOWN_GPIO_PIN)
  if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_PORT, KEY_DOWN_GPIO_PIN) == KEY_PRESSED_STATE)
    *pState =  (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT        ? BSP_KEY_DOWN :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT_ROT180  ? BSP_KEY_UP :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE        ? BSP_KEY_LEFT :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE_ROT180 ? BSP_KEY_RIGHT : BSP_KEY_INVALID))));
  else
#endif /* KEY_DOWN_GPIO_PIN */
#if defined(KEY_LEFT_GPIO_PIN)
  if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_PORT, KEY_LEFT_GPIO_PIN) == KEY_PRESSED_STATE)
    *pState =  (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT        ? BSP_KEY_LEFT :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT_ROT180  ? BSP_KEY_RIGHT :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE        ? BSP_KEY_DOWN :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE_ROT180 ? BSP_KEY_UP : BSP_KEY_INVALID))));
  else
#endif /* KEY_LEFT_GPIO_PIN */
#if defined(KEY_RIGHT_GPIO_PIN)
  if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_PORT, KEY_RIGHT_GPIO_PIN) == KEY_PRESSED_STATE)
    *pState =  (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT        ? BSP_KEY_RIGHT :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_PORTRAIT_ROT180  ? BSP_KEY_LEFT :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE        ? BSP_KEY_UP :
              (KeyCtx[Instance].Orientation == KEY_ORIENTATION_LANDSCAPE_ROT180 ? BSP_KEY_DOWN : BSP_KEY_INVALID))));
  else
#endif /* KEY_RIGHT_GPIO_PIN */

  if(*pState == BSP_KEY_INVALID)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }

  return BSP_ERROR_NONE;
}
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
  * @}
  */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
