;*****************************************************************************
;
; Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
;
;                                 IMPORTANT NOTICE
;
; Infineon Technologies AG (Infineon) is supplying this file for use
; exclusively with Infineon's microcontroller products. This file can be freely
; distributed within development tools that are supporting such microcontroller
; products.
;
; THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
; OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;
;*****************************************************************************
; File: Ifx_firRealQ15a.asm
;
; Compiler: ALTIUM TASKING
;
; $Revision: 131 $
;
; $Date: 2014-03-31 13:38:11 +0200 (Mo, 31 Mrz 2014) $
;
;******************************************************************************
; void Ifx_firRealQ15a(sint16        *X,
;                      sint16        *R,
;                      CircBuf       H,
;                      CircBuf       *DLY,
;                      uint32        nX
;                 );
;
; INPUTS:   
;       X       Ptr to Input-Buffer
;       R       Ptr to Output-Buffer
;       H       Circ-Ptr of Coeff-Buffer of size nH
;       DLY     With DSPEXT - Ptr to Circ-Ptr of Delay-Buffer of size nH
;               Without DSPEXT - Ptr to Circ Struct.
;       (nH)    Transfered as a part of Circ-Ptr data type in a DLY parameter
;               nH = 8,12,16,...
;       nX      Size of Input_Buffer
;
; OUTPUTS: 
;       DLY     Updated Circ-Ptr with index set to the oldest value of the 
;               filter Delay-Buffer
;       R(nX)   Output-Buffer
;
; RETURN:   
;       None
; 
; DESCRIPTION: 
;       FIR filter transversal structure(direct form),
;       Input block processing,
;       16-bit fractional input, coefficients and output data format,
;       Optimal implementation, requires filter order to be multiple of 8.
;
; ALGORITHM:        
;       An FIR filter with filter order nH can be represented by following 
;       mathematical equation       
;
;       R(n) = X(n) * H(0) + X(n-1) * H(1) + ..+ X(n-nH+2) * H(nH-2) + 
;              X(n-nH+1) * H(nH-1)  
;
;       where, 
;       R(n)                : Output sample of the filter at index n
;       X(n)                : Input sample of the filter at index n
;       H(0),H(1),H(2),..   : Filter coefficients
;       nH                  : Filter order (number of coefficients)  
;               
; TECHNIQUES:
;       1)  Loop unrolling, 8 taps/loop.
;       2)  Use of packed data load/store
;       3)  Delay line implemented as circular buffer.
;       4)  Coefficient buffer implemented as circular buffer
;       5)  Use of dual MAC instructions.
;       6)  Intermediate results stored in 64 bit register (16 guard bits).
;       7)  Instruction ordering for zero overhead Load/Store       
;               
; ASSUMPTIONS:
;       1)  Filter order should be multiple of 8 and minimum filter order is 16
;       2)  Inputs, outputs, coefficients should be in 1Q15 format. 
;       3)  Filter order is not explicitely sent as an argument instead it is 
;           sent through the argument DLY as a size of Circ-Delay-Buffer
;       4)  Delay-Buffer should be in internal memory
;
; MEMORY NOTE:
;       ______________________________________________________________________
;       
;       Pointer     Pointer      Variable                      Alignment
;                    Type                                IntMem       ExtMem
;       ______________________________________________________________________
;   
;         H         circular     H(0), H(1), H(2),...    doubleword  doubleword 
;                                H(nH-1)                                
;       ______________________________________________________________________          
;
;        DLY        Circular     X(n-1), X(n-2), X(n-3), doubleword   Not 
;                                .....,X(n-nH)                        Allowed    
;       ______________________________________________________________________
;           
;          X        Linear       X(n), X(n+1), X(n+2),..  halfword   halfword
;       ______________________________________________________________________ 
;       
;          R        Linear       R(n), R(n+1), R(n+2),..  halfword   halfword
;       ______________________________________________________________________
;
;
; REGISTER USAGE:
;       a2, a3, a4, a5, a6, a7, a12, a13, a14, a15.
;       d4, d5, d6, d8, d9, d10, d11, d12, d13, d14 
;
; 
;******************************************************************************

