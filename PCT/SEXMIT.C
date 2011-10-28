/****************************************************************/
/*								*/
/*	S E  X M I T -						*/
/*								*/
/*	S & E  PC Transmit key routines.			*/
/*								*/
/****************************************************************/

#include <dos.h>
#include "SE.H"
#include "SEFDEF.H"

	extern	union	REGSS inregs,outregs;

	extern byte semask[4],tsmode,tmflg,fdata[][35],pollcd,filename[];
	extern int  maskp,mkfldp,fieldp,tvmaxc,crttim,prstat,label,cpsc2;
	extern int  offseq,offflg,STDCOLOR,escflg;

	byte	__sexmit;

	int scflag,inqflg;

	byte xmsg[256];
#define xlen xmsg[1]
#define xdata &xmsg[8]

	void	_sexmit_(),xmit(),alttst(),moptst2(),valtst();

void _sexmit_()
{}

/****************************************************************/
/*								*/
/*    XMIT - Transmit key Action routine.			*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

void xmit()
		
{	int FC,i,MC;

	byte *p,x;

	i=0x0600;					/* Line 7  */
	
	if ( (inqflg==0) && (( (x=CSSEQ(maskp)) <'1') || (x > '2') ) ) {
		if (SMALLRES(maskp))	{
					smallscr(YES);
					if (x=='3') i=0x0C00;
					}
		else 			largescr(YES);

		dspmsk();  
		}
	else {
		inregs.h.ah = 0x0f;
		int86s(16,&inregs,&outregs);
		}

	curset(tvmaxl,tvmaxc-5,0);
	rprintf(STDCOLOR,0,"*%d*",pollcd);
	crtrtc(i);
	escflg=10;  

	offflg=FALSE;
	inqflg=FALSE;

	if (locfld(FINQ)) {
			x=fdata[FINQ][0];
			if ( (x=='1') || (x==0) || (x==128) ) inqflg=TRUE;
			}

	tsmode = CSSEQ(maskp);
	tmflg  = MONYTRAN(maskp) >>6;

	if ( (MC=MSKCODE(maskp) )==83) {
				ccrd();
				return;
				}
	if ( (MC>85) && (MC<90) )	{
/*			raw();	*/			
			strcpy(filename,"TEMPFILE.RAW");
		    	offinit();
			}

	xlen = 0;
	p = xdata;
	mkfldp=maskp+2;

	for (mkfldp=maskp+2; (FC=FLDCODE(mkfldp))!=FIN; ++mkfldp) {
		 x=fdata[FC][0];
		 if ((x>0) && (x <128)) {
			*p++ = FC;
			*p++ = x;
			xlen+=2;
				for (i=1; i<35; ++i) {
				if ((x=fdata[FC][i])==0) break;
				if (x!=FILL) {
					*p++=x;
					xlen++;
					}
				}
			}
		}    

 	if (MC==80) 	{	
			strcpy(filename,"TEMPFILE.R71");
			offinit();
			}						      	
	scflag=0;

	if (tsmode >= '0' && tsmode < '3')  swpscr();

	sesend(xmsg);

	if (tsmode >= '0' && tsmode < '3')  {
						ttyt(LF);
						swpscr();
						}

	matrix(CR); matrix(LF);

	alttst();
	moptst2();
	valtst();

	mkfldp=maskp+2;
	fndfld(FLDCODE(mkfldp));
	newfld();
}

/****************************************************************/
/*								*/
/*     ALTTST - Altok Test, if Altok field is displayed,	*/
/*		set no display bit, and clear from screen.	*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
void alttst()
{	byte *x;

	if (!locfld(FALTOK)) return;

	x=&fdata[FALTOK][0];
	
	if (*x==128) return;

	*x=128;

       ratsay(DISPROW(mkfldp),DISPCOL(mkfldp),label,0,"      ");
}
/****************************************************************/
/*								*/
/*   	MOPTST2 - if MOP field is present in current mask	*/
/*		  clear data & re-display field.		*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
void moptst2()

{ 	byte x;

	if (!locfld(FINQ)) return;

	x =fdata[FINQ][0];

	if ((x=='2') | (x=='3')) ;
	else return;

	if (!locfld(FMOP)) return;

	if (fdata[FMOP][0]==128) return;

	clrfdx(FMOP);
	dspfld();
}

/****************************************************************/
/*								*/
/*     VALTST - Starting Date Test, if Start Date field is 	*/
/*		displayed, set no display bit, and clear from	*/
/*		screen.						*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
void valtst()
{	byte *x;

	if (!locfld(FINQ)) return;

	x =&fdata[FINQ][0];

	if ((*x=='2') | (*x=='3')) ;
	else return;

	if (!locfld(FCDVAL)) return;

	x=&fdata[FCDVAL][0];
	
	if (*x==128) return;

	*x=128;

       ratsay(DISPROW(mkfldp),DISPCOL(mkfldp),label,0,"     ");
}
