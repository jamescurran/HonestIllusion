/****************************************************************/
/*								*/
/*	S E I N I T						*/
/*								*/
/*	S & E  PC terminal initialization routine.		*/
/****************************************************************/

#include <dos.h>		/* Defines register names	*/
#include "se.h"			/* Standard PCT Header file 	*/
#include "sekdef.h"		/* Keyboard Definations     	*/
#include <fcntl.h>		/* Defines Flags for OPEN	*/
#include <grnleaf\color.h>	/* Defines colors		*/

	extern	unsigned int	itstat,itopt1,npres,toppre,custno,mode;
	extern	byte		unitsl,usbase,pollcd,vernum,prstat;
	extern	int		STDBACK,STDFORE,STDCOLOR;
	extern	int		foreg,backg,fldfore,fldback;
	extern	int		tvmaxc,crttim,hlpfor,hlpback,cpsc2,magflag;
	extern	int		label,prback,prfore,border;
	extern	int		col1st,blk;
	extern	int		ENGFLG,escflg;

	extern			everysec();
	extern	union	REGSS inregs,outregs;

	static	byte	*mdnam[9]= {
			{"0"},
			{"      Theatre (SE) "},
			{"        Camp"},
			{"3"},
			{"Theatre - Telephone (CSE)"},
			{"5"},
			{"6"},
			{"7"},
			{"Camp - Telephone (CCMP)"}
			};

	static	byte	*name = "seterm.pra";  /* name of parameter file */

#define	MAXMEM (unsigned) 0xA000    /* Highest usable memory address (for presets) */
#define NOPROTECT S_IREAD|S_IWRITE 

#define col(x)  (x!=STDBACK) ? (x+STDBACK*16) : STDCOLOR
	/* Col(x) return an screen attribute byte which is comprised of */
	/*  color x on the stanard background color, unless x is STDBACK*/
	/*  in which case the stanard color scheme is used.		*/

	int	file;		/* File number of prameter file		*/
	int	status;		/* status of disk read			*/
	int	termbuf[18];	/* Input buffer for parameter file	*/
	int	termtype;	/* Type of terminal (1=TRS, 2=SHU)	*/
	int	mono;		/* TRUE if this is a Monochrone monitor */

	void	init(),asinit(),goodbye(),inithelp(),colorchg(),termstat();
	void	modechg(),datain(),dataout();

	byte	kbin();

void init(flg)
	int	flg;	/* flg==TRUE if paramenter on Command line */
			/*   Maybe we can find a use for it......  */

{
	unsigned	int i,x;

	datain();	/* Read SETERM.PRA for Poll Code etc. */

	tickon(&everysec);  /* Start Timers */

	mono=FALSE;
	x=equipment(0);	/* do we have a monochrome board ?? */
	if (x==7) {
			mono =TRUE;
			STDFORE=15;
			prfore=15;
			fldfore=15;
			hlpfor=15;
			STDBACK=0;
			prback=0;
			fldback=0;
			hlpback=0;
			border=0;
			tvmaxc=79;
			}
	
	inithelp();		/* Prints opening Screen */

	asinit(comm_port,  1200,0,1,8);
	asinit(ticket_port,9600,2,2,7);

/*	rclose();		/* close port, just in case.... */
	icm();			/* Start Buffered input		*/

	lampon(0xC0);	       	/* "On" & "Rdy" lamps (not used */

	pollcd=unitsl & 0x0F;
	unitsl= usbase;
	
	magflag=FALSE;
	escflg=0;

	i=_memsize();
	toppre= i*64;
	if (toppre > MAXMEM) toppre= MAXMEM;

	x=&_iob; 		  /* This section attempts to calculate   */
			          /* the total length of the program, by  */
	x=(getds()/64)+(x/1024);  /* assuming that _iob is at, or at     */
				  /* close to the end of the program.     */
	npres= (i-x)/8;		  /* This assures that the presets, which */
	if (npres>26) npres=26;   /* are coming down from the top of 	  */
				  /* memory do not over-write the program */
}
/**************************************************************************/
/*									  */
/*	ASINIT - This routine duplications the actions (as much as	  */
/*		 needed) of the similar named Greenleaf function (Which	  */
/*		 I had trouble linking in).  It initializes the COM ports */
/*									  */
/*************************************************************************/
void asinit(port,baud,parity,stop,data)
	int	port,baud,parity,stop,data;
{
	int	y;

	inregs.h.dh = 0;
	inregs.h.dl = port;

	if (baud ==1200) y=0x0080;
	else		 y=0x00E0;
	
	y+= ((parity*12)&0x18) + ((stop*2)&4) + 2 + (data >>3);

	inregs.h.al = y;
	inregs.h.ah = 0;

	int86s(20,&inregs,&outregs);
}
  
