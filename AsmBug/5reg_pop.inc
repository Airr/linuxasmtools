  extern window_write_lines
  extern trace_fxregsget
  extern trace_fxregsset
  extern buffer_hex_line
;%include "buffer_hex_line.inc"
  extern ftword_to_ascii

struc fxsr
.cwd	resw 1;
.swd	resw 1;
.twd	resw 1;
.fop	resw 1;
.fip	resd 1;
.fcs	resd 1;
.foo	resd 1;
.fos	resd 1;
.mxcsr	resd 1;
.reserved resw 2 ;
;.fpregs	resd 32  ; 8*16 bytes for each FP-reg = 128 bytes
.fd0	resb 16
.fd1	resb 16
.fd2	resb 16
.fd3	resb 16
.fd4	resb 16
.fd5	resb 16
.fd6	resb 16
.fd7	resb 16
;.fxregs  resd 32  ; 8*16 bytes for each XMM-reg = 128 bytes
.xmm0	resb 16
.xmm1	resb 16
.xmm2	resb 16
.xmm3	resb 16
.xmm4	resb 16
.xmm5	resb 16
.xmm6	resb 16
.xmm7	resb 16
.padding resd 56;
endstruc

;------------------------------------------------------
;fpregs pop up
key_f1:
  mov	esi,reg_buf
  call	trace_fxregsget
;fill in control word
  mov	ax,[reg_buf+fxsr.cwd]	;get control word
  mov	edi,rpt_stuff1
  call	wordto_hexascii
;fill in status word
  mov	ax,[reg_buf+fxsr.swd]	;get control word
  mov	edi,rpt_stuff2
  call	wordto_hexascii
;fill in tag word
  mov	ax,[reg_buf+fxsr.twd]	;get control word
  mov	edi,rpt_stuff3
  call	wordto_hexascii
;fill in reg fd0
  lea	eax,[reg_buf+fxsr.fd0]
  mov	edi,rpt_stuff4
  call	stuff_reg
;fill in reg fd1
  lea	eax,[reg_buf+fxsr.fd1]
  mov	edi,rpt_stuff5
  call	stuff_reg
;fill in reg fd2
  lea	eax,[reg_buf+fxsr.fd2]
  mov	edi,rpt_stuff6
  call	stuff_reg
;fill in reg fd3
  lea	eax,[reg_buf+fxsr.fd3]
  mov	edi,rpt_stuff7
  call	stuff_reg
;fill in reg fd4
  lea	eax,[reg_buf+fxsr.fd4]
  mov	edi,rpt_stuff8
  call	stuff_reg
;fill in reg fd5
  lea	eax,[reg_buf+fxsr.fd5]
  mov	edi,rpt_stuff9
  call	stuff_reg
;fill in reg fd6
  lea	eax,[reg_buf+fxsr.fd6]
  mov	edi,rpt_stuffa
  call	stuff_reg
;fill in reg fd7
  lea	eax,[reg_buf+fxsr.fd7]
  mov	edi,rpt_stuffb
  call	stuff_reg

;fill in xmm0 reg
  lea	esi,[reg_buf+fxsr.xmm0]
  mov	edi,xmm_stuff4
  call	do_hex

;fill in xmm1 reg
  lea	esi,[reg_buf+fxsr.xmm1]
  mov	edi,xmm_stuff5
  call	do_hex

;fill in xmm2 reg
  lea	esi,[reg_buf+fxsr.xmm2]
  mov	edi,xmm_stuff6
  call	do_hex

;fill in xmm3 reg
  lea	esi,[reg_buf+fxsr.xmm3]
  mov	edi,xmm_stuff7
  call	do_hex

;fill in xmm4 reg
  lea	esi,[reg_buf+fxsr.xmm4]
  mov	edi,xmm_stuff8
  call	do_hex

;fill in xmm5 reg
  lea	esi,[reg_buf+fxsr.xmm5]
  mov	edi,xmm_stuff9
  call	do_hex

;fill in xmm6 reg
  lea	esi,[reg_buf+fxsr.xmm6]
  mov	edi,xmm_stuffa
  call	do_hex

;fill in xmm7 reg
  lea	esi,[reg_buf+fxsr.xmm7]
  mov	edi,xmm_stuffb
  call	do_hex

;set box color
  mov	ebx,[popButFColor]
  mov	ecx,[popBColor]
  call	window_id_color
;clear box
   mov	eax,09	;left column
   mov	ebx,0	;top row
   mov	ecx,50  ;  ecx = width (chars)
   mov	edx,22  ;  edx = height (chars)
   mov	esi,1   ;  esi = flag 1=current color
  call	window_clear_area
;show text
  mov	edx,reg_txt_blk
  call	window_write_lines
  call	x_wait_event
  mov	[win_bit_map],byte 1fh
;restore registers to origional state
  mov	esi,reg_buf
  call	trace_fxregsset
  ret
;-----------------------------------------------
; input: eax = ptr to tword reg
;        edi = stuff point
; output: edi = ptr to end of line char 
;
stuff_reg:
  xor	ecx,ecx
  cmp	[eax+8],ecx	;reg present?
  jz	sr_pad		;jmp if no register
  mov	ebx,7		;precision
  call	ftword_to_ascii
sr_pad:
  mov	al,' '		;space char
sr_loop:
  stosb			;blank this char
  cmp	byte [edi],0ah	;end of line
  jne	sr_loop		;loop till done
  ret

;-----------------------------------------------  

do_hex:
  mov	ecx,16
  mov	dl,0		;skip ascii append
  call	buffer_hex_line
  dec	edi
  mov	al,' '
  stosb
  ret

;-----------------
  [section .data]

reg_buf: times 512 db 0

reg_txt_blk:
  dd 21		;number of rows
  dd 50		;number of columns
  dd 1		;starting row
  dd 10		;starting column
  dd reg_pop_txt

reg_pop_txt:
 db 'fctrl='
rpt_stuff1: db '                          ',0ah

 db 'fstat='
rpt_stuff2: db '                          ',0ah

 db 'tags='
rpt_stuff3: db '                          ',0ah

 db 'st0='
rpt_stuff4: db '                               ',0ah

 db 'st1='
rpt_stuff5: db '                               ',0ah

 db 'st2='
rpt_stuff6: db '                               ',0ah

 db 'st3='
rpt_stuff7: db '                               ',0ah

 db 'st4='
rpt_stuff8: db '                               ',0ah

 db 'st5='
rpt_stuff9: db '                               ',0ah

 db 'st6='
rpt_stuffa: db '                               ',0ah

 db 'st7='
rpt_stuffb: db '                               ',0ah

 db 'xmm0='
xmm_stuff4: db '                                 ',0ah

 db 'xmm1='
xmm_stuff5: db '                                 ',0ah

 db 'xmm2='
xmm_stuff6: db '                                 ',0ah

 db 'xmm3='
xmm_stuff7: db '                                 ',0ah

 db 'xmm4='
xmm_stuff8: db '                                 ',0ah

 db 'xmm5='
xmm_stuff9: db '                                 ',0ah

 db 'xmm6='
xmm_stuffa: db '                                 ',0ah

 db 'xmm7='
xmm_stuffb: db '                                 ',0ah
 
 db 0ah
 db ' ** any key to exit **',0ah
 db 0 ;end of message

  [section .text]
;---------------------------------------------------------

