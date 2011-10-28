/****************************************************************/
/*								*/
/*     S E   U T lities						*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
#include <dos.h>
#include "se.h"
#include <grnleaf\color.h>

	static byte lamps;
	static byte lampline[41];

	union	REGSS inregs,outregs, filregs;

	extern	tvmaxc;

	void	_seut_(),zero (),blank(),lampon(),lampoff(),lamp();
	void	ascirj(),ascilj(),scrlup(),scrldn();


void _seut_()
 {}


void zero (area,len)
	byte area[];
	int len;

{	int i;

	for (i=0; i < len; i++) {area[i] = 0;};
}

/****************************************************************/

void blank(area,len)
	byte area[];
	int len;

{	int i;

	for (i=0; i < len; i++) {area[i] =' ';};
}


void lampon(x)

	int	x;
{
	lamps |= x;
/*	lamp();     */
}

void lampoff(x)
	int	x;
{

	lamps &= -(x+1);
/*	lamp();  */
}

void lamp()
{	int x,y,z;

	x= (tvmaxc+1) * 48+1;   /* x= address of attribute of 1st char of
last line */
	for (y=128;y>0;y>>=1) {
		if (lamps & y) z=BROWN * 16;
		else	       z=BLUE  * 16;
		pokeb(0xb800,x,z);
		x+=4;
		}			
	

}							  
/****************************************************************/
/*								*/
/*   	STRINS - Insert one string on top, of another 		*/
/*		 for specifity number of characters.		*/
/*								*/
/*	returns - Pointer to end of Destination.		*/
/*								*/
/*								*/
/****************************************************************/

byte *strins(p,s,n)
	byte *p,*s;
	int	n;
{
	for(;n>0;--n) {
			*p=*s;
			p++;
			s++;
			}
	return(p);
} 	     

/****************************************************************/
/*								*/
/*   	ADASCI - add an integer value to a number written as	*/
/*		 an ASCII string.				*/
/*								*/
/*		Pass address of the string, value to be added	*/
/*		and length of ascii number.			*/
/*								*/
/****************************************************************/
	     
adasci(ascnum,num,len)

	byte ascnum[];
	int	num,len;
{
	int	pos,flg,x,t,n;
	byte	c;

	flg=FALSE;

	pos=TRUE;
	if (num < 0) pos = FALSE;

	for (x=0;x<len;++x) {
		c=ascnum[x];
		if (c==' ') {
			if (flg) return (FALSE);}	/* imbedded space */
		else 	    {
			flg=TRUE;
			if (isdigit(c)==0) return(FALSE); /* non-digit     */
			}
		}
	if (flg==FALSE) return(FALSE);			/* all spaces     */

	for (x=len-1;x>=0; --x) {
		c=ascnum[x];
		if (c==' ') c='0';
		t=num / 10;
		n=num % 10;
		c +=n;					/* actual add   */

		if (pos) {
			if (c> '9') {
				c-=10;
				++t;
				}
			}

		else    {
			if (c < '0') {
			c+=10;
				--t;
				}
			}

		ascnum[x]=c;

		if (t==0) break;
		num=t;
		}

	return(TRUE);

}
/****************************************************************/
/*								*/
/*   	 ASCIRJ - Right justify an ASCII string.		*/
/*								*/
/*		Pass address of the string & length.		*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

void ascirj(area,len)
	byte area[];
	int len;
{
	int x,y,z;

	len--;

	for (y=len; isspace(area[y]); y--) {};

	if (y<0) return;		/* All Spaces */
	if (y==len) return;		/* No Spaces */

	x=len;
	for(z=y; z>=0; --z,--x)
		area[x]=area[z];

	strblf(area,len-y);
}

/****************************************************************/
/*								*/
/*   	ASCILJ - left justify an ASCII string.			*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
void ascilj(area,len)

	byte area[];
	int  len;
{
	int x,y,z;

	len--;

	for (y=0;isspace(area[y]); ++y) {};
	
	if (y==0) return;		/* No blanks */
	if (y>len) return;		/* All Blanks */

	x=y;
	for (z=0; z<=len-y; ++z,++x)
		area[z]=area[x];

	strb(area,z,y);
}

/********************************************************/
/*							*/
/*  scroll routines - use software bios video int 10	*/
/*                                              	*/
/*							*/
/*							*/
/********************************************************/				


/********************************************************/
/*							*/
/*  scrlup() Scroll window in active page up		*/
/*           with attribute                     	*/
/*							*/
/*							*/
/*							*/
/********************************************************/				

void scrlup(num,attr,fr,fc,tr,tc)
byte num, attr, fr, fc, tr, tc;

{
	inregs.h.ah = 6;
	inregs.h.al = num;
	inregs.h.bh = attr;
	inregs.h.ch = fr;
	inregs.h.cl = fc;
	inregs.h.dh = tr;
	inregs.h.dl = tc;

	int86s(0x10,&inregs,&outregs);
}
 	
/********************************************************/
/*							*/
/*  scrldn() Scroll window in active page down		*/
/*           with attribute                     	*/
/*							*/
/*							*/
/*							*/
/********************************************************/				

void scrldn(num,attr,fr,fc,tr,tc)
byte num, attr, fr, fc, tr, tc;

{
	inregs.h.ah = 7;
	inregs.h.al = num;
	inregs.h.bh = attr;
	inregs.h.ch = fr;
	inregs.h.cl = fc;
	inregs.h.dh = tr;
	inregs.h.dl = tc;

	int86s(0x10,&inregs,&outregs);
}
