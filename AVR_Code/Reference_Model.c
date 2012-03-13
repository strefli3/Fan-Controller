/*********************************************
* Chip type           : ATmega128
* Clock frequency     : 8000000Hz
*********************************************/

/*********************************************
* MRAC Reference Model
*********************************************/

#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MRAC.h"

double Ref_Model(int Td, double Tm)
{
	double Ref_Mod_dot;
	double am=0.001471;             // Reference model am
	double bm=0.001471;             // Reference model bm

	Ref_Mod_dot = bm*Td-am*Tm;

	Tm+=Ref_Mod_dot;

	Tm=Td;

   return Tm;
}


