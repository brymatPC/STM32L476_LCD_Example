/**
  ******************************************************************************
  * File Name          : Target/mem_io.h
  * Description        : This file provides code for the exported APIs
  *                      of the External Memory Driver instances.
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
#ifndef __MEM_IO_H__
#define __MEM_IO_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mem.h"
#include "mem_conf.h"
#include "../mx25l6433f/mx25l6433f.h"

/** @addtogroup DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup External_Memory_Driver External Memory Driver
  * @brief    External Memory Driver API.
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/** @defgroup External_Memory_Exported_Constants External Memory Exported Constants
  * @brief    External Memory Drivers Constants.
  * @{
  */
/** @addtogroup BSP_External_Memory_Error_codes BSP External Memory Error codes
  * @brief    Returned Error code after calls to External Memory Driver APIs.
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
  * @addtogroup External_Memory_Base_Address External Memory Base Address
  * @brief      External Memory Address as configured by the user.
  * @{
  */
#define MEM_BASE_ADDRESS                    0x90000000  /*!< \hideinitializer External Memory Base Address */
/**
  * @}
  */

/**
  * @addtogroup External_Memory_Size External Memory Size
  * @brief      External Memory size as defined by the selected memory driver.
  * @{
  */
#define MEM_FLASH_SIZE                      MX25L6433F_FLASH_SIZE /*!< \hideinitializer External Memory Size */
/**
  * @}
  */

/**
  * @addtogroup External_Memory_Page_Size External Memory Page Size
  * @brief      External Memory page size as defined by the selected memory driver.
  * @{
  */
#define MEM_PAGE_SIZE                       MX25L6433F_PAGE_SIZE  /*!< \hideinitializer External Memory Page Size */
/**
  * @}
  */

/**
  * @addtogroup External_Memory_Block_Size External Memory Block Size
  * @brief      External Memory block size as defined by the selected memory driver.
  * @{
  */
#define MEM_BLOCK_SIZE                      MX25L6433F_BLOCK_64K  /*!< \hideinitializer External Memory Block Size */
/**
  * @}
  */
/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions prototypes ---------------------------------------------*/
int32_t BSP_MEM_Init(uint32_t Instance);
int32_t BSP_MEM_DeInit(uint32_t Instance);
int32_t BSP_MEM_BlockErase(uint32_t Instance, uint32_t BlockAddress, uint32_t BlockSize);
int32_t BSP_MEM_ChipErase(uint32_t Instance);
int32_t BSP_MEM_ReadData(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size);
int32_t BSP_MEM_ReadDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size);
int32_t BSP_MEM_WriteData(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size);
int32_t BSP_MEM_WriteDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size);
int32_t BSP_MEM_EnableMemoryMappedMode(uint32_t Instance);
uint8_t BSP_MEM_GetTransferStatus(uint32_t Instance);
void    BSP_MEM_WaitForTransferToBeDone(uint32_t Instance);
void    BSP_MEM_SignalTransferDone(uint32_t Instance);

/**
  * @}
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif
#endif /* __MEM_IO_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
