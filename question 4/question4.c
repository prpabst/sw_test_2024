/******************************************************************************

Author: Paulo Ricardo Pabst
Date:	03/11/2024
Question 4

*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/*  Answer: Append a CRC (Cyclic Redundancy Check) to the package to be sent.
            Then the receiver compute its own CRC and then check against the 
            CRC received.
            
            If they don't match, the receiver can either ask the sender to 
            resend the package or don't reply anything -- if a timeout feature 
            is implemented.
            
            MODBUS RTU requires reply only when there are errors on function, 
            address or data. Framing errors and CRC errors must not to be 
            replied.
*/



/*  Several ways can be used for packet start/end signaling:
    -- including START/END character(s): which requires ASCII transmission to 
        avoid problems;
    -- first byte as packet length: which result in trouble if a device is 
        powered during an onging transmission;
    -- preamble: as used in wireless communication to sync both transmitter and 
        received;
    -- idle time: like used on MODBUS RTU, which is robust against to 
        incomplete packet, but reduces the data bandwidth due to idle timing.
        
    In this example I will consider a 4ms of idle as end-of-packet indication.
*/


/*  Solar inverter communication example:

    Format:
    [slave_id][payload_size][variable_payload][crc]
    
    Payload commands:
    -- getEnergy (float32)
        [01][31:24][23:16][15:8][7:0]
        
    -- getInstantPower (int16: negative is consumpting, positive is supplying)
        [02][15:8][7:0]
    
    -- getRunningTime (uint32_t -- as linux epoch)
        [03][31:24][23:16][15:8][7:0]
*/


#define _LITTLE_ENDIAN   1
//#define _BIG_ENDIAN      1

volatile uint8_t rx_data;   // To simulate the MCU USART RX register.
volatile bool rxTimerExpired;
uint8_t rxDataBuffer[64];
uint8_t rxDataCount;
uint8_t myNetworkId = 7;    // Unique ID
int32_t times = 20;

float Energy_Wh;
int16_t InstantPower_W;
uint32_t RunningTime_S;

// Call to start the 4ms timer.
void startRxTimer4ms (void) {
    // Does nothing, just to allow compiling.
}

// Call the restart the 4ms timer.
void restartRxTimer4ms (void) {
    // Does nothing, just to allow compiling.
}

// 4 ms timer used to detect the end-of-packet.
void irq_RxTimer (void) {

    rxTimerExpired = true;
}

// Called when a new byte arrives.
void irq_uart_rx (void) {

    // Start the 4ms timer after received the first byte.
    if(0 == rxDataCount) {
        startRxTimer4ms();
    } else {
        restartRxTimer4ms();
    }

    if(rxDataCount< 62) {

        rxDataBuffer[rxDataCount++];
    }
}


void getFloatVariable(uint8_t *_buff, float *_value) {

uint32_t val = 0;

#ifdef _LITTLE_ENDIAN
    val  = *(_buff+3)<<0;
    val |= *(_buff+2)<<8;
    val |= *(_buff+1)<<16;
    val |= *(_buff+0)<<24;
#elif defined (_BIG_ENDIAN)
    val  = *(_buff+0)<<0;
    val |= *(_buff+1)<<8;
    val |= *(_buff+2)<<16;
    val |= *(_buff+3)<<24;
#endif

    *_value = *(float *)&val;
}



void getInt16Variable(uint8_t *_buff, int16_t *_value) {

uint16_t val = 0;

#ifdef _LITTLE_ENDIAN
    val  = *(_buff+1)<<0;
    val |= *(_buff+0)<<8;
#elif defined (_BIG_ENDIAN)
    val  = *(_buff+0)<<0;
    val |= *(_buff+1)<<8;
#endif

    *_value = *(int16_t *)&val;
    
}


void getUint32Variable(uint8_t *_buff, uint32_t *_value) {

uint32_t val = 0;

#ifdef _LITTLE_ENDIAN
    val  = *(_buff+3)<<0;
    val |= *(_buff+2)<<8;
    val |= *(_buff+1)<<16;
    val |= *(_buff+0)<<24;
#elif defined (_BIG_ENDIAN)
    val  = *(_buff+0)<<0;
    val |= *(_buff+1)<<8;
    val |= *(_buff+2)<<16;
    val |= *(_buff+3)<<24;
#endif

    *_value = *(uint32_t *)&val;
}


bool isCrcOk(uint8_t *_buff, uint8_t _size, uint8_t _crc) {
    
    uint8_t computedCrc;
    
    // Compute the CRC
    // ....
    
    return(computedCrc == _crc);
}


void initUartRx (void) {
    
    rxDataCount = 0;
    rxTimerExpired = false;
}


int main()
{
    while(--times) {
        if(rxTimerExpired) {
            // Process the received packet.
            rxTimerExpired = false;
            if(myNetworkId != rxDataBuffer[0])   continue;   // Incorrect address
            if(!isCrcOk(rxDataBuffer, rxDataCount-1, \
                rxDataBuffer[rxDataCount-1]))    continue;   // CRC error.
                
            switch(rxDataBuffer[2]) {
                case 1:
                    if(rxDataBuffer[1] != 5)     continue;   // Wrong size.
                    getFloatVariable(&rxDataBuffer[3], &Energy_Wh);
                break;
                
                case 2:
                    if(rxDataBuffer[1] != 3)     continue;   // Wrong size.
                    getInt16Variable(&rxDataBuffer[3], &InstantPower_W);
                break;
                
                case 3:
                    if(rxDataBuffer[1] != 5)     continue;   // Wrong size.
                    getUint32Variable(&rxDataBuffer[3], &RunningTime_S);
                break;
                
                default:
            }
        }
    }

    return 0;
}
