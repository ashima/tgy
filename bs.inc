;***************************************************************
;* For HK Blue Series with n- and p-Channel FETs (old version) *
;* Fuses should be set to -U lfuse:w:0x3f:m -U hfuse:w:0xd7:m  *
;* or similar (even intrc will work now)                       *
;* 09/2011                                                     *
;* https://github.com/sim-/tgy                                 *
;***************************************************************

.equ	F_CPU		, 16000000
.equ	USE_INT0	, 1
.equ	USE_I2C		, 0	; We could, but FETs are on the I2C ports
.equ	USE_UART	, 0
.equ	USE_ICP		, 0

;*********************
; PORT D definitions *
;*********************
;.equ			, 7
;.equ			, 6
.equ	AnFET		, 5
.equ	ApFET		, 4
;.equ			, 3
.equ	rcp_in		, 2

.equ	INIT_PD		, 0
.equ	DIR_PD		, (1<<AnFET)+(1<<ApFET)

.equ	AnFET_port	, PORTD
.equ	ApFET_port	, PORTD

.MACRO rcp_int_enable r0:req
	ldi	\r0, (1<<INT0)	; enable ext0int
	out	GICR, \r0
.ENDM
.MACRO rcp_int_disable r0
	out	GICR, ZH	; disable ext0int
.ENDM
.MACRO rcp_int_rising_edge r0:req
	ldi	\r0, (1<<ISC01)+(1<<ISC00)
	out	MCUCR, \r0	; set next int0 to rising edge
.ENDM
.MACRO rcp_int_falling_edge r0:req
	ldi \r0, (1<<ISC01)
	out MCUCR, \r0		; set next int0 to falling edge
.ENDM

.MACRO AnFET_on
	sbi PORTD, AnFET
.ENDM
.MACRO AnFET_off
	cbi PORTD, AnFET
.ENDM
.MACRO ApFET_on
	sbi PORTD, ApFET
.ENDM
.MACRO ApFET_off
	cbi PORTD, ApFET
.ENDM

.MACRO AnFET_on_reg r0:req
	sbr \r0, 1<<AnFET
.ENDM
.MACRO AnFET_off_reg r0:req
	cbr \r0, 1<<AnFET
.ENDM
.MACRO ApFET_on_reg r0:req
	sbr \r0, 1<<ApFET
.ENDM
.MACRO ApFET_off_reg r0:req
	cbr \r0, 1<<ApFET
.ENDM


;*********************
; PORT C definitions *
;*********************
.equ	mux_b		, 7	; ADC7
.equ	mux_a		, 6	; ADC6
.equ	BpFET		, 5
.equ	BnFET		, 4
.equ	CpFET		, 3
;.equ	accu_adc	, 2	; ADC2 -> not used
;.equ	AnREF		, 1	; ADC1 -> not used
.equ	mux_c		, 0	; ADC0

.equ	INIT_PC		, 0
.equ	DIR_PC		, (1<<BnFET)+(1<<BpFET)+(1<<CpFET)

.equ	BpFET_port	, PORTC
.equ	BnFET_port	, PORTC
.equ	CpFET_port	, PORTC

.MACRO BnFET_on
	sbi PORTC, BnFET
.ENDM
.MACRO BnFET_off
	cbi PORTC, BnFET
.ENDM
.MACRO BpFET_on
	sbi PORTC, BpFET
.ENDM
.MACRO BpFET_off
	cbi PORTC, BpFET
.ENDM
.MACRO CpFET_on
	sbi PORTC, CpFET
.ENDM
.MACRO CpFET_off
	cbi PORTC, CpFET
.ENDM

.MACRO BnFET_on_reg r0:req
	sbr \r0, 1<<BnFET
.ENDM
.MACRO BnFET_off_reg r0:req
	cbr \r0, 1<<BnFET
.ENDM
.MACRO BpFET_on_reg r0:req
	sbr \r0, 1<<BpFET
.ENDM
.MACRO BpFET_off_reg r0:req
	cbr \r0, 1<<BpFET
.ENDM
.MACRO CpFET_on_reg r0:req
	sbr \r0, 1<<CpFET
.ENDM
.MACRO CpFET_off_reg r0:req
	cbr \r0, 1<<CpFET
.ENDM

.MACRO comp_init r0:req
	in \r0, SFIOR
	sbr \r0, 1<<ACME		; switch to comparator multiplexed
	out SFIOR, \r0
	cbi ADCSRA, ADEN	; disable ADC
.ENDM
.MACRO set_comp_phase_a r0:req
	ldi \r0, mux_a		; set comparator multiplexer to phase A
	out ADMUX, \r0
.ENDM
.MACRO set_comp_phase_b r0:req
	ldi \r0, mux_b		; set comparator multiplexer to phase B
	out ADMUX, \r0
.ENDM
.MACRO set_comp_phase_c r0:req
	ldi \r0, mux_c		; set comparator multiplexer to phase C
	out ADMUX, \r0
.ENDM


;*********************
; PORT B definitions *
;*********************
;.equ			, 7
;.equ			, 6
;.equ			, 5	(sck stk200 interface)
;.equ			, 4	(miso stk200 interface)
;.equ			, 3	(mosi stk200 interface)
;.equ			, 2
;.equ			, 1
.equ	CnFET		, 0

.equ	INIT_PB		, 0
.equ	DIR_PB		, (1<<CnFET)

.equ	CnFET_port	, PORTB

.MACRO CnFET_on
	sbi PORTB, CnFET
.ENDM
.MACRO CnFET_off
	cbi PORTB, CnFET
.ENDM

.MACRO CnFET_on_reg r0:req
	sbr \r0, 1<<CnFET
.ENDM
.MACRO CnFET_off_reg r0:req
	cbr \r0, 1<<CnFET
.ENDM


;********************
; Some other Macros *
;********************

.MACRO nFET_brake r0
	AnFET_on
	BnFET_on
	CnFET_on
.ENDM

.MACRO all_nFETs_off r0
	AnFET_off
	BnFET_off
	CnFET_off
.ENDM

.MACRO all_pFETs_off r0
	ApFET_off
	BpFET_off
	CpFET_off
.ENDM

.MACRO RED_on
.ENDM
.MACRO RED_off
.ENDM
.MACRO GRN_on
.ENDM
.MACRO GRN_off
.ENDM
