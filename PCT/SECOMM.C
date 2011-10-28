/****************************************************************/
/*                                                              */
/*      S E   C O M M                                           */
/*                                                              */
/*            Application Level Communication Routines          */
/*                                                              */
/*                                                              */
/*                                                              */
/****************************************************************/

#include "se.h"			/* Standard PCT Header file 	*/
#include "sekdef.h"		/* Keyboard Definations     	*/
#include "sefdef.h"		/* Field/Mask definations	*/

        extern  noscrl,scflag,tvmaxc,maskp,rtstmr,crttim,__sedscp;
	extern	offend,escflg,STDFORE,STDBACK,foreg,ccrdflg;


/*      Message Codes						*/

#define POLL    0x0E		/* Poll Code 			*/
#define CRAUO   0x10		/* Credit authorization OK	*/
#define CRAUB   0x11		/* Credit authorization bad	*/
#define CRSLD   0x12		/* Credit authorization done	*/
#define FORCE   0x15		/* Terminal force load 		*/
#define PGLOAD  0x17		/* terminal memory page		*/
#define TTY     0x18		/* TTY (Crt message) data	*/
#define TIXOLD  0x19		/* Ticket Buffer (old format)	*/
#define TIPOFF  0x1A		/* Transaction-in-progress off  */
#define TIXNEW  0x1B		/* Ticket buffer (new format)	*/
#define ACK     0x1D		/* Acknowledgement request	*/
#define RETRAN  0x1E		/* Retransmit request		*/

        static byte messcode[13] ={POLL,CRAUO,CRAUB,CRSLD,FORCE,PGLOAD,TTY,
                                    TIXOLD,TIPOFF,TIXNEW,ACK,RETRAN,0};

	static byte pgmsg[13] = {0,8,0,0,0x12,0,0,0,0,0,0,FINISH,0};

#define ENDTXT  0x1F            /* end of ticket text */
#define STATID  0x1B            /* Terminal statistics ID character */
#define CUSTID	0xB9		/* Customer Number ID Character */
#define TSMCOD  0x80            /* telephone sale mode code. */
#define ACKID   0x70            /* Acknowledgement id character */
#define VERCOD  0x18            /* Version code field ID character */
#define TSMODE  0x80            /* Telephone sales mode code ID Character */
#define PSCOD   0xBF            /* Printer status code */

#define CHECKERR -1             /* Flag for checksum error */
#define MXTTY    54             /* Maximum number of TTY character to accept */
#define MXRTRN   10
#define ACKL     14             /* Intemediate ACK length */
#define ACKCL    18             /* total ACK Length */
#define MXRACK    3             /* Maximum ticker reacknowledgement tries */
#define SZMAX   250             /* Maximum size of any ticket section. */
#define NMXTSC  240             /* Maximum character + a few in new ticket */
#define MXTSEC  113             /* Maximum character inold ticket section + 1 */


/*	HOWPRT responses						*/

#define CRTMTX    0
#define MTX       1
#define OFFON     2
#define DONT	  3

#define LOHI(x) ((x <<8) + (x>>8)) /* reverse bytes of an interger */

        byte    __secomm;	/* Label start of data in PCT.MAP 	*/

        int     itstat;         /* Terminal status word 		*/
        int     itopt1;         /* Terminal option word 1 		*/
	int	custno;		/* Customer Number (32750 = Super Terminal) */
        byte    vernum;         /* Terminal progran version number 	*/
        byte    pollcd;         /* Terminal poll code (0-15) 		*/
        byte    unitsl;         /* Terminal unit select code (0x60 - 0x6F) */
        byte    tmflg;          /* money / tix expected flag 		*/
        byte    tsmode;         /* Telephone mode flag :
                                        00  = not in credit sales mode
                                        "0" = Ticket mask
                                        "1" = credit card correction mask
                                        "2" = Customer data mask
                                        "3" = report (ts/credit sales) */

        byte    txsec1[250];    /* Tixket - section 1      These are   	*/
        byte    txsec2[250];    /* Ticket - Section 2      set directly */
        byte    txsec3[250];    /* ticket - Section 3      from input   */
        byte    txsec4[250];    /* Ticket - section 4      messages     */

        byte    tfb7[308];      /* 7 line ticket buffer  (44 x 7)       */
        byte    numtix;         /* number of tickets in pseudo- break   */
        byte    ltixi;          /* last ticket indicator (MORE/LAST)    */
        int     seatpp[3];      /* seat print positions                 */
        int     trcpp;          /* return code print postion            */
        byte    seatnm[4];      /* ascii seat number                    */
        byte    trcnm[8];       /* ascii return code                    */
        byte    seatit;         /* seat indicator  (NUM / LETTER)       */
        byte    seatin;         /* seat increment (can be negitive)     */
        int     auditl;         /* number of characters in audit trail  */
        byte    audit[56];      /* audit trail                          */
        int     rcinc;          /* return code inc (set: sradj used:seinsr) */
        int     ackflg;         /* terminal ack retry counter */
        byte    prstat;         /* printer status (2 = disabled, enabled otherwise) */
	int	offflg;		/* True if currently processing an off report */
	int	offseq;		/* Sequence number of next R71 block	*/


        union {
                byte buf[18];   /* Ticket Acknowledgement */
                int  bufi[9];
                } ack;
