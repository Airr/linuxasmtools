  [section .data]
;---------------------------------------------------------------------
; the following data statements define a form for user input.
; One screen of data is displayed and filled in by user.
; Routine form_engine is called to process the display_tbl
;
; A form uses one screen.  It is defined by normal ascii
; text with embedded control characters.  The control characters
; are:
;    0 - end of table          5 - blank rest of screen
;    1 - color normal          6 - string, block# follows
;    2 - color field           7 - key, key# follows
;    3 - color active field    8 - process, process# follows
;    4 - color button          9 - end of line (eol)
;
; For more information see equates.
;
; As the user types or moves the mouse to fill out the
; form, all data is stored in the form.  Upon exit the
; form can be scanned by caller to obtain the data.
;
; string data will be stored in the table as entered.
; button selection is indicated by color code.  When selected
; the code "3" will indicate this button was selected.
;
; table codes use negative byte values in secondary
; fields, thus, ascii characters in range of 80h+ are not available
; as text.
;----------------------------------------------------------------------

; ----------  embedded codes --------
norm_color	equ	1	;normal text
field_color	equ	2
selected_color	equ	3	;used for selected buttons
button_color	equ	4	;used for unselected buttons
blank_rows	equ	5	;blank all rows to bottom of screen
string		equ	6	;string block# follows, block1 is -1
key		equ	7	;key# follows, alt keys alt-a is -1
process		equ	8	;process# follows, process1 = -1
eol		equ	9	;end of data for this line.

; ---------- keys ------------------
alt_a		equ	-1	;code for key alt-a
alt_b		equ	-2
alt_c		equ	-3
alt_d		equ	-4
alt_e		equ	-5
alt_f		equ	-6
alt_g		equ	-7
alt_h		equ	-8
alt_i		equ	-9
alt_j		equ	-10
alt_k		equ	-11
alt_l		equ	-12
alt_m		equ	-13
alt_n		equ	-14
alt_o		equ	-15
alt_p		equ	-16
alt_q		equ	-17
alt_r		equ	-18
alt_s		equ	-19
alt_t		equ	-20
alt_u		equ	-21
alt_v		equ	-22
alt_w		equ	-23
alt_x		equ	-24
alt_y		equ	-25
alt_z		equ	-26

; --------- process numbers -------------------
process_string	equ	-1	;string entry, call string handler
process_button	equ	-2	;select button, call single button process
process_buttons	equ	-3	;select only this button on this line, process
null_process	equ	-4	;do nothing
return_0	equ	-5	;exit program and return code 0
return_1	equ	-6	;exit program and return code 1

;--------- string codes to select def block ----
str_blk1	equ	-1	;string descriptive block #
str_blk2	equ	-2
str_blk3	equ	-3
str_blk4	equ	-4
str_blk5	equ	-5
; etc.

;------- order of codes --------
; the codes are optional and can be combined to define
; a button or string.  When combined they must be in the
; following order:
;
;    string code - key code - process code - color - text - eol
;
;------ button example ------------
; db 7,-14	;use alt-n key for this button
; db 8,-2	;this button toggles, does not react with other buttons on line
; db 4		;use button color
; db "button"	;text for button is -button-
; db 1		;switch back to normal color to end button
;
;***********************************************************************
; the form table is next
;***********************************************************************

dummy_process:  ret

 [section .data]

; table pointers ---------

table_pointers:
	dd	dummy_process		;not used
	dd	edit_color		;colors
	dd	string_block1		;strings
	dd	display_tbl		;display table

; ----------- colors ---------------
; colors = aaxxffbb  (aa-attribute ff-foreground  bb-background)
;   30-black 31-red 32-green 33-brown 34-blue 35-purple 36-cyan 37-grey
;   attributes 30-normal 31-bold 34-underscore 37-inverse

