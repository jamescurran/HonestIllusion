/****************************************************************/
/*								*/
/*	  SEIN 							*/
/*								*/
/*	S & E PC Keyborad / CRT input Routines			*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

#include "se.h"			/* Standard PCT Header file 	*/
#include "sekdef.h"		/* Keyboard Definations     	*/
#include "sefdef.h"		/* Field/Mask definations	*/
#include <grnleaf\color.h>	/* Color definations		*/


	extern	int	noscrl,inqflg,itopt1,xlpt,magflag;
	extern	int	crttim,tvmaxc,cpsc2,backg,foreg,STDCOLOR;
	extern  byte 	unitsl,pollcd,prstat,mgtkey,svmask[],tsmode;

		char	__sein;		/* Start of data label */

		int	fldpos;		/* Offset into field */
	static	byte	key;		/* Current key data */
	static	byte	keyprv;		/* previous key data */
	static	byte	keypr2;		/* 2nd previous key */
	static	byte	keypr3;		/* 3rd previous key */
	static	byte	kbuf[10];	/* buffer for Date translations */
		int	label;		/* Color for field prompt labels */
		int	prback;		/* Color for field prompt Background */
		int	prfore;		/* Color for field prompt Foreground */
		int	fldback;	/* Color for field background */
		int	fldfore;	/* Color for field foreground */
		byte	usbase;		/* original unit select */
		int	predsk;		/* Flag it indicate if preset to disk */
		int	mgterr;		/* Flag indicating management key required */
		int	xmtflg;		/* Flag indicates Transmit on return */
		int	retnfg;		/* Flag : NZ indicates entered RETN mask from */
				/*      CRET or FRET mask (used to allow response */
				/* 	to remain on the screen.  Also used in */
				/*	SEEXEC to allow response to remain during */
				/*	auto mask sequence)			*/

		byte	mode;		/* Terminal mode (1 = SE, 2 = CAMP) */
		int	maskp;		/* pointer to Mask Definition */
		int	fieldp;		/* Pointer to Static field definition */
		int	mkfldp;		/* Pointer to Local Field definition */ 
	static	int	xlpt;		/* Pointer for Do_xlate */
		byte	fdata[256][35];	/* Field data storage */
		int	ENGFLG;		/* TRUE = This is an engineering Terminal */
		int	escflg;		/* >0 if in esc-n-n-esc seq.  */
		int	esccd;		/* Poll code built form esc-n-n-esc seq */

	void	 _sein_(),inqtst(),kfld(),kesc(),kact(),keyret(),aclear();
	void	aclrfd(),aclrmk(),axmit(),anfld(),anext(),anmsk(),abmsk();
	void	dispflds(),aaltok(),aspmenu(),spf1(),aprmenu(),ascrol();
	void	premod(),kspt(),helpchk(),dftmsk(),clrmsk(),setnds(),dspfld();
	void	dspmsk(),fwdspc(),bckspc(),bckdel(),newmsk(),newfld(),bckfld();
	void	fwdfld(),clrdat(),chretn();

#define abspc() 	bckspc()
#define afspc() 	fwdspc()
#define abfld() 	bckfld()
#define alast() 	getpre(' ')
#define aprint() 	crtprt()
#define clrfld() 	zero(fdata[FLDCODE(mkfldp)],35)

#define tstalp(x) (isalpha(x) | isspace(x))
#define tstnum(x) isdigit(x)

void  _sein_() {}  /*Indicates start of sub-program in .MAP file */

/****************************************************************/
/*								*/
/*	KEYPRO - Process a single key.				*/
/*								*/
/*		Arguement = the key.				*/
/*		Returns XMIT (if Transmit Key was hit)		*/
/*		    or  NOXMIT (if another key was hit)		*/
/*								*/
/****************************************************************/

keypro(x)
	byte	x;
{
	int z,FC;
	byte do_xlate();

	key = x;		/* Save Key for later */

	xmtflg = NOXMIT;	/* Flag that is returned */

 	bright;			/* Brighten Screen (currently inactive) */
	seeclr();		/* Clear any error message present */
	
	if (keyprv == KPRSET) {			/* Ifg last key key */
				premod(x);	/* was PRESET (ins)....*/
				keyret();
				return(NOXMIT);
				}
	else	predsk=FALSE;
		
	if (x==ESC) {			/* Is it an ESC Key ??*/
			kesc();
			return(NOXMIT);
			}	

	if (tstact(x)) {		/* Is it an Action Key ?? */
			kact();
			return(xmtflg);
			}

	if (tstfc(x)) {			/* Is it a Field Code ?? */
			kfld();
			return(NOXMIT);
			}

	if (valpre(x,NO)) {		/* Is it a Valid Preset ?? */
			escflg=0;
			getpre(x);
			return(NOXMIT);
			}

	if (fldpos < NUMTRAN(fieldp)) {		/* Handle Translation */
	   if (( (FC=FLDCODE(mkfldp)) ==FDATE) || (FC==FDATE2) || (FC==FDOP) || (FC==FLDATE) ) {
				z=do_xlate(x);
				switch(z) {
					case -1:
						seemsg(50+beep);
						keyret();
						return(NOXMIT);
					case 1:
						keyret();
						return(NOXMIT);
					default:
						x=z;
						key=x;
						break;
					}
				}

		     if (fdlen(key)==0) {
					seemsg(50+beep);
					keyret();
					return(NOXMIT);
					}
				}
		else if (!valcha(key)) {
					seemsg(50+beep);
					keyret();
					return(NOXMIT);
					}
	
	if (fldpos == 0) {clrfld();}   /* If first character in field, Clear the rest */

	escflg--;

	x =insert(key);			/* Move character into field*/

	if (x) { 			/* If this is NOT the last */
		dspfld();		/* character in the field, */
		inqtst();		/* Display the field, &    */
		keyret();		/* Go home.		   */
		return(NOXMIT);
		}
		
		dspfld();		/* If this is the last     */
		inqtst();		/* character in the field  */
		x=FLDCODE(mkfldp);	/* Move to next field      */
		fwdfld();
		if (x == FSPEC) kspt();
	
	keyret();
	return(NOXMIT);
}

