#ifndef RTDH_EECFG_H
#define RTDH_EECFG_H


#define RTDRUID_CONFIGURATOR_NUMBER 1278



/***************************************************************************
 *
 * Common defines ( CPU 0 )
 *
 **************************************************************************/

    /* TASK definition */
    #define EE_MAX_TASK 11
    #define IFX_OSTASK_EVENT1 0
    #define IFX_OSTASK_EVENT2 1
    #define IFX_OSTASK_EVENT3 2
    #define IFX_OSTASK_1MS 3
    #define IFX_OSTASK_5MS 4
    #define IFX_OSTASK_10MS 5
    #define IFX_OSTASK_20MS 6
    #define IFX_OSTASK_50MS 7
    #define IFX_OSTASK_100MS 8
    #define IFX_OSTASK_BACKGROUND 9
    #define IFX_OSTASK_INIT 10

    /* MUTEX definition */
    #define EE_MAX_RESOURCE 0U

    /* EVENT definition */
    #define IFX_OSCFG_EVENT1 0x1U
    #define IFX_OSCFG_EVENT2 0x2U
    #define IFX_OSCFG_EVENT3 0x4U

    /* ALARM definition */
    #define EE_MAX_ALARM 6U
    #define IFX_OSTASK_ALARM_1MS 0U
    #define IFX_OSTASK_ALARM_5MS 1U
    #define IFX_OSTASK_ALARM_10MS 2U
    #define IFX_OSTASK_ALARM_20MS 3U
    #define IFX_OSTASK_ALARM_50MS 4U
    #define IFX_OSTASK_ALARM_100MS 5U

    /* SCHEDULING TABLE definition */
    #define EE_MAX_SCHEDULETABLE 0U

    /* COUNTER OBJECTS definition */
    #define EE_MAX_COUNTER_OBJECTS (EE_MAX_ALARM + EE_MAX_SCHEDULETABLE)

    /* CPU CLOCK definition */
    #define EE_CPU_CLOCK      200000000U

    /* COUNTER definition */
    #define EE_MAX_COUNTER 1U
    #define IFX_OSTASK_COUNTER 0U

    /* APPMODE definition */
    #define EE_MAX_APPMODE 2U
    #define TRICORE_CPU 1

    /* CPUs */
    #define EE_MAX_CPU 1
    #define EE_CURRENTCPU 0

    /* Number of isr 2 */
    #define EE_MAX_ISR2   0
    #define EE_MAX_ISR_ID 0

#ifndef __DISABLE_EEOPT_DEFINES__


/***************************************************************************
 *
 * User options
 *
 **************************************************************************/


/***************************************************************************
 *
 * Automatic options
 *
 **************************************************************************/
#define __RTD_CYGWIN__
#define EE_TRICORE__
#define EE_TC27X__
#define EE_TASKING__
#define __OO_ECC2__
#define __OO_EXTENDED_STATUS__
#define __IRQ_STACK_NEEDED__
#define __MULTI__
#define __OO_NO_RESOURCES__
#define __OO_AUTOSTART_TASK__
#define __ALLOW_NESTED_IRQ__

#endif

    /* System stack size */
    #define EE_SYS_STACK_SIZE     8192


/* Max ISR priority */
#define EE_TC_MAX_ISR_ID     EE_ISR_UNMASKED



/***************************************************************************
 *
 * Counter defines
 *
 **************************************************************************/
#define OSMAXALLOWEDVALUE_IFX_OSTASK_COUNTER 10000U
#define OSTICKSPERBASE_IFX_OSTASK_COUNTER    1U
#define OSMINCYCLE_IFX_OSTASK_COUNTER        5U



/***************************************************************************
 *
 * Vector size defines
 *
 **************************************************************************/
    #define EE_ACTION_ROM_SIZE 6
    #define EE_ALARM_ROM_SIZE 6
    #define EE_COUNTER_OBJECTS_ROM_SIZE 6
    #define EE_COUNTER_ROM_SIZE 1
    #define EE_OO_AUTOSTART_TASK_MODE_OSDEFAULTAPPMODE_SIZE 0
    #define EE_OO_AUTOSTART_TASK_MODE_TRICORE_CPU_SIZE 1
    #define EE_RQ_PAIRS_NEXT_SIZE 11
    #define EE_RQ_PAIRS_TID_SIZE 11
    #define EE_RQ_QUEUES_HEAD_SIZE 16
    #define EE_RQ_QUEUES_TAIL_SIZE 16
    #define EE_TC_SYSTEM_TOS_SIZE 4


#endif

