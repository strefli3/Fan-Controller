/*********************************************
* Chip type           : ATmega128
* Clock frequency     : 8000000Hz
*********************************************/

/*********************************************
* MRAC
*********************************************/

#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MRAC.h"

//int uh=24;
int uh=76;
int ul=-70;

double MRAC(int Ts, int Td, double Tm, int Fan_Bank)
{
	double u;
	u=24; // Just in case....

	if (Ts != 0)
	{

		Tm=Ref_Model(Td,Tm);

		switch(Fan_Bank)
		{
		case 1:
			u=Adaptive_Law_1(Ts,Tm); 
			break;
		case 2:
			u=Adaptive_Law_2(Ts,Tm); 
			break;
		case 3:
			u=Adaptive_Law_3(Ts,Tm); 
			break;
		case 4:
			u=Adaptive_Law_4(Ts,Tm); 
			break;
		}


		if (u > uh-1)
		{
			u = uh;
		}
		else if (u < ul+1)
		{
			u = ul;
		}

	}
	else
	{
		u=-128;
		//u=24; // Lets not let the controller turn the fans off..
	}


	switch(Fan_Bank)
		{
		case 1:
			OCR0=(int)(128+u); 
			break;
		case 2:
			OCR1A=(int)(128+u); 
			break;
		case 3:
			OCR1B=(int)(128+u); 
			break;
		case 4:
			OCR1C=(int)(128+u); 
			break;

	}
	char str[100];
	//sprintf(str, "Fan_Bank %d: %d, Tm: %2.5f\r\n",Fan_Bank, (int)(128+u),Tm);
	sprintf(str, "Fan_Bank %d: %d, Ts: %d, u: %2.5f, Tm: %2.5f\r\n",Fan_Bank, (int)(128+u),Ts,u,Tm);
        uart_puts(str);

	return Tm;

}
