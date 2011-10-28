/****************************************************************/
/*								*/
/*	S E   H E L P						*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

#include "se.h"
#include "sefdef.h"
#include <grnleaf\color.h>

#define HLPATTR		(HLPFOR | ( HLPBACK << 4 ))

#define SE		0xFFF5		/* Negative w/SE & CSE mode bits */
#define CAMP		0xFFFA		/* Negative w/CAMP & CCMP Mode bits */
#define ALL		0		/* Default for all modes */

	extern 	int 	maskp, mkfldp;
	extern	int	crttim,foreg,backg,tvmaxc,cpsc2;
	extern	byte	mode;

	void	ahelpkey(),helptop(),ahelpfld(),ahelpmsk(),hlpmsg();

	int	HLPFOR,HLPBACK;
	
	byte *helpkey[10] = {
"  F1\tTransmit  Transmits request to_main computer\n  F3\tDisp Fld  Displays all fields_in current mask\n  F5\tNext\t  Requests next set of_tickets\n  F7\tLAST\t  Recalls last screen_transmitted\n  F9\tSPEC\t  Moves cursor to_Special field",
"  F2	INQ	  Requests a inquiry_about seating.\n  F4	BUY	  Requests that tickets_be printed.\n  F6	ALT	  Requests that the_alternate tickets be_printed\n  F8	ALTOK	  Allows a BUY from_a SO ALT\n  F10	CLEAR	  Returns to the_default mask for this_mode.",
"  \x18	Back Fld  Moves cursor to the_previous field\n  \x19\tNext Fld  Moves cursor to the_next field\n Return Next Fld  Moves cursor to the_next field\n Enter\tNext Fld  Moves cursor to the_next field\n",
"  \033\t \tMoves cursor back one_position within field\n  \032\t \tMoves Cursor Forward_one position\n Back Space\tMove cursor back one_position removing old_data",
"  Home\t \t  Clears the current_mask\n  End\tStop\t  Interrupts certain_reports\n  Pg Dn\t \t  Switchs screen to_next mask\n  Pg Up\t \t  Step back one mask\n  Del\tClr Fld   Clears the current_field", 
" Ins <preset>     Store current mask_(including data) into_memory for recall.\n Ins Ins <preset> Store current mask_(including data) onto_disk for recall.\n <preset>\t  Recalls a preset\n\n    <preset> is any letter with the_\"Alt\" held down", 
"  Ins   *\t  Prints a list of_stored presets\n  Ins   - \t  Clears all Presets_from memory\n F10 Ins <Preset> Removes the preset_(From memory or disk)\n\n    <preset> is any letter with the_\"Alt\" held down", 
"\nFor these, the \"Ctrl\" must be held down\n\n  F1\tNot Currently used\n  F2\tNot Currently Used\n  F3\tNot Currently used\n",
"\nFor these, the \"Ctrl\" must be held down\n\n  F4\tDisplays These Keyboard_Help Screens\n  F5\tDisplays Mask Help Message\n  F6\tDisplays Field Help Message", 
"\nFor these, the \"Ctrl\" must be held down\n\n  F7\tGoes to a special menu for_changing printer_status\n  F8\tGoes to a special menu for_changing mode\n  F9\tToggles Scrolling on & off.\n  F10\tExits to DOS", 
};

