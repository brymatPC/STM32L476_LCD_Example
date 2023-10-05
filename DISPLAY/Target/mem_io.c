/**
  ******************************************************************************
  * File Name          : Target/mem_io.c
  * Description        : This file provides code for the configuration
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

/* Includes ------------------------------------------------------------------*/
#include "mem_os.h"
#include "mem_io.h"

/** @addtogroup DISPLAY
  * @brief      DISPLAY Software Expansion Pack.
  * @{
  */
/** @defgroup External_Memory_Driver External Memory Driver
  * @brief    External Memory Driver API.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

#define BSP_MEM_CHECK_PARAMS(Instance)

/* Exported variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

static void                 *MemCompObj;
static MEM_Drv_t            *MemDrv;
static MX25L6433F_IO_t      IOCtx;
static MX25L6433F_Object_t  ObjCtx;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

static int32_t MEM_IO_Init(void);
static int32_t MEM_IO_DeInit(void);
static int32_t MEM_Probe(uint32_t Instance);

/* Deprecated functions ------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup External_Memory_Exported_Functions External Memory Exported Functions
  * @brief    External Memory Drivers API.
  * @{
  */
/**
  * @brief  Initializes the External Memory.
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_Init(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  /* MEM OS Initialize */
  if(MEM_OS_Initialize(0) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_NO_INIT;
  }
  else
  {
    /* Reset Handlers */
    MemCompObj = NULL;
    MemDrv = NULL;

    ret = MEM_Probe(Instance);
  }

  return ret;
}