/****************************************************************/
/*								*/
/* 	INQTST - Resets inqflag if data was entered into	*/
/*		any field except name, MOP, INQ, or ALTOK.	*/
/*								*/
/*		if flag is reset, screen will be cleared 	*/
/*		on transmit.					*/
/*								*/
/****************************************************************/

void inqtst()
{
	int x;

	x=FIELDCD(fieldp);
	       
	if ((x == FMOP) || (x == FNAME) || (x == FINQ) || (x == FALTOK))
		return;
	
	inqflg = 0;
}

/****************************************************************/
/*								*/
/*     KFLD - A field key was pressed.				*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

void kfld()

{	int x;
	
	escflg=0;		/* reset escape seq counter */

	x= key;			/* Get the key back */

	if (locfld(x) == 0) {	seemsg(50+beep);    /* Invalid Key */
				keyret();
				return;
				}
	fdata[x][0] &= 0x7f;   /* remove high (no-disp) bit from field */

	fldpos=0;		/* Start field position at the beginning */
	xlpt=0;			/* Start Translation counter.....        */
	clrfld();		/* Clear old data from field		 */
	dspfld();		/* Display the field (Prompt)		 */

}

/****************************************************************/
/*								*/
/*    KESC - The ESC  key was pressed.				*/
/*								*/
/****************************************************************/

void kesc()
{
	if (ENGFLG==FALSE) {	seemsg(50+beep);  /* "Invalid Key"*/
			    	return;
				}

	if (escflg==3) {		/* monitor same poll Code */
			escflg=100;
			if (SMALLRES(maskp))	smallscr(YES);
			else 			largescr(YES);
			dspmsk();
			curset(tvmaxl,tvmaxc-5,0);
			rprintf(STDCOLOR,0,"*%d*",pollcd);
			crtrtc(0x0600);

			segmsg();
			dspfld();
			esccd=0;
			}

	if (escflg<1) escflg=3;		/* First Escape is sequence */

	if ( (escflg<3) && (esccd>15) ) { /* Inputted Poll code was bad */
			escflg=3;
			esccd=0;
			return;
			}
	if (escflg<3) {		   	/* Change poll code & Monitor   */
			escflg=100;
			pollcd=esccd;	
			unitsl=pollcd+0x60;

			if (SMALLRES(maskp))	smallscr(YES);
			else 			largescr(YES);
			dspmsk();
			curset(tvmaxl,tvmaxc-5,0);
			rprintf(STDCOLOR,0,"*%d*",pollcd);
			crtrtc(0x0600);

			segmsg();
			esccd=0;
			dspfld();
			}

}

/****************************************************************/
/*								*/
/*    KACT - An action key was pressed.				*/
/*								*/
/****************************************************************/

void kact()

