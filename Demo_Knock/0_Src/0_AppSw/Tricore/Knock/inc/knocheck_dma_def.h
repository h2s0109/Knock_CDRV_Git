/*
 * knocheck_dma_def.h
 *
 *  Created on: 2015. 3. 2.
 *      Author: hansteve
 */

#ifndef KNOCHECK_DMA_DEF_H_
#define KNOCHECK_DMA_DEF_H_

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "IfxDma_cfg.h"
#include "IfxDma_bf.h"
#include "IfxDma_reg.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Convert local DSPR address to global DSPR address which can be accessed from the SRI bus.
 * Use this macro to convert a local DSPR address (in segment 0xd00.....) to
 * a global DSPR address (in segment 0x700....., 0x600....., 0x500..... downwards) depending on
 * the CPU number.
 * Example usage:
 *  \code
 *     dmaChConfig.sourceAddress      = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &sourceBuffer[i][0]);
 *     dmaChConfig.destinationAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), &destinationBuffer[i][0]);
 *  \endcode
 */
#define CPU_GLB_ADDR_DSPR(cpu, address) ((((((unsigned)(address) & 0xF0000000) == 0xD0000000) ? ((((unsigned)(address) & 0x000fffff) | 0x70000000) - ((cpu) * 0x10000000)) : (unsigned)(address))))
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup Lld_Dma_Std_Enum
 * \{ */
/** \brief Bus Master Mode definition
 * Definition in _DMA.MODE[4].B.MODE
 */
typedef enum
{
    Dma_BusMasterMode_user       = 0, /**< \brief Selected hardware resource performs Bus access in user mode */
    Dma_BusMasterMode_supervisor = 1  /**< \brief Selected hardware resource performs Bus access in supervisor mode */
} Dma_BusMasterMode;

/** \brief Channel Bus Priority definition
 */
typedef enum
{
    Dma_ChannelBusPriority_low    = 0,  /**< \brief low priority */
    Dma_ChannelBusPriority_medium = 1,  /**< \brief medium priority */
    Dma_ChannelBusPriority_high   = 2   /**< \brief high priority */
} Dma_ChannelBusPriority;

/** \brief DMA channel resources definition
 */
typedef enum
{
    Dma_ChannelId_none = -1,  /**< \brief None of the _DMA Channels */
    Dma_ChannelId_0    = 0,   /**< \brief _DMA Channel 0  */
    Dma_ChannelId_1,          /**< \brief _DMA Channel 1  */
    Dma_ChannelId_2,          /**< \brief _DMA Channel 2  */
    Dma_ChannelId_3,          /**< \brief _DMA Channel 3  */
    Dma_ChannelId_4,          /**< \brief _DMA Channel 4  */
    Dma_ChannelId_5,          /**< \brief _DMA Channel 5  */
    Dma_ChannelId_6,          /**< \brief _DMA Channel 6  */
    Dma_ChannelId_7,          /**< \brief _DMA Channel 7  */
    Dma_ChannelId_8,          /**< \brief _DMA Channel 8  */
    Dma_ChannelId_9,          /**< \brief _DMA Channel 9  */
    Dma_ChannelId_10,         /**< \brief _DMA Channel 10  */
    Dma_ChannelId_11,         /**< \brief _DMA Channel 11  */
    Dma_ChannelId_12,         /**< \brief _DMA Channel 12  */
    Dma_ChannelId_13,         /**< \brief _DMA Channel 13  */
    Dma_ChannelId_14,         /**< \brief _DMA Channel 14  */
    Dma_ChannelId_15,         /**< \brief _DMA Channel 15  */
    Dma_ChannelId_16,         /**< \brief _DMA Channel 16  */
    Dma_ChannelId_17,         /**< \brief _DMA Channel 17  */
    Dma_ChannelId_18,         /**< \brief _DMA Channel 18  */
    Dma_ChannelId_19,         /**< \brief _DMA Channel 19  */
    Dma_ChannelId_20,         /**< \brief _DMA Channel 20  */
    Dma_ChannelId_21,         /**< \brief _DMA Channel 21  */
    Dma_ChannelId_22,         /**< \brief _DMA Channel 22  */
    Dma_ChannelId_23,         /**< \brief _DMA Channel 23  */
    Dma_ChannelId_24,         /**< \brief _DMA Channel 24  */
    Dma_ChannelId_25,         /**< \brief _DMA Channel 25  */
    Dma_ChannelId_26,         /**< \brief _DMA Channel 26  */
    Dma_ChannelId_27,         /**< \brief _DMA Channel 27  */
    Dma_ChannelId_28,         /**< \brief _DMA Channel 28  */
    Dma_ChannelId_29,         /**< \brief _DMA Channel 29  */
    Dma_ChannelId_30,         /**< \brief _DMA Channel 30  */
    Dma_ChannelId_31,         /**< \brief _DMA Channel 31  */
    Dma_ChannelId_32,         /**< \brief _DMA Channel 32  */
    Dma_ChannelId_33,         /**< \brief _DMA Channel 33  */
    Dma_ChannelId_34,         /**< \brief _DMA Channel 34  */
    Dma_ChannelId_35,         /**< \brief _DMA Channel 35  */
    Dma_ChannelId_36,         /**< \brief _DMA Channel 36  */
    Dma_ChannelId_37,         /**< \brief _DMA Channel 37  */
    Dma_ChannelId_38,         /**< \brief _DMA Channel 38  */
    Dma_ChannelId_39,         /**< \brief _DMA Channel 39  */
    Dma_ChannelId_40,         /**< \brief _DMA Channel 40  */
    Dma_ChannelId_41,         /**< \brief _DMA Channel 41  */
    Dma_ChannelId_42,         /**< \brief _DMA Channel 42  */
    Dma_ChannelId_43,         /**< \brief _DMA Channel 43  */
    Dma_ChannelId_44,         /**< \brief _DMA Channel 44  */
    Dma_ChannelId_45,         /**< \brief _DMA Channel 45  */
    Dma_ChannelId_46,         /**< \brief _DMA Channel 46  */
    Dma_ChannelId_47,         /**< \brief _DMA Channel 47  */
    Dma_ChannelId_48,         /**< \brief _DMA Channel 48  */
    Dma_ChannelId_49,         /**< \brief _DMA Channel 49  */
    Dma_ChannelId_50,         /**< \brief _DMA Channel 50  */
    Dma_ChannelId_51,         /**< \brief _DMA Channel 51  */
    Dma_ChannelId_52,         /**< \brief _DMA Channel 52  */
    Dma_ChannelId_53,         /**< \brief _DMA Channel 53  */
    Dma_ChannelId_54,         /**< \brief _DMA Channel 54  */
    Dma_ChannelId_55,         /**< \brief _DMA Channel 55  */
    Dma_ChannelId_56,         /**< \brief _DMA Channel 56  */
    Dma_ChannelId_57,         /**< \brief _DMA Channel 57  */
    Dma_ChannelId_58,         /**< \brief _DMA Channel 58  */
    Dma_ChannelId_59,         /**< \brief _DMA Channel 59  */
    Dma_ChannelId_60,         /**< \brief _DMA Channel 60  */
    Dma_ChannelId_61,         /**< \brief _DMA Channel 61  */
    Dma_ChannelId_62,         /**< \brief _DMA Channel 62  */
    Dma_ChannelId_63,         /**< \brief _DMA Channel 63  */
} Dma_ChannelId;

/** \brief DMA circular buffer (wrap around) definition
 * Definition in _DMA.CH[64].ADICR.B.CBLS
 */
typedef enum
{
    Dma_ChannelIncrementCircular_none  = 0,   /**< \brief no circular buffer operation */
    Dma_ChannelIncrementCircular_1     = 0,   /**< \brief circular buffer size is 1 byte */
    Dma_ChannelIncrementCircular_2     = 1,   /**< \brief circular buffer size is 2 byte */
    Dma_ChannelIncrementCircular_4     = 2,   /**< \brief circular buffer size is 4 byte */
    Dma_ChannelIncrementCircular_8     = 3,   /**< \brief circular buffer size is 8 byte */
    Dma_ChannelIncrementCircular_16    = 4,   /**< \brief circular buffer size is 16 byte */
    Dma_ChannelIncrementCircular_32    = 5,   /**< \brief circular buffer size is 32 byte */
    Dma_ChannelIncrementCircular_64    = 6,   /**< \brief circular buffer size is 64 byte */
    Dma_ChannelIncrementCircular_128   = 7,   /**< \brief circular buffer size is 128 byte */
    Dma_ChannelIncrementCircular_256   = 8,   /**< \brief circular buffer size is 256 byte */
    Dma_ChannelIncrementCircular_512   = 9,   /**< \brief circular buffer size is 512 byte */
    Dma_ChannelIncrementCircular_1024  = 10,  /**< \brief circular buffer size is 1024 byte */
    Dma_ChannelIncrementCircular_2048  = 11,  /**< \brief circular buffer size is 2048 byte */
    Dma_ChannelIncrementCircular_4096  = 12,  /**< \brief circular buffer size is 4096 byte */
    Dma_ChannelIncrementCircular_8192  = 13,  /**< \brief circular buffer size is 8192 byte */
    Dma_ChannelIncrementCircular_16384 = 14,  /**< \brief circular buffer size is 16384 byte */
    Dma_ChannelIncrementCircular_32768 = 15   /**< \brief circular buffer size is 32768 byte */
} Dma_ChannelIncrementCircular;

/** \brief DMA incrementation direction definition
 * Definition in _DMA.CH[64].ADICR.B.INCS
 */
typedef enum
{
    Dma_ChannelIncrementDirection_negative = 0,  /**< \brief pointer is decremented */
    Dma_ChannelIncrementDirection_positive = 1   /**< \brief pointer is incremented */
} Dma_ChannelIncrementDirection;

/** \brief DMA incrementation definition
 * Definition in _DMA.CH[64].ADICR.B.SMF
 */
typedef enum
{
    Dma_ChannelIncrementStep_1   = 0, /**< \brief increment by 1 width */
    Dma_ChannelIncrementStep_2   = 1, /**< \brief increment by 2 width */
    Dma_ChannelIncrementStep_4   = 2, /**< \brief increment by 4 width */
    Dma_ChannelIncrementStep_8   = 3, /**< \brief increment by 8 width */
    Dma_ChannelIncrementStep_16  = 4, /**< \brief increment by 16 width */
    Dma_ChannelIncrementStep_32  = 5, /**< \brief increment by 32 width */
    Dma_ChannelIncrementStep_64  = 6, /**< \brief increment by 64 width */
    Dma_ChannelIncrementStep_128 = 7  /**< \brief increment by 128 width */
} Dma_ChannelIncrementStep;

/** \brief Channel Transfer Interrupt generation mechanism.
 * Definition in _DMA.CH[64].ADICR.B.INTCT (bit 0)
 */
typedef enum
{
    Dma_ChannelInterruptControl_thresholdLimitMatch      = 0, /**< \brief interrupt when transfer count (TCOUNT) equals the threshold limit (IRDV) */
    Dma_ChannelInterruptControl_transferCountDecremented = 1  /**< \brief interrupt when transfer count (TCOUNT) is decremented */
} Dma_ChannelInterruptControl;

/** \brief DMA transfer definition
 * Definition in _DMA.BLK0.ME.CHCR.B.BLKM and _DMA.BLK1.ME.CHCR.B.BLKM
 */
typedef enum
{
    Dma_ChannelMove_1  = 0,  /**< \brief 1 DMA move per DMA transfer */
    Dma_ChannelMove_2  = 1,  /**< \brief 2 DMA move per DMA transfer */
    Dma_ChannelMove_4  = 2,  /**< \brief 4 DMA move per DMA transfer */
    Dma_ChannelMove_8  = 3,  /**< \brief 8 DMA move per DMA transfer */
    Dma_ChannelMove_16 = 4,  /**< \brief 16 DMA move per DMA transfer */
    Dma_ChannelMove_3  = 5,  /**< \brief 3 DMA move per DMA transfer */
    Dma_ChannelMove_5  = 6,  /**< \brief 5 DMA move per DMA transfer */
    Dma_ChannelMove_9  = 7   /**< \brief 9 DMA move per DMA transfer */
} Dma_ChannelMove;

/** \brief DMA move size definition
 * Definition in _DMA.BLK0.ME.CHCR.B.CHDW and _DMA.BLK1.ME.CHCR.B.CHDW
 */
typedef enum
{
    Dma_ChannelMoveSize_8bit   = 0,  /**< \brief 1 DMA move is 8 bit wide */
    Dma_ChannelMoveSize_16bit  = 1,  /**< \brief 1 DMA move is 16 bit wide */
    Dma_ChannelMoveSize_32bit  = 2,  /**< \brief 1 DMA move is 32 bit wide */
    Dma_ChannelMoveSize_64bit  = 3,  /**< \brief 1 DMA move is 64 bit wide */
    Dma_ChannelMoveSize_128bit = 4,  /**< \brief 1 DMA move is 128 bit wide */
    Dma_ChannelMoveSize_256bit = 5   /**< \brief 1 DMA move is 256 bit wide */
} Dma_ChannelMoveSize;

/** \brief DMA operation mode
 * Definition in _DMA.BLK0.ME.CHCR.B.CHMODE and _DMA.BLK1.ME.CHCR.B.CHMODE
 */
typedef enum
{
    Dma_ChannelOperationMode_single     = 0, /**< \brief channel disabled after transaction */
    Dma_ChannelOperationMode_continuous = 1  /**< \brief channel stays enabled after transaction */
} Dma_ChannelOperationMode;

/** \brief Pattern detection selection
 * Definition in _DMA.BLK0.ME.CHCR.B.PATSEL and _DMA.BLK1.ME.CHCR.B.PATSEL
 */
typedef enum
{
    Dma_ChannelPattern_0_disable = 0,  /**< \brief Pattern detect 0 disabled */
    Dma_ChannelPattern_0_mode1   = 1,  /**< \brief Compare match configuration 1 : pattern compare of MExR.RD[0] to PAT0[0] masked by PAT0[2] */
    Dma_ChannelPattern_0_mode2   = 2,  /**< \brief Compare match configuration 2 : pattern compare of MExR.RD[0] to PAT0[1] masked by PAT0[3] */
    Dma_ChannelPattern_0_mode3   = 3,  /**< \brief Compare match configuration 3 : pattern compare of MExR.RD[0] to PAT0[0] masked by PAT0[2] of actual DMA read move and pattern compare of MExR.RD[0] to PAT0[1] masked by PAT0[3] of previous DMA read move */
    Dma_ChannelPattern_1_disable = 4,  /**< \brief Pattern detect 1 disabled */
    Dma_ChannelPattern_1_mode1   = 5,  /**< \brief Compare match configuration 1 : pattern compare of MExR.RD[0] to PAT1[0] masked by PAT1[2] */
    Dma_ChannelPattern_1_mode2   = 6,  /**< \brief Compare match configuration 2 : pattern compare of MExR.RD[0] to PAT1[1] masked by PAT1[3] */
    Dma_ChannelPattern_1_mode3   = 7   /**< \brief Compare match configuration 3 : pattern compare of MExR.RD[0] to PAT1[0] masked by PAT1[2] of actual DMA read move and pattern compare of MExR.RD[0] to PAT1[1] masked by PAT1[3] of previous DMA read move */
} Dma_ChannelPattern;

/** \brief Channel Priority definition
 */
typedef enum
{
    Dma_ChannelPriority_low    = 0,  /**< \brief low priority */
    Dma_ChannelPriority_medium = 1,  /**< \brief medium priority */
    Dma_ChannelPriority_high   = 2   /**< \brief high priority */
} Dma_ChannelPriority;

/** \brief DMA request mode
 * Definition in _DMA.BLK0.ME.CHCR.B.RROAT and _DMA.BLK1.ME.CHCR.B.RROAT
 */
typedef enum
{
    Dma_ChannelRequestMode_oneTransferPerRequest         = 0, /**< \brief a request initiates a single transfer */
    Dma_ChannelRequestMode_completeTransactionPerRequest = 1  /**< \brief a request initiates a complete transaction */
} Dma_ChannelRequestMode;

/** \brief DMA request selection
 * Definition in _DMA.BLK0.ME.CHCR.B.PRSEL and _DMA.BLK1.ME.CHCR.B.PRSEL
 */
typedef enum
{
    Dma_ChannelRequestSource_peripheral = 0,  /**< \brief Transfer Request via Hardware Trigger */
    Dma_ChannelRequestSource_daisyChain = 1   /**< \brief Transfer Request via next (higher priority) channel */
} Dma_ChannelRequestSource;

/** \brief shadow definition definition
 * Definition in _DMA.CH[64].ADICR.B.SHCT
 */
typedef enum
{
    Dma_ChannelShadow_none                                 = 0,   /**< \brief shadow address register not used. Source and destination address register are written directly */
    Dma_ChannelShadow_src                                  = 1,   /**< \brief Shadow address register used for source address buffering. When writing to SADRmx, the address is buffered in SHADRmx and transferred to SADRmx with the start of the next DMA transaction */
    Dma_ChannelShadow_dst                                  = 2,   /**< \brief Shadow address register used for destination address buffering. When writing to DADRmx, the address is buffered in SHADRmx and transferred to DADRmx with the start of the next DMA transaction */
    Dma_ChannelShadow_srcDirectWrite                       = 5,   /**< \brief Shadow address used for source buffering. When writing to SADRz, the address is buffered in SHADRz and transferred to SADRz with the start of the next DMA transaction */
    Dma_ChannelShadow_dstDirectWrite                       = 6,   /**< \brief Shadow address used for destination buffering. When writing to DADRz, the address is buffered in SHADRz and transferred to DADRz with the start of the next DMA transaction */
    Dma_ChannelShadow_doubleSourceBufferingSwSwitch        = 8,   /**< \brief Software switch only. Shadow address used for double buffering */
    Dma_ChannelShadow_doubleSourceBufferingHwSwSwitch      = 9,   /**< \brief Automatic Hardware and Software switch. Shadow address used for double buffering */
    Dma_ChannelShadow_doubleDestinationBufferingSwSwitch   = 10,  /**< \brief Software switch only. Shadow address used for double buffering */
    Dma_ChannelShadow_doubleDestinationBufferingHwSwSwitch = 11,  /**< \brief Automatic Hardware and Software switch. Shadow address used for double buffering */
    Dma_ChannelShadow_linkedList                           = 12,  /**< \brief The DMA controller reads a DMA channel transaction control set and overwrites 8 X words in the corresponding DMARAM channel z */
    Dma_ChannelShadow_accumulatedLinkedList                = 13,  /**< \brief The DMA controller reads a DMA channel transaction control set and overwrites 6 X words in the corresponding DMARAM channel z */
    Dma_ChannelShadow_safeLinkedList                       = 14,  /**< \brief The DMA controller reads a DMA channel transaction control set. The Linked List only proceeds with the next DMA transaction if the existing SDCRC checksum matches the expected SDCRC checksum in the loaded from the new DMA transaction control set */
    Dma_ChannelShadow_conditionalLinkedList                = 15   /**< \brief Shadow address register (MExSHADR) and source and destination address CRC register (MExSDCRC) are used as address pointers to a Linked List. The selection of the address pointer is determined by DMA channel pattern detection conditions */
} Dma_ChannelShadow;

typedef enum
{
    Dma_HardwareResourcePartition_0 = 0,      /**< \brief "Set of DMA channels are associated with hardware resource partition " + str(x)  */
    Dma_HardwareResourcePartition_1,          /**< \brief "Set of DMA channels are associated with hardware resource partition " + str(x)  */
    Dma_HardwareResourcePartition_2,          /**< \brief "Set of DMA channels are associated with hardware resource partition " + str(x)  */
    Dma_HardwareResourcePartition_3,          /**< \brief "Set of DMA channels are associated with hardware resource partition " + str(x)  */
} Dma_HardwareResourcePartition;

/** \brief DMA move engine definition
 */
typedef enum
{
    Dma_MoveEngine_0 = 0,  /**< \brief first move engine */
    Dma_MoveEngine_1 = 1   /**< \brief second move engine */
} Dma_MoveEngine;

/** \brief Enable/disable the sensitivity of the module to sleep signal\n
 * Definition in _DMA.CLC.B.EDIS
 */
typedef enum
{
    Dma_SleepMode_enable  = 0, /**< \brief enables sleep mode */
    Dma_SleepMode_disable = 1  /**< \brief disables sleep mode */
} Dma_SleepMode;

/** \} */

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/


typedef struct
{
	Ifx_DMA         *dma;           /**< \brief Specifies the pointer to the DMA registers */
    Dma_ChannelId channelId;     /**< \brief Specifies the DMA channel */
    Ifx_DMA_CH      *channel;       /**< \brief Specifies the pointer to DMA channel registers */
} Dma_Channel;

/** \brief Configuration data structure of the channel
 */
typedef struct
{
    Dma_ChannelId                 channelId;                                  /**< \brief Specifies the channel being used */
    uint32                        sourceAddress;                              /**< \brief Source address for the DMA channel */
    uint32                        destinationAddress;                         /**< \brief Destination address for the DMA channel */
    uint32                        shadowAddress;                              /**< \brief Initial content of shadow address for the DMA channel */
    uint32                        readDataCrc;                                /**< \brief Checksum for read data of the channel */
    uint32                        sourceDestinationAddressCrc;                /**< \brief Checksum for source and destination address of channel */
    uint16                        transferCount;                              /**< \brief Number of transfers in a transaction */
    Dma_ChannelMove               blockMode;                                  /**< \brief Number of moves in a transfer */
    Dma_ChannelRequestMode        requestMode;                                /**< \brief A service request initiates a single transfer, or the complete transaction */
    Dma_ChannelOperationMode      operationMode;                              /**< \brief keep enable/disable the hardware channel request after a transaction */
    Dma_ChannelMoveSize           moveSize;                                   /**< \brief Read/write data size */
    Dma_ChannelPattern            pattern;                                    /**< \brief Pattern selection operation modes */
    Dma_ChannelRequestSource      requestSource;                              /**< \brief Request of channel transfer through hardware or daisy chain. channel transfer complete interrupt of previous channel will trigger the next channel request */
    Dma_ChannelBusPriority        busPriority;                                /**< \brief Bus priority selection */
    boolean                       hardwareRequestEnabled;                     /**< \brief Enabling channel transaction via hardware request */
    Dma_ChannelIncrementStep      sourceAddressIncrementStep;                 /**< \brief Describes the address offset with which the source address should be modified after each move */
    Dma_ChannelIncrementDirection sourceAddressIncrementDirection;            /**< \brief Decides whether the source address offset after each move should be added or decremented from the exisiting address */
    Dma_ChannelIncrementCircular  sourceAddressCircularRange;                 /**< \brief Determines which part of the source address remains unchanged and therby not updated after each move */
    Dma_ChannelIncrementStep      destinationAddressIncrementStep;            /**< \brief Describes the address offset with which the destination address should be modified after each move */
    Dma_ChannelIncrementDirection destinationAddressIncrementDirection;       /**< \brief Decides whether the destination address offset after each move should be added or decremented from the exisiting address */
    Dma_ChannelIncrementCircular  destinationAddressCircularRange;            /**< \brief Determines which part of the destination address remains unchanged and therby not updated after each move */
    Dma_ChannelShadow             shadowControl;                              /**< \brief selects the shadow transfer mode */
    boolean                       sourceCircularBufferEnabled;                /**< \brief Enables/Disables the source circular buffering */
    boolean                       destinationCircularBufferEnabled;           /**< \brief Enables/Disables the destination circular buffering */
    boolean                       timestampEnabled;                           /**< \brief Enables/Disables the appendage of the time stamp after end of the last DMA move in a transaction */
    boolean                       wrapSourceInterruptEnabled;                 /**< \brief An interrupt should be triggered whenever source address is wrapped */
    boolean                       wrapDestinationInterruptEnabled;            /**< \brief An interrupt should be triggered whenever destination address is wrapped */
    boolean                       channelInterruptEnabled;                    /**< \brief The channel transfer interrupt should be triggered. See also channelInterruptControl */
    Dma_ChannelInterruptControl   channelInterruptControl;                    /**< \brief The channel transfer interrupt can either be triggered depending on the interruptRaiseThreshold, or each time the transaction count is decremented */
    uint8                         interruptRaiseThreshold;                    /**< \brief The value of the transferCount at which the interrupt should be raised */
    boolean                       transactionRequestLostInterruptEnabled;     /**< \brief Enables/Disables the channel transaction request lost interrupt */
} Dma_ChannelConfig;

/** \brief Configuration data structure of the Module
 */
typedef struct
{
    Ifx_DMA *dma;     /**< \brief Specifies the pointer to the DMA registers */
} Dma_Config;

/** \} */

typedef struct
{
    Dma_Channel chn[MAX_CHANNEL];     /**< \brief Dma channel handles */
}Dma_ChannelSetting;

typedef enum
{
    Cpu_ResourceCpu_0 = 0,  /**< \brief CPU 0 */
    Cpu_ResourceCpu_1 = 1,  /**< \brief CPU 1 */
    Cpu_ResourceCpu_2 = 2,  /**< \brief CPU 2 */
    Cpu_ResourceCpu_none    /**< \brief None of the CPU */
} Cpu_ResourceCpu;

#endif /* KNOCHECK_DMA_DEF_H_ */
