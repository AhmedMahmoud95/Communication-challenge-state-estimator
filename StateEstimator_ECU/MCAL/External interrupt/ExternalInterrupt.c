#include "ExternalInterrupt.h"
#include "std_types.h"
#include "DIO.h"
#include "registers.h"
#include "interrupt.h"
/************************************************************************/
/*				 		Static global variables                         */
/************************************************************************/
static uint8 u8_ExternalInterrptInitCheck[3]    = {0, 0, 0};
static uint8 u8_ExternalInterrptNULLPTRCheck[3] = {0, 0, 0};
/************************************************************************/
/*				 		pointers to callback functions                  */
/************************************************************************/
static ERROR_STATUS (*CBK_ExIntFunc[3]) (void) = {NULL, NULL, NULL};

/************************************************************************/
/*				 		Functions definitions                            */
/************************************************************************/

/*
 * Function: ExternInt_Init
 * Inputs:	ExternIntCfg -> pointer of type ExternInt_Cfg_s which points to structure that contain the initialized data.
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Initiate the required external interrupt configuration as it specify
 *	event of interrupt and mode if polling or Interrupt.
 */

ERROR_STATUS ExternInt_Init(ExternInt_Cfg_s *ExternIntCfg)
{
    DIO_Cfg_s DioExIntObject;

    if (ExternIntCfg == NULL)
    {
        return E_NOK;
    }
    switch (ExternIntCfg->ExternInt_No)
    {
    case EXTRN_INT_0:
        DioExIntObject.GPIO = GPIOD;
        DioExIntObject.pins = BIT2;
        DioExIntObject.dir  = INPUT;
        DIO_init( &DioExIntObject);
        CBK_ExIntFunc[EXTRN_INT_0] = ExternIntCfg->ExternInt_CBF_Ptr;
        switch (ExternIntCfg ->ExternInt_Event)
        {
        case LOW_LEVEL:
            ASSIGN_BIT(MCUCR, BIT0, LOW);
            ASSIGN_BIT(MCUCR, BIT1, LOW);
            break;
        case LOGICAL_CHANGE:
            ASSIGN_BIT(MCUCR, BIT0, HIGH);
            ASSIGN_BIT(MCUCR, BIT1, LOW);
            break;
        case FALLING_EDGE:
            ASSIGN_BIT(MCUCR, BIT0, LOW);
            ASSIGN_BIT(MCUCR, BIT1, HIGH);
            break;
        case RISING_EDGE:
            ASSIGN_BIT(MCUCR, BIT0, HIGH);
            ASSIGN_BIT(MCUCR, BIT1, HIGH);
            break;
        default:
            return E_NOK;
        }
        u8_ExternalInterrptInitCheck[EXTRN_INT_0] = TRUE;
        return E_OK;

    case EXTRN_INT_1:
        DioExIntObject.GPIO = GPIOD;
        DioExIntObject.pins = BIT3;
        DioExIntObject.dir  = INPUT;
        DIO_init( &DioExIntObject);
        CBK_ExIntFunc[EXTRN_INT_1] = ExternIntCfg->ExternInt_CBF_Ptr;
        switch (ExternIntCfg ->ExternInt_Event)
        {
        case LOW_LEVEL:
            ASSIGN_BIT(MCUCR, BIT2, LOW);
            ASSIGN_BIT(MCUCR, BIT3, LOW);
            break;
        case LOGICAL_CHANGE:
            ASSIGN_BIT(MCUCR, BIT2, HIGH);
            ASSIGN_BIT(MCUCR, BIT3, LOW);
            break;
        case FALLING_EDGE:
            ASSIGN_BIT(MCUCR, BIT2, LOW);
            ASSIGN_BIT(MCUCR, BIT3, HIGH);
            break;
        case RISING_EDGE:
            ASSIGN_BIT(MCUCR, BIT2, HIGH);
            ASSIGN_BIT(MCUCR, BIT3, HIGH);
            break;
        default:
            return E_NOK;
        }
        u8_ExternalInterrptInitCheck[EXTRN_INT_1] = TRUE;
        return E_OK;

    case EXTRN_INT_2:
        DioExIntObject.GPIO = GPIOB;
        DioExIntObject.pins = BIT2;
        DioExIntObject.dir  = INPUT;
        DIO_init( &DioExIntObject);
        CBK_ExIntFunc[EXTRN_INT_2] = ExternIntCfg->ExternInt_CBF_Ptr;
        switch (ExternIntCfg ->ExternInt_Event)
        {
        case FALLING_EDGE:
            ASSIGN_BIT(MCUCSR, BIT6, LOW);
            break;
        case RISING_EDGE:
            ASSIGN_BIT(MCUCSR, BIT6, HIGH);
            break;
        default:
            return E_NOK;
        }
        u8_ExternalInterrptInitCheck[EXTRN_INT_2] = TRUE;
        return E_OK;

    default:
        return E_NOK;
    }

}

