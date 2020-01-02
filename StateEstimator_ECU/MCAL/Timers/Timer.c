#include "Timer.h"
#include "registers.h"
#include "DIO.h"
#include "std_types.h"
#include "common_macros.h"
#include "interrupt.h"
/************************************************************************/
/*						Global static variables                         */
/************************************************************************/

static uint8 gen_timer0prescaler;
static uint8 gen_timer1prescaler;
static uint8 gen_timer2prescaler;


/************************************************************************/
/*					Declaration of callback functions                   */
/************************************************************************/

ERROR_STATUS (*CBK_TimerFuncPtr[3])(void) = {NULL, NULL, NULL};

/************************************************************************/
/*					Error checking variables                            */
/************************************************************************/

static uint8 TimerInitializationCheck[3];

/************************************************************************/
/*						Global static enums                             */
/************************************************************************/

typedef enum En_timer0Mode_t
{
	T0_NORMAL_MODE = 0x00, T0_Up_Count=0x07, T0_Down_count = 0x06
}En_timer0Mode_t;

typedef enum En_timer0perscaler_t
{
	T0_NO_CLOCK=0,T0_PRESCALER_NO=0x01,T0_PRESCALER_8=0x02,T0_PRESCALER_64=0x03,
	T0_PRESCALER_256=0x04,T0_PRESCALER_1024=0x05
}En_timer0perscaler_t;

typedef enum En_timer0Interrupt_t
{
	T0_POLLING=0,T0_INTERRUPT_NORMAL=0x01
}En_timer0Interrupt_t;





// for timer 1
typedef enum En_timer1Mode_t{
	T1_NORMAL_MODE=0x0000,T1_Up_Count=0x0007, T1_Down_count = 0x0006

}En_timer1Mode_t;

typedef enum En_timer1perscaler_t{
	T1_NO_CLOCK=0x0000,T1_PRESCALER_NO=0x0001,T1_PRESCALER_8=0x0002,T1_PRESCALER_64=0x0003,
	T1_PRESCALER_256=0x0004,T1_PRESCALER_1024=0x0005
}En_timer1perscaler_t;

typedef enum En_timer1Interrupt_t{
	T1_POLLING=0,T1_INTERRUPT_NORMAL=0x04
}En_timer1Interrupt_t;




// for timer 2
typedef enum En_timer2Mode_t{
	T2_NORMAL_MODE=0x00
}En_timer2Mode_t;


typedef enum En_timer2perscaler_t{
	T2_NO_CLOCK=0,T2_PRESCALER_NO=0x01,T2_PRESCALER_8=0x02, T2_PRESCALER_32=0x03,
	T2_PRESCALER_64=0x04, T2_PRESCALER_128=0x05, T2_PRESCALER_256 = 0x06, T2_PRESCALER_1024=0x07
}En_timer2perscaler_t;

typedef enum En_timer2Interrupt_t{
	T2_POLLING=0, T2_INTERRUPT_NORMAL=0x40
}En_timer2Interrupt_t;


