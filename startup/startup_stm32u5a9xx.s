/**
  ******************************************************************************
  * @file      startup_stm32u5a9xx.s
  * @author    MCD Application Team
  * @brief     STM32U5A9xx devices vector table GCC toolchain.
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address,
  *                - Configure the clock system
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M33 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *******************************************************************************
  */

  .syntax unified
	.cpu cortex-m33
	.fpu softvfp
	.thumb

.global	g_pfnVectors
.global	Default_Handler
.global	Init

/* start address for the initialization values of the .data section.
defined in linker script */
.word	_sidata
/* start address for the .data section. defined in linker script */
.word	_sdata
/* end address for the .data section. defined in linker script */
.word	_edata
/* start address for the .bss section. defined in linker script */
.word	_sbss
/* end address for the .bss section. defined in linker script */
.word	_ebss

/* Place all code in .text section */
.section .text

/* Reset Handler */
.weak	Reset_Handler
.type	Reset_Handler, %function
Reset_Handler:
	/* 确保处于特权级别 */
	mrs    r0, CONTROL
	bic    r0, r0, #1
	msr    CONTROL, r0
	isb

	/* 设置栈指针 */
	ldr    r0, =_estack
	msr    MSP, r0        /* 使用MSP而不是SP */
	
	/* 禁用所有中断 */
	cpsid  i
	
	/* 设置向量表 */
	ldr    r0, =g_pfnVectors
	ldr    r1, =0xE000ED08
	str    r0, [r1]
	
	/* 确保所有写操作完成 */
	dsb
	isb
	
	/* 跳转到Init */
	ldr    r0, =Init
	blx    r0             /* 使用blx而不是bx */

.size	Reset_Handler, .-Reset_Handler

/* Default Handler */
.weak	Default_Handler
.type	Default_Handler, %function
Default_Handler:
	b  .
	.size	Default_Handler, .-Default_Handler

/* Vector Table */
.section	.isr_vector,"a",%progbits
.type	g_pfnVectors, %object
.size	g_pfnVectors, .-g_pfnVectors

g_pfnVectors:
	.word	_estack
	.word	Reset_Handler
	.word	Default_Handler        /* NMI */
	.word	Default_Handler        /* Hard Fault */
	.word	Default_Handler        /* MemManage */
	.word	Default_Handler        /* BusFault */
	.word	Default_Handler        /* UsageFault */
	.word	0                      /* Reserved */
	.word	0                      /* Reserved */
	.word	0                      /* Reserved */
	.word	0                      /* Reserved */
	.word	Default_Handler        /* SVCall */
	.word	Default_Handler        /* Debug Monitor */
	.word	0                      /* Reserved */
	.word	Default_Handler        /* PendSV */
	.word	Default_Handler        /* SysTick */

	/* External Interrupts */
	.word	Default_Handler        /* Window Watchdog */
	.word	Default_Handler        /* PVD through EXTI Line detect */
	.word	Default_Handler        /* Tamper */
	.word	Default_Handler        /* RTC */
	.word	Default_Handler        /* Flash */
	.word	Default_Handler        /* RCC */
	.word	Default_Handler        /* EXTI Line 0 */
	.word	Default_Handler        /* EXTI Line 1 */
	.word	Default_Handler        /* EXTI Line 2 */
	.word	Default_Handler        /* EXTI Line 3 */
	.word	Default_Handler        /* EXTI Line 4 */

.end