/*
 * Function: ExternInt_Enable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Enable the required external interrupt from GICR but to allow the interrupt global interrupt must be set.
 */
ERROR_STATUS ExternInt_Enable(uint8 ExternInt_No)
{
    switch (ExternInt_No)
    {
    case EXTRN_INT_0:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_0] == TRUE)
        {
            sei();
            ASSIGN_BIT(GICR, BIT6, HIGH);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_1:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_1] == TRUE)
        {
            sei();
            ASSIGN_BIT(GICR, BIT7, HIGH);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_2:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_2] == TRUE)
        {
            sei();
            ASSIGN_BIT(GICR, BIT5, HIGH);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    default:
        return E_NOK;
    }
}



/*
 * Function: ExternInt_SetEvent
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * 	       InterruptEvent -> indicate required event for INT0 and INT1 there are 4 events to check (RISING_EDGE,FALLING_EDGE,LOW_LEVEL,LOGICAL_CHANGE).
							  But for Interrupt 2 there are only Two cases (Rising,Falling)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to set event of required external interrupt.
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS ExternInt_SetEvent(uint8 ExternInt_No,uint8 InterruptEvent)
{
    switch (ExternInt_No)
    {
    case EXTRN_INT_0:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_0] == TRUE)
        {
            switch (InterruptEvent)
            {
            case LOW_LEVEL:
                ASSIGN_BIT(MCUCR, BIT0, LOW);
                ASSIGN_BIT(MCUCR, BIT1, LOW);
                return E_OK;
            case LOGICAL_CHANGE:
                ASSIGN_BIT(MCUCR, BIT0, HIGH);
                ASSIGN_BIT(MCUCR, BIT1, LOW);
                return E_OK;
            case FALLING_EDGE:
                ASSIGN_BIT(MCUCR, BIT0, LOW);
                ASSIGN_BIT(MCUCR, BIT1, HIGH);
                return E_OK;
            case RISING_EDGE:
                ASSIGN_BIT(MCUCR, BIT0, HIGH);
                ASSIGN_BIT(MCUCR, BIT1, HIGH);
                return E_OK;
            default:
                return E_NOK;
            }
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_1:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_1] == TRUE)
        {
            switch (InterruptEvent)
            {
            case LOW_LEVEL:
                ASSIGN_BIT(MCUCR, BIT2, LOW);
                ASSIGN_BIT(MCUCR, BIT3, LOW);
                return E_OK;
            case LOGICAL_CHANGE:
                ASSIGN_BIT(MCUCR, BIT2, HIGH);
                ASSIGN_BIT(MCUCR, BIT3, LOW);
                return E_OK;
            case FALLING_EDGE:
                ASSIGN_BIT(MCUCR, BIT2, LOW);
                ASSIGN_BIT(MCUCR, BIT3, HIGH);
                return E_OK;
            case RISING_EDGE:
                ASSIGN_BIT(MCUCR, BIT2, HIGH);
                ASSIGN_BIT(MCUCR, BIT3, HIGH);
                return E_OK;
            default:
                return E_NOK;
            }
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_2:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_2] == TRUE)
        {
            switch (InterruptEvent)
            {
            case FALLING_EDGE:
                ASSIGN_BIT(MCUCSR, BIT6, LOW);
                return E_OK;
            case RISING_EDGE:
                ASSIGN_BIT(MCUCSR, BIT6, HIGH);
                return E_OK;
            default:
                return E_NOK;
            }
        }
        else
        {
            return E_NOK;
        }

    default:
        return E_NOK;
    }
}

/*
 * Function: ExternInt_GetStatus
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2).
 * Outputs: *status -> points to the value of selected interrupt flag.
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: function is used to get flag of required interrupt status.
 * 				note: usually used to poll on the flag.
 */
