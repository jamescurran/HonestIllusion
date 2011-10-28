/****************************************************************/
/*								*/
/*	SEFMTB - S & E fromat multi-break tickets & print	*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

#include "se.h"
#undef toascii

#define HILO(x) 	((x<<8)+ ((x>>8)&255) ) /* convert integer in cyber Hi:Lo*/
#define toascii(x,p,y)  sprintf(x,p,y)
#define tfb7l		308


	extern	byte	txsec1[250];	/* Tixket - section 1   These are    */
	extern	byte	txsec2[250];	/* Ticket - Section 2   set directly */
	extern	byte	txsec3[250];	/* ticket - Section 3   from input   */
	extern	byte	txsec4[250];	/* Ticket - section 4   messages     */

	extern	byte	tfb7[308];	/* 7 line ticket buffer  (44 x 7)    */
	extern	byte	tfbseq; 	/* ticket buffer sequence last recieved */
	extern	byte	numtx;		/* ticket counter for break 	     */
	extern	int	seatpp[3];	/* seat print positions 	     */
	extern	int	trcpp;		/* return code print postion	     */
	extern	byte	seatnm[5];	/* ascii seat number		     */
	extern	byte	trcnm[9];	/* ascii return code		     */
	extern	byte	seatit; 	/* seat indicator  (NUM / LETTER)    */
	extern	char	seatin; 	/* seat increment (can be negitive)  */
	extern	int	auditl; 	/* number of characters in audit trail */
	extern	byte	audit[56];	/* audit trail			     */
	extern	int	rcinc;		/* return code inc (set: sradj used:seinsr) */

	extern	byte	tslite; 	/* credit sales mode flag :
					0x00 = not in credit sales mode
					0x01 = receiving credit sales tix
					"1"  = in credit sales referral processes */



	extern	byte	tsmode; 	/* Telephone mode flag :
					00  = not in credit sales mode
					"0" = Ticket mask
					"1" = credit card correction mask
					"2" = Customer data mask
					"3" = report (ts/credit sales) */
	extern	byte	ntick[8][52];

	byte	__SEFMTB;
	byte	numbk;		/* Number of breaks */
	byte	ntsc;		/* Multi-break tix buffer select code. */
	int 	*ctb;		/* address of compressed ticket buffer */
	int	curbk;		/* Current break being processesed */
	int	slen;		/* seat number length */
	int	bseatn;		/* Seat Number in Binary */
	long	brc;		/* return code in binary */

	int	pftab[9];

#define pftab1	pftab[0]
#define pftab2	pftab[1]
#define pftab3	pftab[2]
#define pftab4	pftab[3]
#define pftab5	pftab[4]
#define pftab6	pftab[5]
#define pftab7	pftab[6]
#define pftab8	pftab[7]
#define pftab9	pftab[8]

	int	pie[6];
#define pie1	pie[0]
#define pie2	pie[1]
#define pie3	pie[2]
#define pie4	pie[3]

	union {
		byte	bm[944];
		int	bmb[472];
		} tix;

#define bfctrl	tix.bm[0]
#define ipit	tix.bmb[4]
#define ircit	tix.bmb[5]
#define isnit	tix.bmb[6]
#define iat	tix.bmb[7]
#define icdht	tix.bmb[8]
#define igit	tix.bmb[9]
#define iuit	tix.bmb[10]
#define isrit	tix.bmb[11]
#define itfb	tix.bmb[12]
#define inwtmb	tix.bmb[13]

	long	estp;
	byte	atotp[13],aestp[13],ataxp[6],aregp[12];
	byte	*strins();
	void	ckt3(),tixrc(),longswp(),longascii(),tixprc(),tixpmv();


void _sefmtb_() {}