/**
 * Input: Pointer to a structure contains the information needed to initialize the timer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module.
 *
 */

ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg)
{
  if (Timer_cfg == NULL)
  {
      return E_NOK;
  }
  CBK_TimerFuncPtr[Timer_cfg->Timer_CH_NO] = Timer_cfg->Timer_CBK_PTR;
  switch (Timer_cfg->Timer_CH_NO)
  {
	case TIMER_CH0:
	  switch (Timer_cfg->Timer_Mode)
	  {
		case TIMER_MODE:
		  TCCR0 = T0_NORMAL_MODE;
		  break;
		case COUNTER_UP_MODE:
		  TCCR0 = T0_Up_Count;
		  break;
		case COUNTER_DOWN_MODE:
		  TCCR0 = T0_Down_count;
		  break;
		default:
		  return E_NOK;
	  }
	  switch (Timer_cfg->Timer_Polling_Or_Interrupt)
	  {
		case TIMER_POLLING_MODE:
		  ASSIGN_BIT(TIMSK, BIT0, LOW);
		  ASSIGN_BIT(TIMSK, BIT1, LOW);
		  break;
		case TIMER_INTERRUPT_MODE:
		  ASSIGN_BIT(TIMSK, BIT0, HIGH);
		  ASSIGN_BIT(TIMSK, BIT1, LOW);
		  break;
		default:
		  return E_NOK;
	  }
	  if (Timer_cfg->Timer_Mode == T0_NORMAL_MODE && Timer_cfg->Timer_Prescaler != T0_NO_CLOCK)
      {
        switch (Timer_cfg->Timer_Prescaler)
        {
          case TIMER_PRESCALER_NO:
            gen_timer0prescaler = T0_PRESCALER_NO;
            break;

          case TIMER_PRESCALER_8:
            gen_timer0prescaler = T0_PRESCALER_8;
            break;

          case TIMER_PRESCALER_64:
            gen_timer0prescaler = T0_PRESCALER_64;
            break;

          case TIMER_PRESCALER_256:
            gen_timer0prescaler = T0_PRESCALER_256;
            break;

          case TIMER_PRESCALER_1024:
            gen_timer0prescaler = T0_PRESCALER_1024;
            break;

          default:
            return E_NOK;
        }
      }
	  else if (Timer_cfg->Timer_Mode == T0_Up_Count || Timer_cfg->Timer_Mode == T0_Down_count)
      {

      }
      else
      {
          return E_NOK;
      }
	  TimerInitializationCheck[TIMER_CH0] = TRUE;
	  return E_OK;

	case TIMER_CH1:
	  switch (Timer_cfg->Timer_Mode)
	  {
		case TIMER_MODE:
		  TCCR1 = T1_NORMAL_MODE;
		  break;
		case COUNTER_UP_MODE:
		  TCCR1 = T1_Up_Count;
		  /*
		  * Consider initiating B1 as input
		  * DIO_init()
		  */
		  break;
		case COUNTER_DOWN_MODE:
		  TCCR1 = T1_Down_count;
		  break;
		default:
		  return E_NOK;
	  }

	  switch (Timer_cfg->Timer_Polling_Or_Interrupt)
	  {
		case TIMER_POLLING_MODE:
		  ASSIGN_BIT(TIMSK, BIT2, LOW);
		  ASSIGN_BIT(TIMSK, BIT3, LOW);
		  ASSIGN_BIT(TIMSK, BIT4, LOW);
		  ASSIGN_BIT(TIMSK, BIT5, LOW);
	      break;

		case TIMER_INTERRUPT_MODE:
		  ASSIGN_BIT(TIMSK, BIT2, HIGH);
		  ASSIGN_BIT(TIMSK, BIT3, LOW);
		  ASSIGN_BIT(TIMSK, BIT4, LOW);
		  ASSIGN_BIT(TIMSK, BIT5, LOW);
		  break;

		default:
		  return E_NOK;
	  }
	  if (Timer_cfg->Timer_Prescaler == TIMER_NO_CLOCK)
      {
        return E_NOK;
      }
	  else if (Timer_cfg->Timer_Mode == T1_NORMAL_MODE && Timer_cfg->Timer_Prescaler != T1_NO_CLOCK)
      {
        switch (Timer_cfg->Timer_Prescaler)
        {
          case TIMER_PRESCALER_NO:
            gen_timer1prescaler = T1_PRESCALER_NO;
            break;

          case TIMER_PRESCALER_8:
            gen_timer1prescaler = T1_PRESCALER_8;
            break;

          case TIMER_PRESCALER_64:
            gen_timer1prescaler = T1_PRESCALER_64;
            break;

          case TIMER_PRESCALER_256:
            gen_timer1prescaler = T1_PRESCALER_256;
            break;

          case TIMER_PRESCALER_1024:
            gen_timer1prescaler = T1_PRESCALER_1024;
            break;

          default:
            return E_NOK;
        }
      }
      else
      {
        /* Counter mode */
      }
	  TimerInitializationCheck[TIMER_CH1] = TRUE;
	  return E_OK;

	case TIMER_CH2:
	  switch (Timer_cfg->Timer_Mode)
	  {
		case TIMER_MODE:
		  TCCR2 = T2_NORMAL_MODE;
		  break;
		default:
		  return E_NOK;
	  }

	  switch (Timer_cfg->Timer_Polling_Or_Interrupt)
	  {
		case TIMER_POLLING_MODE:
		  ASSIGN_BIT(TIMSK, BIT6, LOW);
		  ASSIGN_BIT(TIMSK, BIT7, LOW);
	      break;

		case TIMER_INTERRUPT_MODE:
		  ASSIGN_BIT(TIMSK, BIT6, HIGH);
		  ASSIGN_BIT(TIMSK, BIT7, LOW);
		  break;

        default:
          return E_NOK;
	  }
      if (Timer_cfg->Timer_Prescaler == TIMER_NO_CLOCK)
      {
        return E_NOK;
      }
	  else if (Timer_cfg->Timer_Mode == T2_NORMAL_MODE && Timer_cfg->Timer_Prescaler != T2_NO_CLOCK)
      {
        switch (Timer_cfg->Timer_Prescaler)
        {
          case TIMER_PRESCALER_NO:
            gen_timer2prescaler = T2_PRESCALER_NO;
            break;

          case TIMER_PRESCALER_8:
            gen_timer2prescaler = T2_PRESCALER_8;
            break;

          case TIMER_PRESCALER_32:
            gen_timer2prescaler = T2_PRESCALER_32;
            break;

          case TIMER_PRESCALER_64:
            gen_timer2prescaler = T2_PRESCALER_64;
            break;

          case TIMER_PRESCALER_128:
            gen_timer2prescaler = T2_PRESCALER_128;
            break;

          case TIMER_PRESCALER_256:
            gen_timer2prescaler = T2_PRESCALER_256;
            break;

          case TIMER_PRESCALER_1024:
            gen_timer2prescaler = T2_PRESCALER_1024;
            break;

          default:
            return E_NOK;
        }
      }
      else
      {
        /* Counter mode */
      }
	  TimerInitializationCheck[TIMER_CH2] = TRUE;
	  return E_OK;

    default:
      return E_NOK;
  }
}

