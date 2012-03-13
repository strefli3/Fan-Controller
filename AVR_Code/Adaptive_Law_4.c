/*********************************************
* Chip type           : ATmega128
* Clock frequency     : 8000000Hz
*********************************************/

/*********************************************
* Adaptive Law
*********************************************/

#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MRAC.h"


volatile double g14=.00001;      // Gains for the adaptive Law
volatile double g24=.00001;     // Gains for the adaptive Law
volatile double g34=.0001;      // Gains for the adaptive Law


volatile double k14=0;           // Adaptive Law k1
volatile double k24=0;           // Adaptive Law k2
volatile double k34=0;           // Adaptive Law k3
volatile double e4=0;            // Adaptive Law e
volatile double u4=0;            // Adaptive Law u


double Adaptive_Law_4(int Ts, double Tm)
{
	e4=Ts-Tm;

	double k14_dot;
	k14_dot=g14*e4*Ts;
	k14_dot=PP(u4,k14,k14_dot);
	k14=k14-k14_dot;

	double k24_dot;
	k24_dot=g24*e4*Tm;
	k24_dot=PP(u4,-k24,k24_dot);
	k24=k24+k24_dot;

	double k34_dot;
	k34_dot=g34*e4;
	k34_dot=PP(u4,-k34,k34_dot);
	k34=k34+k34_dot;

	u4=(-k14*Ts+k24*Tm+k34);

	return u4;
}
