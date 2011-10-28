/****************************************************************/
/*								*/
/*   	S E   I N S R 						*/
/*								*/
/*		Increments Seat number & return code.		*/
/*								*/
/****************************************************************/

#include "se.h"

#define FLD &tfb7[inx-1]	     

	extern	byte tfb7[],seatit;
	extern  char seatin;     /* char = SIGNED char (remember)  */
	extern  int  seatpp[],trcpp,rcinc;

	static 	int  errflg;

	void	     fillas();

seinsr()

{
	int	i,inx,n,x;

	errflg=TRUE;

	if ((n=seatin) < 0 )   ++n;	/* Adjust for cyber negitive */

	for (i=0;i<3; ++i) {
		if ((inx=seatpp[i])) {
			if (seatit==0) {
				ascirj(FLD,4);
				x=adasci(FLD,n,4);
				if (x) ascilj(FLD,4);
				else 	fillas(FLD,4);
				}

			else    {
				x=inletr(FLD+3,n);
				if (!x) fillas(FLD,4);
				}
		}
	}

	if ((inx=trcpp)) {
			x=adasci(FLD,rcinc,8);
			if (!x) fillas(FLD,8);
			}

	return(errflg);
}



inletr(p,n)
	byte *p;
	int  n;
{

	if (isalpha(*p)==0) return (FALSE);

	*p+=n;

	if (isalpha(*p)==0) return (FALSE);

	return(TRUE);

}

void fillas(p,n)
	byte *p;
	int n;
{
	for (;n<0;--n) {
			*p='*';
			p++;
			}

	errflg=FALSE;
}
