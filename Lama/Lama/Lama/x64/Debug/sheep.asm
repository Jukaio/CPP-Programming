; Listing generated by Microsoft (R) Optimizing Compiler Version 19.27.29111.0 

include listing.inc

INCLUDELIB MSVCRTD
INCLUDELIB OLDNAMES

msvcjmc	SEGMENT
__367CC694_corecrt_memcpy_s@h DB 01H
__DC9673E3_corecrt_wstring@h DB 01H
__A29A7DFB_string@h DB 01H
__F7309570_SDL_stdinc@h DB 01H
__DEF1ED59_SDL_endian@h DB 01H
__D291391D_SDL_rect@h DB 01H
__09340588_corecrt_math@h DB 01H
__2C0C7D8D_sheep@c DB 01H
msvcjmc	ENDS
PUBLIC	sheep_flock_init
PUBLIC	sheep_flock_spawn_sheep
PUBLIC	sheep_flock_update
PUBLIC	sheep_flock_draw
PUBLIC	sheep_flock_clean
PUBLIC	sheep_move
PUBLIC	__JustMyCode_Default
PUBLIC	__real@3f000000
EXTRN	__imp_calloc:PROC
EXTRN	__imp_free:PROC
EXTRN	__imp_rand:PROC
EXTRN	memset:PROC
EXTRN	SDL_RenderCopy:PROC
EXTRN	lerp:PROC
EXTRN	interpolate:PROC
EXTRN	grid_set_data_at:PROC
EXTRN	grid_world_to_grid:PROC
EXTRN	grid_grid_to_world:PROC
EXTRN	grid_open_neighbours_at:PROC
EXTRN	_RTC_CheckStackVars:PROC
EXTRN	_RTC_InitBase:PROC
EXTRN	_RTC_Shutdown:PROC
EXTRN	__CheckForDebuggerJustMyCode:PROC
EXTRN	__GSHandlerCheck:PROC
EXTRN	__security_check_cookie:PROC
EXTRN	__security_cookie:QWORD
EXTRN	_fltused:DWORD
;	COMDAT pdata
pdata	SEGMENT
$pdata$sheep_flock_init DD imagerel $LN9
	DD	imagerel $LN9+414
	DD	imagerel $unwind$sheep_flock_init
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$sheep_flock_spawn_sheep DD imagerel $LN8
	DD	imagerel $LN8+363
	DD	imagerel $unwind$sheep_flock_spawn_sheep
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$sheep_flock_update DD imagerel $LN13
	DD	imagerel $LN13+643
	DD	imagerel $unwind$sheep_flock_update
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$sheep_flock_draw DD imagerel $LN7
	DD	imagerel $LN7+356
	DD	imagerel $unwind$sheep_flock_draw
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$sheep_flock_clean DD imagerel $LN4
	DD	imagerel $LN4+110
	DD	imagerel $unwind$sheep_flock_clean
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$sheep_move DD imagerel $LN3
	DD	imagerel $LN3+209
	DD	imagerel $unwind$sheep_move
pdata	ENDS
;	COMDAT __real@3f000000
CONST	SEGMENT
__real@3f000000 DD 03f000000r			; 0.5
CONST	ENDS
;	COMDAT rtc$TMZ
rtc$TMZ	SEGMENT
_RTC_Shutdown.rtc$TMZ DQ FLAT:_RTC_Shutdown
rtc$TMZ	ENDS
;	COMDAT rtc$IMZ
rtc$IMZ	SEGMENT
_RTC_InitBase.rtc$IMZ DQ FLAT:_RTC_InitBase
rtc$IMZ	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$sheep_move DD 025052a01H
	DD	010e2313H
	DD	070070021H
	DD	05006H
xdata	ENDS
;	COMDAT CONST
CONST	SEGMENT
sheep_move$rtcName$0 DB 06eH
	DB	065H
	DB	078H
	DB	074H
	DB	00H
	ORG $+11
sheep_move$rtcVarDesc DD 028H
	DD	08H
	DQ	FLAT:sheep_move$rtcName$0
	ORG $+48
sheep_move$rtcFrameData DD 01H
	DD	00H
	DQ	FLAT:sheep_move$rtcVarDesc
CONST	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$sheep_flock_clean DD 025052a01H
	DD	010e2313H
	DD	07007001dH
	DD	05006H
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$sheep_flock_draw DD 025064119H
	DD	01142319H
	DD	0700d002aH
	DD	0500b600cH
	DD	imagerel __GSHandlerCheck
	DD	0148H
