  .syntax unified
  .cpu cortex-m4
  .thumb

  .global g_pfnVectors
  .global Reset_Handler

  .extern main
  .extern _estack
  .extern _etext
  .extern _sdata
  .extern _edata
  .extern _sbss
  .extern _ebss

  .section .isr_vector, "a", %progbits
g_pfnVectors:
  .word _estack
  .word Reset_Handler

  .section .text.Reset_Handler, "ax", %progbits
  .thumb_func
Reset_Handler:
  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_etext

CopyData:
  cmp r0, r1
  bcc CopyDataWord
  b ZeroBss

CopyDataWord:
  ldr r3, [r2], #4
  str r3, [r0], #4
  b CopyData

ZeroBss:
  ldr r0, =_sbss
  ldr r1, =_ebss
  movs r2, #0

ZeroBssLoop:
  cmp r0, r1
  bcc ZeroBssWord
  b CallMain

ZeroBssWord:
  str r2, [r0], #4
  b ZeroBssLoop

CallMain:
  bl main
  b .
