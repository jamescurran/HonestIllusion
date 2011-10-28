/****************************************************************/
/*								*/
/*   	S E   P R E 						*/
/*	             						*/
/*	Preset module.  Calls PRE.ASM.				*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
#include "se.h"
#include <fcntl.h>
#include <grnleaf\color.h>
#include <grnleaf\ibmkeys.h>

#define NOPROTECT S_IREAD|S_IWRITE 

		byte	___sepre=0;
		byte	dskpre[]="A.PRE            ";

		byte	__sepre;
		byte	addr[27];
		int	npres;
	extern	byte	mode;
	extern	int	fieldp,mkfldp,retnfg,foreg,predsk,STDCOLOR;
	extern	int	filenum;
	extern	int	errno;

	void	setpre(),getpre(),lstpre(),clrpre(),delpre();
	
void _sepre_() {}

/****************************************************************/
/*								*/
/*   	Set Preset.						*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
void setpre(x)
	int x;
{	int i;

	if (predsk==TRUE)  {     
			dskpre[0]= (x & 0x7f);
			filenum = open(dskpre,O_WRONLY|O_CREAT|O_RAW,NOPROTECT),;
			write(filenum,&mode,8192);
			close(filenum);
			predsk=FALSE;
			return;
			}

	if ((i=fndpre(x))==0) {
		if((i=fndpre(0))==0) { 
			seemsg(55+beep);
			return;
			}
		else addr[i]=(x & 0x7f);

		}

	setpres(i);
}

/****************************************************************/
/*								*/
/*   	GETPRE - Get preset 					*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
void getpre(ch)
	byte ch;
		
{	int i,x,y;

	if ((i=fndpre(ch))) getpres(i);
	else {
		dskpre[0]=(ch & 0x7f);
		filenum = open(dskpre,O_RDONLY|O_RAW);
 		if (filenum !=-1) {
				read(filenum,&mode,8192);
			       	close(filenum);
			       	}
		else { 
			seemsg(56+beep); 
			return;
			}
		}

	x=fieldp;
	y=mkfldp;

	retnfg=FALSE;
	newmsk();

	fieldp=x;
	mkfldp=y;

	newfld();

}

/****************************************************************/
/*								*/
/*   	LSTPRE- List presets in use on CRT.			*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
void lstpre()

{	int i,x,stat;


	x=getcur(0);

	ratsay(8,1,STDCOLOR,0,"Presets Saved:\n");
	stat=0;

	for (i=2; i<=npres; ++i) {
			if (addr[i]) {
				putcinc(addr[i],STDCOLOR,0);
				putcinc(' ',STDCOLOR,0);
				stat=1;
				}
				}
	if (stat==0) rprints(STDCOLOR,0,"None");
	

	stat=dosfirst("?.pre",0,dskpre);
	if (stat==0) ratsay(11,1,STDCOLOR,0,"Permanent Presets:\n");

	while (stat==0) {
				putcinc(dskpre[0],STDCOLOR,0);
				putcinc(' ',STDCOLOR,0);
			stat=dosnext("?.pre",0,dskpre);
			}
		      
	curset(13,1,0);
	rprintf(STDCOLOR,0,"Maximum Temporary Presets : %d",npres); 
	
	crtrtc(x);
}
/****************************************************************/
/*								*/
/*   	CLRPRE - Clear all presets				*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
void clrpre()

{
	zero(addr,npres);
}

/****************************************************************/
/*								*/
/*   	DELPRE - delete a single preset.			*/
/*								*/
/*								*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	    
void delpre(ch)
	byte ch;
{
	int i;

	if ( (i=fndpre(ch)) ) addr[i]=0;
	else {
		dskpre[0]= (ch & 0x7f);
		i=remove(dskpre);
		if (i) seemsg(56+beep);
		}
}
/****************************************************************/
/*								*/
/*   	FNDPRE - Find preset.					*/
/*	Enter with desired code. return either ADDR offset	*/
/*	or zero, if not found. 					*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
fndpre(ch)
	byte ch;
{	int i;

	if (ch==' ') return(1);

	ch &= 0x7f;			/* Remove Alt-Shifting */

	for (i=2; i<=npres; ++i) {
		if (addr[i]==ch) return(i);
		}

	return(0);
}
/****************************************************************/
/*								*/
/*   	VALPRE - Validates preset code.				*/
/*								*/
/*      returns - TRUE if OK.					*/
/*	  	- FALSE if not, & displays "Invalid Preset"	*/
/*								*/
/*								*/
/****************************************************************/
	     
valpre(ch,x)
	byte ch;
	int   x;
{

	if ((ch>=ALTA) && (ch<=ALTZ)) return(TRUE);

	if (x==YES)	seemsg(57+beep);
	
	return(FALSE);
}
