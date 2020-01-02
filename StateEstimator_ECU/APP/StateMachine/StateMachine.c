/**********************************************************************/
/*                          global includes                           */
/**********************************************************************/
#include "StateMachine.h"
#include "softwareDelay.h"
/**********************************************************************/
/*                          global defines                            */
/**********************************************************************/
#define INC_BTN     BTN_0
#define DEC_BTN     BTN_1

/**********************************************************************/
/*                          global variables                          */
/**********************************************************************/

uint8  gu8_Speed = 0;
bool   gb_FetchReceived  = FALSE;
bool   gb_SpeedInitiated = FALSE;
uint8  gu8_Time          = 0;
bool gb_TimeInitiated    = FALSE;
bool FetchingTime        = TRUE;
/**********************************************************************/
/*                          CAllback functions                        */
/**********************************************************************/

ERROR_STATUS ExInt_CBK_StartTimer (void)
{
    gb_FetchReceived = TRUE;
    UART_SendByte('D');
    return E_OK;
}

/*
void UART_CBK_PCInit(void)
{
    UART_SendByte('E');
    if (FetchingTime == TRUE)
    {
        Timer_Start(TIMER_CH1, COUNTS_FOR_ONE_SECOND);
        gb_SpeedInitiated = TRUE;
        UART_ReceiveByte( &gu8_Speed);
        FetchingTime = FALSE;
    }
    else
    {
        UART_ReceiveByte( &gu8_Time);
        gb_TimeInitiated = TRUE;
    }

}
*/

ERROR_STATUS Timer_CBK_OverflowCount (void)
{
    UART_SendByte('F');
    SPI_SendByte(gu8_Speed);
    SwDelay_ms(100);
    SPI_SendByte(gu8_Time);
    UART_SendByte('G');
    Timer_Start(TIMER_CH1, COUNTS_FOR_ONE_SECOND);
    gu8_Time++;
    UART_SendByte(gu8_Speed);
    UART_SendByte(gu8_Time);
    return E_OK;
}


/********************************************************************************************************
*   Function name:  StateMachine_init
*   @parameters:    Nothing
*   @Input:          Nothing
*   @Output:         ERROR_STATUS
*                       E_OK -> Functions finished successfully
*                       E_NOK-> Function terminated unsuccessfully
*   @Description:    The function initiates Timer module, External interrupt, UART, SPI, Push button
********************************************************************************************************/
ERROR_STATUS StateMachine_init (void)
{
    ExternInt_Cfg_s ExInt_Object = {EXTRN_INT_1, RISING_EDGE, ExInt_CBK_StartTimer};
    if (ExternInt_Init ( &ExInt_Object) == E_NOK)
    {
        return E_NOK;
    }

    if (ExternInt_Enable(EXTRN_INT_1) == E_NOK)
    {
        return E_NOK;
    }

    SPI_Cfg_s SPI_Object = {MASTER, MODE_0, LSB, POLLING, SPI_PRESCALER_128, NULL};
    if (SPI_Init( &SPI_Object) == E_NOK)
    {
        return E_NOK;
    }

    Timer_cfg_s TimerObject ={TIMER_CH1, TIMER_MODE,
     TIMER_INTERRUPT_MODE, TIMER_PRESCALER_1024, Timer_CBK_OverflowCount};
    if (Timer_Init( &TimerObject) == E_NOK)
    {
        return E_NOK;
    }

    UART_cfg_s UARTObject = {UART_POLLING, TRANSCEIVER,
    UART_NO_DOUBLE_SPEED, UART_ONE_STOP_BIT, UART_NO_PARITY,
    UART_8_BIT, 9600, NULL, NULL, NULL};

    if (UART_Init( &UARTObject) == E_NOK)
    {
        return E_NOK;
    }

    if (BTN_Init(BTN_0) == E_NOK)
    {
        return E_NOK;
    }
    if (BTN_Init(BTN_1) == E_NOK)
    {
        return E_NOK;
    }
    return E_OK;
}



/************************************************************************************************
*   @Function name:  StateMachine_init
*   @parameters:     Nothing
*   @Input:          Nothing
*   @Output:         ERROR_STATUS
*                       E_OK -> Functions finished successfully
*                       E_NOK-> Function terminated unsuccessfully
*   @Description:    The function waits until speed is received from the PC to start communication
**************************************************************************************************/
ERROR_STATUS WaitForSpeedInit(void)
{

    UART_ReceiveByte(&gu8_Speed);
    UART_SendByte('E');
    return E_OK;
}


/************************************************************************************************
*   @Function name:  StateMachine_init
*   @parameters:     Nothing
*   @Input:          Nothing
*   @Output:         ERROR_STATUS
*                       E_OK -> Functions finished successfully
*                       E_NOK-> Function terminated unsuccessfully
*   @Description:    The function waits until speed is received from the PC to start communication
**************************************************************************************************/
ERROR_STATUS WaitForTimeInit(void)
{

    UART_ReceiveByte(&gu8_Time);
    UART_SendByte('E');
    Timer_Start(TIMER_CH1, COUNTS_FOR_ONE_SECOND);
    return E_OK;
}


/************************************************************************************************
*   @Function name:  StateMachine_init
*   @parameters:     Nothing
*   @Input:          Nothing
*   @Output:         ERROR_STATUS
*                       E_OK -> Functions finished successfully
*                       E_NOK-> Function terminated unsuccessfully
*   @Description:    The function Checks for speed change
**************************************************************************************************/
ERROR_STATUS SpeedChange(void)
{
    bool b_btnStatus = FALSE;
    if (BTN_GetStatus(INC_BTN, &b_btnStatus) == E_NOK)
    {
        return E_NOK;
    }
    if (b_btnStatus == PRESSED)
    {
            /* To handle overflow */
        if (gu8_Speed < 255)
        {
            gu8_Speed ++;
        }
        }
    SwDelay_ms(100);
    if (BTN_GetStatus(DEC_BTN, &b_btnStatus) == E_NOK)
    {
        return E_NOK;
    }
    if (b_btnStatus == PRESSED)
    {
            /* To handle underflow */
        if (gu8_Speed > 1)
        {
            gu8_Speed --;
        }
    }
    SwDelay_ms(100);
    return E_OK;
}