/****************************************************************/
/*								*/
/*     SEFMTB							*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

sefmtb()

{
	byte	*ptr,c;
	int	len,x,flg;

	ntsc= txsec1[0];
	len = txsec1[1];
	ptr = tix.bm;

	ptr = strins(ptr,&txsec1[4],len);

	c= txsec2[0];	/* get code segment for later test */
	len = txsec2[1];

	if (len) {
		  if (c!=ntsc) return (FALSE);

		  ptr = strins(ptr,&txsec2[4],len);

		  c=txsec3[0];
		  len = txsec3[1];
		}

	if (len) {

		if (c != ntsc) return (FALSE);

		ptr = strins(ptr,&txsec3[4],len);

		c = txsec4[0];
		len = txsec4[1];
		}

	if (len) {

		if (c != ntsc) return (FALSE);

		ptr = strins(ptr,&txsec4[4],len);
		}

	if (ircit) 	ircit = HILO(ircit);
	if (isnit)	isnit = HILO(isnit);
	if (icdht)	icdht = HILO(icdht);
	if (ipit)	ipit  = HILO(ipit);

	if (icdht) {
		if ((igit) | (iuit) | (isrit) | (itfb)) {;}
		else return(FALSE);

		x=uncmp(&tix.bmb[icdht-1]);
		if (x==FALSE) return(FALSE);

		len = 154-x;

		if (igit)   igit = HILO(igit)-len;

		if (iuit)   iuit = HILO(iuit)-len;

		if (isrit)  isrit= HILO(isrit)-len;

		if (itfb)   itfb = HILO(itfb)-len;

		numtx=1;
		septb();

		}

	tfbseq=sequp(tfbseq);

	ctb = &tix.bmb[itfb-1];
	x=uncmp(ctb);
	if (x==FALSE) return(FALSE);

	seclri();
	flg=0;
	switch (tix.bm[0]) {
		case '0':
			numtx=1;
			septb();
			break;
		case '1':
			numtx=tix.bm[1];
			septb();
			break;
		case '2':
			flg=ckt2();
			if (flg==0) septb();
			break;
		default:
			numbk=tix.bm[1];
			curbk=1;
			for ever {
				ckt3();
				septb();
				if (++curbk > numbk) break;
				uncmp(ctb);
				seclri();
				}
		}

	if ((tsmode) | (tslite)) {;}
	else  tslite = 1;

	if (iat) {
		iat = HILO(iat);

		if (icdht) x = icdht;
		else if (igit) x = igit;
		     else if (iuit) x = iuit;
			  else	if (isrit) x = isrit;
				else x = itfb;

		tix.bmb[x-1] = 0x0A0D;

		auditl = (x - iat +1) * 2;
		strins(&audit,&tix.bmb[iat-1],auditl);

		}

	ackaud();

	if (tslite==1) tslite =0;

	return(TRUE);
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

