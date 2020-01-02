/*
*   File name:  main.c
*   Owner:      Tamooh
*   version:    1.0
*   Date:       29/12/2019
*/

#include "StateMachine.h"
int main(void)
{

    if (StateMachine_init() == E_NOK)
    {
        return E_NOK;
    }
    UART_SendByte('A');

    if (WaitForSpeedInit() == E_NOK)
    {

        return E_NOK;
    }
    UART_SendByte('B');

    if (WaitForTimeInit() == E_NOK)
    {
        return E_NOK;
    }
    UART_SendByte('C');
    while(1)
    {
        if (SpeedChange() == E_NOK)
        {
            return E_NOK;
        }
    }
    return 0;
}
