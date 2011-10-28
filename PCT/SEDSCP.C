/****************************************************************/
/*								*/
/*   	S E   D S C P   					*/
/*								*/
/*		- Data scope program.   This segment handles	*/
/*		  Inputting of the byte & keyboard scan.	*/
/*		  Formatting & printing done in DSCP.ASM	*/
/*								*/
/****************************************************************/
	     

#include "Sekdef.h"
#include "se.h"
#define HILO(x) 	(((x<<4)&112) + ((x>>4)&15) ) /* reverse byte */

	extern byte lchar,crttim,tvmaxc,STDCOLOR;

	int	color2;
	byte	__sedscp,kbin();


void _sedscp_() {}


void sedscp()

{	int ch,x,d=0xff;
	byte key;

	Smallscr(NO);
	crttim=0;		/*  disable dimming   */
	rcvi();

	__sedscp=0;

	color2=HILO(STDCOLOR);

dsc1:   lchar=segetc();

dsc2:		while ( (kbhit()) || (d==0) ||(__sedscp) ) {

		   if (__sedscp) {
		   	key=__sedscp;
		   	__sedscp=0;
		   	}
                   else key=kbin();

			if (key==KCLEAR) {
					 __sedscp=0;
					 return;
					 }

			if (key=='C'){ 
					smallscr(NO);
					d=0xff;
					 }

			if (key=='S') d=0;

			if (key==' ') {
				d ^= 0xff;
				rcvi();
				}
			}
	
		ch=segetc();

		x=dscp(ch);

		switch (x) {
			case 1:
				goto dsc1;
			case 2:
				goto dsc2;
			}
}