struct mask_stru {
		byte mask;
		byte *mmsg;
		} maskmsg [] = {

		{1,"SE - Inquire for or buy available seats."},
		{2,"CRET - Find return code after date change"},
		{3,"FRET - Find return code"},
		{4,"NOFS - Change performance status"},
		{5,"PCHG - Price change"},
		{6,"PRIC - Price availibility change"},
		{7,"RESV - Reserve seats"},
		{8,"R0 - Short status report"},
		{9,"R15 - Statement control table display"},
		{10,"R19 - Box office report (one location)"},
		{11,"R2 - Total advance sales"},
		{12,"R22 - Total advance sales for perfs."},
		{13,"R32 - Total sales today (advance perfs)"},
		{14,"R37 - List performances for event"},
		{15,"R39 - Performance options report"},
		{16,"R5 - Box office status"},
		{17,"R6 - Detailed seat status"},
		{18,"R7 - Sales by discount"},
		{19,"R8 - Advance sales by performance"},
		{20,"R9 - Box office report"},
		{21,"R97 - Modified R9 report"},
		{22,"R99 - Area City information"},
		{23,"RETN - Return tickets"},
		{24,"RLSE - Change seat status"},
		{25,"SCTA - Add statement control table"},
		{26,"SCTC - Change statement control table"},
		{27,"SCTD - Delete statement control table"},
		{28,"SER - Series buy"},
		{29,"CAMP - Campground buy mask"},
		{30,"CTRL - Adjust campsite availabilty"},
		{31,"R16 - Specific Park Sales Today Report"},
		{32,"R2 - Specific park advanced sales report"},
		{33,"R39 - Performance options report"},
		{34,"R5 - Group Statistic Report."},
		{35,"R6 - Group Sales Report."},
		{36,"R7 - Group Status Report"},
		{37,"R8 - Specific Park Status Report."},
		{38,"R9 - Detailed Sales Report."},
		{39,"RLSE - Restrict or Release Sales"},
		{40,"RETN - Return Reservation"},
		{41,"CSE - Inquire for or buy available seats. (Telephone Sales)"},
		{42,"CSE - Customer data"},
		{43,"CCMP - Campground Buy mask (telephone sales)"},
		{44,"R1 - Cash settlement"},
		{45,"R4 - Operator ID and passsword info."},
		{46,"R11 - Method of payment report"},
		{47,"R14 - Terminal options"},
		{48,"R33 - Transaction report"},
		{49,"R38 - Telephone sales availability"},
		{50,"R49 - Method of payment history"},
		{51,"CASH - Changes cash, check totals"},
		{52,"MGT - Management transaction"},
		{53,"OFF - Operator sign off"},
		{54,"ON - Operator sign on"},
		{55,"OPID - Change operator ID info."},
		{56,"PASS - Change password info."},
		{57,"PREV - Recall previous transaction"},
		{58,"RPRT - Reprint last buy"},
		{59,"TEST - Test ticket printer"},
		{60,"VOID - Void previous buy"},
		{61,"CRTN - Return previous credit buy"},
		{62,"CDFR -Deferred ticket transaction update"},
		{63,"CR51 - Tickets to be printed report."},
		{64,"PTIX - Deferred ticket printing"},
		{65,"CR52 - Ticket printed batch control\n      number report"},
		{66,"CDSP - Display Transaction"},
		{67,"CR54 - Mailing Labels"},
		{68,"CR55 - Sales receipts"},
		{69,"CR56 - Performance tranactions listing"},
		{70,"CTRP - Ticket reprinting procedure"},
		{71,"CMOD - Customer data update"},
		{74,"R3 - Individual performance total report"}, 
		{75,"RETC - Return tickets for credit"},

		{77,"RETC - Return tickets for credit"},
		{78,"CR58 - Deposit report by merchant"},
		{79,"R70 - Requests list of off-line reports"}, 
		{80,"R71 - Request off-line report"},
		
		{82,"R12 - Specific Park year to date report."},

		{86,"RDEU - Raw Data : Event Update."},
		{87,"RDPS - Raw Data : Performance Sales."},
		{88,"RDTS - Raw Data : Terminal Sales."},
		{89,"RDOS - Raw Data : Operator Sales."},

		{FIN,"\nhelp for this mask is currently\nunder development\n"}

		};

