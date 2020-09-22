; Listing generated by Microsoft (R) Optimizing Compiler Version 19.27.29111.0 

include listing.inc

INCLUDELIB MSVCRTD
INCLUDELIB OLDNAMES

msvcjmc	SEGMENT
__09340588_corecrt_math@h DB 01H
__0BF8BE11_vector2@c DB 01H
msvcjmc	ENDS
PUBLIC	lerp
PUBLIC	interpolate
PUBLIC	__JustMyCode_Default
PUBLIC	__real@3f50624dd2f1a9fc
EXTRN	_RTC_CheckStackVars:PROC
EXTRN	_RTC_InitBase:PROC
EXTRN	_RTC_Shutdown:PROC
EXTRN	__CheckForDebuggerJustMyCode:PROC
EXTRN	_fltused:DWORD
;	COMDAT pdata
pdata	SEGMENT
$pdata$lerp DD	imagerel $LN3
	DD	imagerel $LN3+226
	DD	imagerel $unwind$lerp
pdata	ENDS
;	COMDAT pdata
pdata	SEGMENT
$pdata$interpolate DD imagerel $LN6
	DD	imagerel $LN6+147
	DD	imagerel $unwind$interpolate
pdata	ENDS
;	COMDAT __real@3f50624dd2f1a9fc
CONST	SEGMENT
__real@3f50624dd2f1a9fc DQ 03f50624dd2f1a9fcr	; 0.001
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
$unwind$interpolate DD 025053101H
	DD	0116231bH
	DD	0700f001dH
	DD	0500eH
xdata	ENDS
;	COMDAT xdata
xdata	SEGMENT
$unwind$lerp DD	025053401H
	DD	0118231dH
	DD	070110021H
	DD	05010H
xdata	ENDS
;	COMDAT CONST
CONST	SEGMENT
lerp$rtcName$0 DB 074H
	DB	06fH
	DB	05fH
	DB	072H
	DB	065H
	DB	074H
	DB	075H
	DB	072H
	DB	06eH
	DB	00H
	ORG $+6
lerp$rtcVarDesc DD 028H
	DD	08H
	DQ	FLAT:lerp$rtcName$0
	ORG $+48
lerp$rtcFrameData DD 01H
	DD	00H
	DQ	FLAT:lerp$rtcVarDesc
CONST	ENDS
; Function compile flags: /Odt
;	COMDAT __JustMyCode_Default
_TEXT	SEGMENT
__JustMyCode_Default PROC				; COMDAT
	ret	0
__JustMyCode_Default ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\vector2.c
;	COMDAT interpolate
_TEXT	SEGMENT
min$ = 224
max$ = 232
current$ = 240
interpolate PROC					; COMDAT

; 14   : {

$LN6:
	mov	QWORD PTR [rsp+24], r8
	mov	DWORD PTR [rsp+16], edx
	mov	DWORD PTR [rsp+8], ecx
	push	rbp
	push	rdi
	sub	rsp, 232				; 000000e8H
	lea	rbp, QWORD PTR [rsp+32]
	mov	rdi, rsp
	mov	ecx, 58					; 0000003aH
	mov	eax, -858993460				; ccccccccH
	rep stosd
	mov	ecx, DWORD PTR [rsp+264]
	lea	rcx, OFFSET FLAT:__0BF8BE11_vector2@c
	call	__CheckForDebuggerJustMyCode

; 15   :     if(*current < min)

	mov	rax, QWORD PTR current$[rbp]
	mov	ecx, DWORD PTR min$[rbp]
	cmp	DWORD PTR [rax], ecx
	jge	SHORT $LN2@interpolat

; 16   :     {
; 17   :         *current = min;

	mov	rax, QWORD PTR current$[rbp]
	mov	ecx, DWORD PTR min$[rbp]
	mov	DWORD PTR [rax], ecx

; 18   :         return true;

	mov	al, 1
	jmp	SHORT $LN1@interpolat

; 19   :     }

	jmp	SHORT $LN3@interpolat
$LN2@interpolat:

; 20   :     else if(*current > max)

	mov	rax, QWORD PTR current$[rbp]
	mov	ecx, DWORD PTR max$[rbp]
	cmp	DWORD PTR [rax], ecx
	jle	SHORT $LN4@interpolat

; 21   :     {
; 22   :         *current = max;

	mov	rax, QWORD PTR current$[rbp]
	mov	ecx, DWORD PTR max$[rbp]
	mov	DWORD PTR [rax], ecx

; 23   :         return true;

	mov	al, 1
	jmp	SHORT $LN1@interpolat
$LN4@interpolat:
$LN3@interpolat:

; 24   :     }
; 25   :     return false;

	xor	al, al
$LN1@interpolat:

; 26   : }

	lea	rsp, QWORD PTR [rbp+200]
	pop	rdi
	pop	rbp
	ret	0
interpolate ENDP
_TEXT	ENDS
; Function compile flags: /Odtp /RTCsu /ZI
; File E:\CPP-Programming\Lama\Lama\Lama\vector2.c
;	COMDAT lerp
_TEXT	SEGMENT
to_return$ = 8
from$ = 256
to$ = 264
step_ms$ = 272
lerp	PROC						; COMDAT

; 6    : {

$LN3:
	mov	DWORD PTR [rsp+24], r8d
	mov	QWORD PTR [rsp+16], rdx
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
	lea	rcx, OFFSET FLAT:__0BF8BE11_vector2@c
	call	__CheckForDebuggerJustMyCode

; 7    :     vector2 to_return;
; 8    :     to_return.x = (to.x - from.x) * (float)(step_ms * 0.001) + from.x;

	mov	eax, DWORD PTR from$[rbp]
	mov	ecx, DWORD PTR to$[rbp]
	sub	ecx, eax
	mov	eax, ecx
	cvtsi2ss xmm0, eax
	cvtsi2sd xmm1, DWORD PTR step_ms$[rbp]
	mulsd	xmm1, QWORD PTR __real@3f50624dd2f1a9fc
	cvtsd2ss xmm1, xmm1
	mulss	xmm0, xmm1
	cvtsi2ss xmm1, DWORD PTR from$[rbp]
	addss	xmm0, xmm1
	cvttss2si eax, xmm0
	mov	DWORD PTR to_return$[rbp], eax

; 9    :     to_return.y = (to.y - from.y) * (float)(step_ms * 0.001) + from.y;

	mov	eax, DWORD PTR from$[rbp+4]
	mov	ecx, DWORD PTR to$[rbp+4]
	sub	ecx, eax
	mov	eax, ecx
	cvtsi2ss xmm0, eax
	cvtsi2sd xmm1, DWORD PTR step_ms$[rbp]
	mulsd	xmm1, QWORD PTR __real@3f50624dd2f1a9fc
	cvtsd2ss xmm1, xmm1
	mulss	xmm0, xmm1
	cvtsi2ss xmm1, DWORD PTR from$[rbp+4]
	addss	xmm0, xmm1
	cvttss2si eax, xmm0
	mov	DWORD PTR to_return$[rbp+4], eax

; 10   :     return to_return;

	mov	rax, QWORD PTR to_return$[rbp]

; 11   : }

	mov	rdi, rax
	lea	rcx, QWORD PTR [rbp-32]
	lea	rdx, OFFSET FLAT:lerp$rtcFrameData
	call	_RTC_CheckStackVars
	mov	rax, rdi
	lea	rsp, QWORD PTR [rbp+232]
	pop	rdi
	pop	rbp
	ret	0
lerp	ENDP
_TEXT	ENDS
END