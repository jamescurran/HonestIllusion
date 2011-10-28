/************************************************************************/
/*									*/
/*	S E   M G S T - 						*/
/*									*/
/*		Mag stripe reader driver				*/
/*									*/
/*									*/
/************************************************************************/

#define ACK	     0x04
#define NAK	     0x03
#define STX          0x02
#define ETX          0x03
#define DELIM        0x5E
#define NOCHAR	     0xFF

#define ccard_port   1
#define incard in(ccard_port,&st)
 

#include "se.h"
#include "sefdef.h"

	static	byte    c,ck1,ck2;
	static  int     fldp1,fldp2,fldp3,flg,st;
		int	magflag;

	extern	int	fldpos,fieldp,actime,mkfldp,maskp;

	void	getnam(),clip(),getexp(),getval(),comcks(),savepos();
	void	putit(),error2();


semgst()
{
	byte incardnz();		/* define the function. */
	int	x;

	rts1();
	
	if (magflag==TRUE) return(FALSE);

	x=outnz(ccard_port,STX);

	if (x) {
		magflag=TRUE;
		return(FALSE);
		}

	if ((c=incard) != STX) return(FALSE);
	if (st) 	       return(FALSE);

	ck1 = STX;			/* Start off both checksums */
	ck2 = STX;

	while ((c=incardnz()) != 'B') {  /* Skip "B" or "1B" at beginning */
		if (c==ACK) return(FALSE);
		}

	savepos();

	flg =0;
	
	x=getcrd();			/* Get card account number */

	if (x==FALSE) {
			outnz(ccard_port,ACK);
			goto done;
			}

	if ( (MSKCODE(maskp)!=1) && (MSKCODE(maskp)!=41) ) {
			outnz(ccard_port,ACK);
			goto done;
			}

	getnam();			/* Get Holder"s name */

	flg=0;

	getexp();			/* Get the cards experation date */

	flg=0;

	getval();			/* Get the cards starting date (if there). */

	flg=0;

	comcks();			/* Compare checksums */

done:	fldpos=fldp2;
	fieldp=fldp1;
	mkfldp=fldp3;

	dspfld();
	iclr1();

	
}

/*************************************************************************/

getcrd()
{	int x;

	x=locfld(FCARD);
	if (!x) return(FALSE);

	KEYPRO(FCARD);


	while ((c=incardnz())!=DELIM) {
		if (c!=' ') putit(c);
		}

	return(TRUE);

}
/*************************************************************************/

void getnam()
{
	keypro(FNAME);

	while ((c=incardnz())!=DELIM) {
		if (c==' ') {
			if (flg & 0x02) return;
			else flg |= 0x02;
			}
		else {
			flg &=0xFD;
			if (c=='/') c=',';	/* Change slash to a comma */
			}

		if (flg & 0x01) {
				clip();
				return;
				}
		else putit(c);
	}
}

void clip()
{
	if (flg & 0x02) return;

	bckspc();
	flg=0;
	putit('+');
}


/*************************************************************************/
void getexp()
{
	byte  yy[2];

	keypro(FEXP);
	
	do {c=incardnz();
		} while (!isdigit(c));

	yy[0]=c;				/* Change from YYMM to Mmm YY  */
	yy[1]=incardnz();

	putit(incardnz());
	putit(incardnz());
	
	putit(yy[0]);
	putit(yy[1]);
	
}



void getval()
{	byte ch[4];
 	int  i;

	for (i=0; i<4; i++) {		/* Try to get 4 more numbers from the card */
			c = incardnz();
			if (c==ETX) return;
			ch[i]=c;
			}

	if (ch[0] !='8') return;  /* if we got less than 4 or the first isn't */
				  /* a digit, it can be ignored.		    */	

	keypro(FCDVAL);

	for (i=0;i<4;i++)
		putit(ch[i]);


}


byte incardnz()
{
	actime=0;

	do {
		c=incard;		 /* Get the next Character */
		if (actime==1) return(ACK);
		} while (st==2);


	ck1+=c;				/* Add to one checksum       */
	ck2^=c;				/* XOR into the other        */
	c= c & 0x7f;

	return(c);			/* Return without parity bit.*/
}
/***********************************************************************/

void comcks()
{    
	while (c != ETX) {
			  c=incardnz(); /* skip rest of card (add to checksums) */
			}
	incardnz();		    	/* get checksum (incardnz adds it in) */

	ck1 &= 0x7f;			/* remove high bit from both */
	ck2 &= 0x7f;


	if ((ck2!=0) && (ck1!=0)) outnz(ccard_port,NAK);
	else   	               	  outnz(ccard_port,ACK);
}

/************************************************************************/
/*									*/
/*									*/
/*									*/
/*									*/
/*									*/
/*									*/
/************************************************************************/

void savepos()
{	int FC;

	FC=FIELDCD(fieldp);

	if ((FC==FNAME) || (FC==FEXP)) fwdfld();

	if ( FC==FCARD) {
			fwdfld();
			fwdfld();
			}
	fldp1=fieldp;
	fldp2=fldpos;
	fldp3=mkfldp;
	
}

/************************************************************************/
/*									*/
/*									*/
/*									*/
/*									*/
/*									*/
/*									*/
/************************************************************************/

void putit(c)
	byte c;

{
	int x;

	if (flg & 0x01) error2();
	else {
		x=insert(c);
		if (!x) flg |= 0x01;
		dspfld();
		}
}

void error2()

{	getkey();

    	rclose();
	exit();

	keypro(FCARD);
	keypro(FNAME);
	keypro(FEXP);
	seemsg(12);

	outnz(ccard_port,ACK);

/*	goto done; */
}