{
	escflg=0;		/* Abort escape sequence */

	switch(key) {
		case KCLEAR:		/* Clear Key */
			aclear();
			break;
		case KCLRFD:		/* Clear Field */
			aclrfd();
			break;
		case KCLRMSK:		/* Clear Mask */
			aclrmk();
			break;
		case KXMIT:		/* Transmit */
			axmit();
			break;
		case KBFLD:		/* Back Field */
			abfld();
			break;
		case KNFLD:		/* Next Field */
			anfld();
			break;
		case KBSPC:		/* Back Space (no Del) */
			abspc();
			break;
		case KBSPCDEL:		/* Back Space w/Del */
			bckdel();
			break;
		case KFSPC:		/* Forward Space */
			afspc();
			break;
		case KBMSK:		/* Page back, mask */
			abmsk();
			break;
		case KNMSK:		/* Page forward, Next Mask */
			anmsk();
			break;
		case FNEXT:		/* NEXT key */
			anext();
			break;
		case FALTOK:		/* ALTOK key */
			aaltok();
			break;
		case KINQ:		/* INQ key */
		case KBUY:		/* BUY key */
		case KBUYAL:		/* But ALT */
			ainq(key);
			break;
		case KF3:		/* Display All fields */
			dispflds();
			break;
		case KSCROL:		/* Scroll On/Off */
			ascrol();
			break;
		case KSPMENU:		/* Special Menu (mode change) */
			aspmenu();
			break;
		case KPRMENU:		/* Printer change Menu */
			aprmenu();
			break;
		case KEXIT:		/* EXIT key (cntrl-F10) */
			goodbye();
		case KPRSET:		/* PRESET key */
			break;	
		case KPRLST:		/* LAST key */
			alast();
			break;
		case KHELPKEY:		/* Keyborad Help */
			ahelpkey();
			break;
		case KHELPMSK:		/* Mask Help */
			ahelpmsk();
			break;
		case KHELPFLD:		/* Field Help */
			ahelpfld();
			break;	  
		default :
			seemsg(50+beep); /* "Invalid Key" */
			break;
		}

	keyret();
}
/****************************************************************/
/*								*/
/*	 KEYRET - Stores previous keys & returns to EXEC.	*/
/*								*/
/*								*/
/****************************************************************/


void keyret()
{
	keypr3=keypr2;
	keypr2=keyprv;
	keyprv=key;   
			/*     return to exec. */
}
/****************************************************************/

#define valmod(x) ((x & mode)? TRUE:FALSE)

/****************************************************************/
/*								*/
/*	ACLEAR - Returns to, and clears default mask. 		*/
/*								*/
/*								*/
/****************************************************************/

void aclear()

{	 int	x;

	retnfg=FALSE;

	dftmsk();
	clrmsk();
	newmsk();

	x=getcur(0);				/* Print Poll code */
	curset(tvmaxl,tvmaxc-5,0);		/* in corner of  screen */
	rprintf(STDCOLOR,0,"*%d*",pollcd);	
	crtrtc(x);

	magflag=FALSE;

}

/****************************************************************/
/*								*/
/*	ACLRFD - Clears current field.				*/
/*								*/
/****************************************************************/
	     
void aclrfd()

{
	clrfld();
	dspfld();
	newfld();
}
/****************************************************************/
/*								*/
/*     ACLRMK - Clear current mask.				*/
/*								*/
/****************************************************************/

void aclrmk()
{
	retnfg=FALSE;

	clrmsk();

	if (!DEFLTMSK(maskp)) strins(fdata[FSPEC],MASKNAME(maskp),4);

	newmsk();
}

/****************************************************************/
/*								*/
/*	AXMIT - Transmit. 					*/
/*								*/
/****************************************************************/
		 
void axmit()

{
	if ((DEFLTMSK(maskp)==0) || (fdata[FCARD][0] & 0x7F) ||     /* if not the default mask, or */
		(fdata[FINQ][0] == '1') || (fdata[FINQ][0] == 0) || /* there is something in the card */
		(fdata[FINQ][0] ==0x80))  inqflg =0;		    /* field, or this is an inquriy,  */
								    /* clear inqflg */

	xmtflg= XMIT;

	setpre(' ');	/* set "Last" preset */

	kspt();
}

/****************************************************************/
/*								*/
/*	ANFLD - Move cursor to next field.			*/
/*		Change mask if old field was SPEC.		*/
/*								*/
/****************************************************************/
		 
void anfld()

{
	int	x = FIELDCD(fieldp);

	fwdfld();

	if (x==FSPEC) kspt();
	if (x==FEVENT) helpchk();
}


/****************************************************************/
/*								*/
/* 	AINQ - Handles INQ, BUY, ALT, & NEXT			*/
/*								*/
/****************************************************************/
		 
ainq(x)
	int 	x;
{
	x += 0x14;		/* adjust to ASCII numbers */

	if (locfld(FINQ) ==0) {
				seemsg(50+beep);
				return(NOXMIT);
				}

	fdata[FINQ][0] &=0x7F; /* Remove no disp bit */

	newfld();
	insert(x);
	dspfld();
	inqtst();
	fwdfld();

	return(NOXMIT);
}

/****************************************************************/
/*								*/
/*	ANEXT - Handles NEXT key.				*/
/*								*/
/****************************************************************/

void anext()  {  ainq(0x20);  }


/****************************************************************/
/*								*/
/*    ANMSK - Go to next mask for this mode.			*/
/*								*/
/****************************************************************/

void anmsk()

{
	do {nxtmsk();
		} while (!valmod(MSKMODE(maskp)));

	if (DEFLTMSK(maskp))   fdata[FSPEC][0] = 0x80;
		else strins(fdata[FSPEC],MASKNAME(maskp),4);

	newmsk();
}

/****************************************************************/
/*								*/
/*   	ABMSK -  Go to previous mask for this mode.     	*/
/*								*/
/****************************************************************/
void abmsk()	     
{
	do {
		if (! bckmsk()) {
				dftmsk();
				break;
				}

		} while (!valmod(MSKMODE(maskp)));

	if (DEFLTMSK(maskp))   fdata[FSPEC][0] = 0x80;
		else strins(fdata[FSPEC],MASKNAME(maskp),4);

	newmsk();
}

