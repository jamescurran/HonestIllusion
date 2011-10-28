/****************************************************************/
/*								*/
/*   	S E   T I X 						*/
/*								*/
/*	Ticket Printing Routines 				*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     

#include "se.h"
#include "sekdef.h"

#define REQ     1 
#define GET     0

#define stat_req     0x05
#define FFeed	     0x0C

#define out_of_stock 0x10
#define X_ON         0x11
#define X_OFF	     0x13
#define ribbon_low   0x16
#define ribbon_out   0x17
#define ticket_jam   0x18
#define ACK	     0x06
#define NAK	     0x15

	extern byte tfb7[][44];
	extern byte numtx;
	extern int actime,STDCOLOR,ENGFLG;

	byte	__SETIX;
	int	err=0;

	byte    ntick[8][52];

	void	septb(),trans(),trans1(),ptick(),print_tix_line();

/****************************************************************/
/*								*/
/*   SEPTB - Print ticket break.				*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

void septb()

{	int x,c;
	byte	getstatus();

	iclr1();
	rts1();

	if ( (c=getstatus(REQ)) == X_OFF) {
					c=badprinter(2);
					if (c==FALSE) return;
					}
	for (x=1;x<=numtx;++x) {
			trans();
			ptick(ntick,(x==numtx) ? LAST:NOTLAST);
			seinsr();
			}
}
	     

/**************************************************************************/
/*  getstatus - inputs the status of the ticket printer.                  */
/*									  */
/*  getstatus(GET) returns the last status sent from the printer.	  */
/*  getstatus(REQ) transmits a status request first, then receives and    */
/*                 returns the new status.				  */
/**************************************************************************/

byte getstatus(mode)
    int mode;
{
	byte c;
	int st;
	
	if (mode == REQ) {
			iclr1();
			outnz(ticket_port, stat_req); 
			}

	actime =0;

	do {
		c=in(ticket_port,&st);
		testss();
		if (actime==3) break;
		}  while (st==2);

	if (st==2) c=X_OFF;
	
	return (c);
}
/*********************** end of getstatus *********************************/


/**************************************************************************/
/* 									  */
/* service_status - gets status and displays appropriate warning messages */
/*		    Mode set as in getstatus.				  */
/*									  */
/* Returns when X_ON (0x11) received. 					  */
/*									  */
/* Returns Error condition						  */
/*  Bit 0 (x01) Hard Error indicator  0 = printer ok   1 = hard error     */
/*  Bit 1 (x02)	Ribbon Low indicator  0 = ribbon ok.   1 = ribbon low.	  */
/*									  */
/*    (Bit 0 indicates that a hard error had occured.  Control will not   */
/*     return to the calling program until after it is corrected.)        */
/*									  */
/**************************************************************************/

service_status(mode,err)
	int mode,err;
{
	byte 	c;
	int	b;

	err &= 6;   /* remove hard error bit, leave ribbon low */
 	b=FALSE;

	do {
		c=getstatus(mode);

		switch (c) {
		case out_of_stock:
			badprinter(3);
			err|=1;
			break;	
		case ribbon_low:
			err|=2;
			b=TRUE;
			break;
		case ribbon_out:
			badprinter(1);
			err|=1;
			break;
		case ticket_jam:
			badprinter(1);
			err|=1;
			break;
		case X_OFF:
			badprinter(2);
			break;
		case X_ON:
			b=TRUE;
			break;
		}
	} while (b=FALSE);

	seeclr();

	return (err);
}
/*********************** end of service_status ***************************/



/*************************************************************************/
/*									 */
/*  getack()  Gets Ack or Nak from ticket printer.			 */
/* 									 */
/*  Returns  0 = ACK (0x06) received.					 */
/*           1 = NAK (0x15) received.					 */
/*    (will not return until one or the other is received.)		 */
/*									 */
/*************************************************************************/

getack()
{
	byte c;

	while ( ( (c=getstatus(GET)) != ACK) && (c!=NAK) ) {kbhit();}

	return ( (c==NAK) ? 1 : 0);
}
/************************** end of getack ********************************/

/*************************************************************************/
/*									 */
/*  trans()  Translates 44 x 7 ticket into 50 x 7 with double sized char */
/* 									 */
/*  									 */
/*           								 */
/*    									 */
/*									 */
/*************************************************************************/

void trans()

{	int i;

	for (i=0;i<7;++i) {
			strncpy(ntick[i],tfb7[i],44);
			strncpy(&ntick[i][44],"      ",6);
			}
		 	       
	trans1(2);
	trans1(3);
	trans1(5);
	trans1(7);
}


void trans1(x)
	int x;

{	int i;
	byte b,c;

	x--;
 
	for (i=0;i<6;i++) {
			c=tfb7[x][38+i];
			if (c==' ') b=' ';
			else b=0x1f;
			ntick[x][39+2*i]=c;
			ntick[x][38+2*i]=b;
			}
}

/*************************************************************************/
/*									 */
/*  ptick(tick,ticnum) - print ticket.					 */
/*									 */
/*	tick[8][51] array of ticket data.				 */
/*	ticnum - LAST = this is the last ticket of break (display ribbon */
/*								  low)   */
/*		 NOTLAST = this is not the last ticket.			 */
/*									 */
/*************************************************************************/


void ptick(tick,ticnum)
	byte tick [8][52];
	int ticnum;

{
	int i;
   	err=service_status(REQ,err); 
      
	do {
	    for (i=0; i<7; ++i) { print_tix_line(ticket_port, tick[i]); }
	    outnz(ticket_port,FFeed);
	    testss();
            } while (getack());  

	  while ( (err=service_status(GET,err))  & 1) {};

       if ((ticnum==LAST) && (err & 2)) {
					seemsg(30);
					err=4;
					}

}
/************************** end of ptick **********************************/


/**************************************************************************/
/*  print_tix_line - prints one line of a ticket.			  */
/*	ch - port to output data to (ticket_port)			  */
/*	s  - points to line of ticket to be printed			  */
/**************************************************************************/

void print_tix_line(ch, s)
int ch;
byte *s;
{

	while(*s)  {
		if (*s=='>') *s=' ';
		if (*s==0xEC) *s=0x1F;
	      outnz(ch,*s++);
	}
	
	outnz(ch,CR);
}

/*************************************************************************/


badprinter(e)
	int	e;
{
	byte	c,kbin();
	int	d;

	seemsg(e+beep);
	curset(18,1,0);
	rprints(STDCOLOR,0,"Fix ticket printer & press ENTER to");
	rprints(STDCOLOR,0,"\n   continue.");

	if (ENGFLG) {
		rprints(STDCOLOR,0,"or CLEAR (F10) to skip");
		rprints(STDCOLOR,0,"\nticket printing (You must then void");
		rprints(STDCOLOR,0,"\nthe ticket)...");
		}  	

kbloop:	c=kbin();
	if (c==KNFLD) d=TRUE;
	   else if (c==KCLEAR) d=FALSE;
		else goto kbloop;

	curset(18,1,0);
	rprints(STDCOLOR,0,"                                      ");
	rprints(STDCOLOR,0,"\n                                      ");

	if (ENGFLG) {
		rprints(STDCOLOR,0,"\n                                      ");
		rprints(STDCOLOR,0,"\n                                      ");
		}

	return(d);
}
