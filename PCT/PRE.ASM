	page 	60,132
title	Pre set  module 	
;---------------------------------------------------
;
;	P R E   - 
;		Transfers 8196 bytes of data between
;		field storage area (fdata & related
;		variables) and block in upper memory
;
;
;---------------------------------------------------
; Data and Stack segments
;---------------------------------------------------

DGROUP		group		DATA
DATA 		segment 	word public 'DATA'

		extrn		mode:byte

		public		toppre
toppre		dw		0A000h


data		ends

;---------------------------------------------------
; Program mainline
;---------------------------------------------------


PGROUP          group 		PROG
PROG 		segment 	byte public 'PROG'
;--------------------------------------------------------
 	public	        _pre_,setpres,getpres

;
;
     		assume cs:pgroup, ds:dgroup 


_pre_	proc	near
	nop				; for Se.map label.
_pre_	endp
;

;
setpres	proc		near
	push	bp
	push	ds
	mov	bp,sp
;
	mov	si,[bp+6]		; get preset number from stack.
	call	getaddr			; Calculate address 
;
	mov	es,bx			; to   = Addr:0000  ==> es:di
	mov	di,0
	lea	si,mode			; From =  ds:mode   ==> ds:si
	jmp	prmove
;
;
;
getpres	proc		near
	push	bp
	push	ds
	mov	bp,sp
;
	mov	si,[bp+6]		; Get preset number from stack.
	call	getaddr			; Calculate address
;
	push	ds
	pop	es			; From = Addr:0000 ==>ds:si
	mov	si,0
	lea	di,mode			; to   = ds:mode   ==>es:di
	mov	ds,bx
;
prmove:				     	; Move 8196 bytes. (256 X 32)
	mov	cx,2000h
	cld    				; Move upward.
	rep	movsb			; Do the move.
;
;
	mov	sp,bp			; Clean up and go home.
	pop	ds
	mov	bp,ds
	mov	es,bp			; make sure ES = DS
	pop	bp
	ret

getpres	endp
setpres	endp

getaddr	proc	near			; Multiply by 2000h = 
	mov	cl,9			; Multiply by 2^9   =
	shl	si,cl			; Shift Left by 9
	mov	bx,[toppre]		; bx= A000h = top of first preset.
	sub	bx,si			; subtract : bx=start of nth preset.
	ret
getaddr	endp

;
;
;
;
;
;
;
prog	ends
	end

