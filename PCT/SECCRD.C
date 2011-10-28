/****************************************************************/
/*								*/
/*	S E  C C R D -						*/
/*								*/
/*	S & E  Spec CCRD formatting routines.			*/
/*								*/
/****************************************************************/

#include "SE.H"
#include "SEFDEF.H"
#include <fcntl.h>

#define xlen xmsg[1]

	extern byte 	xmsg[256];
	extern int	file,status;
	extern	int	scflag;
	extern	int	prstat;
	extern	int	mkfldp;
	extern	int	maskp;
	extern	int	upttn;

	extern	byte	fdata[][35];


	byte	__seccrd;
	byte	ccrdflg;
	static	byte	filename[10];
	byte	card[85];

	void	_seccrd_(),ccrd();

void _seccrd_()
{}

/****************************************************************/
/*								*/
/*    CCRD -                                  			*/
/*								*/
/*								*/
/*								*/
/****************************************************************/

void ccrd()
		
{
	strins(filename,&fdata[FCRDTY],8);
	strcat(filename,".CIF");

	file=open(filename,O_RDONLY|O_RAW,0);

	if (file==-1) { 
			puts(filename);
			seemsg(25+beep);
			ccrdflg=FALSE;
			mkfldp=maskp+2;
			fndfld(FLDCODE(mkfldp));
			newfld();
			return;
			}

	ccrdxmit();
	ccrdflg=TRUE;
}


/*****************************************************************/

ccrdxmit()
{
	status=read(file,card,82);

	if (status!=82)  {
			 ccrdflg=FALSE;
			 close(file);
			 mkfldp=maskp+2;
			 fndfld(FLDCODE(mkfldp));
			 newfld();
			 return(4);
			 }

	xlen = 86;

	xmsg[8]=FSPEC;
	strins(&xmsg[9],"CCRD",4);
	xmsg[13]=FCRDTY;
	strins(&xmsg[14],card,80);

	upttn=YES;
	scflag=0;
	sesend(xmsg);
	matrix(CR); matrix(LF);

	return(3);

}
	

