/****************************************************************/
/*								*/
/*   	S E  K B I N - Handles input and translation   		*/
/*		into PCTerminal codes				*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
#include "se.h"
#include "sekdef.h"
#include "sefdef.h"

		byte	__KBIN=0;
		byte tran[256] = 

/*  0 -  9 */	{0,0,0,0,0,0,0,0,KBSPCDEL,0,

/* 10 - 19 */	 0,0,0,KNFLD,0,0,0,0,0,0,

/* 20 - 29 */	 0,0,0,0,0,0,0,ESC,0,0,

/* 30 - 39 */	 0,0,' ',0,0,0,0x60,0,0,0,

/* 40 - 49 */ 	 0,0, '*', '+', ',', '-', '.', '/', '0', '1',

/* 50 - 59 */	'2','3','4','5','6','7','8','9',':',';',

/* 60 - 69 */	 0,0,0,0,0,'A','B','C','D','E',

/* 70 - 79 */ 	'F','G','H','I','J','K','L','M','N','O',

/* 80 - 89 */	'P','Q','R','S','T','U','V','W','X','Y',

/* 90 - 99 */	'Z', 0,  0,  0,  0,  0,  0, 'A','B','C',

/*100 -109 */	'D','E','F','G','H','I','J','K','L','M',

/*110 -119 */	'N','O','P','Q','R','S','T','U','V','W',

/*120 -127 */	'X','Y','Z', 0,  0,  0,  0,  0,

 /* 80-87 */	KXMIT,KINQ,KF3,KBUY,FNEXT,KBUYAL,KPRLST,FALTOK,FSPEC,
 /* 88-8F */	KCLEAR,KCLRMSK,KBSPC,KSTOP,KBFLD,KNFLD,0,
 /* 90-97 */ 	0,0,0,0,0,0,0,0,
 /* 98-9F */	0,0,KBMSK,KFSPC,KNMSK,KPRSET,KCLRFD,0,
 /* A0-A7 */	0,0,0,KHELPKEY,KHELPMSK,KHELPFLD,KPRMENU,KSPMENU,
 /* A8-AF */    KSCROL,KEXIT,0,0,0,0,0,0};

byte kbin()
{	byte 	ch1,ch;

	if (kbhit() ==0) return(FALSE);

	ch = getkey();

	ch1 = (ch < 0xAF) ? tran[ch] : ch;

	return(ch1);
}
