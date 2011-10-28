/****************************************************************/
/*								*/
/*   	S E  F O R M  - This program takes data and converts	*/
/*		it in to a 7-line ticket format.  Also, It 	*/
/*		gives labels to seat increments and related	*/
/*		information.					*/
/*								*/
/*								*/
/****************************************************************/
	     

/****************************************************************/
/*								*/
/*    This program takes ticket text & blank compressed		*/
/*	characters from strings TXSEC1 TXSEC2, & TXSEC3		*/
/*	and places them in TBF7, The buffer the simulates	*/
/*	a 7-line ticket.  In the ticket data, each blank 	*/
/*	has it's own byte. Also, data received in TXSEC4	*/
/*	are given labels for easy referencing.			*/
/****************************************************************/
#include "se.h"

	extern byte	txsec1[250];	/* Tixket - section 1      These are   */
	extern byte	txsec2[250];	/* Ticket - Section 2	   set directly */
	extern byte	txsec3[250];	/* ticket - Section 3	   from input   */
	extern byte	txsec4[250];	/* Ticket - section 4	   messages	*/

	extern byte	tfb7[308];	/* 7 line ticket buffer  (44 x 7)	*/
	extern byte	numtix;		/* number of tickets in pseudo- break   */
	extern byte	ltixi;		/* last ticket indicator (MORE/LAST)	*/
	extern int	seatpp[3];	/* seat print positions			*/
	extern int	trcpp;		/* return code print postion		*/
 	extern byte	seatnm[4];	/* ascii seat number 			*/
	extern byte	trcnm[8];	/* ascii return code			*/
	extern byte	seatit;		/* seat indicator  (NUM / LETTER)	*/
	extern char	seatin;		/* seat increment (cam be negitive)	*/
	extern int	rcinc;		/* return code inc (set: sradj used:seinsr) */ 
	extern int	ackflg;		/* terminal ack retry counter */
	extern byte	prstat;		/* printer status (2 = disabled, enabled otherwise) */
	extern int	auditl;
	extern byte	audit[56];

	extern byte	prtcbk[10];	/* ticket printer control block */

#define pstat	prtcbk[7]	/* printer status */

#define TFB7L   308
#define MAX1    112
#define MAX2     98
#define MAX3     98
	
#define HILO(x) (*(x++) * 256) +  *(x++) /* convert integer in cyber Hi:Lo*/
					

	static 	byte *p;		/* pointer into TFB7 */
	void	clear(),seclri();


/****************************************************************/
/*								*/
/*   	SEFORM - Main program.  Calls subroutines in 		*/
/*		 proper sequence.				*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
seform()	     

{	

	clear();

	p=tfb7;
	
	if (!format(txsec1,MAX1)) return(FALSE);
	if (!format(txsec2,MAX2)) return(FALSE);
	if (!format(txsec3,MAX3)) return(FALSE);
	if (!labels())	          return(FALSE);

	return(TRUE);
}
/****************************************************************/
/*								*/
/*   								*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
void clear()
{
	blank(tfb7,TFB7L);
	seclri();
}

/****************************************************************/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

void seclri()
{
	zero(seatpp,6);
	trcpp=0;
	blank(seatnm,4);
	zero(trcnm,8);
	seatit=0;
	seatin=0;
	auditl=0;

	blank(audit,56);
}
/****************************************************************/
/*								*/
/*   								*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
format(txp,y)
	byte *txp;
	int y;
{	int i,ch,z;

	++txp;

	z = *txp;

	for (i=0; i<z; ++i) {
		ch=*(++txp);
		if (ch > 128) {
			ch-=128;
			if (ch>y) return(FALSE);	
			p+=ch;
			y-=ch;
			}
		else {
			if (y==0) return(FALSE);
			y-=1;
			*(p++) = ch;
			}
		}

	return(TRUE);
}
/****************************************************************/
/*								*/
/*   								*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
labels()

{	int i;

	p=txsec4+5;
		  
	numtix = *p++;
	ltixi  = *p;
	p+=2;
	
	if (*p !=0) {
			++p;
			for(i=0;i<3;++i)
				seatpp[i] = HILO(p);
			p+=2;
			trcpp=HILO(p);

			strncpy(seatnm,p,4);
			p+=4;
			strncpy(trcnm,p,8);
			p+=8;
			seatit = *p;
			p++;
			seatin = *p;
			sercaj();
			}
	p++;

	auditl=*p;

	if (auditl)	strncpy(audit,++p,auditl);

	return(TRUE);
}
