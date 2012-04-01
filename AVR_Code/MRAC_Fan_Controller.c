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
* CPU Frequencies
********************************************/
#define F_CPU 4000000		           /* oscillator-frequency in Hz */
#include <util/delay.h>

/*********************************************
* USART Definitions
*********************************************/
#define UART_BAUD_RATE 19200
#define UART_BAUD_CALC(UART_BAUD_RATE,F_CPU) ((F_CPU)/((UART_BAUD_RATE)*16l)-1)

/*********************************************
* Output usefull shit to the usart
*********************************************/
#define output 0

/*********************************************
* Global Variables
*********************************************/
char 		usart_array[100];
volatile int 	wi=0;
volatile uint16_t 	handshake=0;
/*********************************************
* MRAC Controller Global Variables
*********************************************/

double am=0.001471;		// Reference model am
double bm=0.001471;		// Reference model bm
volatile double Tm_1=30.00;	// Assume the reference model starts out at 40 deg C.
volatile double Tm_2=30.00;	// Assume the reference model starts out at 40 deg C.
volatile double Tm_3=30.00;	// Assume the reference model starts out at 40 deg C.
volatile double Tm_4=30.00;	// Assume the reference model starts out at 40 deg C.



/*********************************************
* Initiation Sequence for the USART
*********************************************/


void init(void) {
	// set baud rate
	UBRR1H = (uint8_t)(UART_BAUD_CALC(UART_BAUD_RATE,F_CPU)>>8);
	UBRR1L = (uint8_t)UART_BAUD_CALC(UART_BAUD_RATE,F_CPU);
	// Enable receiver and transmitter; enable RX interrupt
	UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
	//asynchronous 8N1
	UCSR1C = (0<<UMSEL1)|(0<<UPM10)|(0<<USBS1)|(1<<UCSZ10)|(1<<UCSZ11);
}
/*********************************************
* PWM Initiliazation
*********************************************/

void InitPWM(void)
{
        // PWM Init F1

	DDRB  |= _BV(PB4); //OCR0 pin as output
        TCCR0 |= _BV(WGM01) | _BV(WGM00); //mode 3, Fast PWM
        //TCCR0 |= _BV(WGM00); //mode 1, Phase Correct PWM
        TCCR0 |= _BV(COM01); //Clear OCR0 on compare match, set OC00 at BOTTOM
        TCCR0 |= _BV(CS00); //prescaler divider 1

        // PWM Init F2

        DDRB |= _BV(PB5); //OCR1B pin as output
        TCCR1A |= _BV(COM1A1); //Clear OC1A on compare match, set OC1A/OC1B at BOTTOM, (non-inverting mode)
        TCCR1A |= _BV(WGM10); //mode 5, Fast PWM 8-bit
        TCCR1B |= _BV(WGM12); //mode 5, Fast PWM 8-bit
        TCCR1B |= _BV(CS10); // Prescaler 1

        // PWM Init F3

        DDRB |= _BV(PB6); //OCR1B pin as output
        TCCR1A |= _BV(COM1B1); //Clear OC1B on compare match, set OC1A/OC1B at BOTTOM, (non-inverting mode)
        TCCR1A |= _BV(WGM10); //mode 5, Fast PWM 8-bit
        TCCR1B |= _BV(WGM12); //mode 5, Fast PWM 8-bit
        TCCR1B |= _BV(CS10); // Prescaler 1

        // PWM Init F4

        DDRB |= _BV(PB7); //OCR1C pin as output
        TCCR1A |= _BV(COM1C1); //Clear OC1B on compare match, set OC1A/OC1B at BOTTOM, (non-inverting mode)
        TCCR1A |= _BV(WGM10); //mode 5, Fast PWM 8-bit
        TCCR1B |= _BV(WGM12); //mode 5, Fast PWM 8-bit
        TCCR1B |= _BV(CS10); // Prescaler 1



}


/*********************************************
* The interupt for RX.
*********************************************/