#define ablth  ack.buf[1]       /* ack internal length 			*/
#define ab3    ack.buf[2]       /* ver/stat control code 		*/
#define abver  ack.buf[3]       /* version 				*/	
#define ab56   ack.bufi[2]      /* ack status word 			*/
#define ab78   ack.bufi[3]       /* ack option word  			*/
#define abcod  ack.buf[8]       /* ack control code 			*/
#define abseq  ack.buf[9]       /* ack sequence number 			*/
#define abstat ack.buf[10]      /* terminal status control code 	*/
#define ab12   ack.buf[11]
#define ab1314 ack.bufi[6]      /* ack.buf[12] & [13] 			*/
#define ab1516 ack.bufi[7]
#define abcksm ack.buf[16]      /* ack checksum 			*/
#define abfin  ack.buf[17]      /* finish 				*/

        byte    prtcbk[10];     /* ticket printer control block 	*/
#define pstat   prtcbk[7]       /* printer status 			*/

        byte    *msgout;        /* address of last request sent 	*/
        int     msglth;         /* length of  last request sent 	*/
        byte    *tba;           /* address of ticket section being received */
        byte    xtctr;          /* crt error flag since last ack sent 	*/
        byte    tixflg;         /* next old ticket section expected 	*/
        byte    xactr;          /* tix reacknowlegdement counter 	*/
        byte    dom;            /* day of month from poll message 	*/
        int     lttn;           /* last terminal transaction number 	*/
        byte    xectr;          /* retransmit counter 			*/
        byte    tfbseq;         /* ticket buffer sequence last recieved */
        byte    stopt;          /* stop tty/crt keyboard code entered flag */
        byte    numtx;          /* ticket counter for break 		*/
        int     actime;         /* active communication line timer count*/
        int     ustime;         /* unit select timer count 		*/
        byte    ttyrec;         /* flag if tty has been received 	*/
        byte    lfflag;         /* Flag if last byteacter was a line feed */
        byte    upttn;          /* Flag if lttn should be updated next poll */
        int     mtxonly;        /* flag to send data to matrix only 	*/
        byte    tslite;         /* credit sales mode flag :
                                        0x00 = not in credit sales mode
                                        0x01 = receiving credit sales tix
                                        "1"  = in credit sales referral processes */
        byte    svmask[4];      /* saved mask name (during cedit sales referal)*/
        byte    semask[4];      /* if non-zero, mask to display (set:secomm, used:sein) */


	void	_secomm_(),seout(),testss(),segmsg(),sesend(),ackaud();
	void	sndack(),senack(),ackrdy();

        byte 	nxtchr();

void _secomm_() {}

/****************************************************************/
/*                                                              */
/*  SE OUT   - Outputs a string in character arry MSG to basic. */
/*             Length of string in int LEN.  Adds second Finish */
/*             if length is odd.                                */
/*								*/
/****************************************************************/

void seout(msg,len)
        byte *msg;
        int  len;

{       int  j;

        for (j=0; j < len; ++j)
                outnz(comm_port,*msg++);

        if (odd(len)) outnz(comm_port,FINISH);

        outnz(comm_port,0);             /* make sure everything is done */
        outnz(comm_port,0);
        outnz(comm_port,0);
        outnz(comm_port,0);

        NRTS0();			/* Turn off RTS 		*/
}


/****************************************************************/
/* SE GETC   - Receives and returns a character from the basic. */
/*             Prints errors messages if input buffer overflows */
/*             or more than one minute passes without anything  */
/*             being received.                                  */
/****************************************************************/

byte segetc()