/**************************************************************************/
/*									  */
/*	GOODBYE- Closes everything & returns to DOS.              	  */
/*									  */
/*************************************************************************/
void goodbye()
{
	vmode(3);
	rclose();
	tickoff();
	exit();
}

/****************************************************************/
/*								*/
/*	Inithelp - Displays initial Help screen, and allows	*/
/*		   changing assigned default values (poll 	*/
/*		   Code, color, etc.				*/
/*								*/
/****************************************************************/

void inithelp()
{
	int x,statchk;
	byte c;
	byte buf[50];
	byte *p;
	FILE *fp;


top:	vmode(4);
	palette(0,1);
	palette(1,0);
	logo(termtype);

	curset(1,5,0);

	fp=fopen("TITLE.PCT","r");   	/* Special header line */
	if (fp) {			/* If one exists, print it */
	 	  p=fgets(buf,35,fp);	/* if not, print something */
		  rprints(3,0,p);	/* boring (below)	   */
		  }
	else if (termtype==2) rprints(3,0,"SHUBERT/TELECHARGE PCT");
		else          rprints(3,0,"TICKETRON/TELETRON PCT");

	curset(4,0,0);
	rprints(1,0,	" [1] ..... To Start PCT");
	rprints(1,0,  "\n [2] ..... Change Mask/Printer Defaults");
	if (mono==FALSE) 
		rprints(1,0,  "\n [3] ..... To Change Colors");
				 
	curset(8,0,0);
	rprints(3,0,     "From any screen :");
	rprints(1,0,  "\n Ctrl-F4 .... Explains the Keyboard");
	rprints(1,0,  "\n Crtl-F10.... Exits from PCT");
	rprints(1,0,  "\n                   (Returns to DOS)");

	curset(13,1,0);
	rprints(2,0,   "Enter \"HELP\" in EVENT,");
	rprints(2,0,"\n CAMP or SPEC field to");
	rprints(2,0,"\n return here");

	curset(17,1,0);
	rprintf(3,0,   "Poll Code   = %d(%2X)",usbase & 0xf,usbase);
	if (unitsl != usbase) rprints(3,0,"+1");
	rprintf(3,0,"\n Status Word =  %4X",itstat);
	rprintf(3,0,"\n Option Word =  %4X",itopt1);
	rprintf(3,0,"\n Version  No =    %2X",vernum);
	if (custno==32750) 
		rprints(3,0,"\n Customer No = Special");
	else 
		rprintf(3,0,"\n Customer No = %5d",custno);

	curset(23,1,0);
	rprints(3,0,"Current Default Mask =\n");
	rprints(1,0,mdnam[termbuf[12]]);

	x=2;

	statchk=0;

/* This routine allows you to start the PCT (by hitting "1" or the ENTER  */
/* key twice), change the colors (by hitting "3", if you have a color 	  */	
/* monitor, Changing the Mode and printer defaults (by hitting "2"),      */
/* Print the keyboard help screens (Ctrl-F4), Exit the PCT (Ctrl-F10) or  */
/* to enter the Special screen to allow resetting of poll code, status    */
/* word, etc.  (throught a secret combination of keys).   Anything else   */
/* Makes it beep.							  */

	while (x) {
		c=kbin();

		switch(c) {
			case '1':
				x=0;
				break;
			case ESC:
				if (statchk==0) statchk=7;
				if (statchk==4) {
					termstat();
					goto top;
					}
				break;
			case 'C':
				if (statchk==2) statchk=4;
				else {
					statchk=0;
					buzzer;
					}
				break;
			case 'M':
				if (statchk==5) statchk=2;
				else {
					statchk=0;
					buzzer;
					}
				break;
			case 'J':
				if (statchk==7) statchk=5;
				else {
					statchk=0;
					buzzer;
					}
				break;
			case KNFLD:
				x--;
				statchk=0;
				break;
			case KHELPKEY:
				ahelpkey();
				statchk=0;
				goto top;
			case KEXIT:
				goodbye();
			case '3':
				if (mono==FALSE) {
					colorchg();
					statchk=0;
					goto top;
				}
				break;
			case '2':
				modechg();
				goto top;
			default:
				if (c) {
					statchk=0;
					buzzer;
					break;
					}
			}
		}
}

/****************************************************************/
/*								*/
/*     Colorchg - Allows user to change the color of most	*/
/*		  output of the PCT.				*/
/*								*/
/****************************************************************/