/**
  * @brief  De-Initializes the External Memory resources
  * @param  Instance:     External Memory Instance
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_DeInit(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MemDrv == NULL)
  {
    ret = BSP_ERROR_NO_INIT;
  }
  else if(MemDrv->DeInit != NULL)
  {
    if(MemDrv->DeInit(MemCompObj) < 0)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }

  return ret;
}

/**
  * @brief  Erase block of the External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @param  BlockAddress: Memory block address to de erased.
  * @param  BlockSize:    Memory block size to be erased.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_BlockErase(uint32_t Instance, uint32_t BlockAddress, uint32_t BlockSize)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_TIMEOUT_BUSY) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv == NULL)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else if(MemDrv->BlockErase != NULL)
    {
      if(MemDrv->BlockErase(MemCompObj, BlockAddress, BlockSize) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Erase the External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_ChipErase(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_TIMEOUT_BUSY) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv == NULL)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else if(MemDrv->ChipErase != NULL)
    {
      if(MemDrv->ChipErase(MemCompObj) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Read Data from External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to buffer memory to be filled.
  * @param  Address:      Memory address to read from.
  * @param  Size:         Size of the data memory to be read.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_ReadData(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_TIMEOUT_BUSY) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv == NULL)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else if(MemDrv->Read != NULL)
    {
      if(MemDrv->Read(MemCompObj, MEM_INTERFACE_MODE, pData, Address, Size) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Read Data from External Memory Interface using DMA.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to buffer memory to be filled.
  * @param  Address:      Memory address to read from.
  * @param  Size:         Size of the data memory to be read.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_ReadDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_TIMEOUT_BUSY) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv == NULL)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else if(MemDrv->ReadDMA != NULL)
    {
      if(MemDrv->ReadDMA(MemCompObj, MEM_INTERFACE_MODE, pData, Address, Size) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }

  return ret;
}

/**
  * @brief  Write Data to External Memory Interface.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to data memory to be written.
  * @param  Address:      Memory address to write to.
  * @param  Size:         Size of the data memory to be written
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_WriteData(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_TIMEOUT_BUSY) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv == NULL)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else if(MemDrv->PageProgram != NULL)
    {
      MEM_Info_t FlashInfo;
      if(MemDrv->GetFlashInfo(&FlashInfo) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
        uint32_t NumOfData = 0;
        uint8_t* buffer = pData;

        ret = BSP_ERROR_NONE;
        Addr = Address % FlashInfo.ProgPageSize;
        count = FlashInfo.ProgPageSize - Addr;
        NumOfPage =  Size / FlashInfo.ProgPageSize;
        NumOfSingle = Size % FlashInfo.ProgPageSize;

        if (Addr == 0) /*!< Address is FlashInfo.ProgPageSize aligned  */
        {
          if (NumOfPage == 0) /*!< NumByteToWrite < FlashInfo.ProgPageSize */
          {
            NumOfData = Size;
            if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
            {
              ret = BSP_ERROR_COMPONENT_FAILURE;
            }
          }
          else /*!< Size > FlashInfo.ProgPageSize */
          {
            while ((ret == BSP_ERROR_NONE) && (NumOfPage--))
            {
              NumOfData = FlashInfo.ProgPageSize;
              if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
              {
                ret = BSP_ERROR_COMPONENT_FAILURE;
              }
              else
              {
                Address +=  FlashInfo.ProgPageSize;
                buffer += FlashInfo.ProgPageSize;
              }
            }

            if ((ret == BSP_ERROR_NONE) && (NumOfSingle != 0))
            {
              NumOfData = NumOfSingle;
              if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
              {
                ret = BSP_ERROR_COMPONENT_FAILURE;
              }
            }
          }
        }
        else /*!< Address is not FlashInfo.ProgPageSize aligned  */
        {
          if (NumOfPage == 0) /*!< Size < FlashInfo.ProgPageSize */
          {
            if (NumOfSingle > count) /*!< (Size + Address) > FlashInfo.ProgPageSize */
            {
              temp = NumOfSingle - count;
              NumOfData = count;
              if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
              {
                ret = BSP_ERROR_COMPONENT_FAILURE;
              }
              else
              {
                Address +=  count;
                buffer += count;
                NumOfData = temp;
                if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
                {
                  ret = BSP_ERROR_COMPONENT_FAILURE;
                }
              }
            }
            else
            {
              NumOfData = Size;
              if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
              {
                ret = BSP_ERROR_COMPONENT_FAILURE;
              }
            }
          }
          else /*!< Size > FlashInfo.ProgPageSize */
          {
            Size -= count;
            NumOfPage =  Size / FlashInfo.ProgPageSize;
            NumOfSingle = Size % FlashInfo.ProgPageSize;

            NumOfData = count;

            if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
            {
              ret = BSP_ERROR_COMPONENT_FAILURE;
            }
            else
            {
              Address +=  count;
              buffer += count;

              while ((ret == BSP_ERROR_NONE) && (NumOfPage--))
              {
                NumOfData = FlashInfo.ProgPageSize;

                if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
                {
                  ret = BSP_ERROR_COMPONENT_FAILURE;
                }
                else
                {
                  Address +=  FlashInfo.ProgPageSize;
                  buffer += FlashInfo.ProgPageSize;
                }
              }

              if (NumOfSingle != 0)
              {
                NumOfData = NumOfSingle;
                if(MemDrv->PageProgram(MemCompObj, MEM_INTERFACE_MODE, buffer, Address, NumOfData) < 0)
                {
                  ret = BSP_ERROR_COMPONENT_FAILURE;
                }
              }
            }
          }
        }
      }
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Write Data to External Memory using DMA.
  * @param  Instance:     External Memory Instance.
  * @param  pData:        Pointer to data memory to be written.
  * @param  Address:      Memory address to write to.
  * @param  Size:         Size of the data memory to be written
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_WriteDataDMA(uint32_t Instance, uint8_t *pData, uint32_t Address, uint32_t Size)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_TIMEOUT_BUSY) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv == NULL)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else if(MemDrv->PageProgramDMA != NULL)
    {
      MEM_Info_t FlashInfo;
      if(MemDrv->GetFlashInfo(&FlashInfo) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        uint32_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
        Addr = Address % FlashInfo.ProgPageSize;
        count = FlashInfo.ProgPageSize - Addr;
        NumOfPage =  Size / FlashInfo.ProgPageSize;
        NumOfSingle = Size % FlashInfo.ProgPageSize;

        /* Writing Data using DMA is allowed only within a PAGE */
        if ( ((Addr == 0) && (NumOfPage == 0))              /*!< Address is FlashInfo.ProgPageSize aligned and NumByteToWrite <= FlashInfo.ProgPageSize */
          || ((NumOfPage == 0) && (NumOfSingle <= count)) ) /*!< (Size + Address) <= FlashInfo.ProgPageSize */
        {
          if(MemDrv->PageProgramDMA(MemCompObj, MEM_INTERFACE_MODE, pData, Address, Size) < 0)
          {
            ret = BSP_ERROR_COMPONENT_FAILURE;
          }
          else
          {
            ret = BSP_ERROR_NONE;
          }
        }
        else
        {
          ret = BSP_ERROR_WRONG_PARAM;
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  Enable Memory Mapped Mode.
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
int32_t BSP_MEM_EnableMemoryMappedMode(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;

  BSP_MEM_CHECK_PARAMS(Instance);

  if(MEM_OS_TryLock(Instance, MEM_OS_TIMEOUT_BUSY) != MEM_OS_ERROR_NONE)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    if(MemDrv == NULL)
    {
      ret = BSP_ERROR_NO_INIT;
    }
    else if(MemDrv->EnableMemoryMappedMode != NULL)
    {
      if(MemDrv->EnableMemoryMappedMode(MemCompObj, MEM_INTERFACE_MODE) < 0)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }

    MEM_OS_Unlock(Instance);
  }

  return ret;
}

/**
  * @brief  Get the status of the SPI Transfer.
  * @param  Instance:     External Memory Instance.
  * @retval uint8_t:      Zero if no Transfer, Transfer Operation code otherwise.
  */
uint8_t BSP_MEM_GetTransferStatus(uint32_t Instance)
{
  BSP_MEM_CHECK_PARAMS(Instance);

  return MEM_OS_IsLocked(Instance);
}

/**
  * @brief  Wait for until complete SPI Transfer.
  * @param  Instance:     External Memory Instance.
  */
void BSP_MEM_WaitForTransferToBeDone(uint32_t Instance)
{
  if (Instance < MEM_INSTANCES_NBR)
  {
    MEM_OS_WaitForTransferToBeDone(Instance);
  }
}

/**
  * @brief  Signal Transfer Event Done.
  * @param  Instance:     External Memory Instance.
  */
__WEAK void BSP_MEM_SignalTransferDone(uint32_t Instance)
{
  /* This is the user's Callback to be implemented at the application level */
}
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PF */

/* USER CODE END PF */

/**
  * @brief  Register Bus IOs for instance 0 if MX25L6433F ID is OK
  * @param  Instance:     External Memory Instance.
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_Probe(uint32_t Instance)
{
  int32_t ret = BSP_ERROR_NONE;
  uint32_t id = 0;

  /* Configure the lcd driver : map to MEM_IO function*/
  IOCtx.Init              = MEM_IO_Init;
  IOCtx.DeInit            = MEM_IO_DeInit;
  IOCtx.SendData          = 0;
  IOCtx.SendDataDMA       = 0;
  IOCtx.RecvData          = 0;
  IOCtx.RecvDataDMA       = 0;

  /* Set Base Address */
  IOCtx.Address           = MEM_BASE_ADDRESS;

  /* Register OSPI handle */
  ObjCtx.handle           = &hMEMSPI;

  /* Reset Initialization state */
  ObjCtx.IsInitialized = 0;

  /* Register the BusIO Driver */
  if(MX25L6433F_RegisterBusIO(&ObjCtx, &IOCtx) != MX25L6433F_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    /* Set MEM component object */
    MemCompObj = &ObjCtx;

    /* Check for Device ID */
    if((MX25L6433F_MEM_Driver.ReadID(MemCompObj, &id) == MX25L6433F_OK) && (id == MX25L6433F_ID))
    {
      /* Initialize the memory driver */
      if(MX25L6433F_MEM_Driver.Init(MemCompObj) == MX25L6433F_OK)
      {
        MemDrv = (MEM_Drv_t *)&MX25L6433F_MEM_Driver;
      }
      else
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
    }
    else
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
  }

  return ret;
}

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  /**
  * @brief  Transfer Read Complete CallBack
  * @param  hspi SPI Handler
  */
static void SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if(hspi == ObjCtx.handle)
  {
    /* Set the nCS */
    MEM_CS_HIGH();

    /* Unlock MEM ressources */
    MEM_OS_UnlockFromISR(0);

    /* Signal Transfer Done Event */
    BSP_MEM_SignalTransferDone(0);
  }
}
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

/**
  * @brief  Initializes MEM low level.
  * @param  None
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  ret = MEM_SPI_Init();

#if (USE_HAL_SPI_REGISTER_CALLBACKS == 1U)
  if(ret == BSP_ERROR_NONE)
  {
    HAL_SPI_RegisterCallback((SPI_HandleTypeDef *)ObjCtx.handle, HAL_SPI_RX_COMPLETE_CB_ID, SPI_RxCpltCallback);
  }
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */

  return ret;
}

/**
  * @brief  DeInitializes MEM low level
  * @param  None
  * @retval int32_t:      BSP status.
  */
static int32_t MEM_IO_DeInit(void)
{
  int32_t ret = BSP_ERROR_NONE;

  ret = MEM_SPI_DeInit();

  return ret;
}
/**
  * @}
  */
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
