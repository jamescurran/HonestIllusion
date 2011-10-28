	page 	60,132
title	DSCP 	Data Scope Routines
;---------------------------------------------------
;
;	DSCP	Machine Language Data Scope Routine
;		Interacts with SEDSCP.C
;
;---------------------------------------------------
; Data and Stack segments
;---------------------------------------------------

DGROUP		group		DATA
DATA 		segment 	word public 'DATA'
__dscp		db 	0


dsmsg1		db	'FF=',0		; Message for a series of nulls.
dsmsg2		db	'CC ',0		; 
lchar		db	0
nulls		db	0
lincount	db	0
;
	extrn	STDCOLOR:word
	extrn	color2:word
	extrn	unitsl:byte
;
data		ends

;---------------------------------------------------
; Program mainline
;---------------------------------------------------


PGROUP          group 		PROG
PROG 		segment 	byte public 'PROG'
;--------------------------------------------------------
	public	_dscp_,__dscp,dscp,lchar
	extrn	crtout:near,putcinc:near,rprints:near
;
;
    		assume cs:pgroup, ds:dgroup 
;
_dscp_	proc	near
	nop
_dscp_	endp
;
;

;
dscp	proc	near
	push	bp
	mov	bp,sp
	mov 	ax,[bp+4]
;
	cmp	al,lchar	; Does Current character equal last char ?
	je 	dsequ
;
	cmp	lchar,0ffh	; Was the Character a Null ?
	je	lastff
;
dsoutc:
	mov	nulls,0		; clear nulls counter.
;
dscout:	call	bytout		; Print ascii of byte in lchar.
	jmp	ret2
;
lastff:	cmp	nulls,0		; test if we had any before.
	jz	dsoutc		; if this is a single null, print it.

	mov	bl,nulls
	mov	lchar,bl	; Pretend count is a data byte.
	jmp	dscout		; Print it and return to looping.
;
dsequ:	inc	al		; Check for null.
	jz	dsff
;
	mov	al,lchar
	cmp	al,60h
	jl	dsoutc		; to low for unit select; just print it.
;
	cmp	al,6fh
	jg	dsoutc		; To hign for unit select; just print it;
;
	mov	lincount,29	; force a new line.
	call	bytout
	call	bytout		; Print the byte twice
	jmp	ret1		; start over
;
;
;
dsff:	cmp	nulls,0		; Have we had any nulls before ?
	jz	dsff2		; if not, these are the first two of the series.
;
	mov	al,nulls
	inc	al
	daa			; Adjust for Decimal.
	mov	nulls,al
	jc	dsff99		; If over 99 FF's
;
ffout:	jmp	ret2
;
dsff2:	mov	nulls,2		; set null count to 2.
	inc	lincount	; add one to line counter.
	lea	ax,dsmsg1	; "FF="
	call	stroutc		; DOS Interupt
	jmp	ffout
;
dsff99:	mov	nulls,0
	lea	ax,dsmsg2	; "CC "
	call	stroutc
	jmp	ret1		; Start over.
;
;
;
bytout:	mov	bl,al		; save current character.
;
	inc	lincount
	cmp	lincount,25
	jle	byout1		; 25 is the max on one line.
;
	mov	al,0Dh		; 0D = Carriage Return
	call	stdout
	mov	al,0Ah		; 0A = Line Feed.
	call	stdout
;
	mov	lincount,1	; Reset Line counter.
;
;
byout1:	mov	dl,lchar
	call	hexout
;
	mov	al,' '		; Print Trailing Space.
	call	stdoutc
;
	mov	lchar,bl	; Make Current Char the new last char.
	ret
;
;
;
ret1:	mov	ax,1	
	pop	bp
	ret
;
ret2:	mov	ax,2
	pop	bp
	ret
;
ret3:	mov	ax,3
	pop	bp
	ret
;
;
dscp	endp
;
;
;
;
hexout	proc	near
;
	push	cx
	push	ax
;
	mov	cx,2		; We're coming through the loop twice
hexout1:
	push	cx		; Save the count.
	mov	cl,4		; We'll Be shifting over 4 bits.
	rol	dl,cl		; Rotate DL left;
;
	mov	al,dl		; Move into al
	and	al,00fh		; Remove upper digit
	daa			; Add 6 if A-F
	add	al,0f0h		; Bump a carry if A-F
	adc	al,040h		; Here is the ASCII
	call	stdoutc		; Print it.
	pop	cx
	loop	hexout1
;
	pop	ax
	pop	cx
	ret

hexout	endp
;
;
;
stdout	proc	near
	push	dx
	push	bx
;			;       mov	bp,sp	(approved method)
;
	push	ax
	call	crtout
	pop	ax	;	mov	sp,bp
;
	pop	bx
	pop	dx
	ret
stdout	endp
;
stdoutc	proc	near
	push	dx
	push	bx
	push	bp
	mov	bp,sp
;
	xor	bx,bx
	push	bx	; page =0
	mov	bx,STDCOLOR
	mov	cl,lchar
	cmp	cl,unitsl
	jne	std
	mov	bx,color2
;
std:	push	bx
	push	ax
	call	putcinc
;
	mov	sp,bp
	pop	bp
	pop	bx
 	pop	dx
;
	ret
stdoutc	endp
;
;
stroutc	proc	near
	push	ax
	push	bx
	push	bp
	mov	bp,sp
;
	push	ax
;
	xor	bx,bx
	push	bx	; page =0
;
	mov	bx,STDCOLOR
	push	bx
;
	call	rprints
;
	mov	sp,bp
	pop	bp
	pop	bx
 	pop	ax
;
	ret
;
stroutc endp
;
;
;
prog	ends
	end