void colorchg()
{
	int 	z,x;
	byte  	buf[20];
	byte	*c;
	char    *gets();


	SMALLSCR(NO);

	curset(0,0,0);

	STDCOLOR=STDFORE + STDBACK*16;

	/* Prints Name and number of each color in that color    */

	rprints(STDCOLOR,0,"Enter code for new color as follows");
	rprints(col(BLACK),0,"\nBlack         = 0");
	rprints(col(BLUE),0,"\nBlue          = 1");
	rprints(col(GREEN),0,"\nGreen         = 2");
	rprints(col(CYAN),0,"\nCyan          = 3");
	rprints(col(RED),0,"\nRed           = 4");
	rprints(col(MAGENTA),0,"\nMagenta       = 5");
	rprints(col(BROWN),0,"\nBrown         = 6");
	rprints(col(LTGREY),0,"\nLight Grey    = 7");
	rprints(STDCOLOR,0,"\n\nUse only 0-7 for Background colors\nFor foreground, also available :\n");
	rprints(col(DKGREY),0,"\nDark Grey     = 8");
	rprints(col(LTBLUE),0,"\nLight Blue    = 9");
	rprints(col(LTGREEN),0,"\nLight Green   = 10");
	rprints(col(LTCYAN),0,"\nLight Cyan    = 11");
	rprints(col(LTRED),0,"\nLight Red     = 12");
	rprints(col(LTMAGENTA),0,"\nLight Magenta = 13");
	rprints(col(YELLOW),0,"\nYellow        = 14");
	rprints(col(WHITE),0,"\nWhite         = 15");


	curset(5,40,0);
	rprintf(STDCOLOR,0,"TTY foreground     (%2d) :",STDFORE);

	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  STDFORE=x&15;
		  }

	STDCOLOR=STDFORE + STDBACK*16;
	curset(7,40,0);
	rprintf(STDCOLOR,0,"TTY background     (%2d) :",STDBACK);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  STDBACK=x & 7;	/* Keep in range */
		  }

	STDCOLOR=STDFORE + STDBACK*16;

	curset(9,40,0);
	rprintf(label,0,"Field Prompt Labels  (%2d) :",prfore);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  prfore=x&15;
		  }

	curset(11,40,0);
	rprintf(label,0,"Field Prompt Background  (%2d) :",prback);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  prback=x & 7;
		  }

	label = prfore + prback *16;

	z=fldfore + fldback*16;
	curset(13,40,0);
	rprintf(z,0,"Field data Foreground (%2d) :",fldfore);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  fldfore=x & 15;
		  }

	z=fldfore + fldback*16;
	curset(15,40,0);
	rprintf(z,0,"Field data Backgound (%2d) :",fldback);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  fldback=x & 7;
		  }

	z=hlpfor + hlpback*16;
	curset(17,40,0);
	rprintf(z,0,"Field/Mask Help Foregound (%2d) :",hlpfor);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  hlpfor=x & 15;
		  }

	z=hlpfor + hlpback*16;
	curset(19,40,0);
	rprintf(z,0,"Field/Mask Help Backgound (%2d) :",hlpback);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  hlpback=x & 7;
		  }

	curset(21,40,0);
	rprintf(STDCOLOR,0,"Border Color              (%2d) :",border);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2d",&x);
		  border=x & 15;
		  }


	foreg=STDFORE;
	backg=STDBACK;

	rprints(STDCOLOR,0,"Do you wish do save these permanently (Y/N) ?");
	x=getyn();
	if (x==YES) {	
			rprints(STDCOLOR,0,"\nAre you sure (Y/N)? ");
			x=getyn();
			if (x==YES) dataout(2);
			}
}

/****************************************************************/
/*								*/
/*	TERMSTAT - Allows resetting of terminal statistics.	*/
/*								*/
/*	     Accessable only by secret combination of keys.	*/
/*								*/
/****************************************************************/
void termstat()


{	int x;
	byte  buf[20];
	byte  *c;
	char  *gets();

	smallscr(NO);

	ratsay(2,15,STDCOLOR,0,"P C T Initialization");

	rprintf(STDCOLOR,0,"\n\n\n Poll Code (Default = %d) :",usbase & 0x0F);
	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%d",&x);
		  usbase=x+0x60;
		  pollcd=x;
		  unitsl=usbase;
		  }

	rprintf(STDCOLOR,0,"\n\n Terminal Status Word (%4X) :",itstat);

	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%4x",&x);
		  itstat=x;
		  }

	rprintf(STDCOLOR,0,"\n\n Terminal Option Word (%4X) :",itopt1);

	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%4x",&x);
		  itopt1=x;
		  }

	rprintf(STDCOLOR,0,"\n\n Version Number       (%2X) :",vernum);

	c=gets(buf);
	if (*c>0) {
		  sscanf(buf,"%2x",&x);
		  vernum=x;
		  }


	rprintf(STDCOLOR,0,"\n\n Customer Number      (%5d) :",custno);

	c=gets(buf);
	if (*c>0) {
		  if (buf[0]=='S') custno=32750;
		  else {
			sscanf(buf,"%d",&x);
			custno=x;
			}

		}

	rprints(STDCOLOR,0,"\n\n Do you wish do save these permanently (Y/N) ?");
	x=getyn();
	if (x==YES) dataout(1);

}
/****************************************************************/
/*								*/
/*	MODECHG - Allows user to change the default mode	*/
/*		  setting, and default printer setting.		*/
/*								*/
/****************************************************************/