struct field_stru {
		byte 	field;
		int	fmask;
		byte 	*fmsg;
		} fieldmsg [] = {

		{FPHONE,ALL,		"PHONE - Camper's phone number\n        AAA-PPP-NNNN"},
 		{FDATA,ALL,"DATA - Up to 35 Characters.\n     Varies by Request"},
		{FNAME,29, 		"NAME - Camper's Name\n        ex. T M SMITH or T SMITH."},
		{FNAME,ALL,"NAME - Name on credit card."},
		{FSYSTM,ALL,"SYSTEM - System of requesting Terminal\n     Defaults to system of this terminal"},
		{FSECT,ALL,"SEC - 3 char. Section of seat location."},
		{FROW,ALL,"ROW - 4 char. Row of seat location."},
		{FCAT,SE, "CAT - 2 digit Category type.\n    Valid categories 00, 10-14, 20-24"},
		{FSITNO,CAMP,		"CAMPSITE NO - Index of first site in\n       Campsite type to begin adjusting."},
		{FSEAT,ALL,"SEAT - 4 digit Seat # of seat location."},

/*		{FNEXT,ALL, "     "},	*/
/*		{FALTOK,ALL, "     "},	*/

		{FMOP,ALL,"MOP - Method of payment."},

		{FDATE2,ALL, "Date 2 - Date to begin search.\n         Same format as DATE."},
		{FTAXES,ALL,"TAXES (1 2 3) - Tax amounts associated\n        with the new prices"},
		{FEVENT,SE,"EVENT - 1 char. system prefix and\n        up to 4 digit event number.\n        ex. A151"},
		{FCAMP,CAMP,"CAMP - 1 char. system prefix and\n       up to 4 digit camp number.\n  ex.  E1501"},
		{FDATE,CAMP,"DATE - Jan       = 1/dd or 01dd\n       Feb - Sep =  mdd or 0mdd\n       Oct - Dec = mmdd"},
		{FDATE,SE,"DATE - Jan       = 1/dd or 01dd\n       Feb - Sep =  mdd or 0mdd\n       Oct - Dec = mmdd\n also  SUn Mon TUes WEd THurs Fri SAt\n       TODay   TOMwr    WKday"},
		{FLOCTN,SE,"LOCATION - 2 digit seat type."},
		{FEQPMT,CAMP,"EQPMT - 2 digit code for the particular\n        type of equipment involved."},
		{FTIX,ALL,"TICKETS - # of tickets to inq. or buy.\n          A = Price 1, B =Price 2 etc."},
		{FMODIF,ALL,"MODIFIER - Allows a special condition\n       i.e. L = Left Aisle, Z1 = Zone 1"},	
		{FSPEC,ALL,"SPEC - 4 char name of mask to switch to."},
		{FLDATE,ALL,"LAST DATE - Jan       = 1/dd or 01dd\n            Feb - Sep =  mdd or 0mdd\n            Oct - Dec = mmdd"},
		{FPERF,ALL,"PERF - 1 digit performance # (1-5)."},

/*		{FINQ,ALL, "     "},	*/

		{FTYPE2,17,"TYPE - 0 = All Seats (Default) \n1 = Available Seats   2 = Sold Seats\n3, 4, 5 = same as 0,1,2 w/ return code\n6, 7, 8 = same as 3,4,5 w/ just row/box\n10-15 = same as 0-5 w/ # of seats in row"},
		{FTYPE2,ALL,"TYPE -  Entry number to modify."},
		{FTITLE,55,"TITLE - 24 chars.  Operator's name"},
		{FTITLE,ALL,"TITLE - 24 Chars.  Name of line item"},
		{FPW,ALL,"PW - Up to 8 Chars. Operator's password"},
		{FPW2,ALL,"PW2 - Up to 8 Chars. New Password Name"},
		{FOPID,ALL,"OPID - 3 digits. Operator's ID Number"},
		{FAMTS,ALL,"AMTS (OLD NEW) - The former & new prices\n                 ex.   10.00 11.00"},
		{FPRIC2,ALL,"PRICE - Dollar value of ticket. Required\n only for event w/Ancillary Price Tables"},
		{FAN,62,"A/N - Authorization Reference Number"},
		{FAN,ALL,"A/N - \"Amount\" when used for category 0 or 10\n  \"Number\" when used for Category 20. \n  Not used otherwise."},	
		{FPF,ALL, "P/F - \"Percent\" for categories 10-14\n   \"Fee\" for categories 20-24."},	
		{FPM,ALL, "PM - Price Mask.  Can only be used with\n   Categories 12-14 & 20-24"},	
		{FDOP,ALL,"DOP - Date of Purchase\n    Same formats as DATE."},
		{FTOTAL,ALL,"TOTAL - Sum of CASH, CHECK, and COMM."},
		{FCOMM,ALL,"COMM -  Amount to adjust total comm.\n         amount by. Decimal required"},
		{FCHECK,ALL,"CHECK -  Amount to adjust total check\n          amount by. Decimal required"},
		{FCASH,ALL,"CASH -  Amount to adjust total cash\n         amount by. Decimal required"},
		{FACCT,ALL,"ACCT - 6 Digits.  Account Number"},
		{FTHTRE, 61,"THEATER - Theater number"},
		{FTHTRE, 62,"THEATER - Blank for all, else\n          theater number."},
		{FTHTRE,ALL,"THEATER - 2 digit theater number\n           Enter 99 for group report."},
		{FTYPE,6,"TYPE - 1=Box Office, 2=Remotes\n    3=Obstructed view, 4=B.O. & Remotes"},
		{FTYPE,9,"TYPE - 0=Decription of Adjustments &\n       Deductions, 1=Adds names assigned\n       to totals"},
		{FTYPE,11,"TYPE -0=Total adavnced sales\n      1=Advanced sales today\n      4=Today w/telephone sales subtotal\n      5=Yesterday w/ TS subtotals\n      9=Event information."},
		{FTYPE,14,"TYPE - 0=Include availablity status\n   1=Include series buy codes,\n   2=List perf w/ specific buy code.\n   3=List pref w/ specific avail. status"},
		{FTYPE,46,"TYPE - 0 = MOP sales report\n       1 = MOP reconciliation report."},
		{FTYPE,21,"TYPE - 0 = Separate box office report\n       1 = Total by pricce break\n       2 = Recap of total sales."},
		{FTYPE,56,"TYPE - 1 = Establish password\n       2 = Change password\n       3 = Delete Password"},
		{FTYPE,74,"TYPE - 0 = Total advance sale\n       1 = Total sales Today"},
		{FTYPE,ALL,"TYPE - Please see operators Manual"},
		{FAD,ALL,"A/D - \"A\" For Adjustment\n      \"D\" for Deductions."},
		{FCODE,ALL,"CODE - Return code of the ticket"},
		{FCITY,SE,"CITY - 2 Digit.  Area city code.\n     Default = This one"},	
		{FNITES,CAMP,"NIGHTS - Number of nights requested."},
		{FNUMBR,14,"NUMBER - Series buy code (used w/Type 2)"},
		{FNUMBR,50,"NUMBER - MOP table number"},
		{FNUMBR,25,"NUMBER - Order in which entry will print"},
		{FNUMBR,26,"NUMBER - Order in which entry will Print"},
		{FNUMBR,SE,"NUMBER - Number of performances.\n         0  = default = One Performance\n         99 = All Performances."},
		{FNSITS,CAMP,"NUMBER - Number of Sites to adjust."},
		{FLNPL, 56,"L/P - Line/poll for password\n      Default is this terminal."},      
		{FLNPL, 61,"L/P - Line/poll to credit\n      Default is this terminal."},
		{FLNPL, 62,"L/P - Normally blank for any line/poll\n      else line/poll of transactions."},
		{FLNPL,ALL,"L/P - Line & poll of terminal\n     Default is this terminal"},
		{FSTAT, 8,"STAT - Status of Seat Requested\n       0   = All Status Seats 1-7\n       1-8 = Only For Requested Status\n       9   = All Unused Seats."},
		{FSTAT,61,"STAT - Normally blank for default,\n       else status to set returned seats"},
		{FSTAT,24,"STAT - *************** RLSE  **************"},
		{FSTAT,30,"STAT - Please see operators manual"},
		{FSTAT,14,"STAT - Availability Status\n       (used w/Type 3)"},
		{FSTAT,ALL,"STAT- Please see operators manual"},
		{FZONE,SE,"ZONE - 2 digit zone or subzone\n    Default is all zones"},
		{FCSTYP,CAMP,"Campsite Type - 1 digit Campsite Type"},
		{FMODE,4,"MODE - 0=Available to all. 1=Avail BO\n  2 = Not Available  3 = Display Status"},
		{FMODE,ALL,"MODE - 0 = Available to All.\n       2 = Not Available"},
		{FOLD,SE,"OLD - 1 digit \"Change From\" Status"},
		{FVAR1,CAMP,"VAR1 - 1 digit Special Condition Index\n        usually Pet indicator\n        Check Campsite Guide."},
		{FNEW,SE,"NEW - 1 Digit \"Change To\" Status"},
		{FPEOPL,CAMP,"PEOPLE - 1 digit index of number of\n    people in family/party"},
		{FPRICE,SE,"PRICE - 1 digit. 0 = Regular Price\n               1-9 = Special Price"},
		{FPRICE,CAMP,"PRICE - 1 Char \"R\" = Regular Price\n        	       \"D\" = Discounted Price"},
		{FADJ,ALL,"ADJ - 1 Digit Service charge adjustment\n   1= SVC CHG not returned\n   2= SVC CHG returned to customer\n   3= Return carries additional SVC CHG"},
		{FREASN,ALL,"REASON - Reason for return. 1=Exchange\n   2=Misprint, 3=Operator Error\n   4=Customer Refusal, 5=Cancelled Event\n   6=Other, 7=Ticket Refund"},    			      
		{FCARD,ALL,"CARD - Credit card number."},
		{FEXP,ALL,"EXP - Expiration date of credit card."},
		{FTSOPT, 62,"OPT - INQ: Retrieval options\n          Blank=All, 1=Today, 2=Report\n      BUY: Action option\n         A=Approve, D=Defer, V=Void\n          R=Retry Authorization"},
		{FTSOPT, 63,"OPT - For one perf:1=Show all seat types\n      For multiple perf:1=Show all pref\n      else blank=Show only thoses with\n      tickets to be printed."},
		{FTSOPT, 64,"OPT - 1=Report for all dates\n      T=Telephone sales trans (default)\n      N=Credit sales transaction\n      A=Telephone & credit sales."},
		{FTSOPT, 67,"OPT - 0=Single label (default)\n      1=Multiple labels\n      2=Multiple labels\n      /NN=Optional number of lines/label"},
		{FTSOPT,ALL,"OPT - Please see operators manual"},
		{FTSNAM,ALL,"NAME - Name on credit card"},
		{FCONF,ALL,"CONFIRMATION - Confirmation code \n      A B C D Z"},
		{FADDR1,ALL,"ADDRESS - 32 chars, first line of\n       Mailing address"},
		{FADDR2,ALL,"ADDRESS - 32 chars, second line of\n       Mailing address"},
		{FADDR3,ALL,"ADDRESS - 32 chars, third line of\n       Mailing address"},
		{FADDR4,ALL,"ADDRESS - 16 chars, fourth line of\n       Mailing address"},
		{FZIP,ALL,"ZIP - 10 chars  Zip code."},
		{FPHON1,ALL,"PHONE1 - 17 Char. AAA-PPP-NNNN-XXXX\n    Space, comma, period, or dash OK."},
		{FPHON2,ALL,"PHONE2 - 17 Char. AAA-PPP-NNNN-XXXX\n    Space, comma, period, or dash OK."},
		{FMEMO,ALL,"MEMO - Optional memo data (1-32 chars)"},
		{FTSMOD,ALL,"MOD - Method of delivery\n      PM/PH=Point of sale MAIL/HOLD\n      BM/BH=Box office MAIL/HOLD\n      TM/TH=Teletron MAIL/HOLD\n      RH   =Remote HOLD."},
		{FCNAME,ALL,"NAME 2 - 32 char Name of person\n   receiving the ticket, if different\n   than name on card."},
		{FTTN,ALL,"TTN - Batch control number (terminal\n      transaction number) from CR52."},
		{FTSCOD,ALL,"AUTH CODE - Authorization code"},
		{FREQ,ALL,"REQUEST - 4 char management function\n   followed by a space and a 5 char \n   code"},
		{FTSSNM,ALL,"NAME - 27 char Customer name"},
		{FBR,ALL,"B/R -  \"B\"=Ticket bought at box office\n       \"R\"=Ticket bought at remote"},
		{FCDVAL,ALL,"Credit card starting date\n   NEVER entered manually"},
		{FREPRT,79, "REPORT NO. - Number to start list from."},
		{FREPRT,ALL,"REPORT NO. - Requested report number\n	      From R70."},
					    
		{FIN,FIN,"\nhelp for this field currently\nunder development"}

		};				