xdata	ENDS
;	COMDAT CONST
CONST	SEGMENT
sheep_flock_draw$rtcName$0 DB 06fH
	DB	066H
	DB	066H
	DB	073H
	DB	065H
	DB	074H
	DB	05fH
	DB	064H
	DB	065H
	DB	073H
	DB	074H
	DB	069H
	DB	06eH
	DB	061H
	DB	074H
	DB	069H
	DB	06fH
	DB	06eH
	DB	00H
	ORG $+13
sheep_flock_draw$rtcVarDesc DD 068H
	DD	010H
	DQ	FLAT:sheep_flock_draw$rtcName$0
	ORG $+48
sheep_flock_draw$rtcFrameData DD 01H
	DD	00H
	DQ	FLAT:sheep_flock_draw$rtcVarDesc
CONST	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$sheep_flock_update DD 035053b19H
	DD	010e3313H
	DD	07007003fH
	DD	05006H
	DD	imagerel __GSHandlerCheck
	DD	01e8H
xdata	ENDS
;	COMDAT CONST
CONST	SEGMENT
sheep_flock_update$rtcName$0 DB 070H
	DB	06fH
	DB	073H
	DB	069H
	DB	074H
	DB	069H
	DB	06fH
	DB	06eH
	DB	00H
	ORG $+7
sheep_flock_update$rtcName$1 DB 067H
	DB	072H
	DB	069H
	DB	064H
	DB	05fH
	DB	069H
	DB	06eH
	DB	064H
	DB	065H
	DB	078H
	DB	00H
	ORG $+5
sheep_flock_update$rtcName$2 DB 06fH
	DB	070H
	DB	065H
	DB	06eH
	DB	05fH
	DB	069H
	DB	06eH
	DB	064H
	DB	065H
	DB	063H
	DB	065H
	DB	073H
	DB	00H
	ORG $+3
sheep_flock_update$rtcName$3 DB 072H
	DB	061H
	DB	06eH
	DB	064H
	DB	06fH
	DB	06dH
	DB	05fH
	DB	069H
	DB	06eH
	DB	064H
	DB	065H
	DB	078H
	DB	00H
	ORG $+3
sheep_flock_update$rtcName$4 DB 06fH
	DB	070H
	DB	065H
	DB	06eH
	DB	05fH
	DB	063H
	DB	06fH
	DB	075H
	DB	06eH
	DB	074H
	DB	00H
	ORG $+5
sheep_flock_update$rtcVarDesc DD 0114H
	DD	04H
	DQ	FLAT:sheep_flock_update$rtcName$4
	DD	0f8H
	DD	08H
	DQ	FLAT:sheep_flock_update$rtcName$3
	DD	0b8H
	DD	020H
	DQ	FLAT:sheep_flock_update$rtcName$2
	DD	098H
	DD	08H
	DQ	FLAT:sheep_flock_update$rtcName$1
	DD	078H
	DD	08H
	DQ	FLAT:sheep_flock_update$rtcName$0
	ORG $+240
sheep_flock_update$rtcFrameData DD 05H
	DD	00H
	DQ	FLAT:sheep_flock_update$rtcVarDesc
CONST	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$sheep_flock_spawn_sheep DD 025052f01H
	DD	01132318H
	DD	0700c0025H
	DD	0500bH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$sheep_flock_init DD 025063a01H
	DD	011e2323H
	DD	07017002aH
	DD	050156016H
xdata	ENDS
;	COMDAT CONST
CONST	SEGMENT
sheep_flock_init$rtcName$0 DB 064H
	DB	065H
	DB	066H
	DB	061H
	DB	075H
	DB	06cH
	DB	074H
	DB	05fH
	DB	064H
	DB	065H
	DB	073H
	DB	074H
	DB	00H
	ORG $+3
sheep_flock_init$rtcVarDesc DD 068H
	DD	010H
	DQ	FLAT:sheep_flock_init$rtcName$0
	ORG $+48
sheep_flock_init$rtcFrameData DD 01H
	DD	00H
	DQ	FLAT:sheep_flock_init$rtcVarDesc
CONST	ENDS
; Function compile flags: /Odt
;	COMDAT __JustMyCode_Default
_TEXT	SEGMENT
__JustMyCode_Default PROC				; COMDAT
	ret	0
__JustMyCode_Default ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\sheep.c
;	COMDAT sheep_move
_TEXT	SEGMENT
next$ = 8
p_sheep$ = 256
sheep_move PROC						; COMDAT

; 64   : {

$LN3:
	mov	QWORD PTR [rsp+8], rcx
	push	rbp
	push	rdi
	sub	rsp, 264				; 00000108H
	lea	rbp, QWORD PTR [rsp+32]
	mov	rdi, rsp
	mov	ecx, 66					; 00000042H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+296]
	lea	rcx, OFFSET FLAT:__2C0C7D8D_sheep@c
	call	__CheckForDebuggerJustMyCode

