	page ,132
TITLE	PCSIO2 - PC Serial I/O  (version 2- For LAN's)
;----------------------------------------------------------------------
; Program : PCSIO2
;
; Purpose : Provides access to COM1: and COM2: in an
;	    interrupt driven buffered (input) environment
;
; Notes   : Baud rate, parity, etc must be set by
;	    the application program prior to use
;
; Notes   : This program is an abridgement of PORT.ASM which allowed
;	    both buffered input & output.  This forces direct output.
;
;----------------------------------------------------------------------

;* * * * * * * * * * * * * * * * * * * * * * * * * *
;	      Equates and Definitions
;* * * * * * * * * * * * * * * * * * * * * * * * * *

; Define number of supported lines

NLINES		equ		2		; This number must agree with NLINES in port.h

; Declare interrupt vector numbers

ln_vect_00	equ		0CH
ln_vect_01	equ		0BH

; Define size of input buffer

IBUFSIZ 	equ		4096		;

; INS8250 ACE registers

THR		equ		0F8H		; Transmit holding register
RBR		equ		0F8H		; Receive buffer register
IER		equ		0F9H		; Interrupt enable register
IIR		equ		0FAH		; Interrupt ID register
LCR		equ		0FBH		; Line control register
						; Bit 7 of LCR is "DLAB". DLAB must
						; be 0 to access THR, RBR, IER.
						;
MCR		equ		0FCH		; Modem control register
LSR		equ		0FDH		; Line status register
MSR		equ		0FEH		; Modem status register

;  line table status bits

LOCKBIT 	equ		1
OVERFLOW	equ		2
ACTIVE		equ		4

; outnz error codes

BADLIN		equ		8000H		; bad line number
QFULL		equ		8001H		; output queue is full
NODSR		equ		8002H		; no Data Set Ready
NOCTS		equ		8003H		; no Clear To Send
BADUART 	equ		8004H		; uart tx buffer won't empty

; the line table structure
; Note: all locations are defined as word
;	to allow the Lattice compiler access them
;

LINETABLE	STRUC
isr		dw		0		; pointer to interrupt service routine
vector		dw		0		; 8259 interrupt vector
bitmsk		dw		0		; bit mask for 8259
prtbase 	dw		0		; base address of 8250 ACE
rbufadd 	dw		0		; base address of input buffer
rbuftail	dw		0		; tail pointer to input queue
rbufhead	dw		0		; head pointer to input queue
oldisr		dw		0		; address of old ISR
oldseg		dw		0		; segment of old ISR
status		dw		0		; line status word
LINETABLE	ENDS


;* * * * * * * * * * * * * * * * * * * * * * * * * *
; Data and Stack segments
;* * * * * * * * * * * * * * * * * * * * * * * * * *

;.MODEL small
;.data
DGROUP 	group		DATA
DATA		segment 	word public 'DATA'

		public		lnlist,__pcsio,ipbuf_0,ipbuf_1,eoicnt

; define line buffers

__PCSIO 	db		0

ipbuf_0 	db		IBUFSIZ dup (0) ; line 0
ipbuf_1 	db		IBUFSIZ dup (0) ; line 1

ln_tbl_00	LINETABLE	<ln_isr_00, ln_vect_00, NOT 16,03f8H,ipbuf_0,ipbuf_0,ipbuf_0, 0,0,0>
ln_tbl_01	LINETABLE	<ln_isr_01, ln_vect_01, NOT  8,02f8H,ipbuf_1,ipbuf_1,ipbuf_1, 0,0,0>

lnlist		dw		ln_tbl_00
		dw		ln_tbl_01
		dw		0			; end of list !
eoicnt		dw		0

thisprt 	dw		0

; IIR interrupt vector pointer list

vectlist	dw		spurious		; modem status change
		dw		spurious		; transmit hold reg empty
		dw		invect			; receiver data availble
		dw		spurious		; receiver line status

data		ends

;* * * * * * * * * * * * * * * * * * * * * * * * * *
; Program mainline
;* * * * * * * * * * * * * * * * * * * * * * * * * *

		public		_pcsio2_

		public		outnz
		public		icm
		public		in
		public		rclose
		public		rts0
		public		nrts0
		public		rcvi
		public		iclr0
		public		iclr1
		public		rts1


;.CODE
PGROUP		group		PROG
PROG		segment 	byte public 'PROG'
		assume cs:pgroup, ds:dgroup

_pcsio2_	proc	near
_pcsio2_	endp
;

;* * * * * * * * * * * * * * * * * * * * * * * * * *
; Primary blocks
;* * * * * * * * * * * * * * * * * * * * * * * * * *

;  ICM
; program initialization
; -- set up RS232
; -- set up interrupt vector
; -- enable IRQ4
; -- enable RS232 interrupt on data ready
;
;

icm		proc		near

		mov		si,offset lnlist	; address of line table pointers
icm_01:
		mov		bx,[si] 		; get line table pointer
		cmp		bx,0			; NULL pointer = end of list
		je		icm_02


; set up int x'0c' for IRQ4

		cli				; interrupts off during init
		push		ds
		push		bx		; save the bx (it's about to get zapped)
		mov		ax,[bx].vector	; interrupt vector for this line
		mov		ah,35h		; get current ISR into ES:BX
		int		21h
;
		mov		dx,bx
		pop		bx		; get the index back.
		mov		[bx].oldisr,dx
		mov		[bx].oldseg,es	; save address of old ISR
;
		mov		dx,[bx].ISR	; point to RS232 ISR in DS:DX
		push		cs
		pop		ds
		mov		ah,25H		; set interrupt function
		int		21H		; dos call
		pop		ds

; enable IRQ4 on 8259 interrupt controller

		in		al,21H		; get current masks
		and		ax,[bx].bitmsk	; reset IRQ mask
		out		21H,al		;   and restore to IMR

; enable 8250 data ready and tx interrupts

		mov		dx,[bx].prtbase
		mov		dl,LCR		; dx ==> LCR
		in		al,dx
		and		al,7FH		; reset DLAB for IER access
		out		dx,al
		mov		dl,IER		; address IER
		mov		al,1		; enable data ready and tx interrupt
		out		dx,al

; enable OUT2 on 8250

		mov		dl,MCR		; address MCR
		mov		al,8		; enable OUT2
		out		dx,al
		sti				; turn interrupts back on

		mov		ax,data	; establish data segment address
		mov		ds,ax
		add		si,2		; point to next line table
;						; Clear IIR port.
icm_03:
		mov		dl,IIR
		in		al,dx		; Get interrupt
		and		ax,7
		test		al,1		; If no more, we're done here.
		jnz		icm_01
;
		mov		dl,RBR
		in		al,dx		; Read from port.
		jmp		icm_03
;
icm_02:

		ret
icm		endp

; buffer check
;
; This block checks the received data buffer.
; It functions as follows
;
; if the RS232 buffer is not empty
;	 get the first character
;	 update the buffer pointer
;
;	status set to	   0   =   good read
;			   1   =   Buffer over flow
;			   2   =   Buffer empty
;
; Entry : port number & address of status word on stack
;
; Exit	: AX,BX,DX destroyed
;
in		proc		near

		push		bp
		mov		bp,sp
		mov		di,[bp+6]	; get address of status
		mov		si,[bp+4]	; get port number
		shl		si,1		;  make word index
		mov		bx,[si+lnlist]	;   get line table address

; check buffer status

		mov		si,[bx].rbufhead	; buffer head pointer
		cmp		si,[bx].rbuftail	;  compare with tail pointer
		mov		word ptr [di], 2			;   return if no char
		je		BC100			;    branch if empty

; something in buffer - get first character and fix pointers

		mov		al,[si] 	; get the character
		call		Iincptr 	; bump the head pointer
		mov		[bx].rbufhead,si
		xor		ah,ah
		mov		word ptr [di], 0		; status = good read.
;
		test		[bx].status,OVERFLOW
		jz		bc100
		mov		word ptr [di], 1		; status = data lost
		and		[bx].status, not OVERFLOW ; Remove bit.
;
BC100:
		mov		sp,bp
		pop		bp
		ret
in		endp

		comment *

    This is the RS232 interrupt service routine. It is entered
whenever the RS232 port interrupts on a 'data ready'condition.
This routine simply reads the data from the async chip and places
it in the data queue. Note that the process of reading the received
data register clears IRQ4. However the 8259 must be told specifically
that the interrupt service is complete.

 This routine replaces the function 2 of the BIOS interrupt 14H.
The error flag is reported during an IN call (status =1) which then
resets the flag.

 The ISR will set the overflow flag if the buffer should overflow.

			*
		public		ln_isr_00, ln_isr_01
int_serv	proc		near

ln_isr_00:					; line 0 interupt
		push		ax
		mov		ax,offset ln_tbl_00
		jmp	  short	COM_ISR

ln_isr_01:					; line 1 interrupt
		push		ax
		mov		ax,offset ln_tbl_01
;		jmp	  short	COM_ISR

COM_ISR:
		sti				; allow other interrupts
		push		bx
		push		dx
		push		si
		push		di
		push		ds

; establish pointers

		mov		bx,ax		; get line table address
		mov		ax,data
		mov		ds,ax
		mov		dx,[bx].prtbase	; get port base address
		mov		dl,IIR
		in		al,dx		; get interrupt ID
		and		ax,7
		test		al,1
		jnz		spurious
		mov		si,ax
		mov		si,[si+vectlist]
		jmp		si

spurious:
		jmp	short	ISR999


invect:
		mov		dl,LSR		; low base address of RS232
		in		al,dx		; get status
		and		al,00011110b	; mask off non error bits
		jz		ISR010		; skip error set if ok
;		mov		brcv_err,al	; set error bits

; get inboud char and buffer it
ISR010:

		mov		dl,RBR 			; receive data address
		in		al,dx			; get input character
		test		[bx].status,LOCKBIT	; is buffer input locked?
		jnz		ISR999
		mov		si,[bx].rbuftail	; buffer input pointer
		mov		di,si			; save old pointer
		call		Iincptr	    		; bump the input pointer
		cmp		si,[bx].rbufhead	; compare it to the head pointer
		je		ISR020	 		; OVERFLOW????
		mov		[di],al 		; no, put character on queue
		mov		[bx].rbuftail,si	;  and save new tail pointer
		jmp	short	ISR999
ISR020:
		or		[bx].status,OVERFLOW	; set overflow flag
		jmp	short	ISR999

EOI:
		inc		eoicnt
		and		[bx].status,not ACTIVE
		mov		dx,[bx].prtbase
		mov		dl,MCR
		in		al,dx
		and		al,0FDH		; turn off RTS
		out		dx,al
		

; signal end of interupt to 8259
ISR999:
		cli
		mov		al,20H		; non specific EOI
		out		20H,al		; send it to the 8259

; restore registers and return........ IRET restores interrupt enable

		pop		ds
		pop		di
		pop		si
		pop		dx
		pop		bx
		pop		ax
		iret
int_serv	endp

;
;-------------------------------------------------------
; end of program housekeeping
;
; -- disable IRQ4
; -- disable 8250 interrupts
; -- disable OUT2
; -- display offline message
;
;--------------------------------------------------------

rclose	proc		near

		mov		si,offset lnlist	; get address of line table list
		mov		bx,[si]

; disable interrupt on 8259

clean_01:	mov		cx,[bx].bitmsk		; get vector bit mask
		not		cx			; inverted for turnoff
		cli
		in		al,21H		; IMR
		or		al,cl		; set bit
		out		21H,al

; disable 8250 data ready interrupt

		mov		dx,[bx].prtbase
		mov		dl,LCR		; dx ==> LCR
		in		al,dx
		and		al,01111111B	; reset DLAB for IER access
		out		dx,al
		mov		dl,IER		; address IER
		mov		al,0
		out		dx,al		; disable all 8250 interrupts

; disable OUT2 on 8250

		mov		dl,MCR		; address MCR
;		mov		al,0
		out		dx,al		; disable OUT2
;
;  Restore old interrupt
;
		push		ds
		mov		dx,[bx].oldisr
		mov		ax,[bx].vector
		mov		ds,[bx].oldseg
		mov		ah,25h
		int		21h
		pop		ds
;
;
		add		si,2
		mov		bx,[si]
		or		bx,bx
		jnz		clean_01
;
		sti				; restore other interrupts
		ret

rclose	endp

;* * * * * * * * * * * * * * * * * * * * * * * * * *
; I/O and general subroutines
;* * * * * * * * * * * * * * * * * * * * * * * * * *


outnz		proc		near

		push		bp
		mov		bp,sp

; get line number and character to print

		mov		ax,BADLIN
		mov		si,[bp+4]	; line number
		cmp		si,0		;   must >= 0
		jl		RSXIT

		cmp		si,2		;    and < 2
		jge		RSXIT

		shl		si,1		;  make word index
		mov		bx,[si+lnlist]	;   get line table address

; test and set the output active flag
;  INTERUPTS OFF
		cli
		test		[bx].status,ACTIVE	; is output active
		pushf
		or		[bx].status,ACTIVE
		popf
		sti
		mov		ax,0
		jnz		RS050

; set up RS232, output was not buzy, so start it

		mov		dx,[bx].prtbase ; get port base address
		mov		thisprt,dx
		mov		dl,MCR		; modem control register
		mov		al,00001011B
		out		dx,al		; OUT2, DTR, RTS
; wait for DSR
RS050:
		sub		cx,cx
		mov		dx,thisprt
		mov		dl,MSR		; modem status register
RS100:
		in		al,dx
		test		al,20H		; data set ready ?
		jnz		RS150		; yes
		loop		RS100		; no, retry till time out
		mov		ax,NODSR
		jmp	short	RSXIT		;   and quit
RS150:
		mov		cx,4000h
RS200:
		in		al,dx
		test		al,10H		; clear to send ?
		jnz		RS250		; yes
		loop		RS200		; no, retry till time out
		mov		ax,NOCTS
;		jmp	short	RSXIT		;   and quit

RS250:
		mov		dx,thisprt
		mov		dl,LSR		; line status register
		sub		cx,cx
RS300:
		in		al,dx		; get LSR status
		test		al,20H		; transmit holding register empty?
		jnz		RS350		; yes
		loop		RS300		; no, loop until time out
		mov		ax,BADUART	; timeout flag set
		jmp	short	RSXIT

; get line status and send character

RS350:
		mov		ax,[bp+6]	; character to send
		mov		dx,thisprt
		mov		dl,THR		; transmit holding register
		out		dx,al		; out the port it goes!
		mov		ax,0		; Everythings fine with the output !!!!

RSXIT:
		mov		sp,bp
		pop		bp
		ret
outnz		endp

incptr		proc		near
Iincptr:
		push		cx
		inc		si
		mov		cx,[bx].rbufadd
		add		cx,IBUFSIZ
		cmp		si,cx
		jne		IP100
		sub		si,IBUFSIZ
;		jmp	short	IP100


IP100:
		pop	cx
		ret
;
;

incptr		endp
;
;
;
;
nrts0		proc	near
		lea		bx,ln_tbl_00

		and		[bx].status,not ACTIVE
		mov		dx,[bx].prtbase
		mov		dl,MCR
		in		al,dx
		and		al,0FDH 	; turn off RTS
		out		dx,al

		ret
nrts0		endp
;
;
;
;

rts0		proc		near
		lea		bx,ln_tbl_00
		or		[bx].status,ACTIVE

		mov		dx,[bx].prtbase ; get port base address
		mov		thisprt,dx
		mov		dl,MCR		; modem control register
		mov		al,00001011B
		out		dx,al		; OUT2, DTR, RTS
		ret
rts0		endp
;
;
;
;
rcvi		proc		near
iclr0:		lea		bx,ln_tbl_00
		mov		si,[bx].rbufadd
		mov		[bx].rbuftail,si
		mov		[bx].rbufhead,si
		and		[bx].status, not OVERFLOW
		ret
rcvi		endp
;
;
iclr1		proc		near
		lea		bx,ln_tbl_01
		mov		si,[bx].rbufadd
		mov		[bx].rbuftail,si
		mov		[bx].rbufhead,si
		and		[bx].status, not OVERFLOW
		ret
iclr1		endp
;
;
;
;
rts1		proc		near
		lea		bx,ln_tbl_01
		or		[bx].status,ACTIVE

		mov		dx,[bx].prtbase ; get port base address
		mov		thisprt,dx
		mov		dl,MCR		; modem control register
		mov		al,00001011B
		out		dx,al		; OUT2, DTR, RTS
		ret
rts1		endp
;
;
;
;

prog		ends
		end
