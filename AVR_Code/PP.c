/*********************************************
* Chip type           : ATmega128
* Clock frequency     : 8000000Hz
*********************************************/

/*********************************************
* Parameter Profection
*********************************************/

#include <avr/io.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MRAC.h"
#include "globals.h"

double PP(double u, double k, double kd)
{

	if((u >= uh && kd >= 0) || (u <=ul && kd <= 0))
	{
		kd=0;
	}
	else
	{
		kd=kd;
	}

	return kd;
}