{       int   emsg,status;
        byte  chr;

        emsg = 0;

        do  {                                      /* loop until status ok */

                actime = 0;

                do      {                                  /* loop until data ok,  */
                        testss();
			if (escflg==3) break;
                        chr =in(comm_port,&status);        /*   data overflow, or  */
                        if (actime > 60) break;            /*   one minute passes  */
                } while (status==2);

		if (escflg==3) break;
                if (emsg) seeclr();                /* Clear any error msg  */

                switch (status)  {
                   case 1:                         /* data lost */
                        seemsg(6+beep);
                        emsg = 1;
                        for (actime=0;actime<5;) ; /* wait 5 seconds*/
                        rcvi();
                        break;
                   case 2:                         /* Time out */
                        seemsg(5+beep);
                        emsg = 1;
                        bright;
                        break;

                   }

          } while (status >0);

        return (chr);
}


/****************************************************************/
/*                                                              */
/*  SEGUS  Get unit selects and message code.                   */
/*                                                              */
/****************************************************************/

byte segus()
{       int count,ustime=0,emsg=0;
          byte chr;

        for (count=0;count<12;)    /* 12 = 2 unit select + 1 other*/
          { chr = segetc();

            if (escflg==3) break;

            if (chr==unitsl) ++count;
                else count +=10;

                switch (count) {
/*                      case 1:       1st unit select, do nothing   */
/*                      case 2:       2nd US, still do nothing      */
                        case 3:     /*3rd consec. US, pretend its 2 */
                             count =2;
                             break;
                        case 10:   /* non-US before US, start over  */
                             count = 0;
                             break;
                        case 11:   /* non-US after 1 US, start over */
                             count = 0;
                             break;
                        case 12:  /* US, US, message code; That's It !*/
                                if (ustime >180)                /* if it's been more than 3 minutes */
                                        {ustime = 0;            /* print "Busy Bit?"   */
                                         count =  0;
                                         seemsg(4+beep);
					 emsg=1;}

                                if (strchr(&messcode,chr))  /* if 3rd character is a message code */
					{if (emsg) seeclr();}
  
                                else count = 0;                 /* clean up & go home, otherwise start over */
                        }

         }

        return (chr);
}


/****************************************************************/
/*                                                              */
/*      TESTSS - Test if STOP or SCROL keys were typed on the   */
/*               keyboard.  Set NOSCRL or STOPT, if so.         */
/*                                                              */
/****************************************************************/

void testss()
{       byte c;

        if (kbhit()) {
                c=kbin();
                    switch(c) {
                        case KSTOP:
                                stopt = 1;
                                break;
                        case KSCROL:
                                noscrl = 1;
                                break;
			case ESC:
				if (escflg>0) escflg=3;	 /*Abort, if we haven't */
				break;			 /* transmit yet	*/
                        case KEXIT:
                                goodbye();	/* Return to DOS 	*/

                        case KCLEAR:		/* These are all used   */
                        case 'C':		/* by SEDSCP.c		*/
                        case 'S':
                        case ' ':
                                __sedscp =c;
                                break;
                        }
                }
}


/****************************************************************/
/*                                                              */
/*      SEQUP - Increment arguement using modulo 31             */
/*                                                              */
/****************************************************************/

int sequp(x)
        int x;
{
        if (++x > 30) x=2;
        return (x);
}


/****************************************************************/
/*                                                              */
/*      SEGPOL - Get an S & E poll message                      */
/*              returns terminal transaction number from poll   */
/*              and set DOM to day of month from poll message.  */
/*                                                              */
/****************************************************************/

int segpol()
{       int x;
        byte chr;

        rcvi();

        do {    NRTS0();

                do {
                        while ((chr=segus()) != POLL) {
				if (escflg==3) return(0);
				} ;
                   } while ((chr=segetc()) != FINISH);

           } while ((x=gttn()) == CHECKERR);

		RTS0();

        return(x);
}

/****************************************************************/
/*                                                              */
/*      GTTN - Get TTN & Day of Month From Poll. Test Checksum  */
/*              Returns : CHECKERR (-1) if checksum bad.        */
/*                      : TTN if Checksum O.K. (Sets DOM)       */
/*                                                              */
/****************************************************************/

gttn()
{       byte    chr,check;
        int     x;

        check = unitsl + unitsl + POLL + FINISH;

        x = segetc();
        chr = segetc();
        check = check + x + chr;
        x = x * 256 + chr;
        chr = segetc();

        if (chr==check) dom = segetc();
                else x = CHECKERR;

        return (x);
}