/****************************************************************/
/*								*/
/*    DISPFLDS - Resets no display bits on all fields 		*/
/*								*/
/****************************************************************/

void dispflds()

{	int x;

	for (x=0;x<256;++x) {
		fdata[x][0] &= 0x7f;
		}

	dspmsk();
}
	     

/****************************************************************/
/*								*/
/*    AALTOK - Handles ALTOK key                   		*/
/*								*/
/****************************************************************/

void aaltok()

{ 

	if (locfld(FALTOK) == 0) {
				seemsg(50+beep);
				return;
				}

	fdata[FALTOK][0] ^= 0x7f;  /* remove no disp bit */

	newfld();

	insert('1');			

		dspfld();
		inqtst();
		fwdfld();
	
	return;
}

/****************************************************************/
/*								*/
/*   	ASPMENU - Special Menu for mode change.			*/
/*								*/
/****************************************************************/
	     
void aspmenu()
{	char ch;

	largescr(NO);

	ratsay(3,11,STDCOLOR,0,"Special Menu");

	ratsay( 5, 9,STDCOLOR,0,"[1] ....... Poll + 1");
	ratsay( 6, 9,STDCOLOR,0,"[2] ....... Data Scope");
	ratsay( 7, 9,STDCOLOR,0,"[3] ....... SE   Mode");
	ratsay( 8, 9,STDCOLOR,0,"[4] ....... CSE  Mode");
	ratsay( 9, 9,STDCOLOR,0,"[5] ....... CAMP Mode");
	ratsay(10, 9,STDCOLOR,0,"[6] ....... CCMP Mode");
	seemsg(14);

	ch=getkey();

	switch(ch) {
		case '1':
			spf1();
			break;
		case '2':
			sedscp();
			break;
		case '3':
			mode=0x01;
			break;
		case '4':
			mode=0x04;
			break;
		case '5':
			mode=0x02;
			break;
		case '6':
			mode=0x08;
			break;
		default:
			if (SMALLMSK(maskp)) smallscr(YES);
			else largescr(YES);
			dspmsk();
			return;
		}

	aclear();
}

/****************************************************************/


void spf1()

{

	if (POLLUPOK) {
			lampoff(0x10);
			if (usbase == unitsl) {
					unitsl = (unitsl + 1) & 0x6f;
					lampon(0x10);
					}
			else {unitsl = usbase;}
			pollcd = unitsl & 0x0f;
			}
}



/****************************************************************/

/****************************************************************/
/*								*/
/*   	APRMENU - Printer Menu 					*/
/*								*/
/****************************************************************/
	     
void aprmenu()
{	byte ch;

	largescr(NO);

	ratsay(3,11,STDCOLOR,0,"Printer Menu");

	ratsay( 5, 9,STDCOLOR,0,"[1] ....... Slow Matrix");
	ratsay( 6, 9,STDCOLOR,0,"[2] ....... Matrix Disabled");
	ratsay( 7, 9,STDCOLOR,0,"[3] ....... Fast Matrix");
	if (prstat) {
		ratsay(23, 1,STDCOLOR,0,"Presently :");
		seemsg(17+prstat);
		}
	else seemsg(14);

	ch=getkey();

	switch(ch) {
		case '0':
		case '1':
		case '2':
		case '3':
			prstat = ch - '0';
			break;
		}

	if (SMALLMSK(maskp)) smallscr(YES);
	else largescr(YES);

	dspmsk();

	if (ch > '0') seemsg(17+prstat);
	
}

/****************************************************************/


void ascrol()

{
	if (noscrl) {
			noscrl = 0;
			seemsg(51);
			}
	else {
			noscrl = 1;
			seemsg(52);
			}
}

/****************************************************************/
/*								*/
/*    Preset Mode- this key must be either a preset code or	*/
/*		   or an asterisks.				*/
/*								*/
/****************************************************************/
void premod(ch)
	byte ch;

{
	if (ch==KPRSET) {		/* i.e. Ins twice is a row */
			predsk=TRUE;
			return;
			}
	
	if (ch=='-') { 
			clrpre();
			buzzer;
			return;
			}

	if (ch=='*') {
			lstpre();
			return;
			}

	if (valpre(ch,YES)) {
			if (keypr2==KCLEAR) {
				delpre(ch);
  				buzzer;
				}
			else {
				setpre(ch);
  				buzzer;
					}
				}
}

/****************************************************************/

void kspt()

{ 	 if (strncmp(fdata[FSPEC],"HELP",4)==0) {
			inithelp();
			aclear();
			return;
			}
	
	 if ((fdata[FSPEC][0] & 0x7f) &&
	 (strncmp(fdata[FSPEC],MASKNAME(maskp),4))) {

		chretn();
		if ((!fndmsk(fdata[FSPEC])) && (mgterr==NO) && (!xmtflg)) {
			zero(fdata[FSPEC],4);
			seemsg(54+beep);
			}
   		else if (mgterr==NO) {
			if (DEFLTMSK(maskp)) zero(fdata[FSPEC],4);
			newmsk();
			}
   		}

}
/****************************************************************/

