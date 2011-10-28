
/* 							        */
/*    SEEMSG                                                  	*/
/*								*/
/*  Structure EMSG is defined as :				*/
/*	First entry:  The message. Up to 46 characters, in      */
/*			double quotes.				*/
/*	Second entry: Line number to display message (1-24)	*/
/*			0 = last line. 				*/
/*	Third entry:  Column number to display message (0-79)	*/
/*	Fourth entry: Length of message.			*/
/*	Fifth entry:  Format byte 				*/
/*	Sixth entry:  Message number.				*/
/****************************************************************/
/*								*/
/*	Called by    SEEMSG(num);				*/
/* 	where        num  (bits 0-6) = message number.		*/
/*		          (bit  7) =   1 = beep.		*/
/*								*/

#define norm 7
#define blink 	 0x87 
#define reverse  0x20 		/*  Black on Green		*/
#define blnkrev  0xC0		/*  Black on Red, blinking	*/
#include "se.h"

	static struct  
       {byte *msg;
	int  line;
	int  col;
	int  len;
	int  format;
	int  num; } emsg[35] =

       {"Printer Bad",0,0,11,blnkrev,1,
	"Printer Not Ready",0,0,17,blnkrev,2,
	"Out of Stock ",0,0,13,blnkrev,3,

	"Busy Bit ?",0,0,10,reverse,4,
	"Line Inactive ?",0,0,15,reverse,5,
	"Line Too Fast, Re-trying",0,0,24,blink,6,

	"Ticket Receive Error",15,1,20,blnkrev,7,
	"Reprint, Void, Return Now",16,1,26,blnkrev,8,
	"Send Error",16,1,10,blnkrev,9, 

	"Program Reloading, Please Wait",8,1,30,reverse,10,
	"Do Not Use PTIX",0,0,16,blnkrev,11,

	"Invalid Card",0,0,12,reverse,12,
	"Card Not Valid Yet",0,0,18,reverse,13,

	" 87-1  12/31/87 ",0,0,16,reverse,14,
	"Communications Error",15,1,20,reverse,15,
	"Bad Response to ACK",0,0,19,reverse,16,

	"Slow Matrix",0,0,12,reverse,18,
	"Matrix Disabled",0,0,16,reverse,19,
	"Fast Matrix",0,0,12,reverse,20,

	"Disk Write Error",0,0,16,blnkrev,21,
	"Temp file will be overwritten next raw data report",0,0,50,reverse,22,
	"RENAME Successful",0,0,17,reverse,23,
	"RENAME Unsuccessful",0,0,19,reverse,24,
	"Can't Open Disk File",0,0,20,blnkrev,25,

	"Warning: Ticket Printer Ribbon Near End",0,0,39,reverse,30,

	"Invalid Key",0,0,11,reverse,50,
	"Scroll On",0,0,9,reverse,51,
	"Scroll Off",0,0,10,reverse,52,
	"Turn Key to MGT",0,0,15,reverse,53,
	"Invalid Special Field Entry",0,0,28,reverse,54,
	"Preset Storage Full",0,0,20,reverse,55,
	"Preset Empty",0,0,12,reverse,56,
	"Invalid Preset",0,0,14,reverse,57,
/*	"COM Test Program",1,25,16,blnkrev,100,
	"Poll Code (0-F)=",3,1,16,reverse,101,
	"Enter MSG followed by TRANSMIT or LAST or STOP",5,1,46,norm,102,  */
		};
   	static int ref;  
	static byte blanks[65] =
	"                                                            ";
	extern backg;

void seemsg (msgn)
int msgn;

{
	int  line,col,i;
	unsigned cur;

	if (msgn > 128){
		  buzzer;
		  msgn -= 128;}

	for (i=0; i < 35; ++i)
		if (emsg[i].num == msgn) break;

	if (i <35) {

		ref = i;

		line = (emsg[i].line == 0) ? tvmaxl:emsg[i].line-1;
		col  = emsg[i].col;

		cur=getcur(0);
	       	ratsay(line, col, emsg[i].format, 0, emsg[i].msg);
		crtrtc(cur);
	}
}
void seeclr()
{
 	int line,col;
	unsigned cur;

	if (ref ==0) return;

	line = (emsg[ref].line == 0) ? tvmaxl:emsg[ref].line-1;
	col  = emsg[ref].col;

	cur=getcur(0);
	blanks[emsg[ref].len]='\0';
       	ratsay(line, col, backg*16, 0, blanks);
	blanks[emsg[ref].len]=' ';
	crtrtc(cur);
	ref = 0;
}