; 65   : 
; 66   :     vector2 next = lerp(p_sheep->from,

	mov	rax, QWORD PTR p_sheep$[rbp]
	mov	r8d, DWORD PTR [rax+48]
	mov	rax, QWORD PTR p_sheep$[rbp]
	mov	rdx, QWORD PTR [rax+40]
	mov	rax, QWORD PTR p_sheep$[rbp]
	mov	rcx, QWORD PTR [rax+32]
	call	lerp
	mov	QWORD PTR next$[rbp], rax

; 67   :                         p_sheep->to,
; 68   :                         p_sheep->fraction);
; 69   : 
; 70   :     p_sheep->destination.x = next.x;

	mov	rax, QWORD PTR p_sheep$[rbp]
	mov	ecx, DWORD PTR next$[rbp]
	mov	DWORD PTR [rax+176], ecx

; 71   :     p_sheep->destination.y = next.y;

	mov	rax, QWORD PTR p_sheep$[rbp]
	mov	ecx, DWORD PTR next$[rbp+4]
	mov	DWORD PTR [rax+180], ecx

; 72   :     p_sheep->fraction += FRAME_TIME;

	mov	rax, QWORD PTR p_sheep$[rbp]
	mov	eax, DWORD PTR [rax+48]
	add	eax, 16
	mov	rcx, QWORD PTR p_sheep$[rbp]
	mov	DWORD PTR [rcx+48], eax

; 73   : 
; 74   :     return interpolate(0, 1000, &p_sheep->fraction);

	mov	rax, QWORD PTR p_sheep$[rbp]
	add	rax, 48					; 00000030H
	mov	r8, rax
	mov	edx, 1000				; 000003e8H
	xor	ecx, ecx
	call	interpolate

; 75   : }

	mov	rdi, rax
	lea	rcx, QWORD PTR [rbp-32]
	lea	rdx, OFFSET FLAT:sheep_move$rtcFrameData
	call	_RTC_CheckStackVars
	mov	rax, rdi
	lea	rsp, QWORD PTR [rbp+232]
	pop	rdi
	pop	rbp
	ret	0
sheep_move ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\sheep.c
;	COMDAT sheep_flock_clean
_TEXT	SEGMENT
p_flock$ = 224
sheep_flock_clean PROC					; COMDAT

; 170  : {

$LN4:
	mov	QWORD PTR [rsp+8], rcx
	push	rbp
	push	rdi
	sub	rsp, 232				; 000000e8H
	lea	rbp, QWORD PTR [rsp+32]
	mov	rdi, rsp
	mov	ecx, 58					; 0000003aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+264]
	lea	rcx, OFFSET FLAT:__2C0C7D8D_sheep@c
	call	__CheckForDebuggerJustMyCode

; 171  :     if(p_flock->pool)

	mov	rax, QWORD PTR p_flock$[rbp]
	cmp	QWORD PTR [rax+32], 0
	je	SHORT $LN2@sheep_floc

; 172  :         free(p_flock->pool);

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+32]
	call	QWORD PTR __imp_free
$LN2@sheep_floc:

; 173  :     p_flock->pool = NULL;

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	QWORD PTR [rax+32], 0

; 174  : }

	lea	rsp, QWORD PTR [rbp+200]
	pop	rdi
	pop	rbp
	ret	0
sheep_flock_clean ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\sheep.c
;	COMDAT sheep_flock_draw
_TEXT	SEGMENT
index$ = 4
temp$ = 40
offset_destination$ = 72
__$ArrayPad$ = 296
p_renderer$ = 336
p_flock$ = 344
sheep_flock_draw PROC					; COMDAT

; 149  : {

$LN7:
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	push	rbp
	push	rsi
	push	rdi
	sub	rsp, 336				; 00000150H
	lea	rbp, QWORD PTR [rsp+32]
	mov	rdi, rsp
	mov	ecx, 84					; 00000054H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+376]
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rbp
	mov	QWORD PTR __$ArrayPad$[rbp], rax
	lea	rcx, OFFSET FLAT:__2C0C7D8D_sheep@c
	call	__CheckForDebuggerJustMyCode

; 150  :     int index;
; 151  :     sheep* temp;
; 152  :     SDL_Rect offset_destination;
; 153  : 
; 154  :     for(index = 0; index < p_flock->pool_size; index++)

	mov	DWORD PTR index$[rbp], 0
	jmp	SHORT $LN4@sheep_floc
$LN2@sheep_floc:
	mov	eax, DWORD PTR index$[rbp]
	inc	eax
	mov	DWORD PTR index$[rbp], eax
