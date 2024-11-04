/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>


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
    
    printf("%s\r\n", statesNames[actualState]);
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
        break;
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