edit_color		dd	30003734h	;used for normal text
edit_field_color	dd	31003734h	;modifable/slectable field
edit_entry_color	dd	31003336h	;current selection
edit_button_color	dd	30003436h	;buttons and active edit field
;norm_text_color	dd 30003734h ;used for inactive window
;			     ;grey-foreground=7 blue-backgound=4 0=norm attr
;bold_edit_color	dd 31003734h ;used for active window in edit mode
;			     ;grey-foreground=7 blue-backgound=4 0=bold attr
;bold_cmd_color	dd 31003334h ;used for active window in command mode
;			     ;grey-foreground=7 blue-backgound=4 0=bold attr
;high_text_color	dd 31003634h ;used for highlighting block
;			     ;grey-foreground=7 blue-backgound=4 0=inver attr
;asm_text_color	dd 31003234h ;used to highlight comments ";"
;			     ;cyan-foreground=6 blue-backgound=4 0=norm attr
;status_color	dd 30003036h ;used for status line
;status_color1	dd 31003336h ;used for special data on status line
;status_color2	dd 31003331h ;used for error messags or macro record
;exit_screen_color dd 31003334h ;used for error messags on status line

;---------------------------------------------------------------------
; Each string entry field needs a descriptor block.  The codes 6,-1
; specify block #1
;
string_block1:
buf4_ptr	dd	buf4	;buffer ptr
field_len4	dd	buf4_len			;allocate extra byte in form for handlers
color_ptr4	dd	edit_entry_color
blk4_row	db	17
blk4_column	db	16
blk4_flag	db	0
blk4_cursor	db	16

string_block2:
buf5_ptr	dd	buf5	;buffer ptr
field_len5	dd	buf5_len			;allocate extra byte in form for handlers
color_ptr5	dd	edit_entry_color
blk5_row	db	19
blk5_column	db	16
blk5_flag	db	0
blk5_cursor	db	16

string_block3:
buf1_ptr	dd	buf06			;buffer ptr (inside form)
field_len1	dd	buf06_len			;field len, allocate 21 in form
color_ptr1	dd	edit_entry_color
blk1_row	db	24			;string display row
blk1_column	db	15			;starting column for string
blk1_flag	db	0			;
blk1_cursor	db	15			;current cursor position within string

string_block4:
buf2_ptr	dd	buf08	;buffer ptr
field_len2	dd	buf08_len			;allocate extra byte in form for handlers
color_ptr2	dd	edit_entry_color
blk2_row	db	26
blk2_column	db	15
blk2_flag	db	0
blk2_cursor	db	15

string_block5:
buf3_ptr	dd	buf3	;buffer ptr
field_len3	dd	buf3_len			;allocate extra byte in form for handlers
color_ptr3	dd	edit_entry_color
blk3_row	db	28
blk3_column	db	15
blk3_flag	db	0
blk3_cursor	db	15

		times 16 db 0	;block 3
;		times 16 db 0	;4
;		times 16 db 0	;5
;		times 16 db 0	;6
;		times 16 db 0	;7
;		times 16 db 0	;8
;		times 16 db 0	;9
;		times 16 db 0	;10
;		times 16 db 0	;11
;		times 16 db 0	;12
;		times 16 db 0	;13
;		times 16 db 0	;14
;		times 16 db 0	;15
;		times 16 db 0	;16
;		times 16 db 0	;17
;		times 16 db 0	;18
;		times 16 db 0	;block 19
;		times 16 db 0	;20

;----------------------------------------------------------------
; The screen form follows.
	   db process,null_process,eol	;null process definition to trap errant mice
display_tbl:

dt_line01: db norm_color
	   db ' ** AsmEdit Setup Menu ** ',eol

dt_line02: db norm_color,eol

dt_line03: db 'Asmedit can start with:',eol

dt_line04: db key,alt_i,process,process_buttons
dt_b1:	   db button_color,'IDE menu',norm_color,'(alt-i)  '
           db key,alt_m,process,process_buttons