$LN4@sheep_floc:
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	eax, DWORD PTR [rax+12]
	cmp	DWORD PTR index$[rbp], eax
	jge	$LN3@sheep_floc

; 155  :     {
; 156  :         temp = &p_flock->pool[index];

	movsxd	rax, DWORD PTR index$[rbp]
	imul	rax, rax, 192				; 000000c0H
	mov	rcx, QWORD PTR p_flock$[rbp]
	add	rax, QWORD PTR [rcx+32]
	mov	QWORD PTR temp$[rbp], rax

; 157  :         if(temp->active)

	mov	rax, QWORD PTR temp$[rbp]
	movzx	eax, BYTE PTR [rax]
	test	eax, eax
	je	$LN5@sheep_floc

; 158  :         {
; 159  :             offset_destination = temp->destination;

	lea	rax, QWORD PTR offset_destination$[rbp]
	mov	rcx, QWORD PTR temp$[rbp]
	mov	rdi, rax
	lea	rsi, QWORD PTR [rcx+176]
	mov	ecx, 16
	rep movsb

; 160  :             offset_destination.x -= (int)(offset_destination.w * 0.5f);

	cvtsi2ss xmm0, DWORD PTR offset_destination$[rbp+8]
	mulss	xmm0, DWORD PTR __real@3f000000
	cvttss2si eax, xmm0
	mov	ecx, DWORD PTR offset_destination$[rbp]
	sub	ecx, eax
	mov	eax, ecx
	mov	DWORD PTR offset_destination$[rbp], eax

; 161  :             offset_destination.y -= (int)(offset_destination.h * 0.5f);

	cvtsi2ss xmm0, DWORD PTR offset_destination$[rbp+12]
	mulss	xmm0, DWORD PTR __real@3f000000
	cvttss2si eax, xmm0
	mov	ecx, DWORD PTR offset_destination$[rbp+4]
	sub	ecx, eax
	mov	eax, ecx
	mov	DWORD PTR offset_destination$[rbp+4], eax

; 162  :             SDL_RenderCopy(p_renderer,

	mov	rax, QWORD PTR temp$[rbp]
	movsxd	rax, DWORD PTR [rax+24]
	imul	rax, rax, 40				; 00000028H
	mov	rcx, QWORD PTR temp$[rbp]
	movsxd	rcx, DWORD PTR [rcx+20]
	imul	rcx, rcx, 40				; 00000028H
	mov	rdx, QWORD PTR temp$[rbp]
	mov	rax, QWORD PTR [rdx+rax+80]
	add	rax, rcx
	mov	rcx, QWORD PTR temp$[rbp]
	movsxd	rcx, DWORD PTR [rcx+24]
	imul	rcx, rcx, 40				; 00000028H
	lea	r9, QWORD PTR offset_destination$[rbp]
	mov	r8, rax
	mov	rax, QWORD PTR temp$[rbp]
	mov	rdx, QWORD PTR [rax+rcx+56]
	mov	rcx, QWORD PTR p_renderer$[rbp]
	call	SDL_RenderCopy
$LN5@sheep_floc:

; 163  :                            temp->anim[temp->anim_index].texture,
; 164  :                            &temp->anim[temp->anim_index].frames[temp->age_index].data.rect,
; 165  :                            &offset_destination);
; 166  :         }
; 167  :     }

	jmp	$LN2@sheep_floc
$LN3@sheep_floc:

; 168  : }

	lea	rcx, QWORD PTR [rbp-32]
	lea	rdx, OFFSET FLAT:sheep_flock_draw$rtcFrameData
	call	_RTC_CheckStackVars
	mov	rcx, QWORD PTR __$ArrayPad$[rbp]
	xor	rcx, rbp
	call	__security_check_cookie
	lea	rsp, QWORD PTR [rbp+304]
	pop	rdi
	pop	rsi
	pop	rbp
	ret	0
sheep_flock_draw ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\sheep.c
;	COMDAT sheep_flock_update
_TEXT	SEGMENT
index$ = 4
temp$ = 40
position$ = 72
grid_index$ = 104
open_indeces$ = 136
random_index$ = 200
open_count$ = 228
tv173 = 436
__$ArrayPad$ = 440
p_flock$ = 480
sheep_flock_update PROC					; COMDAT

; 79   : {

$LN13:
	mov	QWORD PTR [rsp+8], rcx
	push	rbp
	push	rdi
	sub	rsp, 504				; 000001f8H
	lea	rbp, QWORD PTR [rsp+48]
	mov	rdi, rsp
	mov	ecx, 126				; 0000007eH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+536]
	mov	rax, QWORD PTR __security_cookie
	xor	rax, rbp
	mov	QWORD PTR __$ArrayPad$[rbp], rax
	lea	rcx, OFFSET FLAT:__2C0C7D8D_sheep@c
	call	__CheckForDebuggerJustMyCode