void helpchk()
{
 	 if (strncmp(fdata[FEVENT],"HELP",4)==0) {
		inithelp();
		aclear();
		}
}

/****************************************************************/

void dftmsk()

{
	while ( (valmod(MSKMODE(maskp))==0) || (DEFLTMSK(maskp)==0) )
		{nxtmsk();}
}


/****************************************************************/
/*								*/
/*	NXTMSK - Finds the start of the next mask 		*/
/*		 specification	 				*/
/*		 Returns TRUE with address in maskp.		*/
/*		 Returns FALSE if no more masks exist, with	*/
/*		   maskp pointing to first mask.		*/
/*								*/
/****************************************************************/
	     

nxtmsk()

{	int x;

	for (x=maskp;FLDCODE(x)!=FIN;++x) {};

	if (FLDCODE(++x) == FIN) x=0;
	
	maskp=x;

	return(x ? TRUE:FALSE);	
}
/****************************************************************/
/*								*/
/*	FNDMSK - Find mask (name given).			*/
/*		 Mode must be valid.				*/
/*		 Arguement equals address of name.		*/
/*		 Returns TRUE with Maskp set properly if found.	*/
/*		 Returns FALSE if no match, with maskp unchanged*/
/*								*/
/****************************************************************/
	     

fndmsk(x)
	byte *x;
{
	int y,z;
	
	y=maskp;	/* save current pointer */

	maskp=-1;

	do {	
		maskp++;

		while (strncmp(MASKNAME(maskp),x,4)) {
			z=nxtmsk();
			if (z==0) {		    /* z==0 if we've reached */
					maskp=y;    /* the end w/o finding it*/
					return(FALSE);
					}
			}
		} while (!valmod(MSKMODE(maskp)));

	if (MGTREQ(maskp) && (!mgtkey)) {
				seemsg(53+beep);   /* "MGT key needed" */
				mgterr=YES;	   /* (MGT key always on) */
				xmtflg=NOXMIT;
				maskp=y;
				return(FALSE);
				}
	return(TRUE);
}
/****************************************************************/
/*								*/
/*	FNDFLD - Find static field specifaction 		*/
/*		 Mode must be valid.				*/
/*		 Arguement equals field code.			*/
/*		 Returns TRUE with Fieldp set.			*/
/*		 Returns False if no match, fieldp unchanged.	*/
/*								*/
/****************************************************************/

fndfld(x)	     
	int x;

{	
	int i,FC;

	for (i=0; (FC=FIELDCD(i)) != FIN; i++) {
		if ( (FC==x) && valmod(FIELDMD(i)) ) {
			fieldp=i;
			return(TRUE);
			}
		}
}
/****************************************************************/
/*								*/
/*     FFNVAL - same as above, but no Validation of mode.	*/
/*								*/
/****************************************************************/
	     
ffnval(x)	
	int x;

{
		int i;

	for (i=0;FIELDCD(i)!=x;i++) {
		if (FIELDCD(i)==FIN) return(FALSE);
		};

	fieldp=i;
	return(TRUE);
}
/****************************************************************/
/*								*/
/*	NXTFLD - finds next field in current mask.		*/
/*								*/
/*		Returns TRUE, with field pointers set.		*/
/*		Returns FALSE, if no more fields, Pointer	*/
/*			set to first field.			*/
/*								*/
/****************************************************************/
	     
nxtfld()

{
 	int i =TRUE;

	if (FLDCODE(++mkfldp) ==FIN) {
		mkfldp=maskp+2;
		i=FALSE;
		}

	fndfld(FLDCODE(mkfldp));

	return(i);
}
/****************************************************************/
/*								*/
/*	LSTFLD - Find Previous field in current mask.		*/
/*								*/
/*		Returns TRUE, with field pointers set.		*/
/*		Returns FALSE if already at start of mask.	*/
/*			Pointers set to last field in mask.	*/
/*								*/
/****************************************************************/
	     
lstfld()

{	int i=TRUE;
	int x;

	if (mkfldp==maskp+2) {
			     for (x=mkfldp;FLDCODE(x+1)!=FIN;++x) {};
					mkfldp=x;
					i=FALSE;
			     }
	else mkfldp-=1;


	fndfld(FLDCODE(mkfldp));

	return(i);
}

/****************************************************************/
/*								*/
/*   	bckmsk - Steps backward one mask.			*/
/*								*/
/*		Returns TRUE if O.K.				*/
/*		        FALSE if Current or Previous Is First	*/
/*			      (Pointers set to first mask)	*/
/*								*/
/****************************************************************/
	     
bckmsk()

{

	if (maskp==0) return(FALSE);

	while(maskp>0) {
		maskp-=1;
		if (FLDCODE(maskp-1)==FIN) break;
		}

	return(maskp ? TRUE:FALSE);	
}