ISR(USART1_RX_vect) 
{
        usart_array[wi] = UDR1; // Fetch the recieved byte
        wi++;
/*
	int F;
	F=strcmp(usart_array[wi-1],"a");

	uart_puts(usart_array);
	uart_puts("\r\n");

	if(F == 0)
	{
	}


*/
        if (usart_array[wi-1] == 0)
        {
                wi=0;

		if(strlen(usart_array) == strspn(usart_array, ",0123456789") && strlen(usart_array) == 59)
                {

			handshake=0; // Reset the handshake.
			wdt_reset(); // Reset the watchdog

			int Temps[4][4];
			int Td[4];

			sscanf(usart_array,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&Temps[0][0],&Temps[0][1],&Temps[0][2],&Temps[0][3],&Temps[1][0],&Temps[1][1],&Temps[1][2],&Temps[1][3],&Temps[2][0],&Temps[2][1],&Temps[2][2],&Temps[2][3],&Temps[3][0],&Temps[3][1],&Temps[3][2],&Temps[3][3],&Td[0],&Td[1],&Td[2],&Td[3]);

			//uart_puts(usart_array);

			// Summon the Controllers

			// Fan Bank 1
			int Ts=0;
		        int i;

		        for(i=0; i <=3; i++)
		        {
                		if (Temps[0][i] > Ts)
	        	        {
        	        	        Ts=Temps[0][i];
                		}
		        }

			Tm_1=MRAC(Ts,Td[0],Tm_1,1);

			// Fan Bank 2
			Ts=0;
		        for(i=0; i <=3; i++)
		        {
                		if (Temps[1][i] > Ts)
	        	        {
        	        	        Ts=Temps[1][i];
                		}
		        }

			Tm_2=MRAC(Ts,Td[1],Tm_2,2);

			// Fan Bank 3
			Ts=0;
		        for(i=0; i <=3; i++)
		        {
                		if (Temps[2][i] > Ts)
	        	        {
        	        	        Ts=Temps[2][i];
                		}
		        }

			Tm_3=MRAC(Ts,Td[2],Tm_3,3);

			//Fan Bank 4
			Ts=0;
		        for(i=0; i <=3; i++)
		        {
                		if (Temps[3][i] > Ts)
	        	        {
        	        	        Ts=Temps[3][i];
                		}
		        }

			Tm_4=MRAC(Ts,Td[3],Tm_4,4);

		}

		memset(usart_array, 0, 100);
                usart_array[0]='\0';
                wi=0;


        }
        else if (wi >= 60)

        {
		memset(usart_array, 0, 100);
                usart_array[0]='\0';
                wi=0;
		//handshake++;
		/*
		cli();
		wdt_reset();
		wdt_enable(WDTO_1S);

		uart_puts("RESETING!\r\n");
		for(;;)
		{
		}
		*/

        }
}



/*********************************************
* The main Loop
*********************************************/

void main(void) {

	uint8_t reset_source=MCUCSR;
	MCUCSR=0;

	init();
	InitPWM();

	OCR0 =128;
	OCR1A=128;
	OCR1B=128;
	OCR1C=128;


	//delay_1s();

	uart_puts("MCU online\r\n");

	if (reset_source & 0x01)
	{
		uart_puts("Power On Reset\r\n");
	}
	else if (reset_source & 0x02)
	{
		uart_puts("External Reset\r\n");
	}
	else if (reset_source && 0x04)
	{
		uart_puts("Brown-out Reset\r\n");
	}
	else if (reset_source && 0x08)
	{
		uart_puts("Watchdog Reset\r\n");
	}
	else if (reset_source && 0x10)
	{
		uart_puts("JTAG Reset\r\n");
	}
	else
	{
		uart_puts("WTF Reset\r\n");
	}



	sei(); // enable interrupts

	wdt_enable(WDTO_2S);


	for(;;)
        {

	//_delay_us(1);
	//wdt_reset(); // Reset the watchdog

	delay_1000();
	handshake++;

		if (handshake < 20)
		{
			wdt_reset(); // Reset the watchdog
			//uart_puts("Safe\r\n");
		}
		else
		{
			uart_puts("Bark Bark\r\n");
		}
	// Loop till the cows come home.

        }

}

void delay_1000(void)
{
   uint8_t i;
   for (i = 0; i < 100; i++)
      _delay_ms(10);
}
