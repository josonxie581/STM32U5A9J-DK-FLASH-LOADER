/**
  ******************************************************************************
  * @file    Loader_Src.c
  * @author  MCD Application Team
  * @brief   This file defines the operations of the external loader for
  *          MX25LM51245G OSPI memory of STM32U575I-EVAL.
  *           
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "Loader_Src.h"
#include "stm32u5x9j_discovery_ospi.h"
#include "mx25um51245g.h"
#include <string.h>
 

/* Private variables ---------------------------------------------------------*/
BSP_OSPI_NOR_Init_t Flash;
/* 声明链接器符号 */
extern uint32_t _sbss;
extern uint32_t _ebss;

int error_tls(void)
{
    return 0;
}


/* Private functions ---------------------------------------------------------*/

void HAL_Delay(uint32_t Delay)
{
  (void)Delay;
  int i=0;
  for (i=0; i<0x1000; i++);
}

KeepInCompilation HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{ 
  (void)TickPriority;
  return HAL_OK;
}

uint32_t HAL_GetTick(void)
{
        return 1;
}


/**
  * @brief  System initialization.
  * @param  None
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */
int Init()
{
   int32_t result=0;  

   uint32_t* bss_start = &_sbss;
   uint32_t* bss_end = &_ebss;
   uint32_t size = (uint32_t)bss_end - (uint32_t)bss_start;
   memset(bss_start, 0, size);

   /*  Init system*/
   SystemInit(); 
   HAL_Init();
   
   /*  define InterfaceMode and transferRate*/
  Flash.InterfaceMode = BSP_OSPI_NOR_OPI_MODE; 
  Flash.TransferRate  = BSP_OSPI_NOR_DTR_TRANSFER; 
 
  /* Configure the system clock  */
   SystemClock_Config();
/*Initialaize OSPI*/
   if(BSP_OSPI_NOR_Init(0,&Flash) !=0)
    return 0;
   result=BSP_OSPI_NOR_EnableMemoryMappedMode(0);
  if(result!=0)
    return result;
  
   return 1;
}

/**
  * @brief   Program memory.
  * @param   Address: page address
  * @param   Size   : size of data
  * @param   buffer : pointer to data buffer
  * @retval  1      : Operation succeeded
  * @retval  0      : Operation failed
  */

KeepInCompilation int MassErase (uint32_t Parallelism )
{
  (void)Parallelism;  
  BSP_OSPI_NOR_DeInit(0);
  BSP_OSPI_NOR_Init(0,&Flash);
  BSP_OSPI_NOR_Erase_Chip(0);
   while (BSP_OSPI_NOR_GetStatus(0)!=0);
  return 1;
}
KeepInCompilation int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    
    BSP_OSPI_NOR_DeInit(0);
    Address = Address & 0x0fffffff;
    
    BSP_OSPI_NOR_DeInit(0);
    
    BSP_OSPI_NOR_Init(0,&Flash);
    BSP_OSPI_NOR_Write(0,buffer,Address, Size);
     
   return 1;
}



/**
  * @brief   Sector erase.
  * @param   EraseStartAddress :  erase start address
  * @param   EraseEndAddress   :  erase end address
  * @retval  None
  */
KeepInCompilation int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress)
{
  uint32_t BlockAddr;
  EraseStartAddress &= 0x0FFFFFFF;  // eliminer 0x90 (CubeProgrammer)
  EraseEndAddress &= 0x0FFFFFFF;
  EraseStartAddress = EraseStartAddress -  EraseStartAddress % 0x10000;
  
  BSP_OSPI_NOR_DeInit(0);
  BSP_OSPI_NOR_Init(0,&Flash);
  while (EraseEndAddress>=EraseStartAddress)
  {
    BlockAddr = EraseStartAddress;
  
     BSP_OSPI_NOR_Erase_Block(0,BlockAddr,  MX25UM51245G_ERASE_64K);
    while (BSP_OSPI_NOR_GetStatus(0)!=0)
    {

    }
    
    EraseStartAddress+=0x10000;
  }
  
  return 1;	
}


/**
  * Description :
  * Calculates checksum value of the memory zone
  * Inputs    :
  *      StartAddress  : Flash start address
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : Checksum value
  * Note: Optional for all types of device
  */
uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal)
{
  uint8_t missalignementAddress = StartAddress%4;
  uint8_t missalignementSize = Size ;
  uint32_t cnt;
  uint32_t Val;
	
  StartAddress-=StartAddress%4;
  Size += (Size%4==0)?0:4-(Size%4);
  
  for(cnt=0; cnt<Size ; cnt+=4)
  {
    Val = *(uint32_t*)StartAddress;
    if(missalignementAddress)
    {
      switch (missalignementAddress)
      {
        case 1:
          InitVal += (uint8_t) (Val>>8 & 0xff);
          InitVal += (uint8_t) (Val>>16 & 0xff);
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=1;
          break;
        case 2:
          InitVal += (uint8_t) (Val>>16 & 0xff);
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=2;
          break;
        case 3:   
          InitVal += (uint8_t) (Val>>24 & 0xff);
          missalignementAddress-=3;
          break;
      }  
    }
    else if((Size-missalignementSize)%4 && (Size-cnt) <=4)
    {
      switch (Size-missalignementSize)
      {
        case 1:
          InitVal += (uint8_t) Val;
          InitVal += (uint8_t) (Val>>8 & 0xff);
          InitVal += (uint8_t) (Val>>16 & 0xff);
          missalignementSize-=1;
          break;
        case 2:
          InitVal += (uint8_t) Val;
          InitVal += (uint8_t) (Val>>8 & 0xff);
          missalignementSize-=2;
          break;
        case 3:   
          InitVal += (uint8_t) Val;
          missalignementSize-=3;
          break;
      } 
    }
    else
    {
      InitVal += (uint8_t) Val;
      InitVal += (uint8_t) (Val>>8 & 0xff);
      InitVal += (uint8_t) (Val>>16 & 0xff);
      InitVal += (uint8_t) (Val>>24 & 0xff);
    }
    StartAddress+=4;
  }
  
  return (InitVal);
}


/**
  * Description :
  * Verify flash memory with RAM buffer and calculates checksum value of
  * the programmed memory
  * Inputs    :
  *      FlashAddr     : Flash address
  *      RAMBufferAddr : RAM buffer address
  *      Size          : Size (in WORD)  
  *      InitVal       : Initial CRC value
  * outputs   :
  *     R0             : Operation failed (address of failure)
  *     R1             : Checksum value
  * Note: Optional for all types of device
  */
KeepInCompilation uint64_t Verify (uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement)
{
  uint32_t VerifiedData = 0, InitVal = 0;
  uint64_t checksum;
  Size*=4;

  BSP_OSPI_NOR_EnableMemoryMappedMode(0);
  checksum = CheckSum((uint32_t)MemoryAddr + (missalignement & 0xf), Size - ((missalignement >> 16) & 0xF), InitVal);
  while (Size>VerifiedData)
  {
    if ( *(uint8_t*)MemoryAddr++ != *((uint8_t*)RAMBufferAddr + VerifiedData))
      return ((checksum<<32) + (MemoryAddr + VerifiedData));  
   
    VerifiedData++;  
  }
        
  return (checksum<<32);
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (Cortex-M7 CPU Clock)
  *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static int SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* Enable voltage range 1 for frequency above 100 Mhz */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  __HAL_RCC_PWR_CLK_DISABLE();

  /* MSI Oscillator enabled at reset (4Mhz), activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV1;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLFRACN= 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK 
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2  
                                | RCC_CLOCKTYPE_PCLK3);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  return 1;
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
