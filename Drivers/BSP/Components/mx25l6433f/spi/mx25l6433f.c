/**
 ******************************************************************************
 * @file    mx25l6433f.c
 * @modify  MCD Application Team
 * @brief   This file provides the MX25L6433F QSPI driver.
 ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "mx25l6433f.h"
#include "mem_conf.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup MX25L6433F MX25L6433F
  * @{
  */

/** @defgroup MX25L6433F_Exported_Variables MX25L6433F Exported Variables
  * @{
  */
const MX25L6433F_MEM_Drv_t MX25L6433F_MEM_Driver =
{
  MX25L6433F_Init,
  MX25L6433F_DeInit,
  MX25L6433F_ReadID,
  MX25L6433F_GetFlashInfo,
  MX25L6433F_Read,
  MX25L6433F_ReadDMA,
  MX25L6433F_PageProgram,
  MX25L6433F_PageProgramDMA,
  MX25L6433F_BlockErase,
  MX25L6433F_ChipErase,
  NULL, /* EnableMemoryMappedMode */
  MX25L6433F_ResetEnable,
  MX25L6433F_ResetMemory
};

/** @defgroup MX25L6433F_Exported_Functions MX25L6433F Exported Functions
  * @{
  */
/**
  * @brief  Register component IO bus
  * @param  pObj Component object pointer
  * @param  pIO  Component IO structure pointer
  * @retval Memory status
  */
