/*									*/
/*	S E F D E F 	header						*/
/*									*/
/* Various defines							*/
/* 									*/
/*	(also in sefdef.c)						*/
/*									*/

	extern	byte	masks[][5];
	extern	byte 	fields[][6];
	extern  byte 	*prompts[];
	extern  byte 	*xlat[];	
/*									*/
 	extern	struct xlat_tbl { 	
			byte *key;
			int ident;
			byte xlation;
		      	} xdate[];
/*									*/


/*   Defines for first line of mask defination */
/*						*/
#define MASKNAME(x) &masks[x][0]		/* points to address of mask name (4 characters) */
#define SMALLRES(x) (masks[x][4] & 128)		/* TRUE of response screen should be small */
#define MONYTRAN(x) (masks[x][4] &  64)		/* TRUE of this is a money transaction */
#define SMALLMSK(x) (masks[x][4] &  32)		/* TRUE if mask screen should be small */
#define DEFLTMSK(x) (masks[x][4] &  16)		/* TRUE if this is the default mask for this mode */
#define MSKMODE(x)  (masks[x][4] &  15)		/* Mode Bits				*/

/*						*/
/*	DEfines for second line.		*/
/*						*/

#define MGTREQ(x)   (masks[x+1][0] & 1)		/* TRUE if Management key Required */
#define CSSEQ(x)     masks[x+1][1]		/* Credit Sales sequence number    */
#define MSKCODE(x)   masks[x+1][2]		/* ID Number for masks		   */


/*						*/
/*	Defines for field lines.		*/
/*						*/

#define FLDCODE(x)   masks[x][0]		/* Returns field code. */
#define INVINPUT(x) (masks[x][1] & 4)		/* TRUE if data input should not be displayed */
#define REVFLD(x)   (masks[x][1] & 2)		/* TRUE if data should be displays in reverse video */
#define DONTDISP(x) (masks[x][1] & 1)		/* TRUE if field should not be displayed till accessed */
#define	DISPROW(x) masks[x][2]-1		/* returns row to display field on */
#define DISPCOL(x) masks[x][3]-1		/* returns column to display field at. */



#define FIELDCD(x)  fields[x][0]		/* Return field code */
#define GRAPHOK(x) (fields[x][1] & 0x40)	/* TRUE if graphic character ok as input */
#define ALPHOK(x)  (fields[x][1] & 0x20)	/* TRUE if Alphabetic char ok as input */
#define NUMEROK(x) (fields[x][1] & 0x10) 	/* TRUE if Numeric character ok as input */
#define FIELDMD(x) (fields[x][1] & 0x0f)	/* returns mode bits */
#define NUMCHAR(x)  fields[x][2]		/* Returns Number of characters in display */
#define NUMTRAN(x)  fields[x][3]		/* Returns number of Char. to Translate   */
						/*  (from Left, 0 if no translation) 	*/
#define PROMPT(x)   prompts[fields[x][4]]	/* Prompt Label */
#define TRANTABL(x) xlat[fields[x][5]]		/* Translation Table */


#define	FPHONE	0x3
#define	FDATA	0x5
#define	FNAME	0x6
#define	FSYSTM	0x7

#define	FSECT	0xE
#define	FROW	0xF
#define	FCAT	0x10
#define	FSITNO	0x10
#define	FSEAT	0x11

#define	FNEXT	0x15
#define	FALTOK	0x16

#define	FMOP	0x19
#define	FDATE2	0x1A

#define	FTAXES	0x1D

#define	FEVENT	0x21
#define	FCAMP	0x21
#define	FDATE	0x22
#define	FLOCTN	0x23
#define	FEQPMT	0x23
#define	FTIX	0x24
#define	FMODIF	0x25
#define	FSPEC	0x26
#define	FLDATE	0x27
#define	FPERF	0x28
#define	FINQ	0x29


#define	FTYPE2	0x3A

#define	FTITLE	0x3C
#define	FPW	0x3D
#define	FPW2	0x3E
#define	FOPID	0x3F
#define	FAMTS	0x40


#define	FPRIC2	0x5B
#define	FAN	0x5C
#define	FPF	0x5D
#define	FPM	0x5E
#define	FDOP	0x5F

#define	FTOTAL	0x68
#define	FCOMM	0x69
#define	FCHECK	0x6A
#define	FCASH	0x6B
#define	FACCT	0x6C

#define	FTHTRE	0x6E
#define	FTYPE	0x6F

#define	FAD	0x71
#define	FCODE	0x72
#define	FCITY	0x73
#define	FNITES	0x73
#define	FNUMBR	0x74
#define	FNSITS	0x74
#define	FLNPL 	0x75
#define	FSTAT 	0x76

#define	FZONE	0x78
#define	FCSTYP	0x78
#define	FMODE	0x79
#define	FOLD	0x7A
#define	FVAR1	0x7A
#define	FNEW	0x7B
#define	FPEOPL	0x7B
#define	FPRICE	0x7C
#define	FADJ	0x7D
#define	FREASN	0x7E

#define	FCARD	0x81
#define	FEXP	0x82
#define	FTSOPT	0x83
#define	FTSNAM	0x84
#define	FCONF	0x85
#define	FADDR1	0x86
#define	FADDR2	0x87
#define	FADDR3	0x88
#define	FADDR4	0x89
#define	FZIP	0x8A
#define	FPHON1	0x8B
#define	FPHON2	0x8C
#define	FMEMO	0x8D
#define	FTSMOD	0x8E

#define	FAMNT	0x94
#define	FDESCR	0x95
	
#define	FCNAME	0x97
#define	FTTN	0x98
		
#define	FCRADJ	0x9B
#define	FCRCHG	0x9C
#define	FCRAUT	0x9D
	
#define	FTSCOD	0x9F

#define	FREQ	0xA1

#define	FTSSNM	0xA4		    

#define	FOPTBI	0xB0
#define	FBR	0xB1
#define	FSTATS	0xB2

#define	FCDVAL	0xB5
#define	FTSZIP	0xB6

#define CUSTNO  0xB9
#define FREPRT  0xBA

#define FMERCH  0xBD
#define FCRDTY  0XBE		/***/
#define PRSTAT	0xBF

#define FIN     0XFF