; 80   :     int index;
; 81   :     sheep* temp;
; 82   :     vector2 position;
; 83   :     vector2 grid_index;
; 84   :     vector2 open_indeces[NUM_DIRS];
; 85   :     vector2 random_index;
; 86   :     int open_count;
; 87   : 
; 88   :     for(index = 0; index < p_flock->pool_size; index++)

	mov	DWORD PTR index$[rbp], 0
	jmp	SHORT $LN4@sheep_floc
$LN2@sheep_floc:
	mov	eax, DWORD PTR index$[rbp]
	inc	eax
	mov	DWORD PTR index$[rbp], eax
$LN4@sheep_floc:
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	eax, DWORD PTR [rax+12]
	cmp	DWORD PTR index$[rbp], eax
	jge	$LN3@sheep_floc

; 89   :     {
; 90   :         temp = &p_flock->pool[index];

	movsxd	rax, DWORD PTR index$[rbp]
	imul	rax, rax, 192				; 000000c0H
	mov	rcx, QWORD PTR p_flock$[rbp]
	add	rax, QWORD PTR [rcx+32]
	mov	QWORD PTR temp$[rbp], rax

; 91   :         if(temp->active)

	mov	rax, QWORD PTR temp$[rbp]
	movzx	eax, BYTE PTR [rax]
	test	eax, eax
	je	$LN5@sheep_floc

; 92   :         {
; 93   :             position = *((vector2*) &temp->destination);

	mov	rax, QWORD PTR temp$[rbp]
	mov	rax, QWORD PTR [rax+176]
	mov	QWORD PTR position$[rbp], rax

; 94   :             grid_index = grid_world_to_grid(p_flock->grid_layer,

	mov	rdx, QWORD PTR position$[rbp]
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_world_to_grid
	mov	QWORD PTR grid_index$[rbp], rax

; 95   :                                             position);
; 96   : 
; 97   :             if(!temp->moving)

	mov	rax, QWORD PTR temp$[rbp]
	movzx	eax, BYTE PTR [rax+28]
	test	eax, eax
	jne	$LN6@sheep_floc

; 98   :             {
; 99   :                 if(grid_open_neighbours_at(p_flock->grid_layer,

	mov	DWORD PTR [rsp+32], 16
	mov	r9, QWORD PTR grid_index$[rbp]
	lea	r8, QWORD PTR open_count$[rbp]
	lea	rdx, QWORD PTR open_indeces$[rbp]
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_open_neighbours_at
	movzx	eax, al
	test	eax, eax
	je	$LN8@sheep_floc

; 100  :                                            open_indeces,
; 101  :                                            &open_count,
; 102  :                                            grid_index,
; 103  :                                            HAS_SHEEP))
; 104  :                 {
; 105  :                     temp->from = position;

	mov	rax, QWORD PTR temp$[rbp]
	mov	rcx, QWORD PTR position$[rbp]
	mov	QWORD PTR [rax+32], rcx

; 106  : 
; 107  :                     grid_set_data_at(p_flock->grid_layer,

	mov	rax, QWORD PTR temp$[rbp]
	mov	rdx, QWORD PTR [rax+32]
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_world_to_grid
	xor	r9d, r9d
	mov	r8d, 16
	mov	rdx, rax
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_set_data_at

; 108  :                                      grid_world_to_grid(p_flock->grid_layer,
; 109  :                                      temp->from),
; 110  :                                      HAS_SHEEP, false);
; 111  : 
; 112  :                     random_index = open_indeces[rand() % open_count];

	call	QWORD PTR __imp_rand
	cdq
	idiv	DWORD PTR open_count$[rbp]
	mov	eax, edx
	cdqe
	mov	rax, QWORD PTR open_indeces$[rbp+rax*8]
	mov	QWORD PTR random_index$[rbp], rax

; 113  :                     position.x = random_index.x;

	mov	eax, DWORD PTR random_index$[rbp]
	mov	DWORD PTR position$[rbp], eax

; 114  :                     position.y = random_index.y;

	mov	eax, DWORD PTR random_index$[rbp+4]
	mov	DWORD PTR position$[rbp+4], eax

; 115  : 
; 116  :                     position = grid_grid_to_world(p_flock->grid_layer,

	xor	r8d, r8d
	mov	rdx, QWORD PTR position$[rbp]
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_grid_to_world
	mov	QWORD PTR position$[rbp], rax

; 117  :                                                   position,
; 118  :                                                   ORIGIN_CENTER);
; 119  :                     temp->to = position;

	mov	rax, QWORD PTR temp$[rbp]
	mov	rcx, QWORD PTR position$[rbp]
	mov	QWORD PTR [rax+40], rcx

; 120  :                     grid_set_data_at(p_flock->grid_layer,

	mov	rax, QWORD PTR temp$[rbp]
	mov	rdx, QWORD PTR [rax+40]
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_world_to_grid
	mov	r9d, 1
	mov	r8d, 16
	mov	rdx, rax
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_set_data_at

; 121  :                                      grid_world_to_grid(p_flock->grid_layer,
; 122  :                                                         temp->to),
; 123  :                                      HAS_SHEEP, true);
; 124  :                     temp->fraction = 0;

	mov	rax, QWORD PTR temp$[rbp]
	mov	DWORD PTR [rax+48], 0

; 125  :                     temp->moving = true;

	mov	rax, QWORD PTR temp$[rbp]
	mov	BYTE PTR [rax+28], 1
$LN8@sheep_floc:

; 126  :                 }
; 127  :             }

	jmp	SHORT $LN7@sheep_floc
$LN6@sheep_floc:

; 128  :             else if(temp->moving)

	mov	rax, QWORD PTR temp$[rbp]
	movzx	eax, BYTE PTR [rax+28]
	test	eax, eax
	je	SHORT $LN9@sheep_floc

; 129  :             {
; 130  :                 if(sheep_move(temp))

	mov	rcx, QWORD PTR temp$[rbp]
	call	sheep_move
	movzx	eax, al
	test	eax, eax
	je	SHORT $LN10@sheep_floc

; 131  :                 {
; 132  :                     temp->moving = false;

	mov	rax, QWORD PTR temp$[rbp]
	mov	BYTE PTR [rax+28], 0
$LN10@sheep_floc:
$LN9@sheep_floc:
$LN7@sheep_floc:

; 133  :                 }
; 134  : 
; 135  :             }
; 136  : 
; 137  :             temp->age += FRAME_TIME;

	mov	rax, QWORD PTR temp$[rbp]
	mov	eax, DWORD PTR [rax+4]
	add	eax, 16
	mov	rcx, QWORD PTR temp$[rbp]
	mov	DWORD PTR [rcx+4], eax

; 138  :             temp->age_index = temp->age / (p_flock->max_age / SHEEP_ANIMATION_COUNT);

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	eax, DWORD PTR [rax]
	cdq
	mov	ecx, 3
	idiv	ecx
	mov	DWORD PTR tv173[rbp], eax
	mov	rcx, QWORD PTR temp$[rbp]
	mov	eax, DWORD PTR [rcx+4]
	cdq
	mov	ecx, DWORD PTR tv173[rbp]
	idiv	ecx
	mov	rcx, QWORD PTR temp$[rbp]
	mov	DWORD PTR [rcx+20], eax

; 139  :             if(temp->age > p_flock->max_age)

	mov	rax, QWORD PTR temp$[rbp]
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	ecx, DWORD PTR [rcx]
$LN5@sheep_floc:

; 140  :             {
; 141  :                 // Let sheep die
; 142  :             }
; 143  :         }
; 144  :     }

	jmp	$LN2@sheep_floc