/****************************************************************/
/*                                                              */
/*      SEGMSG - Gets a Message from the Cyber And Processes it */
/*                                                              */
/****************************************************************/

void segmsg()
{       byte chr;
        int  x;

 msgb:  rcvi();

 msga:  testss();

 msgc:  chr=segus();
	if (escflg==3) return;		/* Return to input mask, if they */
					/* hit ESC (before message sent	 */
                switch (chr) {
                        case TTY:
                                x=rtty();
                                break;
                        case TIXOLD:
                                x=rtxold();
                                break;
                        case TIXNEW:
                                x=rtxnew();
                                break;
                        case POLL:
                                x=rpoll();
                                break;
                        case ACK:
                                x=rack();
                                break;
                        case TIPOFF:
                                x=rtipof();
                                break;
                        case RETRAN:
                                x=rretrn();
                                break;
                        case FORCE:
                                x=rforce();
                                break;
			case PGLOAD:
				x=rpgload();
				break;
                        case CRAUO:
                                x=crauok();
                                break;
                        case CRAUB:
                                x=craubd();
                                break;
                        case CRSLD:
                                x=crsldn();
                                break;
                }

        if (x==1) goto msga;
        if (x==2) goto msgb;
        if (x==3) goto msgc;
	if (escflg==100) goto msga;	/* escflg=100 mean monitor only mode */
        if (x==4) {			/* x==4 allows for 5 retransmits     */
                        if (ackflg>0) {
                                ackflg -= 1;
                                if (ackflg >0)  rtrans();
                                else {seemsg(15);
                                seemsg(16+beep);
                                        }
                                }
                        }

/*	if (x==5)  returns to input mask 				*/	

	if (offflg==3) {
				offclose();
				offflg=0;
				}

        x = getcur(0);
	ratsay(tvmaxl,tvmaxc,STDBACK*16,0," ");	/*Remove "S" at bottom  */
	escflg=0;     				/* of screen 		*/
	crtrtc(x);

}
/****************************************************************/
/*                                                              */
/*      The next three routines combine to make the HTVT        */
/*      routine RTTY.                                           */
/*                                                              */
/****************************************************************/
/*                                                              */
/*                                                              */
/****************************************************************/
/*      RTTY - receive tty & display on CRT.                    */
/*                                                              */
/****************************************************************/

rtty()
{
        int i;
        byte check,chr;

        ackflg = 0;

        check = unitsl + unitsl + TTY;

        for (i=0; i<=MXTTY; ) {
                chr= nxtchr(&check, &i);
                if (chr == FINISH) break;
                switch(howprt(chr)) {
                        case CRTMTX:
                                ttyt(chr);
                                break;
                        case MTX:
                                matrix(chr);
                                break;
			case OFFON:
				seoff(chr);
				break;
			}
        }

        if ((chr==FINISH) && ((chr=segetc()) ==check)) {}
        else {
                ttyt('^');
                xtctr = 1;
                };

        return (1);
}

/*                                                              */
/****************************************************************/
/*      HOWPRT - Decides where to print the current TTY         */
/*              character.                                      */
/*              Returns : CRTMTX (0) if O.K. to print on CRT.   */
/*                        MTX    (1) if printing only to MATRIX */
/*			  OFFON	 (2) if it's an offline report  */
/*				     (dealt with in SEOFF)	*/
/*			  DONT   (3) Don't print at all		*/
/****************************************************************/

howprt(chr)
        byte chr;

{       int x,line,col;

	if ((offflg==1) && (chr ==2)) {
				offopen();
				offflg=2;
				return (DONT);
				}	
	if (chr == 3) {
			if (offflg==2) 	offflg=3;
			else offflg=0;
			return(DONT);
			}

	if (offflg==2) 		return(OFFON);

	
        if (ttyrec==0)
                if ((chr==CR) || (chr==LF)) return(MTX);
                else ttyrec = 1;

        if (ttyrec !=0) {
                if (chr==LF) {
                        if (lfflag) return(MTX);
                        else lfflag = 1;
                } else if (chr !=CR) lfflag=0;
        }

        if (noscrl==0) 		return(CRTMTX);

        if (scflag) 		return(MTX);

        if (mtxonly) 		return(MTX);

        x = getcur(0);
        line = (x & 0xff00) >>8;
        col =  (x & 0xff);

        if (line < tvmaxl) 	return(CRTMTX);
        if ((col == tvmaxc) || (chr  == LF)) {
                		mtxonly=YES;
                		return(MTX);
		                }

        return(CRTMTX);
}
/*                                                              */
/****************************************************************/
/*      NXTCHR - return next character to print on TTY.         */
/*              by getting new character or expanding           */
/*              compressed data.                                */
/****************************************************************/