dt_b2:     db button_color,'mouse menu',norm_color,'(alt-m)  '
           db key,alt_k,process,process_buttons
dt_b3:     db button_color,'key status line',norm_color,'(alt-k)  '
           db eol

dt_line05: db norm_color,eol		;blank line

dt_line06: db 'start editor in '
 	   db key,alt_c,process,process_buttons
dt_bc1	   db button_color,'CMD '
 	   db norm_color,'(alt-c) or '
 	   db key,alt_e,process,process_buttons
dt_bc2	   db button_color,'EDIT'
 	   db norm_color,'(alt-e)  mode',eol

           db eol			;blank line


dt_line08: db 'default edit mode is '
	   db key,alt_y,process,process_buttons
dt_bc3	   db button_color,'insert'
	   db norm_color,'(alt-y) or '
	   db key,alt_x,process,process_buttons
dt_bc4	   db button_color,'overtype'
	   db norm_color,'(alt-y)',eol

dt_line09: db eol			;blank line


dt_line10: db 'Ignore case when searching '
	   db key,alt_y,process,process_buttons
dt_yc3	   db button_color,'yes'
	   db norm_color,'(alt-y) or '
	   db key,alt_n,process,process_buttons
dt_yc4	   db button_color,'no'
	   db norm_color,'(alt-n)',eol

           db eol

dt_line12: db 'Keep backup of edit files '
	   db key,alt_f,process,process_buttons
dt_yc5	   db button_color,'yes'
	   db norm_color,'(alt-f) or '
	   db key,alt_g,process,process_buttons
dt_yc6	   db button_color,'no'
	   db norm_color,'(alt-g)',eol

           db eol,eol

dt_line15: db norm_color
	   db 'Default margins for paragaph function: ',eol

           db eol

dt_line17: db norm_color,'left margin    '
	   db string,str_blk1,key,alt_a,process,process_string,button_color
buf4: 	   db '  '	;20 byte string, allocate 21 spaces
buf4_len  equ	$ - buf4
	   db norm_color,'(alt-a)',eol

           db eol

dt_line19: db norm_color,'right margin   '
	   db string,str_blk2,key,alt_b,process,process_string,button_color
buf5: 	   db '  '	;20 byte string, allocate 21 spaces
buf5_len  equ	$ - buf5
	   db norm_color,'(alt-b)',eol

           db eol,eol

dt_line22  db 'Right mouse clicks will look at data under cursor and execute:',eol

           db eol

dt_line24: db norm_color,'email program '
	   db string,str_blk3,key,alt_h,process,process_string,button_color
buf06: 	   db 'string1 preload text '	;20 byte string, allocate 21 spaces
buf06_len  equ	$ - buf06
	   db norm_color,'(alt-h)',eol

dt_line25: db eol

dt_line26: db norm_color,'browser       '
	   db string,str_blk4,key,alt_j,process,process_string,button_color
buf08:	   db 'string2 preload text '	;20 byte string, allocate 21 spaces
buf08_len  equ $ - buf08
	   db norm_color,'(alt-j)',eol

           db eol

dt_line28: db norm_color,'other         '
	   db string,str_blk5,key,alt_l,process,process_string,button_color
buf3:	   db 'string2 preload text '	;20 byte string, allocate 21 spaces
buf3_len  equ $ - buf3
	   db norm_color,'(alt-l)',eol

dt_linezz: db eol			;blank line


dt_line30: db 'Confirm file save/abandon at exit '
	   db key,alt_y,process,process_buttons
dt_yes	   db button_color,'yes'
	   db norm_color,'(alt-v) or '
	   db key,alt_n,process,process_buttons
dt_no	   db button_color,'no'
	   db norm_color,'(alt-w)',eol

dt_end	   db eol
	   db 'Press ESC to exit, AsmEdit can be started by pressing <a>',eol
		db	blank_rows	;clear bottom rows
		db	0		;end of table


