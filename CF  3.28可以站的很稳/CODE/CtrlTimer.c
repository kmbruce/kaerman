/** ###################################################################
**     THIS BEAN MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename  : CtrlTimer.C
**     Project   : CF2_0
**     Processor : MCF52255CAF80
**     Beantype  : TimerInt
**     Version   : Bean 02.157, Driver 01.03, CPU db: 3.00.000
**     Compiler  : CodeWarrior MCF C Compiler
**     Date/Time : 2012-3-23, 16:14
**     Abstract  :
**         This bean "TimerInt" implements a periodic interrupt.
**         When the bean and its events are enabled, the "OnInterrupt"
**         event is called periodically with the period that you specify.
**         TimerInt supports also changing the period in runtime.
**         The source of periodic interrupt can be timer compare or reload
**         register or timer-overflow interrupt (of free running counter).
**     Settings  :
**         Timer name                  : PIT0 (16-bit)
**         Compare name                : PIT0_Modulus
**         Counter shared              : No
**
**         High speed mode
**             Prescaler               : divide-by-1
**             Clock                   : 40000000 Hz
**           Initial period/frequency
**             Xtal ticks              : 8000
**             microseconds            : 1000
**             milliseconds            : 1
**             seconds (real)          : 0.001
**             Hz                      : 1000
**             kHz                     : 1
**
**         Runtime setting             : none
**
**         Initialization:
**              Timer                  : Disabled
**              Events                 : Enabled
**
**         Timer registers
**              Counter                : PCNTR0    [0x00150004]
**              Mode                   : PCSR0     [0x00150000]
**              Run                    : PCSR0     [0x00150000]
**              Prescaler              : PCSR0     [0x00150000]
**
**         Compare registers
**              Compare                : PMR0      [0x00150002]
**
**         Flip-flop registers
**     Contents  :
**         Enable  - byte CtrlTimer_Enable(void);
**         Disable - byte CtrlTimer_Disable(void);
**
**     Copyright : 1997 - 2009 Freescale Semiconductor, Inc. All Rights Reserved.
**     
**     http      : www.freescale.com
**     mail      : support@freescale.com
** ###################################################################*/

/* MODULE CtrlTimer. */

#include "Events.h"
#include "CtrlTimer.h"

static bool EnUser;                    /* Enable/Disable device by user */

/* Internal method prototypes */
static void HWEnDi(void);

/*
** ===================================================================
**     Method      :  CtrlTimer_Enable (bean TimerInt)
**
**     Description :
**         This method enables the bean - it starts the timer. Events
**         may be generated (<DisableEvent>/<EnableEvent>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte CtrlTimer_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
    HWEnDi();                          /* Enable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  CtrlTimer_Disable (bean TimerInt)
**
**     Description :
**         This method disables the bean - it stops the timer. No
**         events will be generated.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte CtrlTimer_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    HWEnDi();                          /* Disable the device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  CtrlTimer_Init (bean TimerInt)
**
**     Description :
**         Initializes the associated peripheral(s) and the beans 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void CtrlTimer_Init(void)
{
  /* PCSR0: ??=0,??=0,??=0,??=0,PRE=0,??=0,DOZE=0,DBG=0,OVW=1,PIE=1,PIF=1,RLD=1,EN=0 */
  setReg16(PCSR0, 0x1EU);              /* Set up control register */ 
  EnUser = FALSE;                      /* Disable device */
  /* PMR0: PM=0x9C3F */
  setReg16(PMR0, 0x9C3FU);             /* Store given value to the modulus register */ 
  HWEnDi();                            /* Enable/disable device according to status flags */
}

/*
** ===================================================================
**     Method      :  CtrlTimer_Interrupt (bean TimerInt)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the bean.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
__declspec(interrupt) void CtrlTimer_Interrupt(void)
{
  setReg16Bit(PCSR0,PIF);              /* Reset interrupt request flag */
  CtrlTimer_OnInterrupt();             /* Invoke user event */
}

/*
** ===================================================================
**     Method      :  HWEnDi (bean TimerInt)
**
**     Description :
**         Enables or disables the peripheral(s) associated with the bean.
**         The method is called automatically as a part of the Enable and 
**         Disable methods and several internal methods.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
static void HWEnDi(void)
{
  if (EnUser) {                        /* Enable device? */
    setReg16Bit(PCSR0,EN);             /* Run counter */
  }
  else {                               /* Disable device? */
    clrReg16Bit(PCSR0,EN);             /* Stop counter */
  }
}

/* END CtrlTimer. */

/*
** ###################################################################
**
**     This file was created by Processor Expert 1.05 [04.27]
**     for the Freescale MCF series of microcontrollers.
**
** ###################################################################
*/