byte nxtchr(y,i)
        byte *y;
	int  *i;

{       byte chr;
        static byte lastchar;
        static int  repeat;

        if (repeat > 0) {
                        repeat -= 1;
                        return (lastchar);
                        }

        do { *y = *y + (chr=segetc()); }
                while (chr==0xFF);         /* Get a character, skipping nulls */

	*i++;

        if (chr & 0x80) {
                repeat = (chr & 0x1f) - 1;
	                if ((chr & 0x40)==0) lastchar = ' ';
                return (lastchar);
                }

        if (chr != FINISH) lastchar = chr;
        return (chr);
}
/****************************************************************/
/*      End of RTTY routines                                    */
/****************************************************************/
/*                                                              */
/****************************************************************/
/*                                                              */
/*      RPOLL - Processes poll received when one is not         */
/*              expected.                                       */
/*                                                              */
/****************************************************************/

rpoll()

{       int x,TTN;

        if (segetc()!=FINISH) return(3);

        if ((TTN=gttn())==CHECKERR) return(3);

	if ( (offflg>0) && (offflg<3) ) {
			errcard();
			if (offreq()) return(3);
			}


	if (ccrdflg) {  if (TTN!=lttn+1) x=rtrans();
			else		 x=ccrdxmit();
			return(x);
			}

        if (tmflg==0) {
                        if ((tixflg !=0) && (ltixi==0)) {
                                x=reack();
                                return(x);
                                }
                        else return(4);
                        }

        if (tixflg != 50) {

                        if ((tixflg >0) || ((tixflg <0) && (ltixi ==0)))  {
                                x=reack();
                                return(x);
                                }
                        return(4);
                        }

        if (TTN!=lttn)  {
                                x=reack();
                                return(x);
                                }

        if (ttyrec==0) {
                                x=rtrans();
                                return(x);
                                }

        return(4);
}

/****************************************************************/
/*                                                              */
/*      RTIPOF - Process T.I.P off light message.               */
/*                                                              */
/****************************************************************/

rtipof()

{       byte check,chr;

        if (segetc() !=FINISH) return(3);

        check = unitsl + unitsl + TIPOFF + FINISH;

        chr = segetc();
        if (chr !=check) return(3);

	if (offflg) {
			if (offreq()) return(3);
			}

	if (ccrdflg) return(3);			/* Wait for Poll */

        ackflg = 0;

        if (tixflg == 50)  {
                        ttyrec = 1;
                        return (3);
                        }

        if ((tixflg) && (ltixi==0)) reack();

        return(4);

}
/****************************************************************/
/*      the following three routines were one in the HTVT       */
/****************************************************************/
/*                                                              */
/****************************************************************/
/*                                                              */
/*      CRAUOK - Credit sales authorization OK lite.            */
/*                                                              */
/****************************************************************/

crauok()

{       byte check,chr;

        if (segetc() !=FINISH) return(3);

        check = unitsl + unitsl + CRAUO + FINISH;

        chr = segetc();
        if (chr !=check) return(3);

        ackflg = 0;

        tslite =0;
        semask[0]=0;

	if (svmask[0]==0) return(3);

	strins(semask,svmask,4);
	svmask[0] = 0;

	return(3);


}

/****************************************************************/
/*                                                              */
/*      CRAUBD - Process Credit authorization Bad               */
/*                                                              */
/****************************************************************/

craubd()

{       byte check,chr;

        if (segetc() !=FINISH) return(3);

        check = unitsl + unitsl + CRAUB + FINISH;

        chr = segetc();
        if (chr !=check) return(3);

        ackflg = 0;

        tslite = '1';

	if (tsmode==0) 
		if (strncmp(masks[maskp],"REFE",4))
                                strins(svmask,masks[maskp],4);

        strins(semask,"REFE",4);
        return(3);

}
/****************************************************************/
/*                                                              */
/*      Credit authoratization done                             */
/*                                                              */
/****************************************************************/


crsldn()

{       byte check,chr;

        if (segetc() !=FINISH) return(3);

        check = unitsl + unitsl + CRSLD + FINISH;

        chr = segetc();
        if (chr !=check) return(4);

        ackflg = 0;

	tslite = 0;
	if (tsmode==0) return(3);

        strins(svmask,masks[maskp],4);
	strins(semask,"CSE1",4);
	return(3);
}