void modechg()
{	byte ch;

	smallscr(NO);

	ratsay(5,19,STDCOLOR,0,"[1] ....... SE   Mode");
	ratsay(6,19,STDCOLOR,0,"[2] ....... CSE  Mode");
	ratsay(7,19,STDCOLOR,0,"[3] ....... CAMP Mode");
	ratsay(8,19,STDCOLOR,0,"[4] ....... CCMP Mode");

	rprints(STDCOLOR,0,"\n\n   Which mode is to be the default (1-4) ");

	ch=getkey();

	switch(ch) {
		case '1':
			mode=0x01;
			break;
		case '2':
			mode=0x04;
			break;
		case '3':
			mode=0x02;
			break;
		case '4':
			mode=0x08;
			break;
		}

	ratsay(13,19,STDCOLOR,0,"[1] ....... Slow Matrix");
	ratsay(14,19,STDCOLOR,0,"[2] ....... Matrix Disabled");
	ratsay(15,19,STDCOLOR,0,"[3] ....... Fast Matrix");

	rprints(STDCOLOR,0,"\n\n   Which setting is to be the default (1-3) ");
	ch=getkey();

	switch(ch) {
		case '0':
		case '1':
		case '2':
		case '3':
			prstat = ch - '0';
			break;
		}

	dataout(4);
}

/****************************************************************/
/*								*/
/*     Datain - Inputs terminal parameters from the disk.	*/
/*								*/
/****************************************************************/

void datain()
{     
      	file = open(name,O_RDONLY|O_RAW,NOPROTECT);

	status = read(file,(char *) termbuf,36);
	close(file);

		if (status ==36) {
			usbase = - termbuf[0] & 0xFF;
			itstat = - termbuf[1];
			itopt1 = - termbuf[2];
			custno = - termbuf[3];
			vernum = - termbuf[4];

			STDBACK = termbuf[5];
			STDFORE = termbuf[6];
			prfore	= termbuf[7];
			prback  = termbuf[13];
			fldback = termbuf[8];
			fldfore = termbuf[9];
			hlpfor	= termbuf[10];
			hlpback = termbuf[11];
			border  = termbuf[14];
			
			mode    = termbuf[12];
			prstat  = termbuf[15];

			ENGFLG  = termbuf[16];
			termtype= termbuf[17];
			}

			else {
				cls();
				curset(5,2,0);
				rclose();
				prints("Can't find valid prameter file");
				exit();
				}

	foreg=STDFORE;
	backg=STDBACK;
	label=prback*16 + prfore;
	STDCOLOR=STDFORE + STDBACK*16;
	unitsl=usbase;

}

/****************************************************************/
/*								*/
/*     Dataout - Outputs terminal parameters to the disk.	*/
/*		 Which parameter that are written are 		*/
/*		 selectable by the arguement.			*/
/*								*/
/****************************************************************/

void dataout(x)
	int x;

{
	file = open(name,O_RDONLY);
	status = read(file,(char *) termbuf,36);
	close(file);

	file = open(name,O_WRONLY|O_CREAT|O_RAW);

	if (x & 1) {			/* Status words = Bit 0 = 1 */
		termbuf[0] = -usbase;
		termbuf[1] = -itstat;
		termbuf[2] = -itopt1;
		termbuf[3] = -custno;
		termbuf[4] = -vernum;
		}
					/* Colors = Bit 1 = 2 */
	if (x & 2) {		       
		termbuf[5] = STDBACK;
		termbuf[6] = STDFORE;
		termbuf[7] = prfore;
		termbuf[13]= prback;
		termbuf[8] = fldback;
		termbuf[9] = fldfore;
		termbuf[10]= hlpfor;
		termbuf[11]= hlpback;
		termbuf[14]= border;
		}

	if (x & 4) {			/* Mode & printer Status = Bit 2 =4 */
		termbuf[12]=mode;
		termbuf[15]=prstat;
		}

 	if (x & 8) {			/* Terminal Type = Bit 3 = 8 */
		termbuf[17]=termtype;	/* Presently not changable */
		}
	
	status = write(file,(char *) termbuf,36);
 
	close(file);
}