/****************************************************************/
/*								*/
/*	AHELPKEY - Displays help screen for special 		*/
/*		   control keys.				*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

void ahelpkey()

{
	int 	x,i,lgscr,page;
	byte	ch;

	if (vstate(0)==4) smallscr(NO);
	
	if (equipment(0)==7) {
				page =0;	
				smallscr(NO);
				}
		else	     {
				page =3;
				}

	vpage(page);
	crttim=0;
	lgscr = (tvmaxc==39) ? TRUE:FALSE;

	for (i=0; i<10; ++i) {

		helptop(page);		
		for (x=0;;++x) {
			ch=*(helpkey[i]+x);
			if (ch==0) break;
			if (ch=='_') {
				if (lgscr) rprints(foreg,page,"\n \t \t  ");
				else rputcinc(' ',0,foreg,NO,page);
				}
			else  rputcinc(ch,0,foreg,NO,page);

			if ((ch==LF) && (lgscr)) putch(LF);   /* 2nd LF */
				
		}

		curset(23,5,page);
		if (i<9) {
				rprints(foreg,page,"Do you want the next page (Y/N) ? ");
				x=getyn();
				if (x==NO) break;
				}
		else 	{
			       	rprints(foreg,page,"Hit any key to return  ");
				getkey();
				}
		}

	bright;			/* Restore page 0 */

	if (page==0) {
			if (SMALLMSK(maskp)) smallscr(NO);
			else largescr(NO);
			
			dspmsk();
			}
}