/****************************************************************/
/*								*/
/*     	CLRMSK - clear data buffers for all fields in 		*/
/*		 current mask.					*/
/*								*/
/****************************************************************/
	     

void clrmsk()

{ 	
	mkfldp=maskp+2;	 

	fndfld(FLDCODE(mkfldp));

	do { clrfld();  } while(nxtfld());

}

/****************************************************************/
/*								*/
/*     	SETNDS - Sets "no Display" Bits in first character	*/
/*		 of each field data buffer as required by	*/
/*		 field specifications				*/
/*								*/
/*		 (field pointer set to first field of mask	*/
/*			on exit)				*/
/****************************************************************/
	     
void setnds()

{
	mkfldp=maskp+2;	 

	fndfld(FLDCODE(mkfldp));

	do {
		if (DONTDISP(mkfldp)) fdata[FLDCODE(mkfldp)][0] |= 0x80;
		else    	      fdata[FLDCODE(mkfldp)][0] &= 0x7f;
	   } while (nxtfld());
}

/****************************************************************/
/*								*/
/*     	NODISP - test current field for display control.	*/
/*		returns TRUE if this field should not be 	*/
/*			displayed.				*/
/*		Returns FALSE if display OK.			*/
/*								*/
/*								*/
/****************************************************************/
	     
#define nodisp() (fdata[FLDCODE(mkfldp)][0] & 0x80)

/****************************************************************/
/*								*/
/*     	MOPTST - test terminal option word for MOP authorization*/
/*		 Arguement equals field code.			*/
/*								*/
/*		 Returns TRUE if FC is MOP & it's not allowed.	*/
/*		 Returns FALSE otherwise.			*/
/*								*/
/****************************************************************/
	     
#define moptst(x) ((x==FMOP) & ((itopt1 & 0x100)==0))

/****************************************************************/
/*								*/
/*	DSPFLD - Displays current field and data.		*/
/*		  (does not alter fldpos)			*/
/*								*/
/****************************************************************/
	     

void dspfld()

{	int i,f,b,x,FC;
	byte ch,dfxlat();

	FC=FIELDCD(fieldp);

	if (nodisp()) return;

	if (moptst(FC)) return;

	curset(DISPROW(mkfldp),DISPCOL(mkfldp),0);

	if (PROMPT(fieldp)) rprints(label,0,PROMPT(fieldp));

	if (REVFLD(mkfldp)) {
				f=fldback;
				b=fldfore;
				}
	else		    {
				f=fldfore;
				b=fldback;
				}
	for (i=0;i<NUMCHAR(fieldp);++i) {
		ch=fdata[FC][i];
		ch = dfxlat(ch,i);
		if (i==fldpos) x=getcur(0);
		rputcinc(ch,b,f,NO,0);
		}

	crtrtc(x);

}
/****************************************************************/

byte dfxlat(ch,i)	/* Does translation */
	byte ch;	/* returns translated character */	
	int   i;

{	static int j,k;
	byte       x;

	if (ch==0) return(' ');

	if (INVINPUT(mkfldp)) return(' ');

	if (ch==FILL) { return(*(TRANTABL(fieldp)+j*8+k++)); }

	if (i>= NUMTRAN(fieldp)) return(ch);

	for (j=0; (x=*(TRANTABL(fieldp)+j*8))!=FIN; j++) {
		if (x==ch) {
			k=2;
			return(*(TRANTABL(fieldp)+j*8+1));
			}
 		}

	return(ch);
}

/****************************************************************/
/*								*/
/*     	DSPMSK - Display current mask & data			*/
/*		 (Field Pointers are saved and restored)	*/
/*								*/
/****************************************************************/

void dspmsk()

{
	int x,y;

	x=fieldp;
	y=mkfldp;

	mkfldp = maskp+2;

	fndfld(FLDCODE(mkfldp));

	do { dspfld(); } while (nxtfld());

	fieldp=x;
	mkfldp=y;
	dspfld();
}
/****************************************************************/
/*								*/
/*	INSERT - Inserts one character into current field	*/
/*		 data buffer.  Pass the character as the 	*/
/*		 arguement.					*/
/*								*/
/*	Returns - 0 if at the end of the field. (Move to next)	*/
/*		  1 otherwise (stay in this field)		*/
/*     								*/
/*		  If not enough room for translated input,	*/
/*		  error will be displayed, data will be 	*/
/*		  ignored, & return (1) 			*/
/*								*/
/*								*/
/****************************************************************/
	     
insert(ch)
	byte ch;

{	int len, ret, FC, i;

	if ( (tstnum(ch)) && (escflg >0) ) esccd = esccd * 10 + ch - 0x30;
	else esccd = 0;

	len=fdlen(ch);

	ret = 1;   				/* Normal return */

	if (len+fldpos > NUMCHAR(fieldp)) {
				seemsg(50+beep);
				return(1);
				}
	if (len+fldpos == NUMCHAR(fieldp)) ret = 0;

	FC= FIELDCD(fieldp);

	fdata[FC][fldpos] = ch;

	if (len > 1) for (i=1; i<len; i++) fdata[FC][fldpos+i]=FILL;

	fldpos += len;

	return(ret);
}

