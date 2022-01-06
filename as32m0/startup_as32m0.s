;/**************************************************************************//**
; * @file     startup_ARMCM4.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM4 Device Series
; * @version  V1.08
; * @date     01. June 2016
; *
; * @note
; * Copyright (C) 2012 ARM Limited. All rights reserved.
; *
; * @par
; * ARM Limited (ARM) is supplying this software for use with Cortex-M 
; * processor based microcontrollers.  This file can be freely distributed 
; * within development tools that are supporting such ARM based processors. 
; *
; * @par
; * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
; *
; ******************************************************************************/
;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x0000800               ; origanal 400h

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     n00_WDG_IRQHandler
                DCD     n01_DMA_IRQHandler
                DCD     n02_ADC_IRQHandler
                DCD     n03_LVD_IRQHandler
                DCD     n04_PWM0_IRQHandler
                DCD     n05_PWM1_IRQHandler
                DCD     n06_PWM2_IRQHandler
                DCD     n07_PWM3_IRQHandler
                DCD     n08_UART0_IRQHandler
                DCD     n09_UART1_IRQHandler
                DCD     n10_TMR0_IRQHandler
                DCD     n11_TMR1_IRQHandler
                DCD     n12_TMR2_IRQHandler
                DCD     n13_I2C_IRQHandler
                DCD     n14_SPI_IRQHandler
                DCD     n15_GIO_IRQHandler
 				DCD     n16_flash_IRQHandler
                
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
;@                LDR     R2,=0x400600A0              ; Trim register
;@                LDR     R3,=0x0000012D              ; 24MHz
;                LDR     R3,=0x00000111              ; 16MHz
;@                STR     R3,[R2]                     ; soft trim 24MHz
		IMPORT  SystemInit
		BL    	SystemInit
                IMPORT  __main
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT     n00_WDG_IRQHandler    [WEAK]
                EXPORT     n01_DMA_IRQHandler    [WEAK]
                EXPORT     n02_ADC_IRQHandler    [WEAK]
                EXPORT     n03_LVD_IRQHandler    [WEAK]
                EXPORT     n04_PWM0_IRQHandler   [WEAK]
                EXPORT     n05_PWM1_IRQHandler   [WEAK]
                EXPORT     n06_PWM2_IRQHandler   [WEAK]
                EXPORT     n07_PWM3_IRQHandler   [WEAK]
                EXPORT     n08_UART0_IRQHandler  [WEAK]
                EXPORT     n09_UART1_IRQHandler  [WEAK]
                EXPORT     n10_TMR0_IRQHandler   [WEAK]
                EXPORT     n11_TMR1_IRQHandler   [WEAK]
                EXPORT     n12_TMR2_IRQHandler   [WEAK]
                EXPORT     n13_I2C_IRQHandler    [WEAK]
                EXPORT     n14_SPI_IRQHandler    [WEAK]
                EXPORT     n15_GIO_IRQHandler    [WEAK]
                EXPORT     n16_flash_IRQHandler  [WEAK]

n00_WDG_IRQHandler    
n01_DMA_IRQHandler    
n02_ADC_IRQHandler    
n03_LVD_IRQHandler    
n04_PWM0_IRQHandler   
n05_PWM1_IRQHandler   
n06_PWM2_IRQHandler   
n07_PWM3_IRQHandler   
n08_UART0_IRQHandler  
n09_UART1_IRQHandler  
n10_TMR0_IRQHandler   
n11_TMR1_IRQHandler   
n12_TMR2_IRQHandler   
n13_I2C_IRQHandler    
n14_SPI_IRQHandler    
n15_GIO_IRQHandler    
n16_flash_IRQHandler  
          
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
                
__user_initial_stackheap
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP
                
                ALIGN

                ENDIF


                END
