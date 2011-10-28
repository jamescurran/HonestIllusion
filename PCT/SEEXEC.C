/****************************************************************/
/*								*/
/*   	S E   E X E C -						*/
/*								*/
/*		Executive for S & E PC Terminal.		*/
/*								*/
/*								*/
/*								*/
/****************************************************************/
	     
#include "se.h"
#include "sefdef.h"

#define valmod(x) (x & mode)

	
	extern maskp,fieldp,mkfldp,mode,actime,retnfg;
	extern byte fdata[][35],semask[4],tsmode;

	int	__SEEXEC=0;	/* Indicates start of data on .MAP file */
	int	mgtkey=TRUE;	/* Management Key Flag */

void main (argc,argv)
	int	argc;
	byte	*argv[];
{
	byte ch,kbin();
	int x,status;

	clrpre();		/* Clear Presets */

	maskp=0;		/* Start With First mask */

	if (argc==2) x=TRUE;  else x=FALSE;

	init(x); 		/* Initialize Ports, Buffer, Terminial Value etc */
	
	dftmsk();		/* Get Default Mask for this mode */
	newmsk();

       	in(comm_port,&status);        
	if (status==2) seemsg(5+beep);	/* Line Inactive */
	else 	       seemsg(14);	/* Date */

	setpre(' ');		/* set LAST to clear screen */

pcloop:	for ever {
 
		semgst();
   
		if ( (ch=kbin()) ) {	  	/* Wait for a key board input */
			x=keypro(ch);	     	/* Process the key */
			if (x==XMIT)  break;	/* If it was a Transmit, Send data */
			}			/* Keep Looping for ever.... */
		}

	xmit();

	if (semask[0]==0) goto pcloop;

	if (!fndmsk(semask)) goto pcloop;

	retnfg=TRUE;
	setnds();

	if (tsmode==0) {
			clrdat(); 
			clretn();
  			}

	mkfldp=maskp+2;

	fndfld(FLDCODE(mkfldp));

	dspmsk();
	newfld();
	goto pcloop;
	
}