void helptop(page)
	int page;
{
	cls(); curset(3,2,page);

	rprints(foreg,page,"Key   Function \tMeaning\n\n");
}


/********************************************************/
/*							*/
/*  ahelpfld() display help for current field		*/
/*                                              	*/
/*							*/
/*							*/
/********************************************************/				

void ahelpfld()
{
	int lp1,FM;
	byte currfld, currmask;

	currmask = MSKCODE(maskp);
	currfld = FLDCODE(mkfldp);

	for (lp1 = 0; fieldmsg[lp1].field != FIN ; lp1++) {
		if (fieldmsg[lp1].field == currfld) {
			while (fieldmsg[lp1].field == currfld) {
				if ( (FM=fieldmsg[lp1].fmask) == 0 ||
				    FM == currmask ||
				   ( (FM < 0) && (mode & FM) ) ) {
					break;	/* exit while loop */
				}
				else {
					++lp1;
				}
			}      
			break;	/* exit for loop */
		}
	}
	hlpmsg(fieldmsg[lp1].fmsg);
}

/********************************************************/
/*							*/
/*  ahelpmsk() display help for current mask		*/
/*                                              	*/
/*							*/
/*							*/
/********************************************************/				

void ahelpmsk()
{
	int lp1;
	byte currmask;

	currmask = MSKCODE(maskp);
	for (lp1 = 0 ; maskmsg[lp1].mask != FIN ; lp1++) {
		if (maskmsg[lp1].mask == currmask || maskmsg[lp1].mask == 0)	{
			break;
		}
	}
	hlpmsg(maskmsg[lp1].mmsg);
}

/********************************************************/
/*							*/
/*  hlpmsg() display message, scroll bottom of screen	*/
/*                                              	*/
/*							*/
/*							*/
/********************************************************/				

void hlpmsg(mess)
byte mess[];
{
	int lp1;

	curset(24,0,0);
	scrlup(2,HLPATTR,20,0,24,39);
	for (lp1 = 0 ; mess[lp1] != '\0' ; lp1++) {
		if (mess[lp1] == 0x0a) {
			scrlup(1,HLPATTR,20,0,24,39);
			curset(24,0,0);
		}
		else 
			rputcinc(mess[lp1],HLPBACK,HLPFOR,0,0);
	}

	dspfld();
}
