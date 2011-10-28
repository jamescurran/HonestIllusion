/****************************************************************/
/*								*/
/*  	SE OFF - Handles reception of offline reports, online	*/
/*		Called from SECOMM.C				*/
/*								*/
/****************************************************************/

#include "se.h"
#include "sefdef.h"
#include <fcntl.h>

	extern	int	itstat,itopt1,custno,msglth,lttn;
	extern	int	offseq,offflg,rtstmr,tvmaxc,crttim,cpsc2,maskp;
	extern	int	backg,foreg,blk,mono,STDCOLOR;
	extern	byte    vernum,pollcd,prstat,xtctr;
	extern	byte	*msgout,fdata[][35];

#define  TOCRT     0
#define  TODISK    1
#define  CRTMTX    0
#define  MTX       1
#define  OFFON     2
#define  CUSTID	 0xB9		/* Customer Number ID Character */
#define  VERCOD  0x18            /* Version code field ID character */
#define  PSCOD   0xBF            /* Printer status code */

#define  offtype data1[card][3]


		byte	__SEOFF;
	
		byte	data1[2][80];	/* Input buffer for cards */
		byte	filename[] = "TEMPFILE.R71";
		byte	newname[50];	/* User New name for received file */
		byte 	r71msg[50];	/* Request message */
		
		int	towhere; /* Flag indicate if TOCRT or TODISK */
		int	offend;	 /* Flag: TRUE if OFFE card has been received */
		int	seq;	 /* internal counter for current card */
		int	offerr;	 /* Flag: TRUE if an error has been detected */
		int	filenum;  /* File number of output disk file */
		int	oneok;	 /* TRUE if one full card has been received ok */

	static	int	card,pos;	/* Indexes for data1[][] */
	
	void	seoff(),chkcard(),errcard(),offinit(),offwrite();
	void	offopen(),offclose();

/****************************************************************/
/*								*/
/*	SEOFF - Main routine for accepting data for an OFFON	*/
/*		report.  Stores the character, and check for 	*/
/*		full card.					*/
/*								*/
/****************************************************************/

void seoff(chr)
	byte	chr;

{
	data1[card][pos++]=chr;		/* Store Character */

	if (pos==80) {	pos=0; 		/* If card is Filled */
			chkcard();	/* Check Card  */
			card^=1;	/* Switch to other card */
			}
}

/****************************************************************/
/*								*/
/*	CHKCARD - Called whenever a full card has been 		*/
/*		  received.  Checks for proper format and	*/
/*		  sequence.  If it passes the tests, writes  	*/
/*		  to the CRT					*/
/*								*/
/****************************************************************/

void chkcard()

{
	int	i;

	towhere = TODISK;

	if (strncmp(data1[card],"OFF",3) !=0) {
			      		errcard();
			      		return;
			      		}

	if (strchr("ACDE",offtype)==0) {
			      		errcard();
			      		return;
			      		}

	if (atoi(&data1[card][4]) !=seq) {
			      		errcard();
			      		return;
			      		}

	if (offtype=='E')  {  if (offseq>seq)  offseq=seq+1;
				offtype='D';
				}

	curset(7,1,0);
	for (i=0; i<9; ++i) crtout(data1[card][i]); /* Print card Number */

	offwrite();			/* Write out the card to disk */
	seq++;				/* Expect next card */	
	oneok=TRUE;			/* Signal at least one good card */
					/* Has been received  */
}


/****************************************************************/
/*								*/
/*	ERRCARD - Sets the number of the next card to be	*/
/*		  requested to the lowest incorrect card.	*/
/*								*/
/****************************************************************/

void errcard()

{ 
	if (offseq>seq)  {	
			offseq=seq;
			if (oneok) offseq--;  
			}	/* do previous only if a full card has received*/
	crtout('e');
}

/****************************************************************/
/*								*/
/*	OFFINIT - Initiate an OFFON report.			*/
/*								*/
/****************************************************************/

void offinit()

{
	offflg=1;	/* =1= We are in the middle of an OFFON report  */
	offseq=47;	/* Next request: ask for #47 			*/
	pos=0;		/* Next character goes in first posision of card*/
	card=0;		/*  "      "        "   "   "	 Card		*/
	seq=1;		/* Expect Next card to be # 1			*/
	towhere=TOCRT;  /* Display to CRT  (TODISK later)		*/
	offend=FALSE;	/* We are not at the end			*/
}
/****************************************************************/
/*								*/
/*	OFFREQ - requests the next set of card images.		*/
/*								*/
/*		 called when a TIP Off message is recieved.	*/
/*								*/
/****************************************************************/

