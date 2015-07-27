;******************************************************************************
; @Module:      FIR
; @Filename:    FirBlk_4_16.asm
; @Project:     DSP Library for Tricore-1 
;------------------------------------------------------------------------------
; @Controller   TriCore-1 V1.2,V1.3
;
; @Compiler:    Tasking 
;
; @Version:     V1.30
;
; @Description: In this file an FIR filter function FirBlk_4_16() has 
;               been implemented 
;
; @See Also:    TriConv.inc,FirBlk_16.asm                 
;                   
; @References:  1. <TriCore Calling Conventions>
;               2. <TriCore Architecture Manual>
;               3. <Infineon Tricore Optimization Guide>        
;
; @Date:        8th April 2004                
;                                             
; @History:     
;               08.12.2003
;               change segments names text -> .text data-> .data
;               08.08.2003
;               Tasking V2.x (Viper) Support
;               Update workarounds (extended with New Errata names June 2003) 
;               ---------------------------------
;               WA compliant with Tasking and TriLib V1.21
;               V1.0 Released on 29th May 2000  
;******************************************************************************

;------------------- Section for Include Files --------------------------------

    .include    "TriConv.inc"

;------------------- Section for External References --------------------------
        
    .global     FirBlk_4_16

;------------------- Section for Code Segment Declaration ---------------------

    .sdecl      ".text.trilib.filter.fir",CODE
    .sect       ".text.trilib.filter.fir"

;------------------------------------------------------------------------------

;******************************************************************************
; void FirBlk_4_16(DataS        *X,         
;                  DataS        *R,                 
;                  cptrDataS    H,
;                  cptrDataS    *DLY,
;                  int          nX
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
;       Optimal implementation, requires filter order to be multiple of 4.
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
;       1)  Loop unrolling, 4 taps/loop.
;       2)  Use of packed data load/store
;       3)  Delay line implemented as circular buffer.
;       4)  Coefficient buffer implemented as circular buffer
;       5)  Use of dual MAC instructions.
;       6)  Intermediate results stored in 64 bit register (16 guard bits).
;       7)  Instruction ordering for zero overhead Load/Store       
;               
; ASSUMPTIONS:
;       1)  Filter order should be multiple of 4 and minimum filter order is 8
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
; CYCLE COUNTS:
;       With DSPEXT     
;           Pre-Kernel : 5                              
;           Kernel : nX * { 5 + [nH/4 - 1] * 2 + 5 } + 3                     
;           Post-Kernel : 1+2               
;
;       Without DSPEXT      
;           Pre-Kernel : 7                              
;           Kernel :  nX * { 5 + [nH/4 - 1] * 2 + 5 } + 3                                        
;           Post-Kernel : 1+2  
;
; CODE SIZE:
;       104 bytes
; 
;******************************************************************************

;------------------- Register Allocation --------------------------------------

    .define     aX          "adArg1"    ;a4 Ptr to Input-Buffer
    .define     aR          "adArg2"    ;a5 Ptr to Output-Buffer

    .if !@DEF('DSPEXT')

    .define     aDLY        "adArg3"    ;a6 Ptr to Ptr to Circ-Ptr of Delay-Buffer
    .define     lBase       "wArg1"     ;d4 Base address - first member of circ 
                                        ;struct. 
    .define     ssSizeIdx   "wArg2"     ;d5 Size|Index - second and third 
                                        ;members of circ struct.
    .define     nX          "wArg3"     ;d6 Input data count
                                
    .define     caH         "a2/a3"     ;Circ-Ptr of Coeff-Buffer
    .define     caeH        "a2"        ;Even-Reg of Circ-Ptr
    .define     caoH        "a3"        ;Odd-Reg of Circ-Ptr

    .else 

    .define     caH         "a6/a7"     ;Circ-Ptr of Coeff-Buffer
    .define     caeH        "a6"        ;Even-Reg of Circ-Ptr
    .define     caoH        "a7"        ;Odd-Reg of Circ-Ptr
    ;if DSPEXT &cptrDLY is read from sp[0] to aDLY 
    .define     aDLY        "a3"        ;Ptr to Circ-Ptr of Delay-Buffer
    .define     nX          "wArg1"     ;d4 Input data count

    .endif

    .define     caDLY       "a12/a13"   ;Circ-Ptr Reg-Pair for Delay-Buffer
    .define     caeDLY      "a12"       ;Even-Reg of Circ-Ptr
    .define     caoDLY      "a13"       ;Odd-Reg of Circ-Ptr

    .define     aTapLoops   "a14"       ;Number of taps
    .define     aTapCnt     "a15"       ;Loop-Cnt-Reg 

    .define     ssssX       "e10"       ;Filter internal state
    .define     sseX        "d10"       ;Even-Reg
    .define     ssoX        "d11"       ;Odd-Reg

    .define     ssssH       "e8"        ;Filter Coeff.
    .define     sseH        "d8"        ;Even-Reg
    .define     ssoH        "d9"        ;Odd-Reg

    .define     llAcc       "d12/d13"   ;Filter result
    .define     leAcc       "d12"       ;Even-Reg
    .define     loAcc       "d13"       ;Odd-Reg

    .define     dTmp        "d14"       ;Generic temporary Data-Reg

;=================== Executable Code ==========================================
    