/****************************************************************/
/*								*/
/*     Character Group Testing:					*/
/*								*/
/*	TSTALP - Test Alphabetic, "A" through "Z" or Space.	*/
/*		  (Pre-processor #DEFINation at top)		*/
/*								*/
/*	TSTNUM - Test Numneric, "0" through "9".		*/
/*     		  (Pre-processor #defination at top)		*/
/*								*/
/*	TSTGRX - Test Graphic, "*" through "/" & 0x60, 0x61	*/
/*		  (0x60 & 0x61 are special codes for "$" & ":"	*/
/*		  Required because the ASCII codes for these	*/
/*		  characters have already been assigned to 	*/
/*     		  field codes. Note that TSTGRX will replace	*/
/*		  these codes with the proper ASCII values.	*/
/*								*/
/*	TSTFC -  Test Field code, as defined in field tables.	*/
/*								*/
/*	TSTACT - Test Keyboard action codes, 0xE0 through 0xFF	*/
/*		  INQ Field Keys, ALTOK.			*/
/*     								*/
/****************************************************************/
	     
tstgrx(ch)
	byte *ch;

{
	if (*ch == '\x60') {
			*ch ='$';
			return(TRUE);
			}

	if (*ch == '\x61') {
			*ch =':';
			return(TRUE);
			}
	     
	if ((*ch <'*') || (*ch > '/')) return (FALSE);

	return(TRUE);

}

tstact(ch)
	byte ch;
	
{
	if ((ch== 0x1d) || (ch == 0x1e) || (ch == 0x1f)) return(TRUE);

	if ((ch == FALTOK) || (ch==FNEXT)) return (TRUE);

	if (ch < 0xDB)  return (FALSE);

	return(TRUE);
}

tstfc(ch)
	byte ch;

{	int i=0;

	do { if (FIELDCD(i)==ch) return(TRUE);
	   } while (FIELDCD(i++) !=FIN);

	return(FALSE);
}

/****************************************************************/
/*								*/
/*	FWDSPC - Forward space cursor and fldpos, if at end	*/
/*		 if the field, move to start of field.		*/
/*								*/
/****************************************************************/
	     
void fwdspc()

{
	int FC;

	FC=FIELDCD(fieldp);

	do {
		if (++fldpos >= NUMCHAR(fieldp))  fldpos=0;
		else if ((fdata[FC][fldpos]==0) && (fdata[FC][fldpos-1]==0))
		      fldpos=0;				     
		} while (fdata[FC][fldpos]==FILL);

	dspfld();
}

/****************************************************************/
/*								*/
/*     	BCKSPC - Backspace cursor and FLDPOS. If at beginning	*/
/*		 of field, move to last postion with data	*/
/*		 entered.					*/
/*								*/
/****************************************************************/
	     
void bckspc()

{	int FC;

	if (xlpt) {
		   kbuf[xlpt--]=0;
		   return;
		   }
	
	FC=FIELDCD(fieldp);

	if (fldpos==0) { while (fdata[FC][fldpos]) {fldpos++;}  }

	else do {fldpos--;} while (fdata[FC][fldpos]==FILL);

	if (fldpos==NUMCHAR(fieldp)) fldpos--;

	dspfld();
}

/****************************************************************/
/*								*/
/*   	BCKDEL - Backspace cursor & FLDPOS, delete characters	*/
/*		 If at beginning of field, do nothing		*/
/*								*/
/****************************************************************/
	     


void bckdel()

{	int FC;

	if (xlpt) {
		   kbuf[xlpt--]=0;
		   return;
		   }
	
	FC=FIELDCD(fieldp);

	if (fldpos==0) return;

	do {
		fdata[FC][fldpos]=0;
		fldpos -=1;
		} while (fdata[FC][fldpos]==FILL);

	fdata[FC][fldpos]=0;
	dspfld();
}
					 	

/****************************************************************/
/*								*/
/*     	LOCFLD - Locate a field in the current mask.		*/
/*		 Pass field code as arguement.			*/
/*								*/
/*	returns - TRUE with pointers set if located.		*/
/*		  FALSE if not in current mask, pointers 	*/
/*		  unchanged.					*/
/****************************************************************/
	     
locfld(FC)
	int FC;
{ 	int i;

	if (moptst(FC)) return(FALSE);

	i=maskp+2;

	while (FLDCODE(i) != FC) {
				if (FLDCODE(i)==FIN) return(FALSE);
				++i;
				}
	mkfldp=i;
	fndfld(FC);
	return(TRUE);
}

/****************************************************************/
/*								*/
/*	NEWMSK - Switch to new mask.				*/
/*		 Enter with Mask Pointer set. 			*/
/*								*/
/****************************************************************/
	     
void newmsk()	
	