offreq()
{
        byte *msgout,check;
        int len,i,MC;

	if (towhere==TOCRT) return(FALSE);

	if (offflg==3)	    return(TRUE);

        r71msg[0] = pollcd;

        r71msg[2] = VERCOD;

        r71msg[3] = vernum;

        r71msg[4] = (itstat & 0xff00) >>8;
        r71msg[5] = (itstat & 0x00ff);

        r71msg[6] = (itopt1 & 0xff00) >> 8;
        r71msg[7] = (itopt1 & 0x00ff);

	r71msg[8] = FSPEC;
	r71msg[9] = 'R';
	len=10;
        if ( (MC=MSKCODE(maskp)) >80) r71msg[len++]='D'; 

	r71msg[len++]= '7';
	r71msg[len++]= '1';

	r71msg[len++] = FNUMBR;
	len += sprintf(&r71msg[len],"%d",offseq);

	if (MC ==80) {
		r71msg[len++] = FREPRT;
		r71msg[len++] = fdata[FREPRT][0];
		if (fdata[FREPRT][1]) 	r71msg[len++] = fdata[FREPRT][1];
		if (fdata[FREPRT][2]) 	r71msg[len++] = fdata[FREPRT][2];
		if (fdata[FREPRT][3]) 	r71msg[len++] = fdata[FREPRT][3];
		}

	r71msg[len++] = CUSTID;
	len+=sprintf(&r71msg[len],"%u",custno);

        r71msg[len++] = PSCOD;
        r71msg[len] = prstat;

        r71msg[1]   = len-1;

        check = 0;
        ++len;

        for (i=1; i<len; i++) { check += r71msg[i]; };

        r71msg[len++] = check;
        r71msg[len++] = FINISH;

        msgout = r71msg;
        msglth = len;

        lttn = segpol();

        rtstmr = RTSTIM;

        seout(msgout,msglth);

        rtstmr = 0;

	seq=offseq;
	offseq = ((offseq/23) + 1)  *23 + 24;   /* Begining of next block */

	pos=0;
	curset(7,10,0);
        for (i=0; i<50; ++i) crtout(' ');       /* Clear eee's		  */
	curset(7,10,0);

	oneok=FALSE;

	return(TRUE);
	
}

/****************************************************************/
/*								*/
/*	OFFWRITE - Write one card image out to disk		*/
/*		   File must already be open.			*/
/*		   Tries 3 times if an error occurs.		*/
/*								*/
/****************************************************************/

void offwrite()
{
	long	pos;
	int	status,i;

	for (i=0; i<3; ++i) {
		pos = (seq-1) * 80L;
		lseek(filenum,pos,0);

		status = write(filenum,(char *) &data1[card],80);

		if (status !=80) seemsg(21);
		else { seeclr();
			i=3;
			}
	}
}

/****************************************************************/
/*								*/
/*	OFFOPEN - Open disk file for output.			*/
/*		  Always uses file in FILENAME[]    		*/
/*		  may be renamed later.				*/
/*								*/
/****************************************************************/

void offopen()
{
	towhere = TODISK;
	filenum = open(filename,O_WRONLY|O_CREAT|O_TRUNC|O_RAW,0);
	if (filenum ==-1 ) {
			seemsg (25);
			towhere=TOCRT;
			}
}

/****************************************************************/
/*								*/
/*	OFFCLOSE - closes file and gives option to rename.	*/
/*								*/
/****************************************************************/

void offclose()
{
	int	status,page;
	int	x;
	byte  	buf[20];
	byte  	*c;
	char    *gets();

	if (offflg!=3)  {
			offflg=0;
			return;
			}

	offtype = 'E';

	for (x=10;x<80;++x) data1[card][x]=' ';

	status = write(filenum,(char *) &data1[card],80);

	status = close(filenum);

	if (mono==TRUE) {		   /* if b/w board  */
				page =0;	
				SMALLSCR(NO);
				}
		else		page =3;

	vpage(page);
	crttim=0;

	curset(3,5,page);  
	cls();

	buzzer; buzzer; buzzer;

	prints("Data stored as disk file >");
	prints(filename);
	prints("<.\n\n");
	
	prints("    Enter new name, or just hit RETURN to\n" );
	prints("    leave file name as it is....");

	buzzer; buzzer; buzzer;

	c=gets(buf);
	if (*c==0) x=22;
	else  	{
		  sscanf(buf,"%s",newname);     
 		  status=dosrename(filename,newname);
		  if (status==0) x=23;
		  else  	 x=24;
		  }

	bright;			/* Restore page 0 */

	if (page==0) {
			if (SMALLMSK(maskp)) SMALLSCR(YES);
			else LARGESCR(YES);
			
			dspmsk();
			}

	seemsg(x);
}
 