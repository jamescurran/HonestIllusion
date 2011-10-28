/*            								*/
/*	S E   F D E F 	Definations					*/
/*									*/
/* Various defines, Mask & field tables					*/
/* 									*/
/*	(Defines also in sefdef.h)     					*/
/*									*/

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
#define FGPTH   0XBB
#define FGROUP  0XBC
#define FMERCH  0xBD
#define FCRDTY  0XBE
#define PRSTAT	0xBF
#define FIN     0XFF

typedef unsigned char  byte;

/*   Defines for first line of mask defination  */
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
#define CSSEQ(x)     masks[x+1][1]		/* Mask Credit Sequence Number     */
#define MSKCODE(x)   masks[x+1][2]		/* ID munber for masks		   */

 
/*						*/
/*	Defines for field lines.		*/
/*						*/

#define FLDCODE(x)   masks[x][0]		/* Returns field code. */
#define INVINPUT(x) (masks[x][1] & 4)		/* TRUE if data input should not be displayed */
#define REVFLD(x)   (masks[x][1] & 2)		/* TRUE if data should be displays in reverse video */
#define NODISP(x)   (masks[x][1] & 1)		/* TRUE if field should not be displayed till accessed */
#define	DISPROW(x) masks[x][2]-1		/* returns row to display field on */
#define DISPCOL(x) masks[x][3]-1		/* returns column to display field at. */

	byte	__SEFDEF=0;

 	byte masks[][5] = {
			{'S', 'E', 0, 0, 0x51},	    
			{0, 0, 1, 0, 0},
			{FEVENT, 0, 1,  1, 0},
			{FDATE,  0, 1, 14, 0},
			{FPERF,  0, 1, 26, 0},
			{FNAME,  1, 2,  1, 0},
			{FSPEC,  1, 2, 24, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FMODIF, 1, 3, 13, 0},
			{FSECT,  1, 4,  1, 0},
			{FROW,   1, 4, 10, 0},
			{FSEAT,  1, 4, 20, 0},
			{FTIX,   0, 5,  1, 0},
			{FINQ,   1, 5, 29, 0},
			{FMOP,   0, 6,  1, 0},
			{FDATE2, 1, 6,  9, 0},
			{FALTOK, 3, 6, 21, 0},
			{FCARD,  1, 7,  1, 0},
			{FEXP,   1, 7, 25, 0},
			{FCDVAL, 1, 6, 29, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', 'E', 'T', 0X1},
			{0, 0, 2, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FCODE,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'F', 'R', 'E', 'T', 0X1},
			{0, 0, 3, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},			
			{FPERF,  0, 2, 26, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FTIX,   0, 4,  1, 0},
			{FSECT,  0, 5,  1, 0},
			{FROW,   0, 5, 10, 0},
			{FSEAT,  0, 5, 20, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'N', 'O', 'F', 'S', 0X1},
			{0, 0, 4, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FNUMBR, 0, 3, 14, 0},
			{FMODE,  0, 3, 26, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'P', 'C', 'H', 'G', 0X1},
			{0, 0, 5, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FPRICE, 0, 3, 14, 0},
			{FAMTS,  0, 4,  1, 0},
			{FTAXES, 0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'P', 'R', 'I', 'C', 0X1},
			{0, 0, 6, 0, 0},
			{FEVENT, 0, 3,  1, 0},
			{FDATE,  0, 3, 14, 0},
			{FPERF,  0, 3, 26, 0},
			{FLOCTN, 0, 4,  1, 0},
			{FPRICE, 0, 5,  1, 0},
			{FMODE,  0, 5, 16, 0},
			{FTYPE,  0, 5, 26, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'E', 'S', 'V', 0X41},
			{0, 0, 7, 0, 0},
			{FEVENT, 0, 1,  1, 0},
			{FDATE,  0, 1, 13, 0},
			{FPERF,  0, 1, 24, 0},
	 		{FDATE2, 1, 1, 31, 0},
			{FNAME,  1, 2,  1, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FMODIF, 1, 3, 13, 0},
			{FSECT,  0, 4,  1, 0},
			{FROW,   0, 4,  9, 0},
			{FSEAT,  0, 4, 18, 0},
			{FTIX,   0, 5,  1, 0},
			{FINQ,   1, 5, 36, 0},
			{FMOP,   0, 5, 28, 0},
			{FCARD,  1, 6,  1, 0},
			{FEXP,   1, 6, 26, 0},
			{FSPEC,  0, 2, 32, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '0', 0, 0, 0X1},
			{1, 0, 8, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FSTAT,  0, 3,  1, 0},
			{FNUMBR, 0, 3, 10, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},
		
			{'R', '1', '5', 0, 0X81},
			{1, 0, 9, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTYPE,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '1', '6', 0, 0x81},
			{0, 0, 84, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTYPE,  0, 3,  1, 0},
			{FSTAT,  0, 3, 14, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '1', '7', 0, 0x81},
			{0, 0, 85, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FSTAT,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '1', '9', 0, 0X81},
			{1, 0, 10, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '2', 0, 0, 0X81},
			{1, 0, 11, 0, 0},
			{FEVENT, 0, 3,  1, 0},
			{FTYPE,  0, 3, 15, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

 			{'R', '2', '2', 0, 0X81},
			{1, 0, 12, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FNUMBR, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},
			
			{'R', '3', 0, 0,   0x81},
			{1, 0, 74, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FNUMBR, 0, 3,  1, 0},
			{FTYPE,  0, 3, 14, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '3', '2', 0, 0X81},
			{1, 0, 13, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FNUMBR, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '3', '7', 0, 0X81},
			{1, 0, 14, 0, 0},
			{FEVENT, 0, 3,  1, 0},
			{FTYPE,  0, 3, 13, 0},
			{FNUMBR, 0, 3, 23, 0},
			{FSTAT,  0, 4,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '3', '9', 0, 0X81},
			{1, 0, 15, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FTYPE,  0, 3, 14, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '5', 0, 0, 0X81},
			{1, 0, 16, 0, 0},
			{FEVENT, 0, 3,  1, 0},
			{FDATE,  0, 3, 14, 0},
			{FPERF,  0, 3, 26, 0},
			{FTYPE,  0, 4,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '6', 0, 0, 0X81},
			{1, 0, 17, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FZONE,  0, 3, 14, 0},
			{FTYPE2, 0, 3, 26, 0},
			{FPRICE, 0, 4,  1, 0},
			{FSTAT,  0, 4, 14, 0},
			{FSPEC,  0, 1,  1, 0},			
			{FIN,    0, 0,  0, 0},

			{'R', '7', 0, 0, 0X81},
			{1, 0, 18, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FNUMBR, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '8', 0, 0, 0X81},
			{1, 0, 19, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTYPE,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '9', 0, 0, 0X81},
			{1, 0, 20, 0, 0},
			{FEVENT, 0, 3,  1, 0},
			{FDATE,  0, 3, 14, 0},
			{FPERF,  0, 3, 26, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '9', '7', 0, 0X81},
			{1, 0, 21, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTYPE,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '9', '9', 0, 0X85},
			{0, 0, 22, 0, 0},
			{FCITY,  0, 3,  1, 0},
			{FSYSTM, 0, 3, 10, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'E', 'T', 'N', 0X41},
			{0, 0, 23, 0, 0},
			{FEVENT, 0, 1, 11, 0},
			{FDATE,  0, 1, 23, 0},
			{FPERF,  0, 1, 34, 0},
			{FLOCTN, 0, 2,  1, 0},
			{FTIX,   0, 2, 13, 0},
			{FCODE,  0, 3,  1, 0},
			{FADJ,   0, 3, 17, 0},
			{FREASN, 0, 3, 24, 0},
			{FLNPL,  0, 4,  1, 0},
			{FSYSTM, 0, 4, 10, 0},
			{FDOP,   0, 4, 20, 0},
			{FPRIC2, 0, 5,  1, 0},
			{FINQ,   1, 5, 17, 0},
			{FMOP,   0, 5, 27, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'E', 'T', 'C', 0XE5},
			{0, 0, 75, 0, 0},
			{FEVENT, 0, 1, 11, 0},
			{FDATE,  0, 1, 23, 0},
			{FPERF,  0, 1, 34, 0},
			{FLOCTN, 0, 2,  1, 0},
			{FTIX,   0, 2, 13, 0},
			{FCODE,  0, 3,  1, 0},
			{FADJ,   0, 3, 17, 0},
			{FREASN, 0, 3, 24, 0},
			{FLNPL,  0, 4,  1, 0},
			{FSYSTM, 0, 4, 10, 0},
			{FDOP,   0, 4, 20, 0},
			{FPRIC2, 0, 5,  1, 0},
			{FINQ,   1, 5, 17, 0},
			{FMOP,   0, 5, 27, 0},
			{FNAME,  0, 6,  1, 0},
			{FCARD,  0, 6, 25, 0},
			{FEXP,   0, 6, 51, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'L', 'S', 'E', 0X1},
			{0, 0, 24, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FZONE,  0, 3, 13, 0},
			{FOLD,   0, 3, 21, 0},
			{FNEW,   0, 3, 27, 0},
			{FTIX,   0, 4,  1, 0},
			{FSECT,  0, 5,  1, 0},
			{FROW,   0, 5, 10, 0},
			{FSEAT,  0, 5, 20, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'S', 'C', 'T', 'A', 0X1},
			{0, 0, 25, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTYPE2, 0, 3,  1, 0},
			{FNUMBR, 0, 3, 14, 0},
			{FAD,    0, 4,  1, 0},
			{FCAT,   0, 4, 14, 0},
			{FTITLE, 0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'S', 'C', 'T', 'C', 0X1},
			{0, 0, 26, 0, 0},
			{FEVENT, 0, 1, 11, 0},
			{FDATE,  0, 1, 23, 0},
			{FPERF,  0, 1, 34, 0},
			{FTYPE2, 0, 2,  1, 0},
			{FNUMBR, 0, 2,  9, 0},
			{FAD,    0, 2, 21, 0},
			{FCAT,   0, 2, 29, 0},
			{FAN,    0, 3,  1, 0},
			{FTITLE, 0, 4,  1, 0},
			{FPF,    0, 5,  1, 0},
			{FPM,    0, 5, 20, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'S', 'C', 'T', 'D', 0X1},
			{0, 0, 27, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTYPE2, 0, 3,  1, 0},
			{FTITLE, 0, 4,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'S', 'E', 'R', 0, 0X41},
			{0, 0, 28, 0, 0},
			{FEVENT, 0, 1,  1, 0},
			{FDATE,  0, 1, 13, 0},
			{FPERF,  0, 1, 24, 0},
			{FDATE2, 1, 1, 31, 0},
			{FNAME,  1, 2,  1, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FMODIF, 1, 3, 13, 0},
			{FSECT,  0, 4,  1, 0},
			{FROW,   0, 4,  9, 0},
			{FSEAT,  0, 4, 18, 0},
			{FTIX,   0, 5,  1, 0},
			{FMOP,   0, 5, 28, 0},
			{FCARD,  1, 6,  1, 0},
			{FEXP,   1, 6, 25, 0},
			{FSPEC,  0, 2, 32, 0},
			{FIN,    0, 0,  0, 0},

/*	MASKS FOR CAMP	*/

			{'C', 'A', 'M', 'P', 0XD2},
			{0, 0, 29, 0, 0},
			{FCAMP,  0, 1,  1, 0},
			{FDATE,  0, 1, 12, 0},
			{FPRICE, 0, 1, 23, 0},
			{FEQPMT, 0, 2,  1, 0},
			{FVAR1,  0, 2, 11, 0},
			{FPEOPL, 0, 2, 19, 0},
			{FNITES, 0, 3,  1, 0},
			{FNAME,  0, 3, 11, 0},
			{FPHONE, 0, 4,  1, 0},
			{FINQ,   1, 4, 29, 0},
			{FMOP,   0, 5,  1, 0},
			{FSPEC,  1, 5, 24, 0},
			{FCARD,  1, 6,  1, 0},
			{FEXP,   1, 6, 26, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'T', 'R', 'L', 0X2},
			{0, 0, 30, 0, 0},
			{FCAMP,  0, 2,  1, 0},
			{FDATE,  0, 2, 12, 0},
			{FSTAT,  0, 2, 23, 0},
			{FCSTYP, 0, 3,  1, 0},
			{FLDATE, 0, 3, 18, 0},
			{FSITNO, 0, 4,  1, 0},
			{FNSITS, 0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '1', '6', 0, 0X82},
			{1, 0, 31, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '2', 0, 0, 0X82},
			{1, 0, 32, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FDATE,  0, 3, 15, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '3', '9', 0, 0X82},
			{1, 0, 33, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '5', 0, 0, 0X82},
			{1, 0, 34, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FTYPE,  0, 3, 14, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '6', 0, 0, 0X82},
			{1, 0, 35, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '7', 0, 0, 0X82},
			{1, 0, 36, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FDATE,  0, 3, 15, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '8', 0, 0, 0X82},
			{1, 0, 37, 0, 0},
			{FCAMP,  0, 2,  1, 0},
			{FDATE,  0, 2, 13, 0},
			{FTYPE,  0, 3,  1, 0},
			{FNITES, 0, 3, 13, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '9', 0, 0, 0X82},
			{1, 0, 38, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FDATE,  0, 3, 15, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '1', '2', 0, 0X82},
			{1, 0, 82, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FDATE,  0, 3, 15, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'L', 'S', 'E', 0X2},
			{0, 0, 39, 0, 0},
			{FCAMP,  0, 3,  1, 0},
			{FSTAT,  0, 3, 15, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'E', 'T', 'N', 0X42},
			{0, 0, 40, 0, 0},
			{FCAMP,  0, 2,  1, 0},
			{FDATE,  0, 2, 12, 0},
			{FPRICE, 0, 2, 23, 0},
			{FCSTYP, 0, 3,  1, 0},
			{FNITES, 0, 3, 18, 0},
			{FCODE,  0, 4,  1, 0},
			{FADJ,   0, 4, 18, 0},
			{FREASN, 0, 5,  1, 0},
			{FLNPL,  0, 5, 10, 0},
			{FSYSTM, 0, 5, 19, 0},
			{FINQ,   1, 5, 29, 0},
			{FMOP,   0, 6,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'E', 'T', 'C', 0XE2},
			{0, 0, 77, 0, 0},
			{FCAMP,  0, 2,  1, 0},
			{FDATE,  0, 2, 12, 0},
			{FPRICE, 0, 2, 23, 0},
			{FCSTYP, 0, 3,  1, 0},
			{FNITES, 0, 3, 18, 0},
			{FCODE,  0, 4,  1, 0},
			{FADJ,   0, 4, 18, 0},
			{FREASN, 0, 5,  1, 0},
			{FLNPL,  0, 5, 10, 0},
			{FSYSTM, 0, 5, 19, 0},
			{FINQ,   1, 5, 29, 0},
			{FMOP,   0, 5, 35, 0},
			{FCARD,  0, 6,  1, 0},
			{FEXP,   0, 6, 26, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

/*	MASKS FOR CREDIT SEç
MODE	*/																										
			
			{'C', 'S', 'E', 0, 0XF4},
			{0, '0', 41, 0, 0},
			{FEVENT, 0, 1,  1, 0},
			{FDATE,  0, 1, 14, 0},
			{FPERF,  0, 1, 26, 0},
			{FLOCTN, 0, 2,  1, 0},
			{FSPEC,  1, 2, 24, 0},
			{FTIX,   0, 3,  1, 0},
			{FINQ,   1, 3, 29, 0},
			{FMOP,   0, 4,  1, 0},
			{FALTOK, 3, 4, 31, 0},
			{FNAME,  0, 5,  1, 0},
			{FCARD,  0, 6,  1, 0},
			{FEXP,   0, 6, 25, 0},
			{FTSMOD, 0, 7,  1, 0},
			{FCDVAL, 1, 5, 29, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'S', 'E', '1', 0XEC},
			{0, '2', 42, 0, 0},
			{FTSNAM, 0,13,  1, 0},
			{FADDR1, 0,14,  1, 0},
			{FADDR2, 0,15,  7, 0},
			{FADDR3, 0,16,  7, 0},
			{FADDR4, 0,17,  7, 0},
			{FZIP,   0,17, 25, 0},
			{FPHON1, 0,18,  1, 0},
			{FPHON2, 0,19,  1, 0},
			{FCNAME, 0,20,  1, 0},
			{FMEMO,  0,21,  1, 0},
/*
			{FMOD2,  0,22,  1, 0},
*/
			{FCONF,  0,22,  8, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'C', 'M', 'P', 0XF8},
			{0, '0', 43, 0, 0},
			{FCAMP,  0, 2,  1, 0},
			{FDATE,  0, 2, 13, 0},
			{FPRICE, 0, 2, 24, 0},
			{FEQPMT, 0, 3,  1, 0},
			{FVAR1,  0, 3, 10, 0},
			{FPEOPL, 0, 3, 18, 0},
			{FNITES, 0, 4,  1, 0},
			{FNAME,  0, 5,  1, 0},
			{FPHONE, 0, 5, 23, 0},
			{FINQ,   1, 3, 34, 0},
			{FMOP,   0, 4, 27, 0},
			{FCARD,  0, 6,  1, 0},
			{FEXP,   0, 6, 27, 0},
			{FTSMOD, 0, 7,  1, 0},
			{FSPEC,  1, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

/*	MASKS FOR BOTH SE & CAMP	*/

			{'R', '1', 0, 0, 0X03},
			{0, 0, 44, 0, 0},
			{FDATE,  0, 1, 12, 0},
			{FNUMBR, 0, 1, 24, 0},
			{FLNPL,  0, 2,  1, 0},
			{FSYSTM, 0, 2, 11, 0},
			{FMODIF, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '4', 0, 0, 0X83},
			{1, 0, 45, 0, 0},
			{FTHTRE, 0, 3,  1, 0},
			{FOPID,  0, 3, 16, 0},
			{FSYSTM, 0, 3, 26, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '1', '1', 0, 0X83},
			{0, 0, 46, 0, 0}, 
			{FDATE,  0, 1, 12, 0},
			{FTYPE,  0, 1, 24, 0},
			{FLNPL,  0, 2,  1, 0},
			{FSYSTM, 0, 2, 11, 0},
			{FTHTRE, 0, 3,  1, 0},
			{FOPID,  0, 3, 13, 0},
			{FNUMBR, 0, 4,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '1', '4', 0, 0X83},
			{1, 0, 47, 0, 0},
			{FLNPL,  0, 3,  1, 0},
			{FSYSTM, 0, 3, 15, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '3', '3', 0, 0X83},
			{1, 0, 48, 0, 0},
			{FLNPL,  0, 2,  1, 0},
			{FTYPE,  0, 2, 10, 0},
			{FINQ,   1, 2, 29, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '3', '8', 0, 0X83},
			{1, 0, 49, 0, 0},
			{FEVENT, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '4', '9', 0, 0X8F},
			{1, 0, 50, 0, 0},
			{FNUMBR, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '7', '0',  0, 0X8F},
			{0, 0, 79, 0, 0},
			{FREPRT, 0, 2, 1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', '7', '1', 0,  0X8F},
			{0, 0, 80, 0, 0},
			{FREPRT, 0, 2, 1, 0},
  /*			{FTYPE,  0, 3, 1, 0},    */
			{FSPEC,  0, 1, 1, 0},
			{FIN,    0, 0, 0, 0},

			{'R', 'D', 'E', 'U', 0X8F},
			{0, 0, 86, 0, 0},
			{FEVENT, 0, 2, 1, 0},
			{FSPEC,  0, 1, 1, 0},
			{FIN,    0, 0, 0, 0},

			{'R', 'D', 'P', 'S', 0X8F},
			{0, 0, 87, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 13, 0},
			{FPERF,  0, 2, 25, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'D', 'T', 'S', 0X8F},
			{0, 0, 88, 0, 0},
			{FLNPL,  0, 2,  1, 0},
			{FDATE,  0, 2, 13, 0},
			{FGPTH,  0, 4,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'D', 'O', 'S', 0X8F},
			{0, 0, 89, 0, 0},
			{FGROUP, 0, 2,  1, 0},
			{FDATE,  0, 2, 13, 0},
			{FOPID,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},			

			{'C', 'A', 'S', 'H', 0X4F},
			{0, 0, 51, 0, 0},
			{FMOP,   0, 1, 12, 0},
			{FACCT,  0, 1, 20, 0},
			{FCASH,  0, 2,  1, 0},
			{FCHECK, 0, 3,  1, 0},
			{FCOMM,  0, 4,  1, 0},
			{FTOTAL, 0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'M', 'G', 'T', 0, 0X8F},
			{0, 0, 52, 0, 0},
			{FREQ,   0, 2,  1, 0},
			{FDATA,  0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'O', 'F', 'F', 0, 0XF},
			{0, 0, 53, 0, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'O', 'N', 0, 0, 0XF},
			{0, 0, 54, 0, 0},
			{FOPID,  0, 3,  1, 0},			
			{FPW,    4, 3, 11, 0},
			{FTYPE,  0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'O', 'P', 'I', 'D', 0XF},
			{1, 0, 55, 0, 0},
			{FOPID,  0, 2,  1, 0},
			{FTITLE, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'P', 'A', 'S', 'S', 0XF},
			{1, 0, 56, 0, 0},
			{FTYPE,  0, 2,  1, 0},
			{FLNPL,  0, 2, 14, 0},
			{FTHTRE, 0, 3,  1, 0},
			{FOPID,  0, 3, 14, 0},
			{FPW,    0, 4,  1, 0},
			{FPW2,   0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'P', 'R', 'E', 'V', 0XF},
			{0, 0, 57, 0, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'P', 'R', 'T', 0X4F},
			{0, 0, 58, 0, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'T', 'E', 'S', 'T', 0XF},
			{0, 0, 59, 0, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'V', 'O', 'I', 'D', 0X4F},
			{0, 0, 60, 0, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},
			
			{'C', 'R', 'T', 'N', 0XEF},
			{0, 0, 61, 0, 0},
			{FEVENT, 0, 1, 13, 0},
			{FDATE,  0, 1, 27, 0},
			{FPERF,  0, 1, 40, 0},
			{FCARD,  0, 2,  1, 0},
			{FTSNAM, 0, 2, 27, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FTHTRE, 0, 3, 15, 0},
			{FLNPL,  0, 3, 28, 0},
			{FTTN,   0, 3, 40, 0},
			{FINQ,   1, 3, 61, 0},
			{FBR,    0, 4,  1, 0},
			{FMOP,   0, 4, 15, 0},
			{FTSMOD, 0, 4, 28, 0},
			{FADJ,   0, 4, 40, 0},
			{FREASN, 0, 5,  1, 0},
			{FSTAT,  0, 5, 15, 0},
			{FDATE2, 1, 5, 23, 0},
			{FMEMO,  0, 5, 40, 0},
			{FAN,    0, 6,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'D', 'F', 'R', 0X8F},
			{0, '3', 62, 0, 0},
			{FTSOPT, 0, 2,  1, 0},
			{FTSCOD, 0, 2, 13, 0},
			{FTHTRE, 0, 3,  1, 0},
			{FLNPL,  0, 3, 13, 0},
			{FAN,    0, 4,  1, 0},
			{FMEMO,  0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FINQ,   1, 1, 13, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', '5', '1', 0X8F},
			{0, '3', 63, 0, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FDATE2, 1, 3,  1, 0},
			{FTHTRE, 0, 4,  1, 0},
			{FTSOPT, 0, 4, 14, 0},
			{FINQ,   1, 4, 29, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'P', 'T', 'I', 'X', 0XCF},
			{0, '3', 64, 0, 0},
			{FTSMOD, 0, 1, 12, 0},
			{FTSOPT, 0, 1, 19, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTSSNM, 0, 3,  1, 0},
			{FCARD,  0, 4,  1, 0},
			{FAN,    0, 4, 25, 0},
			{FLOCTN, 0, 5,  1, 0},
			{FMOP,   0, 5, 14, 0},
			{FTHTRE, 0, 5, 22, 0},
			{FDATE2, 1, 6,  1, 0},
			{FINQ,   1, 6, 29, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', '5', '2', 0X8F},
			{0, '3', 65, 0, 0},
			{FDATE,  0, 2,  1, 0},
			{FTHTRE, 0, 2, 13, 0},
			{FLNPL,  0, 3,  1, 0},
			{FTSOPT, 0, 3, 13, 0},
			{FDATE2, 1, 4,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', '5', '3', 0X83},
			{0, '3', 76, 0, 0 },
			{FDATE,  0, 1, 12, 0},
			{FPERF,	 0, 1, 26, 0},
			{FTSOPT, 0, 2,  1, 0},
			{FDATE2, 0, 2, 12, 0},
			{FLNPL,  0, 3,  1, 0},
			{FTHTRE, 0, 3, 12, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'D', 'S', 'P', 0XAF},
			{0, '3', 66, 0, 0},
			{FEVENT, 0, 1, 11, 0},
			{FDATE, 0, 1, 23, 0},
			{FPERF,  0, 2,  1, 0},
			{FCARD,  0, 2,  9, 0},
			{FAN,    0, 2, 34, 0},
			{FTSSNM, 0, 3,  1, 0},
			{FLOCTN, 0, 4,  1, 0},
			{FTHTRE, 0, 4, 14, 0},
			{FLNPL,  0, 4, 25, 0},
			{FTTN,   0, 4, 34, 0},
			{FMOP,   0, 5,  1, 0},
			{FDATE2, 1, 5,  8, 0},
			{FTSMOD, 0, 5, 20, 0},
			{FTSOPT, 0, 5, 27, 0},
			{FINQ,   1, 5, 36, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', '5', '4', 0XAF},
			{0, '3', 67, 0, 0},
			{FEVENT, 0, 1, 11, 0},
			{FDATE,  0, 1, 23, 0},
			{FPERF,  0, 2,  1, 0},
			{FCARD,  0, 2,  9, 0},
			{FAN,    0, 2, 34, 0},
			{FTSSNM, 0, 3,  1, 0},
			{FLOCTN, 0, 4,  1, 0},
			{FTHTRE, 0, 4, 14, 0},
			{FLNPL,  0, 4, 25, 0},
			{FTTN,   0, 4, 33, 0},
			{FMOP,   0, 5,  1, 0},
			{FDATE2, 1, 5,  8, 0},
			{FTSMOD, 0, 5, 20, 0},
			{FTSOPT, 0, 5, 27, 0},
			{FINQ,   1, 5, 36, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', '5', '5', 0XAF},
			{0, '3', 68, 0, 0},
			{FEVENT, 0, 1, 11, 0},
			{FDATE,  0, 1, 23, 0},
			{FPERF,  0, 2,  1, 0},
			{FCARD,  0, 2,  9, 0},
			{FAN,    0, 2, 34, 0},
			{FTSSNM, 0, 3,  1, 0},
			{FLOCTN, 0, 4,  1, 0},
			{FTHTRE, 0, 4, 14, 0},
			{FLNPL,  0, 4, 25, 0},
			{FTTN,   0, 4, 33, 0},
			{FMOP,   0, 5,  1, 0},
			{FDATE2, 1, 5,  8, 0},
			{FTSMOD, 0, 5, 20, 0},
			{FTSOPT, 0, 5, 27, 0},
			{FINQ,   1, 5, 36, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', '5', '6', 0XAF},
			{0, '3', 69, 0, 0},
			{FEVENT, 0, 1, 11, 0},
			{FDATE,  0, 1, 23, 0},
			{FPERF,  0, 2,  1, 0},
			{FCARD,  0, 2,  9, 0},
			{FTSSNM, 0, 3,  1, 0},
			{FLOCTN, 0, 4,  1, 0},
			{FTHTRE, 0, 4, 14, 0},
			{FLNPL,  0, 4, 25, 0},
			{FTTN,   0, 4, 33, 0},
			{FMOP,   0, 5,  1, 0},
			{FDATE2, 1, 5,  8, 0},
			{FTSMOD, 0, 5, 20, 0},
			{FTSOPT, 0, 5, 27, 0},
			{FINQ,   1, 5, 36, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'R', '5', '8',0XCF},
			{0,'3', 78, 0, 0},
			{FMERCH, 0, 3,  1, 0},
			{FDATE,  0, 4,  1, 0},
			{FDATE2, 0, 4, 12, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'T', 'R', 'P', 0XEF},
			{0, '3', 70, 0, 0},
			{FTSMOD, 0, 1, 14, 0},
			{FTSOPT, 0, 1, 23, 0},
			{FEVENT, 0, 2,  1, 0},
			{FDATE,  0, 2, 14, 0},
			{FPERF,  0, 2, 26, 0},
			{FTSSNM, 0, 3,  1, 0},
			{FCARD,  0, 3, 40, 0},
			{FLOCTN, 0, 4,  1, 0},
			{FMOP,   0, 4, 14, 0},
			{FTHTRE, 0, 4, 22, 0},
			{FTTN,   0, 4, 41, 0},
			{FDATE2, 1, 4, 54, 0},
			{FINQ,   1, 4, 69, 0},
			{FAN,    0, 5,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'M', 'O', 'D', 0XAF},
			{0, '3', 71, 0, 0},
			{FEVENT, 0, 1, 12, 0},
			{FDATE,  0, 1, 25, 0},
			{FPERF,  0, 1, 37, 0},
			{FCARD,  0, 2,  1, 0},
			{FTSNAM, 0, 2, 27, 0},
			{FLOCTN, 0, 3,  1, 0},
			{FTHTRE, 0, 3, 14, 0},
			{FLNPL,  0, 3, 26, 0},
			{FTTN,   0, 3, 36, 0},
			{FMOP,   0, 3, 48, 0},
			{FTSMOD, 0, 3, 56, 0},
			{FADDR1, 0, 4,  1, 0},
			{FADDR2, 0, 5,  6, 0},
			{FADDR3, 0, 6,  6, 0},
			{FADDR4, 0, 7,  6, 0},
			{FZIP,   0, 7, 25, 0},
			{FPHON1, 0, 8,  1, 0},
			{FPHON2, 0, 8, 27, 0},
			{FCNAME, 0, 9,  1, 0},
			{FMEMO,  0,10,  1, 0},
			{FDATE2, 1,10, 48, 0},
			{FCONF,  0,11,  1, 0},
			{FAN,    0,11, 27, 0},
			{FINQ,   1,11, 61, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'C', 'C', 'R', 'D', 0XAF}, 
			{1, 0, 83, 0, 0},
			{FCRDTY, 0, 3,  1, 0},
			{FSPEC,  0, 1,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'E', 'F', 'E', 0X43}, 
			{0, 0, 72, 0, 0},
			{FTSOPT, 0, 2,  1, 0},
			{FTSCOD, 0, 4,  1, 0},
			{FIN,    0, 0,  0, 0},

			{'R', 'E', 'F', 'E', 0XEC},
			{0, '1', 73, 0, 0},
			{FTSOPT, 0, 7,  8, 0},
			{FTSCOD, 0, 7, 19, 0},
			{FIN,    0, 0,  0, 0},
			{FIN,    0, 0,  0, 0}
				
			};

			/*  + R3  WHICH IS 74  */
			/*    RETC (se)    75  */
			/*    CR53         76  */
			/*    RETC (CAMP)  77  */
			/*    CR58         78  */
			/*    r70          79  */
			/*    R71          80  */
			/*    R12 (Camp)   82  */
			/*    CCRD         83  */
			/*    R16          84  */
			/*    R17          85  */
			/*    RDEU	   86  */	
			/*    RDPS         87  */
			/*    RDTS	   88  */
			/*    RDOS	   89  */

#define FIELDCD(x)  fields[x][0]		/* Return field code */
#define GRAPHOK(x) (fields[x][1] & 0x40)	/* TRUE if graphic character ok as input */
#define ALPHOK(x)  (fields[x][1] & 0x20)	/* TRUE if Alphabetic char ok as input */
#define NUMEROK(x) (fields[x][1] & 0x10) 	/* TRUE if Numeric character ok as input */
#define FIELDMD(x) (fields[x][1] & 0x0f)	/* Returns mode bits */
#define NUMCHAR(x)  fields[x][2]		/* Returns Number of characters in display */
#define NUMTRAN(x)  fields[x][3]		/* Returns number of Char. to Translate   */
						/*  (from Left, 0 if no translation) 	*/
#define PROMPT(x)   prompts[fields[x][4]]	/* Prompt Label */
#define TRANTABL(x) xlat[fields[x][5]]		/* Translation Table */



 	byte fields[][6] = {
			{FPHONE,0x5A, 12, 0,  1, 0},
			{FDATA, 0x7F, 35, 0,  2, 0},
			{FNAME, 0x7F, 16, 0,  3, 0},
			{FSYSTM,0x2F,  1, 0,  4, 0},
			{FSECT, 0x75,  3, 0,  5, 0},
			{FROW,  0x75,  4, 0,  6, 0},
			{FCAT,  0x15,  2, 0,  7, 0},
			{FSITNO,0x1A,  3, 0,  8, 0},
			{FSEAT, 0x15,  4, 0,  9, 0},
			{FALTOK,0x15,  5, 1,  0, 1},
			{FMOP,  0x7F,  2, 0, 10, 0},
			{FDATE2,0x1F,  5, 1, 11, 2},
			{FTAXES,0x75, 22, 0, 12, 0},
			{FEVENT,0x35,  5, 0, 13, 0},
			{FCAMP, 0x3A,  5, 0, 14, 0},
			{FDATE, 0x1F,  5, 1, 15, 2},
			{FLOCTN,0x15,  2, 0, 16, 0},
			{FEQPMT,0x1A,  2, 0, 17, 0},
			{FTIX,  0x1F, 18,12, 18, 3},
			{FMODIF,0x7F, 11, 0, 19, 0},
			{FSPEC, 0x7F,  4, 0, 20, 0},
			{FLDATE,0x1A,  5, 1, 21, 2},
			{FPERF, 0x1F,  1, 0, 22, 0},
			{FINQ,  0x1F,  4, 1,  0, 5},
			{FTYPE2,0x7F,  2, 0, 23, 0},
			{FTITLE,0x7F, 24, 0, 24, 0},
			{FPW,   0x7F,  8, 0, 25, 0},
			{FPW2,  0x7F,  8, 0, 26, 0},
			{FOPID, 0x1F,  3, 0, 27, 0},
			{FAMTS, 0x75, 16, 0, 28, 0},
			{FPRIC2,0x55,  8, 0, 54, 0},
			{FAN,   0x75, 12, 0, 29, 0},
			{FPF,   0x75, 12, 0, 30, 0},
			{FPM,   0x75, 10, 0, 31, 0},
			{FDOP,  0x15,  5, 1, 32, 2},
			{FTOTAL,0x5F, 15, 0, 33, 0},
			{FCOMM, 0x5F, 15, 0, 34, 0},
			{FCHECK,0x5F, 15, 0, 35, 0},
			{FCASH, 0x5F, 15, 0, 36, 0},
			{FACCT, 0x1F,  6, 0, 37, 0},
			{FTHTRE,0x1F,  2, 0, 38, 0},
			{FTYPE, 0x1F,  1, 0, 23, 0},
			{FAD,   0x25,  1, 0, 39, 0},
			{FCODE, 0x3F,  9, 0, 40, 0},
			{FCITY, 0x15,  2, 0, 41, 0},
			{FNITES,0x1A,  2, 0, 42, 0},
			{FNSITS,0x1A,  3, 0, 43, 0},
			{FNUMBR,0x15,  2, 0, 43, 0},
			{FLNPL, 0x1F,  4, 0, 45, 0},
			{FSTAT, 0x7F,  2, 0, 46, 0},
			{FZONE, 0x15,  2, 0, 47, 0},
			{FCSTYP,0x1A,  1, 0, 48, 0},
			{FMODE, 0x1F,  1, 0, 49, 0},
			{FOLD,  0x15,  1, 0, 50, 0},
			{FVAR1, 0x1A,  1, 0, 51, 0},
			{FNEW,  0x15,  1, 0, 52, 0},
			{FPEOPL,0x1A,  1, 0, 53, 0},
			{FPRICE,0x15,  1, 0, 54, 0},
			{FPRICE,0x1A,  4, 1, 54, 4},
			{FADJ,  0x1F,  1, 0, 55, 0},
			{FREASN,0x1F,  1, 0, 56, 0},
			{FCARD, 0x1F, 19, 0, 57, 0},
			{FEXP,  0x1F,  4, 0, 58, 0},
			{FTSOPT,0x7F,  4, 0, 59, 0},
			{FTSNAM,0x7F, 32, 0,  3, 0},
			{FCONF, 0x7F, 10, 0, 60, 0},
			{FADDR1,0x7F, 32, 0, 61, 0},
			{FADDR2,0x7F, 32, 0,  0, 0},
			{FADDR3,0x7F, 32, 0,  0, 0},
			{FADDR4,0x7F, 16, 0,  0, 0},
			{FZIP,  0x7F, 10, 0, 62, 0},
			{FPHON1,0x7F, 17, 0, 63, 0},
			{FPHON2,0x7F, 17, 0, 64, 0},
			{FMEMO, 0x7F, 32, 0, 65, 0},
			{FTSMOD,0x7F,  2, 0, 66, 0},
			{FCNAME,0x7F, 32, 0, 67, 0},
			{FTTN,  0x7F,  6, 0, 68, 0},
			{FTSCOD,0x1F,  6, 0, 69, 0},
			{FREQ,  0x7F, 10, 0, 70, 0},
			{FTSSNM,0x7F, 27, 0,  3, 0},

			{FBR,   0x2F,  1, 0, 71, 0},
			{FCDVAL,0x1F,  4, 0,  0, 0},
			{FTSZIP,0x7F,  5, 0, 63, 0},
			{FREPRT,0x7F,  4, 0, 72, 0},
			{FGPTH, 0X1F,  4, 0, 75, 0},
			{FGROUP,0X1F,  2, 0, 76, 0}, 
			{FMERCH,0x1F,  2, 0, 73, 0}, 
			{FCRDTY,0x2F,  8, 0, 74, 0},
			{FIN,      0,  0, 0,  0, 0}
			};
 

	char *prompts[]= {
			{""},
			{"Phone "},
			{"Data "},
			{"Name "},
			{"System "},
			{"Sec "},
			{"R/B "},
			{"Cat "},
			{"Campsite No. "},
			{"Seat "},
			{"MOP "},
			{"Date2 "},
			{"Tax (1 2 3) "},
			{"Event "},
			{"Camp "},
			{"Date "},
			{"Location "},
			{"Eqpmt "},
			{"Tickets "},
			{"Modifier "},
			{"Spec "},
			{"Last Date "},
			{"Perf "},
			{"Type "},
			{"Title "},
			{"PW  "},
			{"PW2 "},
			{"Op ID "},
			{"Amts (Old New) "},
			{"A/N "},
			{"P/F "},
			{"PM "},
			{"DOP "},
			{"Total "},
			{"Comm "},
			{"Check "},
			{"Cash "},
			{"Acct "},
			{"Theater "},
			{"A/D "},
			{"Code "},
			{"City "},
			{"Nights "},
			{"Number "},
			{" "},		/* {"No. of Sites "}, (NO longer used) */
			{"L/P "},
			{"Stat "},
			{"Zone "},
			{"Campsite Type "},
			{"Mode "},
			{"Old "},
			{"Var1 "},
			{"New "},
			{"People "},
			{"Price "},
			{"Adj "},
			{"Reason "},
			{"Card "},
			{"Exp "},
			{"Opt "},
			{"Confirmation "},
			{"Addr "},
			{"Zip "},
			{"Phone 1 "},
			{"Phone 2 "},
			{"Memo "},
			{"MOD "},
			{"Name 2 "},
			{"TTN "},
			{"Auth Code "},
			{"Request "},
			{"B/R "},
			{"Report No."},
			{"Merchant No. "},
			{"Disk File Name "},
			{"Gp/Th "},
			{"Group "},
			};

	byte laltok[][8]= {
			{"1AltOK"},
			{FIN}
			};

	byte ldate[][8]= {
			{"1Jan"},
			{"2Feb"},
			{"3Mar"},
			{"4Apr"},
			{"5May"},
			{"6Jun"},
			{"7Jul"},
			{"8Aug"},
			{"9Sep"},
			{"*Oct"},
			{"0Nov"},
			{"/Dec"},
			{"KMon  "},
			{"LTues "},
			{"MWed  "},
			{"NThurs"},
			{"OFri  "},
			{"PSat  "},
			{"QSun  "},
			{"RWkday"},
			{"SToday"},
			{"TTomrw"},
			{FIN}	
			};

	byte ltix[][8]= {
			{"A Prc1 "},
			{"B Prc2 "},
			{"C Prc3 "},
			{"D Prc4 "},
			{"E Prc5 "},
			{"F Prc6 "},
			{"G Prc7 "},
			{"H Prc8 "},
			{"I Prc9 "},
			{"11"},
			{"22"},
			{"33"},
			{"44"},
			{"55"},
			{"66"},
			{"77"},
			{"88"},
			{"99"},
			{"00"},
			{FIN}
			};


	byte ldisc[][8] = {
			{"RReg "},
			{"DDisc"},
			{FIN}
			};


	byte linq[][8] = {
			{"1Inq "},
			{"2Buy "},
			{"3Alt "},
			{"4Next"},
			{FIN}
			};


	byte (*xlat[]) = {
			{0},		/* don't use xlat[0] */
			{*laltok},
			{*ldate},
			{*ltix},
			{*ldisc},
			{*linq}
			};

	struct xlat_tbl {		/* translation table for date field */
			byte *key;
			int ident;
			byte xlation;
			} xdate[] = {

			{"2", 1, '2'},
			{"3", 1, '3'},
			{"4", 1, '4'},
			{"5", 1, '5'},
			{"6", 1, '6'},
			{"7", 1, '7'},
			{"8", 1, '8'},
			{"9", 1, '9'},
			{"M", 1, 'K'},
			{"F", 1, 'O'},

			{"01", 2, '1'},
			{"02", 2, '2'},
			{"03", 2, '3'},
			{"04", 2, '4'},
			{"05", 2, '5'},
			{"06", 2, '6'},
			{"07", 2, '7'},
			{"08", 2, '8'},
			{"09", 2, '9'},
			{"10", 2, '*'},
			{"11", 2, '0'},
			{"12", 2, '/'},
			{"1/", 2, '1'},
			{"SA", 2, 'P'},
			{"SU", 2, 'Q'},
			{"TH", 2, 'N'},
			{"TU", 2, 'L'},
			{"WE", 2, 'M'},
			{"WK", 2, 'R'},

			{"TOD", 3, 'S'},
			{"TOM", 3, 'T'},
			{"FIN", FIN, FIN}

			};