{
	setnds();		/* Set No-display bits */
	clrdat(); 		/* Clear all fields in this mask */

	if (!clretn()) {  
			if (SMALLMSK(maskp)) smallscr(YES);
			else largescr(YES);
			} 
  
	mkfldp=maskp+2;

	fndfld(FLDCODE(mkfldp));
	dspmsk();
	newfld();
}
/****************************************************************/
/*								*/
/*	NEWFLD - sets up a new field.				*/
/*	BCKFLD - Moves cursor back one field.			*/
/*	FWDFLD - Moves cursor forward one field.		*/
/*								*/
/****************************************************************/

void newfld()
{
	fldpos=0;
	xlpt=0;
	dspfld();
}

void bckfld()
{
	do {  
	     do {lstfld();} while (nodisp());
	   } while (moptst(FIELDCD(fieldp)));

	newfld();
}

void fwdfld()
{
	do {
		do {nxtfld();} while (nodisp());

	   } while (moptst(FIELDCD(fieldp)));

	newfld();
}
/****************************************************************/
/*								*/
/*  VALCHA - Validate input character for current field 	*/
/*	     							*/
/*	 Returns TRUE - if character is valid.			*/
/*		 FALSE- If not.					*/
/*								*/
/****************************************************************/
	     
valcha(ch)	
	byte ch;
{
	if (NUMEROK(fieldp)) { if (tstnum(ch)) return(TRUE);}
	if (ALPHOK(fieldp))  { if (tstalp(ch)) return(TRUE);}
	if (GRAPHOK(fieldp)) { if (tstgrx(&ch)) return(TRUE);}
	return(FALSE);
}
/****************************************************************/
/*								*/
/*	FDLEN - Finds display length for passed data character	*/
/*		in current field.				*/
/*	Returns length.  Returns 1 if no translation needed	*/
/*		Returns 0 if translation required and char.	*/
/*		is not in table.				*/
/*								*/
/****************************************************************/

fdlen(ch)
	byte ch;
{
	int j;
	byte x;
		 
	if (fldpos>=NUMTRAN(fieldp)) return(1);

	for (j=0; (x=*(TRANTABL(fieldp)+j*8))!=FIN; j++) {
		if (x==ch) {return(strlen(TRANTABL(fieldp)+j*8+1)); }
		}

	return(0);
}

/****************************************************************/
/*								*/
/*   	CLRDAT - Clear all fields data buffers not in 		*/
/*		 Current mask.					*/
/*								*/
/****************************************************************/

void clrdat()

{
	int i,FC;

	for (i=0; (FC=FIELDCD(i)) !=FIN; ++i) {
		  if (locfld(FC)==0) clrfdx(FC);
		  if ((fdata[FC][0] & 0x7F)!=0) fdata[FC][0] &= 0x7F; 
		  }
}

/****************************************************************/
/*								*/
/*   	CHRETN - If moving from FRET or CRET to RETN mask	*/
/*		 Set RETNFG indicator, otherwise clear it.	*/
/*								*/
/*		 (if it is set, later only the top half the	*/
/*		  screen will be cleared)			*/
/*								*/
/****************************************************************/
	     
void chretn()	
{
	retnfg=0;

	if ((strncmp(MASKNAME(maskp),"FRET",4)) && (strncmp(MASKNAME(maskp),"CRET",4))) ;
	    else if (strncmp(fdata[FSPEC],"RETN",4)==0) retnfg=1;

}

/****************************************************************/
/*								*/
/*   	CLRETN - If RETNFG is set, Clear top seven lines on CRT	*/
/*		 & return(TRUE).  Otherwise, Do nothing &	*/
/*		   Return(FALSE).				*/
/*								*/
/****************************************************************/
	     
clretn()

{
	int x,y;

	if (retnfg==FALSE) return(FALSE);
	if (tsmode == '0') return(TRUE);

	y=prback*0x1000+32;
	for (x=0;x<(tvmaxc+1)*12;x+=2) 
			pokew(0xb800,x,y);	/* Clear 240 postions from (0,0).  (6 lines) */

	retnfg=FALSE;
	return(TRUE);
}

/****************************************************************/
/*								*/
/*    Do Xlate - Translate keyboard input to date field 	*/
/*		 message.					*/
/*								*/
/*    Returns -  -1  invalid key				*/
/*		  1  non-unique match				*/
/*		 chr Translated character for unique match	*/
/*								*/
/****************************************************************/

byte do_xlate(key)	
	byte key;
{
	byte c;
	int lp1;
	
	kbuf[xlpt++] = key;
	kbuf[xlpt] = '\0';

	for (lp1 = 0; xdate[lp1].ident != FIN; lp1++) {
	        c = xdate[lp1].xlation;

		if (!strncmp(kbuf, xdate[lp1].key, xlpt))
			if (xdate[lp1].ident == xlpt) {
				xlpt=0;
				return (c);	  	/* unique match */
				}
			else 
				return (1);		/* match, but not unique*/
	}

	xlpt=0;
	return (-1);	/* invalid key */
}