$LN3@sheep_floc:

; 145  : }

	lea	rcx, QWORD PTR [rbp-48]
	lea	rdx, OFFSET FLAT:sheep_flock_update$rtcFrameData
	call	_RTC_CheckStackVars
	mov	rcx, QWORD PTR __$ArrayPad$[rbp]
	xor	rcx, rbp
	call	__security_check_cookie
	lea	rsp, QWORD PTR [rbp+456]
	pop	rdi
	pop	rbp
	ret	0
sheep_flock_update ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\sheep.c
;	COMDAT sheep_flock_spawn_sheep
_TEXT	SEGMENT
temp$ = 8
i$1 = 36
p_flock$ = 288
p_spawn_pos$ = 296
sheep_flock_spawn_sheep PROC				; COMDAT

; 36   : {

$LN8:
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	push	rbp
	push	rdi
	sub	rsp, 296				; 00000128H
	lea	rbp, QWORD PTR [rsp+32]
	mov	rdi, rsp
	mov	ecx, 74					; 0000004aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+328]
	lea	rcx, OFFSET FLAT:__2C0C7D8D_sheep@c
	call	__CheckForDebuggerJustMyCode

; 37   :     sheep* temp;
; 38   : 
; 39   :     if(p_flock->pool_size <= p_flock->active_sheep)

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	ecx, DWORD PTR [rcx+16]
	cmp	DWORD PTR [rax+12], ecx
	jg	SHORT $LN5@sheep_floc

; 40   :         return;

	jmp	$LN1@sheep_floc
