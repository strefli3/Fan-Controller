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


volatile double g13=.00001;      // Gains for the adaptive Law
volatile double g23=.00001;     // Gains for the adaptive Law
volatile double g33=.0001;      // Gains for the adaptive Law


volatile double k13=0;           // Adaptive Law k1
volatile double k23=0;           // Adaptive Law k2
volatile double k33=0;           // Adaptive Law k3
volatile double e3=0;            // Adaptive Law e
volatile double u3=0;            // Adaptive Law u


double Adaptive_Law_3(int Ts, double Tm)
{
	e3=Ts-Tm;

	double k13_dot;
	k13_dot=g13*e3*Ts;
	k13_dot=PP(u3,k13,k13_dot);
	k13=k13-k13_dot;

	double k23_dot;
	k23_dot=g23*e3*Tm;
	k23_dot=PP(u3,-k23,k23_dot);
	k23=k23+k23_dot;

	double k33_dot;
	k33_dot=g33*e3;
	k33_dot=PP(u3,-k33,k33_dot);
	k33=k33+k33_dot;

	u3=(-k13*Ts+k23*Tm+k33);

	return u3;
}