;------------------- Section for External References --------------------------

    .global     Ifx_firRealQ15a

;------------------- Section for Code Segment Declaration ---------------------

    .sdecl      ".text.cpu1_psram",CODE
    .sect       ".text.cpu1_psram"

;------------------- Register Allocation --------------------------------------
    .define     W16         "2"         ;Size of halfword in bytes

    .define     aX          "a4"        ;adArg1-a4 Ptr to Input-Buffer
    .define     aR          "a5"        ;adArg2-a5 Ptr to Output-Buffer

    .define     aDLY        "a6"        ;adArg3-a6 Ptr to Ptr to Circ-Ptr of Delay-Buffer
    .define     lBase       "d4"        ;wArg1-d4 Base address - first member of circ
                                        ;struct. 
    .define     ssSizeIdx   "d5"        ;wArg2-d5 Size|Index - second and third
                                        ;members of circ struct.
    .define     nX          "d6"        ;wArg3-d6 Input data count
                                
    .define     caH         "a2/a3"     ;Circ-Ptr of Coeff-Buffer
    .define     caeH        "a2"        ;Even-Reg of Circ-Ptr
    .define     caoH        "a3"        ;Odd-Reg of Circ-Ptr
 
    .define     caDLY       "a12/a13"   ;Circ-Ptr Reg-Pair for Delay-Buffer
    .define     caeDLY      "a12"       ;Even-Reg of Circ-Ptr
    .define     caoDLY      "a13"       ;Odd-Reg of Circ-Ptr

    .define     aTapCnt     "a15"       ;Loop-Cnt-Reg 

    .define     ssssX0       "e10"       ;Filter internal state
    .define     sseX0        "d10"       ;Even-Reg
    .define     ssoX0        "d11"       ;Odd-Reg

    .define     ssssH0       "e8"        ;Filter Coeff.
    .define     sseH0        "d8"        ;Even-Reg
    .define     ssoH0        "d9"        ;Odd-Reg

    .define     ssssX1       "e2"       ;Filter internal state
    .define     sseX1        "d2"       ;Even-Reg
    .define     ssoX1        "d3"       ;Odd-Reg

    .define     ssssH1       "e0"        ;Filter Coeff.
    .define     sseH1        "d0"        ;Even-Reg
    .define     ssoH1        "d1"        ;Odd-Reg

    .define     llAcc       "d12/d13"   ;Filter result
    .define     leAcc       "d12"       ;Even-Reg
    .define     loAcc       "d13"       ;Odd-Reg

    .define     dTapLoops   "d14"       ;Number of taps
    .define     dTmp        "d15"       ;Generic temporary Data-Reg

;=================== Executable Code ==========================================
    
    .align 8
Ifx_firRealQ15a:
    
    add         nX,nX,#-1           ;-1 loop adjust for FirB4_InDataL ||

    mov.a       caeH,lBase          ;Move base address to even reg. of Circ-Ptr 
                                    ;Reg-Pair for Coeff-Buffer  
                                    
    mov.a       caoH,ssSizeIdx      ;Move base address to odd reg. of Circ-Ptr 

;Get the filter order nH from the size of 
;Delay-Buffer stored in Circ-Ptr caDLY
;Based on nH evaluate the Loop-Cnt value aTapCnt
           
    mov.d       dTmp,caoH           ;get the size|index part of Circ-Ptr 
        
    sh          dTapLoops, dTmp,#-(16+1+3);>>16 transfer from 16-msb to 16-lsb
                                    ;>>1 DataL= two bytes
                                    ;>>3 8Taps/loop

    add         dTapLoops, dTapLoops, #-(1+1)   ;-1 loop adjust 
                                    ;-1 loop unrolling ||
    ld.da       caDLY,[aDLY]        ;Load the Circ-Ptr of Delay-Buffer to 
                                    ;Reg-Pair caDLY 
                                        
    ld.q    dTmp, [aX+]W16       ;Get the input sample   
                                 ;the position of the oldest value
    mov.a  aTapCnt, dTapLoops   ;Initialize the aTapCnt
    ld.w    ssoH0, [caH+c]2*W16   ;ssoH <- H(0),H(1)
    