$LN5@sheep_floc:

; 41   :     for(int i = 0; i < p_flock->pool_size; i++)

	mov	DWORD PTR i$1[rbp], 0
	jmp	SHORT $LN4@sheep_floc
$LN2@sheep_floc:
	mov	eax, DWORD PTR i$1[rbp]
	inc	eax
	mov	DWORD PTR i$1[rbp], eax
$LN4@sheep_floc:
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	eax, DWORD PTR [rax+12]
	cmp	DWORD PTR i$1[rbp], eax
	jge	$LN3@sheep_floc

; 42   :     {
; 43   :         temp = &p_flock->pool[i];

	movsxd	rax, DWORD PTR i$1[rbp]
	imul	rax, rax, 192				; 000000c0H
	mov	rcx, QWORD PTR p_flock$[rbp]
	add	rax, QWORD PTR [rcx+32]
	mov	QWORD PTR temp$[rbp], rax

; 44   :         if(!p_flock->pool[i].active)

	movsxd	rax, DWORD PTR i$1[rbp]
	imul	rax, rax, 192				; 000000c0H
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rcx+32]
	movzx	eax, BYTE PTR [rcx+rax]
	test	eax, eax
	jne	$LN6@sheep_floc

; 45   :         {
; 46   :             temp->destination.x = p_spawn_pos.x;

	mov	rax, QWORD PTR temp$[rbp]
	mov	ecx, DWORD PTR p_spawn_pos$[rbp]
	mov	DWORD PTR [rax+176], ecx

; 47   :             temp->destination.y = p_spawn_pos.y;

	mov	rax, QWORD PTR temp$[rbp]
	mov	ecx, DWORD PTR p_spawn_pos$[rbp+4]
	mov	DWORD PTR [rax+180], ecx

; 48   :             temp->age = 0;

	mov	rax, QWORD PTR temp$[rbp]
	mov	DWORD PTR [rax+4], 0

; 49   :             temp->health = p_flock->max_health;

	mov	rax, QWORD PTR temp$[rbp]
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	ecx, DWORD PTR [rcx+8]
	mov	DWORD PTR [rax+16], ecx

; 50   :             temp->hunger = p_flock->max_hunger;

	mov	rax, QWORD PTR temp$[rbp]
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	ecx, DWORD PTR [rcx+4]
	mov	DWORD PTR [rax+12], ecx

; 51   :             temp->active = true;

	mov	rax, QWORD PTR temp$[rbp]
	mov	BYTE PTR [rax], 1

; 52   :             p_flock->active_sheep++;

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	eax, DWORD PTR [rax+16]
	inc	eax
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	DWORD PTR [rcx+16], eax

; 53   : 
; 54   :             grid_set_data_at(p_flock->grid_layer,

	mov	rax, QWORD PTR temp$[rbp]
	mov	rdx, QWORD PTR [rax+176]
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_world_to_grid
	mov	r9d, 1
	mov	r8d, 16
	mov	rdx, rax
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+24]
	call	grid_set_data_at

; 55   :                              grid_world_to_grid(p_flock->grid_layer,
; 56   :                                                 *((vector2*)&temp->destination)),
; 57   :                              HAS_SHEEP, true);
; 58   :             return;

	jmp	SHORT $LN1@sheep_floc
$LN6@sheep_floc:

; 59   :         }
; 60   :     }

	jmp	$LN2@sheep_floc
$LN3@sheep_floc:
$LN1@sheep_floc:

; 61   : }

	lea	rsp, QWORD PTR [rbp+264]
	pop	rdi
	pop	rbp
	ret	0
sheep_flock_spawn_sheep ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\sheep.c
;	COMDAT sheep_flock_init
_TEXT	SEGMENT
index$ = 4
anim_index$ = 36
default_dest$ = 72
p_flock$ = 336
p_grid$ = 344
p_anim$ = 352
p_pool_size$ = 360
p_max_age$ = 368
sheep_flock_init PROC					; COMDAT

