;****************************************************************************** 
; @Module:      Include file 
; @Filename:    TriConv.inc
; @Project:     DSP library for TriCore-1
;------------------------------------------------------------------------------
; @Controller:  TriCore V1.2, V1.3
;
; @Compiler:    Tasking
;
; @Version:     V1.30
;
; @Description: This file includes macros defining the C calling conventions 
;               and the system  level constants. This file should be included 
;               in all the assembly source code files. It works as the
;               header file.
;
; @See Also:    None
;              
; @References:  1. <TriCore Embedded Applications Binary Interface>
;               2. <TriCore Architecture Manual>
;               3. <Infineon Tricore Optimization Guide>        
;
; @Date         8th April 2004
;
; @History:     
;               08.08.2003
;               Tasking V2.x (Viper) Support
;               Update workarounds (extended with New Errata names June 2003) 
;               ---------------------------------
;               V1.0 Released on 29th May 2000       
;
;******************************************************************************

;------------------- Universal Definitions-------------------------------------
    

    FALSE       .equ    0
    TRUE        .equ    2
    SIZE_SHORT  .equ    2
    SIZE_SFRACT .equ    2
    SIZE_CMPLX  .equ    4
    SIZE_CMPLX_32   .equ    8
    MAX_W16     .equ    0X7FFF
    MIN_W16     .equ    0x8000


;------------------- Program Status Bit Definition-----------------------------
    
    C       .equ    31
    V       .equ    30
    SV      .equ    29
    AV      .equ    28
    SAV     .equ    27
    IS      .equ    9
    GW      .equ    8
    CDE     .equ    7

;------------------- Constant Definitions for pointer offset-------------------         

    .define     W16     "2"             ;Size of halfword in bytes
    .define     W32     "4"             ;Size of word in bytes
    .define     W64     "8"             ;Size of double word in bytes

;------------------- Data Passing Convention to a C Function ------------------
    
    .define     wArg1   'D4'            ;First word argument 
    .define     wArg2   'D5'            ;Second word argument
    .define     wArg3   'D6'            ;Third word argument
    .define     wArg4   'D7'            ;Fourth word argument
    .define     dwArg1  'E4'            ;First double word argument
    .define     dwArg2  'E6'            ;Second double word argument

;------------------- Pointer Passing Convention to a C Function ---------------
    
    .define     adArg1  'A4'            ;First pointer argument to 32 bit data
    .define     adArg2  'A5'            ;Second pointer argument to 32 bit data 
    .define     adArg3  'A6'            ;Third pointer argument to 32 bit data
    .define     adArg4  'A7'            ;Fourth pointer argument to 32 bit data
    .define     dadArg1 'A4/A5'         ;First pointer argument to 64 bit data
    .define     dadArg2 'A6/A7'         ;Second pointer argument to 64 bit data

;------------------- Return Data Convention of a C Function -------------------
    
    .define     wRtn    'D2'            ;D2 has returned 32 bit data type
    .define     dwRtn   'E2'            ;E2 has returned 64 bit data type

;------------------- Return Pointer Convention of a C Function ----------------
    
    .define     adRtn   'A2'            ;A2 stores the returned pointer to a 
                                        ;32 bit data type.
    .define     dadRtn  'A2/A3'         ;A2/A3 stores the returned pointer to a
                                        ;64 bit data type.


