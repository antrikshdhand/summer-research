	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0	sdk_version 14, 2
	.globl	_is_non_terminal                ; -- Begin function is_non_terminal
	.p2align	2
_is_non_terminal:                       ; @is_non_terminal
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	strb	w0, [sp, #11]
	ldrb	w8, [sp, #11]
	and	w8, w8, #0x80
	subs	w8, w8, #128
	cset	w8, ne
	tbnz	w8, #0, LBB0_2
	b	LBB0_1
LBB0_1:
	ldrb	w8, [sp, #11]
	and	w8, w8, #0xf
	str	w8, [sp, #12]
	b	LBB0_3
LBB0_2:
	mov	w8, #-1
	str	w8, [sp, #12]
	b	LBB0_3
LBB0_3:
	ldr	w0, [sp, #12]
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_unify_key_inv                  ; -- Begin function unify_key_inv
	.p2align	2
_unify_key_inv:                         ; @unify_key_inv
	.cfi_startproc
; %bb.0:
	stp	x28, x27, [sp, #-32]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w27, -24
	.cfi_offset w28, -32
	mov	w9, #10384
	movk	w9, #16, lsl #16
	adrp	x16, ___chkstk_darwin@GOTPAGE
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	sub	sp, sp, #258, lsl #12           ; =1056768
	sub	sp, sp, #2192
	add	x8, sp, #258, lsl #12           ; =1056768
	add	x8, x8, #88
	str	x8, [sp, #56]                   ; 8-byte Folded Spill
	adrp	x9, ___stack_chk_guard@GOTPAGE
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x9, [x9]
	stur	x9, [x29, #-24]
	strb	w0, [x8, #31]
	str	x1, [x8, #16]
	str	x2, [x8, #8]
	ldrb	w0, [x8, #31]
	bl	_is_non_terminal
	ldr	x9, [sp, #56]                   ; 8-byte Folded Reload
                                        ; implicit-def: $x8
	mov	x8, x0
	sxtw	x8, w8
	str	x8, [x9]
	adds	x8, x8, #1
	cset	w8, eq
	tbnz	w8, #0, LBB1_2
	b	LBB1_1
LBB1_1:
	ldr	x9, [sp, #56]                   ; 8-byte Folded Reload
	ldr	x8, [x9, #16]
	add	x8, x8, #8
	ldr	x9, [x9]
	mov	x10, #8208
	movk	x10, #16, lsl #16
	mul	x9, x9, x10
	add	x1, x8, x9
	add	x0, sp, #72
	str	x0, [sp, #8]                    ; 8-byte Folded Spill
	mov	x2, #8208
	movk	x2, #16, lsl #16
	bl	_memcpy
	ldr	x8, [sp, #8]                    ; 8-byte Folded Reload
	str	xzr, [sp, #64]
	add	x8, x8, #16
	ldr	x9, [sp, #64]
	mov	x10, #1032
	mul	x9, x9, x10
	add	x1, x8, x9
	add	x0, sp, #258, lsl #12           ; =1056768
	add	x0, x0, #1152
	str	x0, [sp, #16]                   ; 8-byte Folded Spill
	mov	x2, #1032
	str	x2, [sp, #24]                   ; 8-byte Folded Spill
	bl	_memcpy
	ldr	x8, [sp, #56]                   ; 8-byte Folded Reload
	ldr	x1, [sp, #16]                   ; 8-byte Folded Reload
	ldr	x2, [sp, #24]                   ; 8-byte Folded Reload
	ldr	x9, [x8, #16]
	str	x9, [sp, #40]                   ; 8-byte Folded Spill
	ldr	x8, [x8, #8]
	str	x8, [sp, #48]                   ; 8-byte Folded Spill
	add	x0, sp, #258, lsl #12           ; =1056768
	add	x0, x0, #120
	str	x0, [sp, #32]                   ; 8-byte Folded Spill
	bl	_memcpy
	ldr	x0, [sp, #32]                   ; 8-byte Folded Reload
	ldr	x1, [sp, #40]                   ; 8-byte Folded Reload
	ldr	x2, [sp, #48]                   ; 8-byte Folded Reload
	bl	_unify_rule_inv
	b	LBB1_3
LBB1_2:
	ldr	x8, [sp, #56]                   ; 8-byte Folded Reload
	ldrb	w9, [x8, #31]
	ldr	x10, [x8, #8]
	add	x10, x10, #8
	ldr	x11, [x8, #8]
	ldr	x11, [x11]
	add	x10, x10, x11
	strb	w9, [x10]
	ldr	x9, [x8, #8]
	ldr	x8, [x9]
	add	x8, x8, #1
	str	x8, [x9]
	b	LBB1_3
LBB1_3:
	ldur	x9, [x29, #-24]
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	subs	x8, x8, x9
	cset	w8, eq
	tbnz	w8, #0, LBB1_5
	b	LBB1_4
LBB1_4:
	bl	___stack_chk_fail
LBB1_5:
	add	sp, sp, #258, lsl #12           ; =1056768
	add	sp, sp, #2192
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	ldp	x28, x27, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_unify_rule_inv                 ; -- Begin function unify_rule_inv
	.p2align	2
_unify_rule_inv:                        ; @unify_rule_inv
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #64
	.cfi_def_cfa_offset 64
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	add	x29, sp, #48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	x0, [sp]                        ; 8-byte Folded Spill
	stur	x0, [x29, #-8]
	stur	x1, [x29, #-16]
	str	x2, [sp, #24]
	str	xzr, [sp, #16]
	b	LBB2_1
LBB2_1:                                 ; =>This Inner Loop Header: Depth=1
	ldr	x9, [sp]                        ; 8-byte Folded Reload
	ldr	x8, [sp, #16]
	ldr	x9, [x9]
	subs	x8, x8, x9
	cset	w8, hs
	tbnz	w8, #0, LBB2_4
	b	LBB2_2
LBB2_2:                                 ;   in Loop: Header=BB2_1 Depth=1
	ldr	x8, [sp]                        ; 8-byte Folded Reload
	add	x8, x8, #8
	ldr	x9, [sp, #16]
	add	x8, x8, x9
	ldrb	w8, [x8]
	strb	w8, [sp, #15]
	ldrb	w0, [sp, #15]
	ldur	x1, [x29, #-16]
	ldr	x2, [sp, #24]
	bl	_unify_key_inv
	b	LBB2_3
LBB2_3:                                 ;   in Loop: Header=BB2_1 Depth=1
	ldr	x8, [sp, #16]
	add	x8, x8, #1
	str	x8, [sp, #16]
	b	LBB2_1
LBB2_4:
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_print_token_array              ; -- Begin function print_token_array
	.p2align	2
_print_token_array:                     ; @print_token_array
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	add	x29, sp, #32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	stur	x0, [x29, #-8]
	str	xzr, [sp, #16]
	b	LBB3_1
LBB3_1:                                 ; =>This Inner Loop Header: Depth=1
	ldr	x8, [sp, #16]
	ldur	x9, [x29, #-8]
	ldr	x9, [x9]
	subs	x8, x8, x9
	cset	w8, hs
	tbnz	w8, #0, LBB3_4
	b	LBB3_2
LBB3_2:                                 ;   in Loop: Header=BB3_1 Depth=1
	ldur	x8, [x29, #-8]
	add	x8, x8, #8
	ldr	x9, [sp, #16]
	ldrb	w10, [x8, x9]
	mov	x9, sp
                                        ; implicit-def: $x8
	mov	x8, x10
	str	x8, [x9]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	bl	_printf
	b	LBB3_3
LBB3_3:                                 ;   in Loop: Header=BB3_1 Depth=1
	ldr	x8, [sp, #16]
	add	x8, x8, #1
	str	x8, [sp, #16]
	b	LBB3_1
LBB3_4:
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #160
	.cfi_def_cfa_offset 160
	stp	x29, x30, [sp, #144]            ; 16-byte Folded Spill
	add	x29, sp, #144
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	str	wzr, [sp, #20]
	str	wzr, [sp, #16]
	b	LBB4_1
LBB4_1:                                 ; =>This Inner Loop Header: Depth=1
	ldr	w8, [sp, #16]
	subs	w8, w8, #1
	cset	w8, ge
	tbnz	w8, #0, LBB4_4
	b	LBB4_2
LBB4_2:                                 ;   in Loop: Header=BB4_1 Depth=1
	add	x2, sp, #24
	str	x2, [sp]                        ; 8-byte Folded Spill
	str	xzr, [sp, #24]
	mov	w8, #128
	strb	w8, [sp, #15]
	ldrb	w0, [sp, #15]
	adrp	x1, _sample_grammar@PAGE
	add	x1, x1, _sample_grammar@PAGEOFF
	bl	_unify_key_inv
	ldr	x0, [sp]                        ; 8-byte Folded Reload
	bl	_print_token_array
	b	LBB4_3
LBB4_3:                                 ;   in Loop: Header=BB4_1 Depth=1
	ldr	w8, [sp, #16]
	add	w8, w8, #1
	str	w8, [sp, #16]
	b	LBB4_1
LBB4_4:
	ldur	x9, [x29, #-8]
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	subs	x8, x8, x9
	cset	w8, eq
	tbnz	w8, #0, LBB4_6
	b	LBB4_5
LBB4_5:
	bl	___stack_chk_fail
LBB4_6:
	mov	w0, #0
	ldp	x29, x30, [sp, #144]            ; 16-byte Folded Reload
	add	sp, sp, #160
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__DATA,__data
	.globl	_sample_grammar                 ; @sample_grammar
	.p2align	3, 0x0
_sample_grammar:
	.quad	6                               ; 0x6
	.byte	128                             ; 0x80
	.space	7
	.quad	1                               ; 0x1
	.quad	1                               ; 0x1
	.byte	129                             ; 0x81
	.space	1023
	.space	1055736
	.byte	129                             ; 0x81
	.space	7
	.quad	1                               ; 0x1
	.quad	2                               ; 0x2
	.byte	130                             ; 0x82
	.byte	131                             ; 0x83
	.space	1022
	.space	1055736
	.byte	130                             ; 0x82
	.space	7
	.quad	1                               ; 0x1
	.quad	2                               ; 0x2
	.byte	132                             ; 0x84
	.byte	133                             ; 0x85
	.space	1022
	.space	1055736
	.byte	131                             ; 0x83
	.space	7
	.quad	3                               ; 0x3
	.quad	1                               ; 0x1
	.byte	5                               ; 0x5
	.space	1023
	.quad	1                               ; 0x1
	.byte	6                               ; 0x6
	.space	1023
	.quad	1                               ; 0x1
	.byte	7                               ; 0x7
	.space	1023
	.space	1053672
	.byte	132                             ; 0x84
	.space	7
	.quad	2                               ; 0x2
	.quad	1                               ; 0x1
	.byte	3                               ; 0x3
	.space	1023
	.quad	1                               ; 0x1
	.byte	4                               ; 0x4
	.space	1023
	.space	1054704
	.byte	133                             ; 0x85
	.space	7
	.quad	3                               ; 0x3
	.quad	1                               ; 0x1
	.space	1024
	.quad	1                               ; 0x1
	.byte	1                               ; 0x1
	.space	1023
	.quad	1                               ; 0x1
	.byte	2                               ; 0x2
	.space	1023
	.space	1053672
	.space	128927648

	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"0x%x\n"

.subsections_via_symbols