/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function strats the needed timer.
 *
 */
ERROR_STATUS Timer_Start(uint8 Timer_CH_NO, uint16 Timer_Count)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
          if (TimerInitializationCheck[TIMER_CH0] == FALSE)
            return E_NOK;
          else
          {
		    TCNT0 = 0xff - (uint8)Timer_Count;
		    TCCR0 |= gen_timer0prescaler;
		    return E_OK;
          }

		case TIMER_CH1:
          if (TimerInitializationCheck[TIMER_CH1] == FALSE)
            return E_NOK;
          else
          {
		    TCNT1 = 0xffff - Timer_Count;
		    TCCR1 |= gen_timer1prescaler;
		    return E_OK;
          }

		case TIMER_CH2:
          if (TimerInitializationCheck[TIMER_CH2] == FALSE)
            return E_NOK;
          else
          {
		    TCNT2 = 0xff - (uint8)Timer_Count;
		    TCCR2 |= gen_timer2prescaler;
		    return E_OK;
          }
		default:
		  return E_NOK;
	}
}
/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the needed timer.
 *
 */
ERROR_STATUS Timer_Stop(uint8 Timer_CH_NO)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
          if (TimerInitializationCheck[TIMER_CH0] == FALSE)
            return E_NOK;
          else
          {
		    TCCR0 &= 0xF8;
		    return E_OK;
          }

		case TIMER_CH1:
          if (TimerInitializationCheck[TIMER_CH1] == FALSE)
            return E_NOK;
          else
          {
		    TCCR1 &= 0xFFF8;
		    return E_OK;
          }

		case TIMER_CH2:
          if (TimerInitializationCheck[TIMER_CH2] == FALSE)
            return E_NOK;
          else
          {
		    TCCR2 &= 0xF8;
		    return E_OK;
          }

		default:
		  return E_NOK;
	}
}
/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:
 * Return: The error status of the function.
 * Description: This function is used to return if the flag of the timer is raised or not.
 *
 */
ERROR_STATUS Timer_GetStatus(uint8 Timer_CH_NO, bool* Data)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
          if (TimerInitializationCheck[TIMER_CH0] == FALSE)
            return E_NOK;
          else
          {
		    *Data = GET_BIT(TIFR, BIT0);
		    return E_OK;
          }
		case TIMER_CH1:
		  if (TimerInitializationCheck[TIMER_CH1] == FALSE)
            return E_NOK;
          else
          {
		    *Data = GET_BIT(TIFR, BIT2);
		    return E_OK;
          }
		case TIMER_CH2:
          if (TimerInitializationCheck[TIMER_CH2] == FALSE)
            return E_NOK;
          else
          {
		    *Data = GET_BIT(TIFR, BIT6);
		    return E_OK;
          }
		default:
		  return E_NOK;
	}
}
/**
 * Input:
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:
 * Return: The error status of the function.
 * Description: This function is used to return the value of the timer.
 *
 */
ERROR_STATUS Timer_GetValue(uint8 Timer_CH_NO, uint16* Data)
{
	switch (Timer_CH_NO)
	{
		case TIMER_CH0:
            if (TimerInitializationCheck[TIMER_CH0] == FALSE)
                return E_NOK;
          else
          {
            *Data = TCNT0;
            return E_OK;
          }

		case TIMER_CH1:
			if (TimerInitializationCheck[TIMER_CH1] == FALSE)
            return E_NOK;
          else
          {
			*Data = TCNT1;
			return E_OK;
          }

		case TIMER_CH2:
		    if (TimerInitializationCheck[TIMER_CH2] == FALSE)
            return E_NOK;
          else
          {
			*Data = TCNT2;
			return E_OK;
          }

		default:
			return E_NOK;
	}
}

ISR (TIMER0_OVF_vect)
{
    if (CBK_TimerFuncPtr[TIMER_CH0] != NULL)
    {
        CBK_TimerFuncPtr[TIMER_CH0]();
    }
}

ISR (TIMER1_OVF_vect)
{
    if (CBK_TimerFuncPtr[TIMER_CH1] != NULL)
    {
        CBK_TimerFuncPtr[TIMER_CH1]();
    }
}

ISR (TIMER2_OVF_vect)
{
    if (CBK_TimerFuncPtr[TIMER_CH2] != NULL)
    {
        CBK_TimerFuncPtr[TIMER_CH2]();
    }
}