;------------------------------------------------------------------------------
;       Silicon Workarounds Definitions
;------------------------------------------------------------------------------


    ;---------------------------------------
    ; ERRATA: CPU_TC.018
    ; Insert isync before LOOP instruction
    ;---------------------------------------
    .if @DEF('__ASTC__')
    TRWA_CPU_TC018 .equ @DEF('__CPU_TC018__') || @DEF('__TC112_COR1__') || @DEF('COR1')
    .else
    TRWA_CPU_TC018 .equ  @DEF('_CPU_TC018') || @DEF('_TC112_COR1') || @DEF('COR1')
    .endif

    ;---------------------------------------
    ; ERRATA: CPU_TC.034
    ; DSYNC cause corruption of up to two
    ; following instructions
    ;---------------------------------------
    .if @DEF('__ASTC__')
    TRWA_CPU_TC034 .equ @DEF('__CPU_TC034__') || @DEF('__TC113_CPU9__')
    .else
    TRWA_CPU_TC034 .equ @DEF('_CPU_TC034') || @DEF('_TC113_CPU9')
    .endif

    ;---------------------------------------
    ; ERRATA: CPU_TC.027
    ; Avoid using store instruction that use
    ; a circular addressing with an offset 
    ; value not equal to zero
    ;---------------------------------------
    .if @DEF('__ASTC__')
    TRWA_CPU_TC027 .equ @DEF('__CPU_TC027__') || @DEF('__TC112_COR10__')
    .else
    TRWA_CPU_TC027 .equ @DEF('_CPU_TC027') || @DEF('_TC112_COR10')
    .endif

    ;---------------------------------------
    ; ERRATA: CPU_TC.031
    ; Protect divide instruction against 
    ; interrupts
    ; Save interrupt state and disable 
    ; interrupts
    ; Finally restore the interrupt state
    ;---------------------------------------
    .if @DEF('__ASTC__')
    TRWA_CPU_TC031 .equ @DEF('__CPU_TC031__') || @DEF('__TC112_COR14__') || @DEF('COR14')
    .else
    TRWA_CPU_TC031 .equ @DEF('_CPU_TC031') || @DEF('_TC112_COR14') || @DEF('COR14')
    .endif

    ;---------------------------------------
    ; ERRATA: CPU_TC.050
    ; A load instruction following a multicycle 
    ; integer instruction can get lost
    ; Solution: NOP after multicycle integer
    ; indtruction 
    ;---------------------------------------
    .if @DEF('__ASTC__')
    TRWA_CPU_TC050 .equ @DEF('__CPU_TC050__')
    .else
    TRWA_CPU_TC050 .equ @DEF('_CPU_TC050')
    .endif



    ;--- NOT IN NEW NAMING CONVENTION
    ;---------------------------------------
    ; ERRATA: - (No new number)
    ; Insert 'isync' instruction before a loop body         
    ;---------------------------------------
    .if @DEF('__ASTC__')
    TRWA_CPU_TC113_CPU5 .equ @DEF('__TC113_CPU5__') || @DEF('CPU5')
    .else
    TRWA_CPU_TC113_CPU5 .equ @DEF('_TC113_CPU5') || @DEF('CPU5')
    .endif

    ;---------------------------------------
    ; ERRATA: - (No new number)
    ; DSYNC should be located at a very top
    ; of each Sub-Routine           
    ;---------------------------------------
    .if @DEF('__ASTC__')
    TRWA_CPU_TC113_CPU14 .equ @DEF('__TC113_CPU14__')
    .else
    TRWA_CPU_TC113_CPU14 .equ @DEF('_TC113_CPU14')
    .endif


;------------------- System Level Macros---------------------------------------

;------------------------------------------------------------------------------
; Macro:        FEnter
;
; DESCRIPTION:  Setup a local frame for C functions.
;               Space indicates the stack space reserved.
;
;------------------------------------------------------------------------------

FEnter          .macro  Space
                .if TRWA_CPU_TC113_CPU14
                dsync;
                .if TRWA_CPU_TC034
                nop;
                nop;
                .endif
                .endif

                .if Space != 0
                    sub.a   sp,#Space
                .endif
                .endm

;------------------------------------------------------------------------------
; Macro:        FReturn
;
; DESCRIPTION:  Return from a C function. 
;
;------------------------------------------------------------------------------

FReturn         .macro
                    ret
                .endm

;------------------------------------------------------------------------------
; Macro:        ClearRpair
;
; DESCRIPTION:  It clears register pair to zero
;
;------------------------------------------------------------------------------

ClearRpair      .macro  Rpair
                    mul Rpair,D0,#0
                    .if TRWA_CPU_TC050
                    nop
                    .endif
                .endm

;------------------------------------------------------------------------------
;Macro:         CONST.A
;
;Description:   Load 32bit constants into address registers
;
;------------------------------------------------------------------------------
CONST.A         .macro  ax,v
                    movh.a  ax,#@his(v)   ; high part with correction for signed addition
                    lea ax,[ax]@los(v)
                .endm

;------------------- END OF FILE ----------------------------------------------
        