/****************************************************************/
/*                                                              */
/*      RFORCE - force terminal re-load                         */
/*                                                             */
/****************************************************************/

RFORCE()

{       byte check,chr;

        if (segetc() !=FINISH) return(3);

        check = unitsl + unitsl + FORCE + FINISH;

        chr = segetc();
        if (chr !=check) return(3);

	seemsg(10);

	rcvi();

	segpol();

	pgmsg[0]=pollcd;
	pgmsg[7]=unitsl;
	pgmsg[10]=unitsl + 26;
      
	seout(pgmsg,12);

	return(3);

}

/****************************************************************/
/*								*/
/*	PGLOAD - Loads the first page of the HTVT terminal	*/
/*		 Program to extract Terminal status and 	*/
/*		 option words etc.				*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

rpgload()
{

	segetc();	/* ignore slot number */

	if (segetc()!=0)   return(3);
	if (segetc()!=255) return(3);
	if (segetc()!=0)   return(3);	
	if (segetc()!=0)   return(3);	

	segetc();	/* ignore program ID number */

	segetc();	/* lose version number */

	itstat = segetc() * 256 + segetc();
	itopt1 = segetc() * 256 + segetc();

	dataout(1);	/* Save the new status word & option word */

	return(2);
}
	
	
/****************************************************************/
/*                                                              */
/*      RACK - Process terminal acknowlegdement request.        */
/*                                                              */
/****************************************************************/

rack()

{       byte check,chr;
	int	x;

        if (segetc() !=FINISH) return(3);

        check = unitsl + unitsl + ACK + FINISH;

        chr = segetc();
        if (chr !=check) return(3);

        ackflg = 0;
        upttn = YES;
        testss();

        senack(xtctr);

        xtctr=0;

	x=send();
        return(x);
}

/****************************************************************/
/*                                                              */
/*      RRETRN - Retransmit request received.                   */
/*                                                              */
/****************************************************************/

rretrn()

{       byte check,chr;

        if (segetc() !=FINISH) return(3);

        check = unitsl + unitsl + RETRAN + FINISH;

        chr = segetc();
        if (chr !=check) return(3);

        ackflg = 0;

        if (xectr < MXRTRN) {
                        xectr += 1;
                        upttn = NO;
                        send();
                        return(4);
                        }

        if (tixflg)  {
                        reack();
                        return(4);
                        }
                else seemsg(9);

        return(3);

}

/****************************************************************/
/*                                                              */
/*      RTRANS -                                                */
/*                                                              */
/****************************************************************/
rtrans()

{

        if (xectr < MXRTRN) {
                        xectr += 1;
                        upttn = NO;
                        send();
                        return(4);
                        }

        if (tixflg)  {
                        reack();
                        return(4);
                        }
                else seemsg(9);

        return(3);

}

/****************************************************************/
/*                                                              */
/*      SEND - send request (ACK or RETRANSmit)                 */
/*                                                              */
/****************************************************************/

send()
{       int x;

        lampoff(LOACT);
        lampon (LOXR);

        x = segpol();

        if (upttn ==YES) lttn=x;

        lampon(LOACT);
        lampoff(LOXR);

        rtstmr = RTSTIM;

        seout(msgout,msglth);

        rtstmr = 0;
        bright;
        return(2);      /* jump to MSGB */
}

/****************************************************************/
/*      SESEND - adds controls, sets flags + lites, sends       */
/*               message, processes response.                   */
/*                                                              */
/*      Enter with arguement pointing to the message.           */
/*              message has character 2 set to the current      */
/*              length, and characters 9-n set to the text.     */
/*                                                              */
/*      The following is added :                                */
/*          CH 1      - Poll code.                              */
/*          CH 2        Length is increased appropriately.      */
/*          CH 3        Software version code (0x18)            */
/*          CH 4        Software Version number (vernum)        */
/*          CH 5-6      Terminal status word (itstat)           */
/*          CH 7-8      Terminal option word (itopt1)           */
/*          CH n+1/2    TS Mask mode control character and code */
/*          CH          Terminal statistics (not implmented yet)*/
/*	    CH m        Customer number code (0xB9)		*/
/*	    CH m+1      Customer Number				*/
/*          CH m+2      Printer status code  (0xBF)             */
/*          CH m+3      Printer status                          */
/*          CH m+4      Checksum (ch 2 thru m+3)                */
/*          CH m+5      finish code (0x0F)                      */
/****************************************************************/

