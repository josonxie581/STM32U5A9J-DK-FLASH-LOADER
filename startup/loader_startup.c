/**
  ******************************************************************************
  * @file    loader_startup.c
  * @brief   STM32 外部Flash下载算法启动文件
  *          所有代码在RAM中运行,启动后直接跳转到Init函数
  ******************************************************************************
  */

/* 定义向量表 */
typedef void (*pFunction)(void);

extern unsigned int _estack;     /* 栈顶地址 */
extern void Init(void);          /* Init函数声明 */

/* 函数声明 */
void Reset_Handler(void);
void Default_Handler(void) __attribute__((naked));

/* 向量表定义 */
__attribute__((section(".isr_vector"), used))
void (* const g_pfnVectors[])(void) = {
    (pFunction)&_estack,
    (pFunction)Reset_Handler,
 
};

/* 默认中断处理函数 */
void __attribute__((naked)) Default_Handler(void)
{
    __asm volatile(
        "bkpt #0\n"
        "b .\n"
    );
}

/* 启动代码 */
/* 启动代码 */
void Reset_Handler(void)
{
    /* 设置栈指针和向量表 */
    __asm volatile (
        "ldr r0, =_estack\n"
        "mov sp, r0\n"
        
        /* 设置向量表位置 */
        "ldr r0, =g_pfnVectors\n"
        "ldr r1, =0xE000ED08\n"  /* SCB->VTOR的地址 */
        "str r0, [r1]\n"
        
        /* 使用bl跳转到Init */
        "bl Init\n"
        
        /* 如果返回则进入死循环 */
        "b .\n"
    );
}
 