int32_t MX25L6433F_RegisterBusIO(MX25L6433F_Object_t *pObj, MX25L6433F_IO_t *pIO)
{
  int32_t ret = MX25L6433F_OK;

  if(pObj == NULL)
  {
    ret = MX25L6433F_ERROR;
  }
  else
  {
    pObj->IO.Init         = pIO->Init;
    pObj->IO.DeInit       = pIO->DeInit;
    pObj->IO.Address      = pIO->Address;
    pObj->IO.SendData     = pIO->SendData;
    pObj->IO.SendDataDMA  = pIO->SendDataDMA;
    pObj->IO.RecvData     = pIO->RecvData;
    pObj->IO.RecvDataDMA  = pIO->RecvDataDMA;
    pObj->IO.GetTick      = pIO->GetTick;

    if(pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initialize the MX25L6433F device
  * @param  pObj Component object pointer
  * @retval Memory status
  */
int32_t MX25L6433F_Init(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;

  if(pObj == NULL)
  {
    ret = MX25L6433F_ERROR;
  }
  else if (pObj->IsInitialized == 0)
  {
    /* Reset Memory */
    ret = MX25L6433F_ResetEnable(pObj);
    if(ret == MX25L6433F_OK)
    {
      ret = MX25L6433F_ResetMemory(pObj);
      if(ret == MX25L6433F_OK)
      {
        ret = MX25L6433F_WriteBurstReadLength(pObj, MX25L6433F_WRAP_AROUND_16BYTE);
        if(ret == MX25L6433F_OK)
        {
          pObj->IsInitialized = 1;
        }
      }
    }
  }

  return ret;
}

/**
  * @brief  De-Initialize the MX25L6433F device.
  * @param  pObj Component object pointer
  * @retval Memory status
  */
int32_t MX25L6433F_DeInit(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;

  if(pObj == NULL)
  {
    ret = MX25L6433F_ERROR;
  }
  else if (pObj->IsInitialized == 1)
  {
    if(pObj->IO.DeInit() == BSP_ERROR_NONE)
    {
      pObj->IsInitialized = 0;
    }
    else
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Get Flash information
  * @param  pInfo pointer to information structure
  * @retval Memory status
  */
int32_t MX25L6433F_GetFlashInfo(MX25L6433F_Info_t *pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize              = MX25L6433F_FLASH_SIZE;
  pInfo->EraseBlockSize         = MX25L6433F_BLOCK_64K;
  pInfo->EraseBlocksNumber      = (MX25L6433F_FLASH_SIZE/MX25L6433F_BLOCK_64K);
  pInfo->EraseSubBlockSize      = MX25L6433F_BLOCK_32K;
  pInfo->EraseSubBlocksNumber   = (MX25L6433F_FLASH_SIZE/MX25L6433F_BLOCK_32K);
  pInfo->EraseSectorSize        = MX25L6433F_SECTOR_4K;
  pInfo->EraseSectorsNumber     = (MX25L6433F_FLASH_SIZE/MX25L6433F_SECTOR_4K);
  pInfo->ProgPageSize           = MX25L6433F_PAGE_SIZE;
  pInfo->ProgPagesNumber        = (MX25L6433F_FLASH_SIZE/MX25L6433F_PAGE_SIZE);

  return MX25L6433F_OK;
};

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  pObj Component object pointer
  * @retval Memory status
  */
int32_t MX25L6433F_AutoPollingMemReady(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;

  uint32_t max_timeout = 5000000U;

  //wait until WIP is cleared
  uint8_t status;
  do
  {
    if(MX25L6433F_ReadStatusRegister(pObj, &status) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
    max_timeout--;
  } while ((ret == MX25L6433F_OK) && (max_timeout > 0) && ((status & MX25L6433F_MEMORY_READY_MASK_VALUE) != 0));

  if(max_timeout == 0)
  {
    ret = MX25L6433F_ERROR;
  }

  return ret;
}

/* Read/Write Array Commands ****************************************************/
/**
  * @brief  Reads an amount of data from the memory.
  *         SPI/DUAL_OUT/DUAL_INOUT/QUAD_OUT/QUAD_INOUT/; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4
  * @param  pObj Component object pointer
  * @param  Mode Interface mode
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @retval Memory status
  */
int32_t MX25L6433F_Read(MX25L6433F_Object_t *pObj, MX25L6433F_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd[4];

  switch(Mode)
  {
    case MX25L6433F_SPI_MODE :
      cmd[0] = MX25L6433F_READ_CMD;
      break;

    case MX25L6433F_DUAL_OUT_MODE :
      cmd[0] = MX25L6433F_DUAL_OUT_READ_CMD;
      break;

    case MX25L6433F_DUAL_IO_MODE :
      cmd[0] = MX25L6433F_DUAL_INOUT_READ_CMD;
      break;

    case MX25L6433F_QUAD_OUT_MODE :
      cmd[0] = MX25L6433F_QUAD_OUT_READ_CMD;
      break;

    case MX25L6433F_QUAD_IO_MODE :
      cmd[0] = MX25L6433F_QUAD_INOUT_READ_CMD;
      break;

    default :
      return MX25L6433F_ERROR;
  }

  cmd[1] = ((ReadAddr>>16) & 0xFF);  // A23~A16
  cmd[2] = ((ReadAddr>>8)  & 0xFF);  // A15~A8
  cmd[3] = (ReadAddr       & 0xFF);  // A7~A0

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command */
  if(MEM_SPI_Send((uint8_t *)cmd, 4) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }
  /* Reception of the data */
  else if(MEM_SPI_Recv(pData, Size) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  return ret;
}

/**
  * @brief  Reads an amount of data using DMA from the memory.
  *         SPI/DUAL_OUT/DUAL_INOUT/QUAD_OUT/QUAD_INOUT/; 1-1-1/1-1-2/1-2-2/1-1-4/1-4-4
  * @param  pObj Component object pointer
  * @param  Mode Interface mode
  * @param  pData Pointer to data to be read
  * @param  ReadAddr Read start address
  * @param  Size Size of data to read
  * @retval Memory status
  */
int32_t MX25L6433F_ReadDMA(MX25L6433F_Object_t *pObj, MX25L6433F_Interface_t Mode, uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd[4];

  switch(Mode)
  {
    case MX25L6433F_SPI_MODE :
      cmd[0] = MX25L6433F_READ_CMD;
      break;

    case MX25L6433F_DUAL_OUT_MODE :
      cmd[0] = MX25L6433F_DUAL_OUT_READ_CMD;
      break;

    case MX25L6433F_DUAL_IO_MODE :
      cmd[0] = MX25L6433F_DUAL_INOUT_READ_CMD;
      break;

    case MX25L6433F_QUAD_OUT_MODE :
      cmd[0] = MX25L6433F_QUAD_OUT_READ_CMD;
      break;

    case MX25L6433F_QUAD_IO_MODE :
      cmd[0] = MX25L6433F_QUAD_INOUT_READ_CMD;
      break;

    default :
      return MX25L6433F_ERROR;
  }

  cmd[1] = ((ReadAddr>>16) & 0xFF);  // A23~A16
  cmd[2] = ((ReadAddr>>8)  & 0xFF);  // A15~A8
  cmd[3] = (ReadAddr       & 0xFF);  // A7~A0

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command */
  if(MEM_SPI_Send((uint8_t *)cmd, 4) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }
  /* Reception of the data */
  else if(MEM_SPI_Recv_DMA(pData, Size) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  return ret;
}

/**
  * @brief  Writes an amount of data to the memory.
  *         SPI/QUAD_INOUT/; 1-1-1/1-4-4
  * @param  pObj Component object pointer
  * @param  Mode Interface mode
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ MX25L6433F_PAGE_SIZE
  * @note   Address size is forced to 3 Bytes when the 4 Bytes address size
  *         command is not available for the specified interface mode
  * @retval Memory status
  */
int32_t MX25L6433F_PageProgram(MX25L6433F_Object_t *pObj, MX25L6433F_Interface_t Mode, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd[4];

  switch(Mode)
  {
    case MX25L6433F_SPI_MODE :
      cmd[0] = MX25L6433F_PAGE_PROG_CMD;
      break;

    case MX25L6433F_QUAD_IO_MODE :
      cmd[0] = MX25L6433F_QUAD_PAGE_PROG_CMD;
      break;

    default :
      return MX25L6433F_ERROR;
  }

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  // Enable Write operation
  if(MX25L6433F_WriteEnable(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  cmd[1] = ((WriteAddr>>16) & 0xFF);  // A23~A16
  cmd[2] = ((WriteAddr>>8)  & 0xFF);  // A15~A8
  cmd[3] = (WriteAddr       & 0xFF);  // A7~A0

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command */
  if(MEM_SPI_Send((uint8_t *)cmd, 4) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }
  /* Transmission of the data */
  else if(MEM_SPI_Send(pData, Size) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Writes an amount of data using DMA to the memory.
  *         SPI/QUAD_INOUT/; 1-1-1/1-4-4
  * @param  pObj Component object pointer
  * @param  Mode Interface mode
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ MX25L6433F_PAGE_SIZE
  * @note   Address size is forced to 3 Bytes when the 4 Bytes address size
  *         command is not available for the specified interface mode
  * @retval Memory status
  */
int32_t MX25L6433F_PageProgramDMA(MX25L6433F_Object_t *pObj, MX25L6433F_Interface_t Mode, uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd[4];

  switch(Mode)
  {
    case MX25L6433F_SPI_MODE :
      cmd[0] = MX25L6433F_PAGE_PROG_CMD;
      break;

    case MX25L6433F_QUAD_IO_MODE :
      cmd[0] = MX25L6433F_QUAD_PAGE_PROG_CMD;
      break;

    default :
      return MX25L6433F_ERROR;
  }

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  // Enable Write operation
  if(MX25L6433F_WriteEnable(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  cmd[1] = ((WriteAddr>>16) & 0xFF);  // A23~A16
  cmd[2] = ((WriteAddr>>8)  & 0xFF);  // A15~A8
  cmd[3] = (WriteAddr       & 0xFF);  // A7~A0

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command */
  if(MEM_SPI_Send((uint8_t *)cmd, 4) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }
  /* Transmission of the data using DMA */
  else if(MEM_SPI_Send_DMA(pData, Size) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  return ret;
}

/**
  * @brief  Erases the specified block of the memory.
  *         MX25L6433F support 4K, 32K and 64K size block erase commands.
  * @param  pObj Component object pointer
  * @param  BlockAddress Block address to erase
  * @param  BlockSize Block size to erase
  * @retval Memory status
  */
int32_t MX25L6433F_BlockErase(MX25L6433F_Object_t *pObj, uint32_t BlockAddress, MX25L6433F_Erase_t BlockSize)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd[4];

  switch(BlockSize)
  {
    case MX25L6433F_ERASE_64K :
      cmd[0] = MX25L6433F_BLOCK_ERASE_CMD;
      break;
    case MX25L6433F_ERASE_32K :
      cmd[0] = MX25L6433F_SUBBLOCK_ERASE_CMD;
      break;
    case MX25L6433F_ERASE_4K :
      cmd[0] = MX25L6433F_SECTOR_ERASE_CMD;
      break;
    default :
      return MX25L6433F_ERROR;
  }

  cmd[1] = ((BlockAddress>>16) & 0xFF);  // A23~A16
  cmd[2] = ((BlockAddress>>8)  & 0xFF);  // A15~A8
  cmd[3] = (BlockAddress       & 0xFF);  // A7~A0

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  // Enable Write operation
  if(MX25L6433F_WriteEnable(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command */
  ret = MEM_SPI_Send((uint8_t *)cmd, 4);

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Whole chip erase.
  * @param  pObj Component object pointer
  * @param  Mode Interface mode
  * @retval Memory status
  */
int32_t MX25L6433F_ChipErase(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd = MX25L6433F_CHIP_ERASE_CMD;

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  // Enable Write operation
  if(MX25L6433F_WriteEnable(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_CHIP_ERASE_CMD */
  ret = MEM_SPI_Send((uint8_t *)&cmd, 1);

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/* ID Commands ****************************************************************/
/**
  * @brief  Read Flash 3 Byte IDs.
  *         Manufacturer ID, Memory type, Memory density
  * @param  pObj Component object pointer
  * @param  ID 3 bytes IDs pointer
  * @retval Memory status
  */
int32_t MX25L6433F_ReadID(MX25L6433F_Object_t *pObj, uint32_t *ID)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd = MX25L6433F_READ_ID_CMD;
  uint8_t __IO id[4] = {0x00, 0x00, 0x00, 0x00};

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_READ_ID_CMD */
  if(MEM_SPI_Send((uint8_t *)&cmd, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }
  /* Reception of the data */
  else if(MEM_SPI_Recv((uint8_t *)id, 3) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  if(ret == MX25L6433F_OK)
  {
    *ID = *(uint32_t *)id;
  }

  return ret;
}

/* Register/Setting Commands **************************************************/
/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  pObj Component object pointer
  * @retval Memory status
  */
int32_t MX25L6433F_WriteEnable(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd = MX25L6433F_WRITE_ENABLE_CMD;

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_WRITE_ENABLE_CMD */
  if(MEM_SPI_Send((uint8_t *)&cmd, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  This function reset the (WEN) Write Enable Latch bit.
  * @param  pObj Component object pointer
  * @retval Memory status
  */
int32_t MX25L6433F_WriteDisable(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd = MX25L6433F_WRITE_DISABLE_CMD;

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_WRITE_DISABLE_CMD */
  if(MEM_SPI_Send((uint8_t *)&cmd, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Read Flash Status register value
  * @param  pObj Component object pointer
  * @param  Value Status register value pointer
  * @retval Memory status
  */
int32_t MX25L6433F_ReadStatusRegister(MX25L6433F_Object_t *pObj, uint8_t *Value)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd = MX25L6433F_READ_STATUS_REG_CMD;
  uint8_t __IO value;

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_READ_STATUS_REG_CMD */
  if(MEM_SPI_Send((uint8_t *)&cmd, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }
  /* Reception of the data */
  else if(MEM_SPI_Recv((uint8_t *)&value, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  *Value = value;

  return ret;
}

/**
  * @brief  Read Flash Configuration Register value
  * @param  pObj Component object pointer
  * @param  Mode Interface mode
  * @param  Value Status register value pointer
  * @retval Memory status
  */
int32_t MX25L6433F_ReadCfgRegister(MX25L6433F_Object_t *pObj, MX25L6433F_Interface_t Mode, uint8_t *Value)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd;
  uint8_t __IO value;

  switch(Mode)
  {
    case MX25L6433F_SPI_MODE :
      cmd = MX25L6433F_READ_CFG_REG_CMD;
      break;
    case MX25L6433F_DUAL_OUT_MODE :
    case MX25L6433F_DUAL_IO_MODE :
    case MX25L6433F_QUAD_OUT_MODE :
    case MX25L6433F_QUAD_IO_MODE :
    default :
      return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_READ_CFG_REG_CMD */
  /* Send the command MX25L6433F_READ_STATUS_REG_CMD */
  if(MEM_SPI_Send((uint8_t *)&cmd, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }
  /* Reception of the data */
  else if(MEM_SPI_Recv((uint8_t *)&value, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  *Value = value;

  return ret;
}

/**
  * @brief  Write to Flash Status register
  * @param  pObj Component object pointer
  * @param  Mode Interface mode
  * @param  Value Status register value to write
  * @retval Memory status
  */
int32_t MX25L6433F_WriteCfgRegister(MX25L6433F_Object_t *pObj, MX25L6433F_Interface_t Mode, uint8_t Value)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd[2] = {MX25L6433F_WRITE_STATUS_CFG_REG_CMD, Value};

  /* Enable write operations */
  if(MX25L6433F_WriteEnable(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_WRITE_STATUS_CFG_REG_CMD with data */
  if(MEM_SPI_Send((uint8_t *)cmd, 2) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Set Burst Read length
  * @param  pObj Component object pointer
  * @param  Value New Burst length
  * @retval Memory status
  */
int32_t MX25L6433F_WriteBurstReadLength(MX25L6433F_Object_t *pObj, uint8_t Value)
{
  int32_t ret = MX25L6433F_OK;

  uint8_t __IO cmd[2] = { MX25L6433F_SET_BURST_LENGTH_CMD, Value};

  /* Wait till the memory is ready */
  if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
  {
    return MX25L6433F_ERROR;
  }

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command */
  if(MEM_SPI_Send((uint8_t *)cmd, 2) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
}

/* Reset Commands *************************************************************/
/**
  * @brief  Flash reset enable command
  * @param  pObj Component object pointer
  * @retval Memory status
  */
int32_t MX25L6433F_ResetEnable(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd = MX25L6433F_RESET_ENABLE_CMD;

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_RESET_ENABLE_CMD */
  if(MEM_SPI_Send((uint8_t *)&cmd, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  return ret;
}

/**
  * @brief  Flash reset memory command
  * @param  pObj Component object pointer
  * @retval Memory status
  */
int32_t MX25L6433F_ResetMemory(MX25L6433F_Object_t *pObj)
{
  int32_t ret = MX25L6433F_OK;
  uint8_t __IO cmd = MX25L6433F_RESET_MEMORY_CMD;

  /* Reset the nCS pin */
  MEM_CS_LOW();

  /* Send the command MX25L6433F_RESET_MEMORY_CMD */
  if(MEM_SPI_Send((uint8_t *)&cmd, 1) != BSP_ERROR_NONE)
  {
    ret = MX25L6433F_ERROR;
  }

  /* Set the nCS */
  MEM_CS_HIGH();

  /* Wait till the memory is ready */
  if(ret == MX25L6433F_OK)
  {
    if(MX25L6433F_AutoPollingMemReady(pObj) != MX25L6433F_OK)
    {
      ret = MX25L6433F_ERROR;
    }
  }

  return ret;
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
