/**
  ******************************************************************************
  * @file    Dev_Inf.c
  * @author  MCD Application Team
  * @brief   This file defines the structure containing informations about the 
  *          external flash memory MX25LM51245G used by STM32CubeProgramer in 
  *          programming/erasing the device.
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

#include "Dev_Inf.h"

/* This structure containes information used by ST-LINK Utility to program and erase the device */
__attribute__((section(".StorageInfo"))) __attribute__ ((__used__)) 
struct StorageInfo const StorageInfo  =  {
   "STM32U5A9J-DK", 	 					// Device Name + EVAL Borad name
   NOR_FLASH,                   					        // Device Type
   0x90000000,                						        // Device Start Address
   0x4000000,              						        // Device Size in 64 MBytes
   0x100,                    						        // Programming Page Size 4096 Bytes
   0xFF,                       						        // Initial Content of Erased Memory
// Specify Size and Address of Sectors (view example below)
{
 {  0x00000400, 0x00010000},     				 		        // Sector Num : 1024 ,Sector Size: 64 KBytes
 {  0x00000000, 0x00000000},      
}
}; 


