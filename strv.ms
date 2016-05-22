
;  GNU C 2.7.2 [AL 1.1, MM 40] RISC NEWS-OS compiled by CC

;  Cc1 defaults:
;  -mmemcpy

;  Cc1 arguments (-G value = 0, Cpu = 3000, ISA = 1):
;  -quiet -dumpbase -O2 -o

gcc2_compiled.:
__gnu_compiled_c:
; APP

		.code
		.align		1
		.global		strv_new
		.global		strv_new_ap
strv_new:
	ld.w	%r12,[%sp+0x1]	; 		xld.w		%r12, [%sp+0x4]			;//%r12    := s
	ld.w	%r13,%sp	; 		xadd		%r13, %sp, 8			;//%r13    := ap
	add	%r13,0x8
		;//---------------------------------------------;//
strv_new_ap:
		pushn		%r1				;//
	sub	%sp,0x1		; 		xsub		%sp, %sp, 4			;//
		ld.w		%r0, %r12			;//%r0     := s
		ld.w		%r1, %r13			;//%r1     := ap
		ld.w		%r12, 1				;//%r12    :=            1
	ext	calloc@rm	; 		xcall.d		calloc				;//%r10    := v = calloc(1, sizeof(char*))
	call.d	calloc@rl
		ld.w		%r13, 4				;//%r13    :=               sizeof(char*)	*delay*
		cmp		%r10, 0				;//if(!v) { DIE() }
		jreq		DIE				;//
	ld.w	[%sp+0x0],%r10	; 		xld.w		[%sp+0x0], %r10			;//[%sp+0] := v
strv_new_ap_LOOP:						;//
		cmp		%r0, 0				;//while(s) {
		jreq		strv_new_ap_RET			;//
		ld.w		%r12, %sp			;//  %r12    :=  &v
	call.d	strv_extend	; 		xcall.d		strv_extend			;//  strv_extend(&v, s)
		ld.w		%r13, %r0			;//  %r13    :=      s				*delay*
		ld.w		%r0, [%r1]+			;//  %r0     := s = va_arg(ap, const char*)
		jp		strv_new_ap_LOOP		;//}
strv_new_ap_RET:						;//
	ld.w	%r10,[%sp+0x0]	; 		xld.w		%r10, [%sp+0x0]			;//%r10    := v
	add	%sp,0x1		; 		xadd		%sp, %sp, 4			;//
		popn		%r1				;//
		ret						;//return     v


		.code
		.align		1
		.global		strv_push
strv_push:


		.code
		.align		1
		.global		strv_extend
strv_extend:


		.code
		.align		1
		.global		strv_append
strv_append:


		.code
		.align		1
		.global		strv_merge
strv_merge:


		.code
		.align		1
		.global		strv_remove
strv_remove:


		.code
		.align		1
		.global		strv_uniq
strv_uniq:


		.code
		.align		1
		.global		strv_find
strv_find:


		.code
		.align		1
		.global		strv_overlap
strv_overlap:


		.code
		.align		1
		.global		strv_sort
strv_sort:


		.code
		.align		1
		.global		strv_print
strv_print:

