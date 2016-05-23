
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
		.global		strv_extend
		.global		strv_push
strv_extend:
		pushn		%r0				;//
		ld.w		%r0, %r12			;//%r0  := pv
	ext	strdup@rm	; 		xcall.d		strdup				;//%r10 := t = strdup(s)
	call.d	strdup@rl
		ld.w		%r12, %r13			;//%r12 :=            s					*delay*
		cmp		%r10, 0				;//if(!t) { DIE() }
		jreq		strv_extend_push_DIE		;//
		ld.w		%r12, %r0			;//%r12 := pv
		ld.w		%r13, %r10			;//%r13 := t
		popn		%r0				;//
		;//---------------------------------------------;//
strv_push:
		pushn		%r2				;//
		ld.w		%r0, %r12			;//%r0  := pv
		ld.w		%r1, %r13			;//%r1  := s
		ld.w		%r2, [%r0]			;//%r2  := v = *pv
	ext	strv_length@rm	; 		xcall.d		strv_length			;//%r10 := n = strv_length(v)				*anti-interlock*
	call.d	strv_length@rl
		ld.w		%r12, %r2			;//%r12 :=                 v				*delay*
		ld.w		%r12, %r2			;//%r12 := v
		ld.w		%r2, %r10			;//%r2  := n
		sla		%r2, 2				;//%r2  := n * sizeof(char*)
		ld.w		%r13, %r2			;//%r13 :=                sizeof(char*) *  n
	ext	realloc@rm	; 		xcall.d		realloc				;//%r10 := v = realloc(v, sizeof(char*) * (n + 2))
	call.d	realloc@rl
		add		%r13, 8				;//%r13 :=                sizeof(char*) * (n + 2)	*delay*
		cmp		%r10, 0				;//if(!v) { DIE() }
		jreq		strv_extend_push_DIE		;//
		add		%r2, %r10			;//%r2  := &v[n + 0]
		ld.w		[%r2]+, %r1			;//         v[n + 0] = s
		ld.w		[%r2], %r8			;//         v[n + 1] = NULL
		ld.w		[%r0], %r10			;//*pv = v
		popn		%r2				;//
		ret.d						;//return  0
		ld.w		%r10, 0				;//%r10 := 0						*delay*

; NO_APP
	.code
	.align	1
strv_extend_push_DIE:
;	.frame	%sp,4,$31		# vars= 0, regs= 1/0, args= 0, extra= 0
;	.mask	0x80000000,-4
;	.fmask	0x00000000,0
	ext	DIE@rm		; 	xcall	DIE
	call	DIE@rl
	ret
; APP

		.code
		.align		1
		.global		strv_append
strv_append:
		pushn		%r0				;//
	sub	%sp,0x1		; 		xsub		%sp, %sp, 4			;//
	ext	strdupv@rm	; 		xcall.d		strdupv				;//%r10    := v = strv_copy(v) = strdupv(v)
	call.d	strdupv@rl
		ld.w		%r0, %r13			;//%r0     := s					*delay*
	ld.w	[%sp+0x0],%r10	; 		xld.w		[%sp+0x0], %r10			;//[%sp+0] := v
		ld.w		%r12, %sp			;//%r12    :=             &v
	call.d	strv_extend	; 		xcall.d		strv_extend			;//%r10    := strv_extend(&v, s)
		ld.w		%r13, %r0			;//%r13    :=                 s			*delay*
	ld.w	%r10,[%sp+0x0]	; 		xld.w		%r10, [%sp+0x0]			;//%r10    := v
	add	%sp,0x1		; 		xadd		%sp, %sp, 4			;//
		popn		%r0				;//
		ret						;//return     v


		.code
		.align		1
		.global		strv_merge
strv_merge:
		pushn		%r0				;//
	sub	%sp,0x1		; 		xsub		%sp, %sp, 4			;//
	ext	strdupv@rm	; 		xcall.d		strdupv				;//%r10    := v1 = strv_copy(v1) = strdupv(v1)
	call.d	strdupv@rl
		ld.w		%r0, %r13			;//%r0     := v2					*delay*
	ld.w	[%sp+0x0],%r10	; 		xld.w		[%sp+0x0], %r10			;//[%sp+0] := v1
strv_merge_LOOP:						;//for(;;) {
		ld.w		%r13, [%r0]+			;//  %r13    := s = *v2++
		cmp		%r13, 0				;//  if(!s) { break }
		jreq		strv_merge_RET			;//
		ld.w		%r12, %sp			;//  %r12    :=  &v1
	call	strv_extend	; 		xcall		strv_extend			;//  strv_extend(&v1, s)
		jp		strv_merge_LOOP			;//
