/****************************************************************/
/*								*/
/*   	S E   T I M E 						*/
/*								*/
/*	Called once every second.  Updates timer counters.	*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

#include "se.h"

  	extern 	actime,ustime,crttim;

	int	rtstmr;
	     
void everysec()

{
	actime++;
	ustime++;

	if (rtstmr) {
		rtstmr-=1;
		if (rtstmr==0) NRTS0();
		}

	if (crttim) {
		crttim-=1;
		if (crttim==0) dim;
		}
  

}
