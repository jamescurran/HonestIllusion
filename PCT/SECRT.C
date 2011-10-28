/****************************************************************/
/*								*/
/*	S E   C R T						*/
/*								*/
/*		Routines, DEfines, & includes controlling	*/
/*		CRT display, and Matrix printing.		*/
/*								*/
/*								*/
/****************************************************************/

#include <dos.h>
#include "se.h"
#include "sefdef.h"
#include <grnleaf\color.h>

	extern	union	REGSS inregs,outregs;

		byte	___SECRT=0;
		int	col1st=0;	/* Leftmost column for TTY	*/
		int	blk=NO;		/* No Blinking			*/

		byte	__secrt;
		int	noscrl; 	/* Flag: set when no scrolling allow */
		int	tvmaxc;		/* Maximun character on a line(80/40)*/
		int	crttim;		/* Time until crt dims (no used)     */
		int	cpsc2;		/* Save "other" cursor position      */
					/*	on split screen 	     */
	static	int	atflag,line;
		int	backg;		/* Background for current character  */
		int	foreg;		/* Foreground for current character  */
		int	STDBACK;	/* Background used for most printing */
		int	STDFORE;	/* Foreground used for most printing */
		int	STDCOLOR;	/* Attribute for most printing       */
		int	border;		/* Color of border (not used on EGA) */

	extern	int	prfore,prback,label,maskp;
	extern	byte	prstat;
	extern	int	mono;

	void	 _secrt_(),ttyt(),crtclr(),swpscr(),largescr(),smallscr();

void _secrt_() {}


/***********************************************************************/


byte crtout(chr)
	byte chr;

{
	if (atflag==1) {
		atflag=0;
			if (col1st ==0) {
					curset(line,chr-32,0);
					return(0);
					}
			else chr=LF;
	}


	if (atflag==2) {
		atflag=1;
		line = chr-30;
		return(0);
		}

	if (chr==0) return(0);
	if (chr==FINISH) return(0);

	if (chr > 31) {
			rputcinc(chr,backg,foreg,blk,0);
			return(chr);
			}

	if (chr==CLS) {
		if (SMALLRES(maskp))	smallscr(YES);
		else 			largescr(YES);

		return(LF);
		}

	if (chr==NORM) {
		backg=STDBACK;
		foreg=STDFORE;
		blk=NO;
		return(0);
		}

	if ( (chr==INVER) ||(chr==DIMINV) ) {
		backg=STDFORE;
		foreg=STDBACK;
		blk=NO;
		return(0);
		}

	if (chr==BLANK) {
		backg=STDBACK;
		foreg=STDBACK;
		blk=NO;
		return(0);
		}

	if (chr==BLINK) {
		blk=YES;
		return(0);
		}

	if (chr==AT) {
		atflag=2;
		return(0);
		}

	if (chr==CR)  {
			crtrtc((getcur(0) & 0xff00) + col1st);
			return(CR);
			}

	if ((chr==LF) && (getcur(0) > 0x17ff))	{
		if (col1st) upscroll(1,10,tvmaxl,col1st,tvmaxc,STDCOLOR);
		else	    upscroll(1, 0,tvmaxl,     0,tvmaxc,STDCOLOR);

		return(LF);
		}

	if (chr==LF)   {
			crtrtc((getcur(0) & 0xff00) + col1st+256);
			return(LF);
			}
}
/***********************************************************************/

void ttyt(chr)
	byte chr;
{
 	chr=crtout(chr);
	matrix(chr);
}

/***********************************************************************/

void crtclr(x)
	int x;

{
	while (x) {
		putchar(' ');
		if ((getcur(0) & 0xff) == tvmaxc) putch('\n');
		x-=1;
		}
}
/***********************************************************************/


void swpscr()

{	int	x;

	if (tvmaxc==39) return;

	x=getcur(0);
	crtrtc(cpsc2);
	cpsc2=x;

	col1st ^=  40;	/* change 0 to 40;  40 to 0 */

}
/***********************************************************************/

void largescr(y)
	int 	y;
{
	int	x,z,w;
	if (mono) {
				cls();
				return;
				}
 
	inregs.h.ah = 0x0f;
	int86s(16,&inregs,&outregs);
	if (outregs.h.al !=1) {
				vmode(1); 
 			 	palette(0,border);
				}


	tvmaxc=39;
	crttim=0;		/* =120 for screen blank */

	w=backg*4096+32;
	if (y==YES) z=prback*4096+32;
	else        z=w;

	for (x=0;x<481;x+=2)	pokew(0xb800,x,z);

	for (x=480;x<2001;x+=2) pokew(0xb800,x,w);
	
 	if (outregs.h.al !=1) palette(0,border);

}
/***********************************************************************/

void smallscr(y)
	int	y;
{
	int	x,z,w;

	if (mono) {
				cls();
				return;
				}
 
	inregs.h.ah = 0x0f;
	int86s(16,&inregs,&outregs);
	if (outregs.h.al !=3) {
				vmode(3); 
 			 	palette(0,border);
				}

	w=backg*4096+32;
	if (y==YES) z=prback*4096+32;
	else        z=w;

	tvmaxc=79;
	crttim=0;		/* = 120 for screen blank */
 	cpsc2=0x0023;

	for (x=0;x<961;x+=2) 	pokew(0xb800,x,z);

	for (x=960;x<4001;x+=2) pokew(0xb800,x,w);

}