; 17   : {

$LN9:
	mov	DWORD PTR [rsp+32], r9d
	mov	QWORD PTR [rsp+24], r8
	mov	QWORD PTR [rsp+16], rdx
	mov	QWORD PTR [rsp+8], rcx
	push	rbp
	push	rsi
	push	rdi
	sub	rsp, 336				; 00000150H
	lea	rbp, QWORD PTR [rsp+32]
	mov	rdi, rsp
	mov	ecx, 84					; 00000054H
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	rcx, QWORD PTR [rsp+376]
	lea	rcx, OFFSET FLAT:__2C0C7D8D_sheep@c
	call	__CheckForDebuggerJustMyCode

; 18   :     int index;
; 19   :     int anim_index;
; 20   :     SDL_Rect default_dest = { 0, 0, 64, 64};

	mov	DWORD PTR default_dest$[rbp], 0
	mov	DWORD PTR default_dest$[rbp+4], 0
	mov	DWORD PTR default_dest$[rbp+8], 64	; 00000040H
	mov	DWORD PTR default_dest$[rbp+12], 64	; 00000040H

; 21   :     p_flock->pool = (sheep*) calloc(sizeof(sheep), p_pool_size);

	movsxd	rax, DWORD PTR p_pool_size$[rbp]
	mov	rdx, rax
	mov	ecx, 192				; 000000c0H
	call	QWORD PTR __imp_calloc
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	QWORD PTR [rcx+32], rax

; 22   :     p_flock->pool_size = p_pool_size;

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	ecx, DWORD PTR p_pool_size$[rbp]
	mov	DWORD PTR [rax+12], ecx

; 23   :     p_flock->grid_layer = p_grid;

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR p_grid$[rbp]
	mov	QWORD PTR [rax+24], rcx

; 24   :     p_flock->max_age = p_max_age;

	mov	rax, QWORD PTR p_flock$[rbp]
	mov	ecx, DWORD PTR p_max_age$[rbp]
	mov	DWORD PTR [rax], ecx

; 25   :     memset(p_flock->pool, NULL, sizeof(sheep) * p_pool_size);

	movsxd	rax, DWORD PTR p_pool_size$[rbp]
	imul	rax, rax, 192				; 000000c0H
	mov	r8, rax
	xor	edx, edx
	mov	rax, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rax+32]
	call	memset

; 26   :     
; 27   : 
; 28   :     for(index = 0; index < p_pool_size; index++)

	mov	DWORD PTR index$[rbp], 0
	jmp	SHORT $LN4@sheep_floc
$LN2@sheep_floc:
	mov	eax, DWORD PTR index$[rbp]
	inc	eax
	mov	DWORD PTR index$[rbp], eax
$LN4@sheep_floc:
	mov	eax, DWORD PTR p_pool_size$[rbp]
	cmp	DWORD PTR index$[rbp], eax
	jge	$LN3@sheep_floc

; 29   :         for(anim_index = 0; anim_index < SHEEP_ANIMATION_COUNT; anim_index++)

	mov	DWORD PTR anim_index$[rbp], 0
	jmp	SHORT $LN7@sheep_floc
$LN5@sheep_floc:
	mov	eax, DWORD PTR anim_index$[rbp]
	inc	eax
	mov	DWORD PTR anim_index$[rbp], eax
$LN7@sheep_floc:
	cmp	DWORD PTR anim_index$[rbp], 3
	jge	SHORT $LN6@sheep_floc

; 30   :         {
; 31   :             p_flock->pool[index].anim[anim_index] = p_anim[anim_index];

	movsxd	rax, DWORD PTR anim_index$[rbp]
	imul	rax, rax, 40				; 00000028H
	movsxd	rcx, DWORD PTR index$[rbp]
	imul	rcx, rcx, 192				; 000000c0H
	mov	rdx, QWORD PTR p_flock$[rbp]
	mov	rdx, QWORD PTR [rdx+32]
	add	rdx, rcx
	mov	rcx, rdx
	movsxd	rdx, DWORD PTR anim_index$[rbp]
	imul	rdx, rdx, 40				; 00000028H
	mov	r8, QWORD PTR p_anim$[rbp]
	lea	rdi, QWORD PTR [rcx+rdx+56]
	lea	rsi, QWORD PTR [r8+rax]
	mov	ecx, 40					; 00000028H
	rep movsb

; 32   :             p_flock->pool[index].destination = default_dest;

	movsxd	rax, DWORD PTR index$[rbp]
	imul	rax, rax, 192				; 000000c0H
	mov	rcx, QWORD PTR p_flock$[rbp]
	mov	rcx, QWORD PTR [rcx+32]
	lea	rdx, QWORD PTR default_dest$[rbp]
	lea	rdi, QWORD PTR [rcx+rax+176]
	mov	rsi, rdx
	mov	ecx, 16
	rep movsb

; 33   :         }

	jmp	SHORT $LN5@sheep_floc
$LN6@sheep_floc:
	jmp	$LN2@sheep_floc
$LN3@sheep_floc:

; 34   : }

	lea	rcx, QWORD PTR [rbp-32]
	lea	rdx, OFFSET FLAT:sheep_flock_init$rtcFrameData
	call	_RTC_CheckStackVars
	lea	rsp, QWORD PTR [rbp+304]
	pop	rdi
	pop	rsi
	pop	rbp
	ret	0
sheep_flock_init ENDP
_TEXT	ENDS
END