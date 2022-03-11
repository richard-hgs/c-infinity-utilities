IFDEF RAX
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; x64 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; .model flat, C
;;;;;;;;;;;;;;;;;;;;;;;;;;;; CURRENT THREAD ID x64
.code
ASMX64GetCurrentThreadId proc
	mov eax, GS:[48h]
	ret
ASMX64GetCurrentThreadId endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;; CURRENT STACK FRAME BOTTOM (RBP)
.code
ASMX64GetStackFrameBottom proc
	mov rax, rbp
	ret
ASMX64GetStackFrameBottom endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;; CURRENT STACK FRAME BOTTOM (RBP2)
.code
ASMX64GetRbp proc
	mov rax, [rbp+4]
	ret
ASMX64GetRbp endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;; CURRENT STACK FRAME BOTTOM (RSP)
.code
ASMX64GetRsp proc
	mov rax, rsp
	ret
ASMX64GetRsp endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;; CURRENT BASE ADDRESS (PEB)
.code
ASMX64GetPeb proc
    mov rax, gs:[60h]
  ret
ASMX64GetPeb endp
ELSE
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; x86 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.model flat, C
;;;;;;;;;;;;;;;;;;;;;;;;;;;; C++ FUNCTIONS EXPORTS
; EXTERN printf_s :PROC
;;;;;;;;;;;;;;;;;;;;;;;;;;;; CURRENT THREAD ID x86
.code
ASMX86GetCurrentThreadId proc
ASSUME FS:NOTHING
	mov eax, FS:[24h]
	ret
ASSUME FS:ERROR
ASMX86GetCurrentThreadId endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;; CURRENT STACK FRAME BOTTOM (EBP)
.code
ASMX86GetStackFrameBottom proc
	mov eax, ebp
	ret
ASMX86GetStackFrameBottom endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;; PRINTF EXAMPLE
.data
tstStr db "Address is: %d l Eip is: %d",0Ah,0 ; 0Ah is the backslash - escapes are not supported

.code
ASMX86PrintfExample proc jmpAddress:DWORD 
	push ebp
	mov ebp, esp
	pop ebp

	; sub esp, 8
    ; pop ebp

	;; PrintF Example
	; mov eax, jmpAddress
	; push eax
	; mov eax, esp
	; push eax
	; push OFFSET tstStr
    ; call printf
	ret
ASMX86PrintfExample endp
;;;;;;;;;;;;;;;;;;;;;;;;;;;; PROCESS THREAD INFORMATION BLOCK (TEB)
.code
ASSUME FS:NOTHING
ASMX86GetTeb proc
	mov eax, FS:[18h]
	ret
ASMX86GetTeb endp
ASSUME FS:ERROR
ENDIF
end