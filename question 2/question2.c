/******************************************************************************

Author: Paulo Ricardo Pabst
Date:	03/11/2024
Question 2

*******************************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define	DEBUG_MSG	1

typedef enum {
    stSTART,
    stON,
    stOFF,
    stPROTECTED,
    stMAX = 0x8FFFFFFF  /* ensure that the enum will use a 32bit for better 
                            performance on 32bit architures.*/
} states_t;


char *statesNames[] = { "START", "ON", "OFF", "PROTECTED" };
volatile states_t actualState;
volatile states_t nextState;
volatile int32_t buttonTimer;



bool isButtonPressed(void) {
    
    return((bool)(rand()&1));
}


// Timer set to 50ms interval.
void isr_fsm_timer (void) {
    
#ifdef DEBUG_MSG
    printf("%s\r\n", statesNames[actualState]);
#endif

    switch(actualState) {
        case stSTART:
            nextState = stOFF;
        break;
        
        case stON:
            if(!isButtonPressed()) {
                nextState = stPROTECTED;
                buttonTimer = 200;
            }
        break;
        
        case stOFF:
            if(isButtonPressed())   nextState = stON;
        break;
        
        case stPROTECTED:
            if(--buttonTimer == 0) {
                nextState = stOFF;
            }
        break;
        
        default:
            // something went wrong...
            nextState = stSTART;
    }
    actualState = nextState;
}

int main()
{
    int32_t test = 1000;
    actualState = stSTART;
    nextState = stSTART;
    srand(time(NULL));
    
    while(--test) {
        isr_fsm_timer();
    }

    return 0;
}