ckt2()
{	int x;

	numtx = tix.bm[1];
	tixprc(1);
	curbk=1;
	tixrc();
	septb();

	numtx = tix.bm[2];
	x=uncmp(ctb);

	tixprc(2);
	curbk=2;
	tixrc();
	septb();

	numtx = tix.bm[3];
	if (numtx == 0) return(1);

	uncmp(ctb);
	tixprc(3);
	curbk=3;
	tixrc();
	return(0);
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

void ckt3()
{
	int 	w,y,z;
	int	*x;

	if ((igit) && ( (y=HILO(tix.bmb[igit-1])) ))
			strins(&tfb7[y-1],&tix.bmb[igit+curbk*3-3],6);

	if ((iuit) && ( (y=HILO(tix.bmb[iuit-1])) ))
			strins(&tfb7[y-1],&tix.bmb[iuit+curbk*3-3],6);

	if (isrit) {
		  for (z=3;z>0;--z) {
			x=&tix.bmb[isrit+curbk*4+1];
			w=HILO(tix.bmb[isrit+z+1]);
			if (w) strins(&tfb7[w-1],x,4);	

			x=&tix.bmb[isrit+curbk*4+3];
			w=HILO(tix.bmb[isrit+z-2]);
			if (w) strins(&tfb7[w-1],x,3);
			}
	}

		if (ipit) {
		y=HILO(tix.bmb[1]);
		y = (y >> (16-(2*curbk))) & 3;
		tixprc(y);
		}
		
	if (ircit) tixrc();

	if (isnit==0) return;

	for (z=0;z<3;++z) seatpp[z]=HILO(tix.bmb[isnit-1+z]);
	
	y = HILO(tix.bmb[isnit+curbk*3]);
	seatin = y & 0x000F;
	seatit = y & 0x0010;
	if (seatit) seatin = - seatin;

	slen=4;
	z=HILO(tix.bmb[isnit+curbk*3+1]);
	y=(z>>1) & 0x1F;
		if (y) {		/* if letter suffix */
			y+=0x40;	/* convert to a letter */
	   		seatnm[3]=y;
			slen=3;
			if (z & 1) {
				seatit=1;
				y=HILO(tix.bmb[isnit+curbk*3+3]);
				if (y & 8) seatin =-1;
				else seatin =0;
				}
			}
	 
	numtx=(z >>6) & 0xFF;
	
	z=HILO(tix.bmb[isnit+curbk*3]);

	bseatn = (z>>5) & 0x7FF;
	toascii(seatnm,"%-4d",bseatn);

 	for (z=0;z<3;++z) 
		if (seatpp[z]) strins(&tfb7[seatpp[z]-1],seatnm,4);

	rcinc=1;
	sercaj();

					
}
/****************************************************************/
/*								*/
/*	UNCMP - Uncompress ticket buffer into TFB7		*/
/*	      Enter with Address of of compressed text		*/
/*								*/
/*	Returns FALSE = Error in decompress    or		*/
/*		Number of words.				*/
/*								*/
/****************************************************************/

uncmp(p)
	byte	*p;
{
	int	x,y,w;
	byte 	*s;

	blank(tfb7,tfb7l);

	x=tfb7l;	
	s=tfb7;
	w=1;

	do {
		w++;
		if (*p>127) {
				y=(*p) & 127;
				x-=y;
				if (x<0) return(FALSE);
				s+=y;
		      		}
		else {
				x--;
				*s++= (*p==0) ? ' ': *p ;
				}
		p++;
	} while (x);

	return(w/2);
}

/****************************************************************/
/*								*/
/*	TIXRC - Obstructed view and return code formatting	*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
void tixrc()
{
  	int 	x,y;

	x=HILO(tix.bmb[ircit-1]);

	if (x) {
 		y = bfctrl << curbk;
		if (y<255) strins(&tfb7[x-1],"OBST VIEW",9);
		}

 	x=HILO(tix.bmb[ircit]);
	if (x==0) return;

	trcpp=x;
	longswp(&brc,&tix.bmb[ircit+curbk*2-1]);
	longascii(&brc,trcnm);
	strins(&tfb7[trcpp-1],trcnm,8);

}			      

void longswp(x,y)
	byte  x[4],y[4];	/* so they're really long int, big deal! */
{
	int z;
	for (z=0;z<4;++z)     x[z]=y[3-z];
}


void longascii(x,y)
	int	x[2];
	byte	y[8];
{	int	z;

	toascii(&y[0],"%4d",x[0]);
	toascii(&y[4],"%4d",x[1]);
	for (z=0;z<8;++z) 
		 if (y[z]==' ')  y[z]='0';
}

/****************************************************************/
/*								*/
/*	TIXPRC - Format prices on ticket.			*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
						
void tixprc(x)
	int	x;
{
	int 	*y;
	int 	z,w;
	float	fl;

	if (ipit==0) return;

	y=&tix.bmb[ipit-1];
	strins(pftab,y,18);

	y=&tix.bmb[ipit + (x*6) +3];
	strins(pie,y,12);

	if ((pie4 & 0xff) < ' ')  strins(&pie4,"      ",6);

	for (z=0;z<9;++z) pftab[z] = HILO(pftab[z]);

	pie1=HILO(pie1);
	pie2=HILO(pie2);
	pie3=HILO(pie3);

	w=pie2;
	z=pie1-pie3;
	while (z<0) {	z+=10000;
			w--;
 			}
    
	if ((pftab8==pie1) && (pftab9==pie2)) pftab1=0;
	else {
		fl=pftab9*100 +(pftab8/100.0);
		toascii(aregp,"$%-7.2f",fl);
		}	
 	
	fl=w*100 + (z/100.00);	   /* equalevalent to ((w*10000) + z ) /100.00 */
 	toascii(aestp,"$%-7.2f",fl);

	fl=pie2*100 + (pie1/100.00);
	toascii(atotp,"$%-7.2f",fl);

	fl=pie3/100.0;
	toascii(ataxp,"%-3.2f",fl);

	tixpmv(pftab1,aregp,11);
	tixpmv(pftab2,aestp,11);
	tixpmv(pftab3,atotp,11);
	tixpmv(pftab4,atotp,11);
	tixpmv(pftab5,ataxp,6);

	if (pftab6) strins(&tfb7[pftab6-1],&pie4,6);
	if (pftab7) strins(&tfb7[pftab7-1],&pie4,2);
		
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

void tixpmv(x,p,y)
	int	x,y;
	byte	*p;

{	byte	*s,ch;

	if (x==0) return;

	s=&tfb7[x-1];

	for (;y>0;--y) {
		ch=*p;
		if ( (ch == 0) || (ch== ' ') ) return;	
		*s=ch;
		p++; s++;
		}
}
				      
	