FirBlk_4_16:
    
    FEnter      0

    add         nX,nX,#-1           ;-1 loop adjust for FirB4_InDataL ||
.if @DEF('DSPEXT')
    ld.a        aDLY,[sp]           ;Load the Ptr to Circ-Ptr of Delay-Buffer
                                    ;to aDLY from [SP]0
.else

    mov.a       caeH,lBase          ;Move base address to even reg. of Circ-Ptr 
                                    ;Reg-Pair for Coeff-Buffer  
                                    
    mov.a       caoH,ssSizeIdx      ;Move base address to odd reg. of Circ-Ptr 
                                    ;Reg-Pair for Coeff-Buffer  
    
.endif

;Get the filter order nH from the size of 
;Delay-Buffer stored in Circ-Ptr caDLY
;Based on nH evaluate the Loop-Cnt value aTapCnt
           
    mov.d       dTmp,caoH           ;get the size|index part of Circ-Ptr 
        
    sh          dTmp,dTmp,#-(16+1+2);>>16 transfer from 16-msb to 16-lsb
                                    ;>>1 DataL= two bytes
                                    ;>>2 4Taps/loop

    add         dTmp,dTmp,#-(1+1)   ;-1 loop adjust 
                                    ;-1 loop unrolling ||
    ld.da       caDLY,[aDLY]        ;Load the Circ-Ptr of Delay-Buffer to 
                                    ;Reg-Pair caDLY 
                                        
    mov.a       aTapLoops,dTmp      ;transfer the count to Loop-Counter

;------------------- Loop for Input Buffer Starts -----------------------------

FirB4_InDataL:      
    
    ld.q        dTmp,[aX+]W16       ;Get the input sample   

    st.q        [caDLY+c]0,dTmp     ;Store input value in Delay-Buffer at 
                                    ;the position of the oldest value

    mov.aa      aTapCnt,aTapLoops   ;Initialize the aTapCnt

    mov         leAcc,#0            ;Clear the Even-Reg of llAcc || 
    ld.w        ssoH,[caH+c]2*W16   ;ssoH <- H(0),H(1) 

    mov         loAcc,#0            ;Clear the Odd-Reg of llAcc ||
    ld.d        ssssX,[caDLY+c]4*W16;ssssX <- X(n),X(n-1),X(n-2),X(n-3)                             

;------------------- Kernel ---------------------------------------------------

;The index i,j of X(i),H(j) (in the comments)are valid for first loop iteration
;For each next loop i,j should be decremented and incremented by 4 resp.
;'n' refers to current instant

    .if TRWA_CPU_TC113_CPU5
    isync
    .endif
    
FirB4_TapL:
                                    
    maddm.h     llAcc,llAcc,sseX,ssoH ul,#1
                                    ;llAcc += X(n)*H(0) + X(n-1)*H(1) ||
    ld.d        ssssH,[caH+c]4*W16  ;ssssH <- H(2),H(3),H(4),H(5)

    maddm.h     llAcc,llAcc,ssoX,sseH ul,#1
                                    ;llAcc += X(n-2)*H(2) + X(n-3)*H(3) ||
    ld.d        ssssX,[caDLY+c]4*W16;ssssX <- X(n-4),X(n-5),X(n-6),X(n-7) 

    .if TRWA_CPU_TC018
    isync
    .endif

    loop        aTapCnt,FirB4_TapL  
    
;------------------- Kernel Ends ----------------------------------------------


    maddm.h     llAcc,llAcc,sseX,ssoH ul,#1
                                    ;llAcc += X(n-nH+4)*H(nH-4) + 
                                    ;         X(n-nH+3)*H(nH-3) ||
    ld.w        sseH,[caH+c]2*W16   ;sseH <- H(nH-2),H(nH-1)    

    maddm.h     llAcc,llAcc,ssoX,sseH ul,#1
                                    ;llAcc += X(n-nH+2)*H(nH-2) +
                                    ;         X(n-nH+1)*H(nH-1) ||
    ld.q        dTmp,[caDLY+c]-W16  ;Dummy load to set caDLY.index to the   
                                    ;oldest value in Delay-Buffer
                                                                            
    shas        loAcc,loAcc,#16     ;Format the filter output to 16-bit 
                                    ;saturated value ||
    st.q        [aR+]W16,loAcc      ;Store result in the Output-Buffer  

    jned        nX,#0,FirB4_InDataL ;Loop executed number of data times

;------------------- Loop for Input Buffer Ends -------------------------------
    
    st.da       [aDLY],caDLY        ;Store the updated caDLY to memory

;------------------------------------------------------------------------------

    FReturn

;------------------- Undefine the Registers -----------------------------------

    .undef      aX
    .undef      aR
    .undef      nX  
    .if !@DEF('DSPEXT')
    .undef      lBase       
    .undef      ssSizeIdx   
    .endif
    .undef      caH 
    .undef      caeH
    .undef      caoH       
    .undef      aDLY
    .undef      caDLY       
    .undef      caeDLY
    .undef      caoDLY         
    .undef      aTapLoops          
    .undef      aTapCnt 
    .undef      ssssX          
    .undef      sseX           
    .undef      ssoX           
    .undef      ssssH          
    .undef      sseH           
    .undef      ssoH                  
    .undef      llAcc          
    .undef      leAcc          
    .undef      loAcc          
    .undef      dTmp      
                           
;------------------- END OF FILE ----------------------------------------------