strv_merge_RET:							;//}
	ld.w	%r10,[%sp+0x0]	; 		xld.w		%r10, [%sp+0x0]			;//%r10    := v1
	add	%sp,0x1		; 		xadd		%sp, %sp, 4			;//
		popn		%r0				;//
		ret						;//return     v1


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
		pushn		%r2				;//
		ld.w		%r0, %r12			;//%r0  := v
		ld.w		%r1, %r13			;//%r1  := s
strv_find_LOOP:							;//for(;;) {
		ld.w		%r2, [%r0]+			;//  %r2  := t = *v++
		cmp		%r2, 0				;//  if(!t) { break }
		jreq		strv_find_RET			;//
		ld.w		%r12, %r1			;//  %r12 :=        s
	ext	strcmp@rm	; 		xcall.d		strcmp				;//  %r10 := strcmp(s, t)
	call.d	strcmp@rl
		ld.w		%r13, %r2			;//  %r13 :=           t		*delay*
		cmp		%r10, 0				;//  if(!strcmp(s, t)) { break }
		jrne		strv_find_LOOP			;//
strv_find_RET:							;//}
		ld.w		%r10, %r2			;//%r10  := t
		popn		%r2				;//
		ret						;//return   t


		.code
		.align		1
		.global		strv_overlap
strv_overlap:
		pushn		%r1				;//
		ld.w		%r0, %r12			;//%r0  := v1
		ld.w		%r1, %r13			;//%r1  := v2
strv_overlap_LOOP:						;//for(;;) {
		ld.w		%r10, [%r0]+			;//  %r10 := s = *v1++
		cmp		%r10, 0				;//  if(!s) { return 0 }	ÑüÑüÑüÑüÑ¢	Ç±Ç±Ç≈ï™äÚÇ∑ÇÈéûÇÕä˘Ç…%r10=0Ç≈Ç∑ÅB
		jreq		strv_overlap_RET		;//  					Ñ†
		ld.w		%r12, %r1			;//  %r12 :=           v2		Ñ†
	call.d	strv_find	; 		xcall.d		strv_find			;//  %r10 := strv_find(v2, s)		Ñ†
		ld.w		%r13, %r10			;//  %r13 :=               s		Ñ†	*delay*
		cmp		%r10, 0				;//  if(strv_find(v2, s)) { return 1 }	Ñ†
		jreq		strv_overlap_LOOP		;//}					Ñ†
		ld.w		%r10, 1				;//%r10  := 1			ÑüÑüÑüÑüÑß
strv_overlap_RET:						;//					Ñ†
		popn		%r1				;//					Ñ†
		ret						;//return 0 or 1		Å©ÑüÑüÑüÑ£


		.code
		.align		1
		.global		strv_sort
strv_sort:
		pushn		%r0				;//
	ext	strv_length@rm	; 		xcall.d		strv_length			;//%r10 :=    strv_length(v)
	call.d	strv_length@rl
		ld.w		%r0, %r12			;//%r0  := v								*delay*
		ld.w		%r12, %r0			;//%r12 := v
		ld.w		%r13, %r10			;//%r13 :=    strv_length(v)
	;//	ld.w		%r14, 4				;//%r14 :=                    sizeof(char*)	ÑüÑüÑüÑüÑüÑüÑüÑüÑ¢
	ext	strv_sort_compar+0x0@h	; 		xld.w		%r15, strv_sort_compar		;//%r15 :=                                   strv_sort_compar	Ñ†
	ext	strv_sort_compar+0x0@m
	ld.w	%r15,strv_sort_compar+0x0@l
	ext	qsort@rm	; 		xcall.d		qsort				;//qsort(  v, strv_length(v), sizeof(char*), strv_sort_compar)	Ñ†
	call.d	qsort@rl
		ld.w		%r14, 4				;//%r14 :=                    sizeof(char*)	Å©ÑüÑüÑüÑüÑüÑüÑüÑ£	*delay*
		ld.w		%r10, %r0			;//%r10 := v
		popn		%r0				;//
		ret						;//return  v
		;//- - - - - - - - - - - - - - - - - - - - - - -;//
strv_sort_compar:
		ld.w		%r12, [%r12]			;//%r12 :=       x    = *(char**)_x
		ld.w		%r13, [%r13]			;//%r13 :=          y = *(char**)_y
	ext	strcmp@rm	; 		xjp		strcmp				;//return strcmp(x, y)
	jp	strcmp@rl


		.code
		.align		1
		.global		strv_print
strv_print:
		pushn		%r0				;//
		ld.w		%r0, %r12			;//%r0  := v
strv_print_LOOP:						;//for(;;) {
		ld.w		%r12, [%r0]+			;//  %r12 := s = *v++
		cmp		%r12, 0				;//  if(!s) { break }
		jreq		strv_print_RET			;//
	ext	puts@rm		; 		xcall		puts				;//  puts(s)
	call	puts@rl
		jp		strv_print_LOOP			;//}
strv_print_RET:							;//
		popn		%r0				;//
		ret						;//