ERROR_STATUS ExternInt_GetStatus(uint8 ExternInt_No,uint8 *Status)
{
    switch (ExternInt_No)
    {
    case EXTRN_INT_0:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_0] == TRUE)
        {
            *Status = GET_BIT(GIFR, BIT6);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_1:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_1] == TRUE)
        {
            *Status = GET_BIT(GIFR, BIT7);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_2:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_2] == TRUE)
        {
            *Status = GET_BIT(GIFR, BIT5);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    default:
        return E_NOK;
    }
}

/*
 * Function: ExternInt_Disable
 * Inputs: ExternInt_No -> Specify one of the external interrupt (INT_0, INT_1, INT2)
 * Outputs:
 * In Out:
 * Return: ERROR_STATUS -> check if there any error occurs, would return E_NOK else it would return E_OK.
 * Description: Disable the required external interrupt.
 */
ERROR_STATUS ExternInt_Disable(uint8 ExternInt_No)
{
    switch (ExternInt_No)
    {
    case EXTRN_INT_0:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_0] == TRUE)
        {
            ASSIGN_BIT(GICR, BIT6, LOW);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_1:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_1] == TRUE)
        {
            ASSIGN_BIT(GICR, BIT7, LOW);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    case EXTRN_INT_2:
        if (u8_ExternalInterrptInitCheck[EXTRN_INT_2] == TRUE)
        {
            ASSIGN_BIT(GICR, BIT5, LOW);
            return E_OK;
        }
        else
        {
            return E_NOK;
        }
    default:
        return E_NOK;
    }
}

ISR (INT0_vect)
{
    if (CBK_ExIntFunc[EXTRN_INT_0] != NULL)
    {
        ERROR_STATUS CBK_FuncCheck = E_NOK;
        CBK_FuncCheck = CBK_ExIntFunc[EXTRN_INT_0]();
        u8_ExternalInterrptNULLPTRCheck[EXTRN_INT_0] = CBK_FuncCheck;
    }
    else
    {
        u8_ExternalInterrptNULLPTRCheck[EXTRN_INT_0] = E_NOK;
    }
}

ISR (INT1_vect)
{
    if (CBK_ExIntFunc[EXTRN_INT_1] != NULL)
    {
        ERROR_STATUS CBK_FuncCheck = E_NOK;
        CBK_FuncCheck = CBK_ExIntFunc[EXTRN_INT_1]();
        u8_ExternalInterrptNULLPTRCheck[EXTRN_INT_1] = CBK_FuncCheck;
    }
    else
    {
        u8_ExternalInterrptNULLPTRCheck[EXTRN_INT_1] = E_NOK;
    }
}

ISR (INT2_vect)
{
    if (CBK_ExIntFunc[EXTRN_INT_2] != NULL)
    {
        ERROR_STATUS CBK_FuncCheck = E_NOK;
        CBK_FuncCheck = CBK_ExIntFunc[EXTRN_INT_2]();
        u8_ExternalInterrptNULLPTRCheck[EXTRN_INT_2] = CBK_FuncCheck;
    }
    else
    {
        u8_ExternalInterrptNULLPTRCheck[EXTRN_INT_2] = E_NOK;
    }
}
