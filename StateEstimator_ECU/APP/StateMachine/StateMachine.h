/*   File:       StateMachine.h
*   Owner:      Tamoo7
*   Version:    1.0
*   Date:       29/12/2019
*/
#ifndef STATE_MACHINE_
#define STATE_MACHINE_
/**********************************************************************/
/*                          Included files                            */
/**********************************************************************/

#include "UART.h"
#include "SPI.h"
#include "Timer.h"
#include "ExternalInterrupt.h"
#include "common_macros.h"
#include "std_types.h"
#include "BTN.h"

/**********************************************************************/
/*                          global defines                            */
/**********************************************************************/

#define COUNTS_FOR_ONE_SECOND   15625
/**********************************************************************/
/*                          Functions declarations                    */
/**********************************************************************/

/*
*   Function name:  StateMachine_init
*   @parameters:    Nothing
*   Input:          Nothing
*   Output:         ERROR_STATUS
*                       E_OK -> Functions finished successfully
*                       E_NOK-> Function terminated unsuccessfully
*   Description:    The function initiates Timer module, External interrupt, UART, SPI, Push button
*/
ERROR_STATUS StateMachine_init (void);


/*
*   Function name:  StateMachine_init
*   @parameters:    Nothing
*   Input:          Nothing
*   Output:         ERROR_STATUS
*                       E_OK -> Functions finished successfully
*                       E_NOK-> Function terminated unsuccessfully
*   Description:    The function waits until speed is received from the PC to start communication
*/
ERROR_STATUS WaitForSpeedInit(void);


/************************************************************************************************
*   @Function name:  StateMachine_init
*   @parameters:     Nothing
*   @Input:          Nothing
*   @Output:         ERROR_STATUS
*                       E_OK -> Functions finished successfully
*                       E_NOK-> Function terminated unsuccessfully
*   @Description:    The function Checks for speed change
**************************************************************************************************/
ERROR_STATUS SpeedChange(void);

ERROR_STATUS WaitForTimeInit(void);

#endif // STATE_MACHINE_
