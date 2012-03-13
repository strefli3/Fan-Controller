/*********************************************
* Chip type           : ATmega128
* Clock frequency     : 8000000Hz
*********************************************/


#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <avr/wdt.h>
#include "MRAC.h"



/*********************************************
* MOAR USART SHIT
*********************************************/

void usart_putc(unsigned char c) {
   // wait until UDR ready
        while(!(UCSR1A & (1 << UDRE1)));
        UDR1 = c;    // send character
}

void uart_puts (char *s) {
        //  loop until *s != NULL
        while (*s) {
                usart_putc(*s);
                s++;
        }
}