void sesend(msg)
        byte msg[];
{
        byte check;
        int len,i,x;

        lampoff(LOACT);
        lampon(LOXRTP);

        msg[0] = pollcd;

        len = msg[1];

        msg[2] =VERCOD;

        msg[3] = vernum;

        msg[4] = (itstat & 0xff00) >>8;
        msg[5] = (itstat & 0x00ff);

        msg[6] = (itopt1 & 0xff00) >> 8;
        msg[7] = (itopt1 & 0x00ff);

        len += 8;       /* msg[len] is now equal to character after text */

	if (offflg) {
			msg[len++] = FNUMBR;
			msg[len++] = '0';	/* request sequence # 0 */
			}

        if ((tsmode) || (tslite)) {
                msg[len++] = TSMCOD;
                msg[len++] = (tslite)? tslite : tsmode;
                }

    /* Good place to add terminal statistics    !!!!!! */

	msg[len++] = CUSTID;
	len+=sprintf(&msg[len],"%u",custno);

        msg[len++] = PSCOD;
        msg[len] = prstat;

        msg[1]   = len-1;

        check = 0;
        ++len;

        for (i=1; i<len; i++) { check += msg[i]; };

        msg[len++] = check;
        msg[len++] = FINISH;

        msgout = msg;
        msglth = len;

        tfbseq = tixflg = xectr = xactr = xtctr = stopt = ltixi = 0;
        ttyrec = lfflag = upttn = mtxonly = ackflg = 0;

        zero(semask,4);
        zero(txsec1,SZMAX);
        zero(txsec2,SZMAX);
        zero(txsec3,SZMAX);
        zero(txsec4,SZMAX);

        lttn = segpol();

	if (escflg!=3) {
	        lampon(LOACT);
        	rtstmr = RTSTIM;

	        lampoff(LOXR);

        	seout(msgout,msglth);

	        x = getcur(0);
		ratsay(tvmaxl,tvmaxc,(STDFORE*16+STDBACK)&0x7f,0,"S");
		escflg=0;     
		crtrtc(x);

	        rtstmr = 0;

	        segmsg();

	        lampoff(LOAXTP);

	        tmflg = 0;
	}
	escflg=0;
}

/****************************************************************/
/*                                                              */
/*      RTXOLD - Receives old ticket section message.           */
/*                                                              */
/****************************************************************/

rtxold()

{
        byte *point,check,chr;
        int  len,i;

        switch (tixflg) {
                case 2:
                        point = txsec2;
                        break;
                case 3:
                        point = txsec3;
                        break;
                case 4:
                        point = txsec4;
                        break;
                default:
                        point = txsec1;
                        tixflg = 1;
                        break;
                }

        tba = point;
        check = TIXOLD;
        (*point++) = check;

        if ((len = segetc()) > MXTSEC) return(3);  /* too big ==> MSGC */
        check += len;
        (*point++) = len;

        for (i=0;i<len;i++) {
                        chr = segetc();
                        check +=chr;
                        (*point++)=chr;
                        }

        if (segetc() != check ) return (3);     /* Test checksum */

        if (segetc() != FINISH) return (3);

        ltixi  = 0;
        ackflg = 0;

        point = tba + 2;

        if(*point != ENDTXT) {
                        if (tixflg != 4) {      /* if this is neither the "LAST"*/
                                tixflg++;       /* txsec, nor the fourth, every-*/
                                rcinc = 1;      /* thing is OK so, far.         */
                                return(3);
                                }
                        else {                  /* if this is the fourth txsec  */
                                tixflg = 1;     /* but not the last, there is a */
                                return (3);     /* problem, so start over.      */
                                }
                        }

        if (tixflg !=4) {                       /* now, if this is the last txsec*/
                        tixflg = 1;             /* (*point==ENDTXT), but not the*/
                        return(3);              /* fourth, this is a problem, so*/
                        }                       /* start over.                  */

/***** if we have reached this point, we have received 4 txsec's (tixflg==4) ****/
/***** the fourth of which was the last (*point == ENDTXT) !!! ******************/
/********            EVERYTHING'S COOL !!!!                    ******************/

        point++;
        i = sequp(tfbseq);

        if (i != *point++) return (3);

        if (*point++ != 3) {
                                tixflg = 1;
                                return (3);
                                }


        if ( (numtx=*point)  > 250 )       {
                                tixflg = 1;
                                return (3);
                                }

        if (!seform())  {			/* Format ticket   	*/
                                tixflg = 1;	/* Abort if there was   */
                                return (3);	/* an error.		*/
                                }

        tfbseq = i;

        tixflg = -1;
        xactr = 0;

        septb();				/* Print the tickets	*/
        ackaud();				/* Prepare acknow.	*/
        i = send();				/* Send ACK		*/
        return(i);
}

