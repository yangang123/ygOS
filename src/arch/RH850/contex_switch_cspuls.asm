.extern _ygos_tcb_current
.extern _ygos_runing_start
.public _os_task_switch_low
.public _ygos_start_high_ready_low
.public _ygos_interrupt_disable
.public _ygos_interrupt_enable
.public _ygos_start_high_ready
.public _os_task_switch
.public _ei_interrupt_common 
.public _INT_OSTM_handler

	.section	".text", text
	.align	2

_context_switch_save:
	st.w  r6 , -4[sp] 
	st.w  r7 , -8[sp] 
	st.w  r8 ,-12[sp] 
	st.w  r9 ,-16[sp] 
	st.w  r10,-20[sp]  
	st.w  r11,-24[sp]  
	st.w  r12,-28[sp]  
	st.w  r13,-32[sp]  
	st.w  r14,-36[sp] 
	st.w  r15,-40[sp] 
	st.w  r16,-44[sp] 
	st.w  r17,-48[sp] 
	st.w  r18,-52[sp] 
	st.w  r19,-56[sp] 
	st.w  r20,-60[sp] 
	st.w  r21,-64[sp] 
	st.w  r22,-68[sp] 
	st.w  r23,-72[sp] 
	st.w  r24,-76[sp] 
	st.w  r25,-80[sp] 
	st.w  r26,-84[sp] 
	st.w  r27,-88[sp] 
	st.w  r28,-92[sp] 
	st.w  r29,-96[sp] 
	st.w  r30,-100[sp]
	st.w  r1 ,-104[sp]
	st.w  r2 ,-108[sp]
	 
	stsr  1,r1
	st.w  r1,-112[sp]  ; FIPSW
	
	stsr  0,r1
	st.w  r1,-116[sp]  ; EIPC
	
	movea -116,sp,sp 
    
    MOVHI   highw1(#_ygos_tcb_current),r0,r1           
	ld.w    loww(#_ygos_tcb_current)[r1],r2  
	st.w    sp, 0[r2]
	
	jmp [lp]

_context_switch_restore:
	MOVHI   highw1(#_ygos_tcb_current),r0,r1           
	ld.w    loww(#_ygos_tcb_current)[r1],sp  

	ld.w    0[sp],r2    ; base = r2

	ld.w    0[r2],r1    ; FIPC
	ldsr 	r1, 0  

	ld.w    4[r2],r1    ; FIPSW           
	ldsr 	r1, 1

	ld.w    12[r2],r1
	ld.w    16[r2],r30
	ld.w    20[r2],r29
	ld.w    24[r2],r28
	ld.w    28[r2],r27
	ld.w    32[r2],r26
	ld.w    36[r2],r25
	ld.w    40[r2],r24
	ld.w    44[r2],r23
	ld.w    48[r2],r22
	ld.w    52[r2],r21
	ld.w    56[r2],r20
	ld.w    60[r2],r19
	ld.w    64[r2],r18
	ld.w    68[r2],r17
	ld.w    72[r2],r16
	ld.w    76[r2],r15
	ld.w    80[r2],r14
	ld.w    84[r2],r13
	ld.w    88[r2],r12
	ld.w    92[r2],r11
	ld.w    96[r2],r10
	ld.w    100[r2],r9
	ld.w    104[r2],r8
	ld.w    108[r2],r7
	ld.w    112[r2],r6

	mov     r2, sp
	movea   116, sp ,sp	
	ld.w    8[r2],r2

	jmp [lp]
	
_ygos_start_high_ready_low:
    jarl _ygos_runing_start,lp
    jarl _context_switch_restore, lp
    dispose 0, lp

    eiret

_os_task_switch_low:
	prepare lp, 0
	jarl _context_switch_save, lp
	jarl _ygos_task_context_switch, lp
	jarl _context_switch_restore, lp
	dispose 0, lp

	eiret
		
_ei_interrupt_common:
	prepare lp, 0
	
	jarl _context_switch_save, lp
	jarl _ygos_interrupt_enter,lp
	jarl _INT_OSTM_handler, lp
	jarl _ygos_interrupt_leave,lp
	jarl _ygos_task_context_switch,lp
	jarl _context_switch_restore, lp
	
	dispose 0, lp

	eiret
	
_os_task_switch:
    trap 0x00
    jmp [lp]
    
_ygos_start_high_ready:
    trap 0x10
    jmp [lp]