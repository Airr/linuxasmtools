;--------------------------------------------------------------
;>1 syscall
; sys_readdir - kernel function                             
;
;    INPUTS 
;     see AsmRef function -> sys_readdir                                         
;
;    Note: functon call consists of four instructions
;          
;          sys_readdir:                                      
;              mov  eax,89     
;              int  byte 80h
;              or   eax,eax
;              ret
;<;
;------------------------------------------------------------------
  [section .text align=1]

  global sys_readdir
sys_readdir:
	mov	eax,89
	int	byte 80h
	or	eax,eax
	ret