/****************************************************************/
/*                                                              */
/*      ACKAUD - print audit trail and ready ACK.               */
/*                                                              */
/****************************************************************/

void ackaud()

{       int i;

        if (auditl) {
                for (i=0; i<auditl; i++)  {ttyt(audit[i]);}
                }

        upttn = YES;

        sndack();
}

/****************************************************************/
/*                                                              */
/*      SNDACK - Ready ack to send.                             */
/*                                                              */
/*     (originially SNDACK & SENACK overlapped.  They no        */
/*       longer to)                                             */
/****************************************************************/

void sndack()

{
        byte chr;

        testss();

        chr = tfbseq;

        if ((tslite) || (tsmode)) {
                        chr |= 0x20;
                        if (stopt) chr |=0x80;
                        }
        ackrdy(chr);

        msgout = ack.buf;
        msglth = ACKCL;

        ackflg = 5;
}
/****************************************************************/
/*                                                              */
/*      SENACK - Non-ticket entry point for ACK.                */
/*                                                              */
/****************************************************************/


void senack(chr)
        byte chr;
{

        if (tsmode) {
                        chr |= 0x40;
                        if (stopt) chr |=0x80;
                        }
        ackrdy(chr);

        msgout = ack.buf;
        msglth = ACKCL;

        ackflg = 5;
}

/****************************************************************/
/*                                                              */
/*      REACK - Reacknowledgement.                              */
/*                                                              */
/****************************************************************/

reack()

{       int i;

        upttn = NO;

        if (++xactr == MXRACK) {	/* if we exceeded the max */
                        seemsg(7);	/* time to do this, abort */
                        seemsg(8+beep);
                        return(5);
                        }

        sndack();

        i = send();
        return (i);
}

/****************************************************************/
/*                                                              */
/*      ACKRDY - set up ack for sending including checksum.     */
/*      Arguement equals ACK sequence character.                */
/*                                                              */
/****************************************************************/

void ackrdy(chr)
        byte chr;

{       byte check;
        int  i;

        abseq = chr;

        ack.buf[0] = pollcd;
        ablth = ACKL;
        ab3 = VERCOD;
        abver = vernum;
        ab56 = LOHI(itstat);
        ab78 = LOHI(itopt1);
        abcod = ACKID;
        abstat = STATID;
        ab12 = 0;
        ab1314 = 0;
        ab1516 = 0;

        check = 0;

        for (i=1; i<=ACKL; i++) {check +=ack.buf[i];}

        abcksm = check;
        abfin = FINISH;
}

/****************************************************************/
/*                                                              */
/*      RTXNEW - Receives new ticket segments                   */
/*                                                              */
/****************************************************************/

rtxnew()


{
        byte *point,check,chr;
        int  len,i;

        switch (tixflg) {
                case 2:
                        point = txsec2;
                        break;
                case 3:
                        point = txsec3;
                        break;
                case 4:
                        point = txsec4;
                        break;
                default:
                        point = txsec1;
                        tixflg = 1;
                        break;
                }

        tba = point;
        *point++ = TIXNEW;
        check = unitsl + unitsl + TIXNEW;

        if ((len = segetc()) > NMXTSC) return(3);  /* too big ==> MSGC */
        check += len;
        *point++ = len;

        chr = segetc();
        *point++ = chr;
        check += chr;

        if ((chr & tixflg) != tixflg) {
                                tixflg = 1;
                                return (3);
                                }

        chr = segetc();         /* get and store spare byte */
        *point++ = chr;
        check += chr;

        for (i=0; i<len; i++) {
                        chr = segetc();
                        *point++ = chr;
                        check += chr;
                        }

        if (segetc() != check) {
                                tixflg = 1;
                                return(3);
                                }

        if (segetc() != FINISH) {
                                tixflg = 1;
                                return(3);
                                }

        ackflg = 0;
        point = tba + 2;
        if ((*point & 0x80)==0) {
                                tixflg ++;
                                return(3);
                                }
        chr = sequp(tfbseq)+0x20;

        if (chr != txsec1[9]) {
                                tixflg =1;
                                return(3);
                                }

        tixflg = 50;

        i = sefmtb();			/* Format tickets & print */

        if (!i)  {
                        tixflg =1;
                        return(3);
                        }

        i = send();
        return(i);
}
