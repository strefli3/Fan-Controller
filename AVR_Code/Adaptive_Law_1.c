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


volatile double g11=.00001;      // Gains for the adaptive Law
volatile double g21=.00001;     // Gains for the adaptive Law
volatile double g31=.0001;      // Gains for the adaptive Law


volatile double k11=0;           // Adaptive Law k1
volatile double k21=0;           // Adaptive Law k2
volatile double k31=0;           // Adaptive Law k3
volatile double e1=0;            // Adaptive Law e
volatile double u1=0;            // Adaptive Law u


double Adaptive_Law_1(int Ts, double Tm)
{
	e1=Ts-Tm;

	double k11_dot;
	k11_dot=g11*e1*Ts;
	k11_dot=PP(u1,k11,k11_dot);
	k11=k11-k11_dot;

	double k21_dot;
	k21_dot=g21*e1*Tm;
	k21_dot=PP(u1,k21,k21_dot);
	k21=k21+k21_dot;

	double k31_dot;
	k31_dot=g31*e1;
	k31_dot=PP(u1,k31,k31_dot);
	k31=k31+k31_dot;

	u1=(-k11*Ts+k21*Tm+k31);


	return u1;
}
