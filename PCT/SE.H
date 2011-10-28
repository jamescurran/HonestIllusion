/****************************************************************/
/*								*/
/*								*/
/*	  SE.H							*/
/*								*/
/*		All-purpose Header for S & E PCT.		*/
/*								*/
/*								*/
/****************************************************************/

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#ifndef LATTICE				/* LATTICE def if dos.h included */
typedef unsigned char  byte;
#endif

#define TRUE	 0xFFFF
#define FALSE	 0x0000

#define YES	 1
#define NO	 0

#define ON	 1
#define OFF	 0

#define LAST	0
#define NOTLAST 1

#define ever   (;;)
#define beep   128

/********** SECOMM .H *********************/

#define LOACT	0x20		/* "active" light. */
#define LOXR	0x04		/* "Transmit Request" light.*/
#define LOAXTP	0x26		/* active, Transmit req, Tran in progress */
#define LOXRTP	0x06		/* Transmit request, Transaction in Progress */
#define RTSTIM	  5
#define even(x) !(x&1)			   /* True if x is even */
#define odd(x)	 (x&1)			   /* True if x is odd	*/
#define FINISH  0x0F

/********** SECRT.H ***********************/

#define CR	0x0D
#define LF	0x0A
#define AT	0x09
#define CLS     0x0E
#define NORM	0x10
#define INVER	0x11
#define BLINK	0x12
#define DIMINV  0x15
#define BLANK   0x18

#define matrix(chr) if ( (prstatus(0)&0x28)||(prstat==2) ); else  prputc(chr)
#define crtprt() if (prstat!=2) prscreen()
#define crtrtc(x) curset((x & 0xff00)>>8, x & 0xff, 0)
#define buzzer	putch('\007')
#define dim	vpage(1)
#define bright	{vpage(0); crttim=0;}
#define tvmaxl	 24

/********** SESIO.H ***********************/
#define comm_port 0

/********* SEIN.H *************************/

#define NOXMIT	 0
#define XMIT	 1
#define FILL	 0xFF
#define clrfdx(x) zero(fdata[x],32)

/********* SETIX.H ************************/

#define ticket_port 1

#define sercaj() ;	     /*  for now */

/********* SETSW.H ************************/

#define POLLUPOK (itopt1 & 0x40)