;------------------- Loop for Input Buffer Starts -----------------------------

FirB4_InDataL:      
    st.q    [caDLY+c]0, dTmp     ;Store input value in Delay-Buffer at 

    mov     leAcc, #0            ;Clear the Even-Reg of llAcc || 
    ld.d    ssssX0, [caDLY+c]4*W16;ssssX <- X(n),X(n-1),X(n-2),X(n-3)

    mov     loAcc, #0            ;Clear the Odd-Reg of llAcc
    ld.d    ssssH1, [caH+c]4*W16   ;ssssH <- H(2),H(3),H(4),H(5)

;------------------- Kernel ---------------------------------------------------

;The index i,j of X(i),H(j) (in the comments)are valid for first loop iteration
;For each next loop i,j should be decremented and incremented by 4 resp.
;'n' refers to current instant

FirB4_TapL:
    maddm.h llAcc,llAcc,sseX0,ssoH0 ul,#1 ;llAcc += X(n)*H(0) + X(n-1)*H(1)
    ld.d    ssssX1, [caDLY+c]4*W16 ;ssssX <- X(n-4),X(n-5),X(n-6),X(n-7) 

    maddm.h llAcc,llAcc,ssoX0,sseH1 ul,#1 ;llAcc += X(n-2)*H(2) + X(n-3)*H(3)
    ld.d    ssssH0, [caH+c]4*W16   ;ssssH <- H(2),H(3),H(4),H(5)

    maddm.h llAcc,llAcc,sseX1,ssoH1 ul,#1 ;llAcc += X(n)*H(0) + X(n-1)*H(1)
    ld.d    ssssX0, [caDLY+c]4*W16 ;ssssX <- X(n-4),X(n-5),X(n-6),X(n-7) 

    maddm.h llAcc,llAcc,ssoX1,sseH0 ul,#1 ;llAcc += X(n-2)*H(2) + X(n-3)*H(3)
    ld.d    ssssH1, [caH+c]4*W16   ;ssssH <- H(2),H(3),H(4),H(5)
    loop    aTapCnt, FirB4_TapL  
    
;------------------- Kernel Ends ----------------------------------------------

    maddm.h llAcc,llAcc,sseX0,ssoH0 ul,#1 ;llAcc += X(n)*H(0) + X(n-1)*H(1)
    ld.d    ssssX1, [caDLY+c]3*W16 ;ssssX <- X(n-4),X(n-5),X(n-6),X(n-7) 
                       ;and set caDLY.index to the oldest value in Delay-Buffer

    maddm.h llAcc,llAcc,ssoX0,sseH1 ul,#1 ;llAcc += X(n-2)*H(2) + X(n-3)*H(3)
    ld.w    sseH0, [caH+c]2*W16   ;sseH <- H(nH-2),H(nH-1)    

    maddm.h llAcc,llAcc,sseX1,ssoH1 ul,#1
                                    ;llAcc += X(n-nH+4)*H(nH-4) + 
                                    ;         X(n-nH+3)*H(nH-3) ||
    ld.w    ssoH0, [caH+c]2*W16   ;ssoH <- H(0),H(1)
    maddm.h llAcc,llAcc,ssoX1,sseH0 ul,#1
                                    ;llAcc += X(n-nH+2)*H(nH-2) +
                                    ;         X(n-nH+1)*H(nH-1) ||
    ld.q    dTmp, [aX+]W16       ;Get the input sample   
    mov.a  aTapCnt, dTapLoops   ;Initialize the aTapCnt
                                                                            
    shas    loAcc, loAcc, #16     ;Format the filter output to 16-bit 
                                    ;saturated value ||
    st.q    [aR+]W16, loAcc      ;Store result in the Output-Buffer  

    jned    nX, #0, FirB4_InDataL ;Loop executed number of data times

;------------------- Loop for Input Buffer Ends -------------------------------
    
    st.da       [aDLY],caDLY        ;Store the updated caDLY to memory

;------------------------------------------------------------------------------

    ret

;------------------- END OF FILE ----------------------------------------------
