/********************************************* * Chip type : ATmega128 * Clock frequency : 8000000Hz 
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

volatile double g12=.00001;      // Gains for the adaptive Law
volatile double g22=.00001;     // Gains for the adaptive Law
volatile double g32=.0001;      // Gains for the adaptive Law


volatile double k12=0;           // Adaptive Law k1
volatile double k22=0;           // Adaptive Law k2
volatile double k32=0;           // Adaptive Law k3
volatile double e2=0;            // Adaptive Law e
volatile double u2=0;            // Adaptive Law u


double Adaptive_Law_2(int Ts, double Tm)
{
	e2=Ts-Tm;

	double k12_dot;
	k12_dot=g12*e2*Ts;
	k12_dot=PP(u2,k12,k12_dot);
	k12=k12-k12_dot;

	double k22_dot;
	k22_dot=g22*e2*Tm;
	k22_dot=PP(u2,-k22,k22_dot);
	k22=k22+k22_dot;

	double k32_dot;
	k32_dot=g32*e2;
	k32_dot=PP(u2,-k32,k32_dot);
	k32=k32+k32_dot;

	u2=(-k12*Ts+k22*Tm+k32);

	return u2;
}
