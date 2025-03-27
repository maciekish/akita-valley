   LIST   P=18f242,C=132,T=ON,N=75,R=DEC,ST=OFF,W=1,X=ON
   TITLE  JRE235 Author: Maciej Swic
VERNO	SET	VERSION_3
;----------------------------------------------------------------------------
;   This Sound Definition Langauge (SDL) was generated
;   by the SPJHelper Design Tool on 2025-03-18
;----------------------------------------------------------------------------
;     FUNCTION KEYS DEFINED:   
;    F2 Horn/Whistle
;    F3 Brake Release
;    F4 Track 3, Doors Closing
;    F7 Tokyo Jingle
;    F8 Akihabara Jingle
;    F10 Next Station: Akihabara
;    F14 Flange
;    F15 Joint
;    F20 Mute On/Off
;----------------------------------
;   SOUND WAVE FILE HANDLES
;----------------------------------
   cblock   0   ; Start assigning location of  Sound Clips 0, 1, 2, etc.
HNDL_MUTE
STARTUP
INC_01
INC_12
INC_23
RUN_0
RUN_1
RUN_2
RUN_3
DEC_32
DEC_21
DEC_10
SHUTDOWN
HORN_BEGIN
HORN_LOOP
HORN_END
   ENDC

;-----------------------------------------
;   INCLUDED DIGITRAX PROPRIETARY FILES
;-----------------------------------------
;NOLIST
   #INCLUDE	../SFX3p0_snd_cmd.inc
   #INCLUDE	../SFX3p0_snd_macs.inc
;LIST
;#INCLUDE	EL7TrigV7.inc
;#INCLUDE	EL7c4x6d30base.inc
;#INCLUDE   SERIES6_Snd_cmd.INC
;#INCLUDE   SERIES6_snd16_macs.INC

;-----------------------------------------
;   SYSTEM VARIABLES
;-----------------------------------------
CUSTOM_SDF			EQU	0x86  ;cv152   AUTHOR code Maciej Swic
USER_SDF_SUB_ID	set	70    ;cv105   AND CV160
USER_SDF_FIX_ID	set	0     ;cv106   AND CV160
PROJECT_ID			EQU	001   ;cv153   000-254
AuthorCode        EQU   0x86  ;cv25    AUTHOR code John McMasters

;------------------------------------------
;   CONSTANTS
;------------------------------------------
; Volume Scaling
SCALE_F	EQU	0x0F
SCALE_E	EQU	0x0E
SCALE_D	EQU	0x0D
SCALE_C	EQU	0x0C
SCALE_B	EQU	0x0B
SCALE_A	EQU	0x0A
SCALE_9	EQU	0x09
SCALE_8	EQU	0x08
SCALE_7	EQU	0x07
SCALE_6	EQU	0x06
SCALE_5	EQU	0x05
SCALE_4	EQU	0x04
SCALE_3	EQU	0x03
SCALE_2	EQU	0x02
SCALE_1	EQU	0x01

SCALE_INCR_1	EQU 0x01
SCALE_INCR_2	EQU 0x02
SCALE_INCR_3	EQU 0x03
SCALE_INCR_4	EQU 0x04
SCALE_INCR_6	EQU 0x06
SCALE_INCR_8	EQU 0x08

;see SCV_LOAD_MIN for Min value to tune minimum volume 1-5 [5] VARY_LOAD / SCALE
SCALE_MIN      EQU	SCALE_8 ; Lowest volume
SCALE_DRIFT		EQU	SCALE_B
SCALE_MID		EQU	SCALE_C
SCALE_MAX		EQU   SCALE_F ; Highest volume
DRIFT_VOLUME	EQU 55	;Drift Volume		0-64	[20]

IMAXP_DIESEL	EQU	0x5E	;0x7E	;MAXP_DIESEL	EQU	0x3E

; Notching
NOTCH_UP_VAL	EQU	16
NOTCH_DOWN_VAL	EQU	-NOTCH_UP_VAL
NOTCH1	EQU	0x00
NOTCH2	EQU	0x10
NOTCH3	EQU	0x20
NOTCH4	EQU	0x30
NOTCH5	EQU	0x40
NOTCH6	EQU	0x50
NOTCH7	EQU	0x60
NOTCH8	EQU	0x70
;NOTCH9	EQU	0x80
MAX_NOTCH_VAL	EQU	NOTCH3
MIN_NOTCH_VAL	EQU	NOTCH1

;USER_INTLK_BITS MASKS:
VARY_DRIFT_MASK7	EQU	0x80	;MASK just do bit7	Vary auto grade crossing 6/25/16	
STATIC_MAINT_MASK6	EQU	0x40	;MASK just do bit6	STATIC MAINTENANCE SOUNDS INTERLOCK
COUPLER_MASK5		EQU	0x20	;MASK just do bit5	AUTO COUPLER INTERLOCK
CYLCOCK_MASK4		EQU	0x10	;MASK just do bit4	OPEN CYLCOCK INTERLOCK
DYN_CTRL_MASK3		EQU	0x08	;MASK just do bit3	NOT_USED INTERLOCK
AUTO_BRAKE_MASK2	EQU	0x04	;MASK just do bit2	AUTOMATIC BRAKING INTERLOCK
START_MASK1			EQU	0x02	;MASK just do bit1	STARTUP SOUNDS INTERLOCK
COMP_MASK0			EQU	0x01	;MASK just do bit0	COMPRESSOR AND AIR SOUNDS INTERLOCK

;------------------------------------------
;   LOCATION FOR CV ADDRESSES
;------------------------------------------
   cblock   SCV_FREEFORM   ; Start assigning (after std Digitrax) for CV140, CV141, etc.
SCV_PRIME_VOLUME    	;CV140 PRIME mover / Chuff vol  0-64	[40]
SCV_LOAD_MIN         ;CV141 Range Volume Scale to a value between 1-5. Drifting volume will diminish to this level.
   ENDC
;--------------------------------------------------------------
; Previously defined SCVs - listed here for reference convience
;--------------------------------------------------------------
;   SCV_RESET (CV8)  Reset Decoder values to factory
;   SCV_MASTER_VOL (CV58) Master Volume Setting
SCV_132 EQU SCV_NOTCH ; Controller Notch Rate
SCV_133 EQU SNDCV_STEAM ; Wheel Diameter. Default 127 for slowest Chuff
SCV_134 EQU SCV_STGEAR ; Wheel Ratio. Default 32 = 100%
SCV_135 EQU SCV_MUTE_VOL; Global volume when muted. Default 0
SCV_139 EQU SCV_DISTANCE_RATE ; Time value till Gauge Trigger. Default 63 = 9 min

	PAGE

;------------------------------------------------------------------
;   LOCAL VARIABLES (Work Registers and Work_User Memory Registers)                                       
;------------------------------------------------------------------
;WORK_SPEED	  EQU	0x00
;WORK_NOTCH	  EQU	0x01
;NOTCH0        EQU 0x00
;NOTCH1        EQU	0x10  ; Gives a ~33% split at a Notch Rate of 64 (CV132)
;NOTCH2        EQU	0x20
;NOTCH3        EQU	0x50
;WORK_SERVO	  EQU	0x02
;WORK_MVOLTS	  EQU	0x03
;WORK_USER_LINES	  EQU	0x05
;WORK_TIMEBASE	  EQU	0x06
;WORK_STATUS_BITS  EQU	0x07
;WORK_GLBL_GAIN	  EQU	0x08
;WORK_GAIN_TRIM	  EQU	0x09
;WORK_PITCH_TRIM	  EQU	0x0A
;WORK_SPEED_DELTA  EQU	0x0B
;WORK_SCATTER4	  EQU	0x10
;WORK_SCATTER5	  EQU	0x11
;WORK_SCATTER6	  EQU	0x12
;WORK_SCATTER7	  EQU	0x13
;WORK_ACHNL_7F	  EQU	0x14
;WORK_ACHNL_7E	  EQU	0x15
;WORK_SKAT_FAST	  EQU	0x16
;WORK_SKAT_SLOW	  EQU	0x17
;WORK_DISTANCE	  EQU	0x18
;WORK_PEAK_SPD	  EQU	0x19

COUNT_LOOP		   EQU WORK_USER_0 ; used to count n times
PREV_NOTCH		   EQU WORK_USER_1 ; used to remember previous Notch	0x00 - 0x70 [run notches 0-8] [increments of 0x10 16decimal]
VARY_LOAD		   EQU WORK_USER_2 ; used to vary scale from outside motor sound loop 	;range 1-15
AIR_PRESS		   EQU WORK_USER_3 ; used to save current air or brake pressure from 90 PSI = Pounds per Square Inch to 140 PSI used to branch to gain set with constants 
USER_INTLK_BITS	EQU WORK_USER_4

WORK_USER_0    EQU	0x1A
WORK_USER_1    EQU	0x1B
WORK_USER_2	   EQU	0x1C
WORK_USER_3	   EQU	0x1D
WORK_USER_4	   EQU	0x1E
WORK_USER_5	   EQU	0x1F

	PAGE

;========================================================================================
; START SCHEME 0                   
;========================================================================================
   ORG   0
   SKEME_START   0
   SDL_VERSION VERSION_2
;---------------------------------------------
;  START CHANNEL   1 
;---------------------------------------------
CHNL_01_S0
   CHANNEL_START   1
;---------------------------------------------

	;----------------------------------------------------
	;	STARTUP
	;----------------------------------------------------
   INITIATE_SOUND    TRIG_SND_ACTV11,NORMAL+NO_PREEMPT_TRIG

   LOAD_MODIFIER     MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B
   LOAD_MODIFIER	   MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~START_MASK1,MERGE_ALL_MASK

   PLAY              HNDL_MUTE,no_loop,loop_STD
   
   SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_MOVING
		PLAY              STARTUP,no_loop,loop_STD
   
   LOAD_MODIFIER	   MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,START_MASK1,MERGE_ALL_MASK
   LOAD_MODIFIER     MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COMP_MASK0,MERGE_ALL_MASK
   LOAD_MODIFIER     MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK
   
   SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_MOVING
      PLAY              RUN_0,no_loop,loop_STD
   
   LOAD_MODIFIER 	   MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,0,0
   LOAD_MODIFIER     MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH1,0
	LOAD_MODIFIER     MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,NOTCH1,0
	LOAD_MODIFIER     MTYPE_WORK_IMMED,FMATH_LODE+WORK_PEAK_SPD,0x00,0
   LOAD_MODIFIER     MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,0,0

   END_SOUND

	;----------------------------------------------------
	;	SHUTDOWN
	;----------------------------------------------------
	INITIATE_SOUND    TRIG_SND_ACTV11,NOT_TRIG	

	MASK_COMPARE      VARY_LOAD,IMMED_DATA,SCV_LOAD_MIN,COMP_ALL,SKIP_GRTR ; Prevent underflow
		LOAD_MODIFIER  MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,-SCALE_INCR_1,SCALE_MAX

	PLAY              RUN_1,no_loop,loop_STD	

	LOAD_MODIFIER     MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~COMP_MASK0,MERGE_ALL_MASK ; Disable Compressor
	
	PLAY              SHUTDOWN,no_loop,loop_STD

	END_SOUND	

;================================================= major routine ===========================		
;----------------------------------------------------
; Increase to run
; ---------------------------------------------------------
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	;	this is MODIFIED by DISCRETE notch change logic to vary AMPL with NOTCH setting, RPMs also track
	;---------------------------------------------------- 
	; ---------------------------------------------------------
	;     Play increase from stopped idle -- no load    
	; pick up notch at prev and increase to new current:                        
	; ---------------------------------------------------------
	INITIATE_SOUND T_SPD_IDLEXIT,NORMAL+NO_PREEMPT_TRIG	;RUN_WHILE_TRIG+ZAP	;leaving IDLE state
	;finish play here due to interrupted idle event: 8/1/19
	
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_MID
	PLAY INC_01,no_loop,loop_STD	; 

	;begin to play acceleration...
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COUPLER_MASK5,MERGE_ALL_MASK	;OR=SETtheBit bit5
	;NOTE: CANNOT PLAY NON PRIME MOVER SOUNDS IN THIS EVENT --
	;		IT INTERRUPTS IDLE SOUNDS...
		;BRANCH_TO	CH1SC0_IDLE_EXIT_IDLERUN
	;Notch1=0 is initial default...
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH1,WORK_NOTCH	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,NOTCH1,WORK_NOTCH	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_PEAK_SPD,0x00,0	

	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,+SCALE_INCR_2,SCALE_MAX   ;increment SCALE

CH1SC0_IDLE_EXIT_IDLERUN

;---
;Fall thru to complete transition

CH1SC0_ACCEL_IDLERUN2
;======================================================================
;	Evaluate WORK_NOTCH  
;===================================================================================
;	WORK_NOTCH is computed by the decoder and is continuously changed.
;	The rate of change is managed by the SCV_NOTCH	 (CV132) Controller Notch Rate where the 
;	default value CV132=127 is mid range. Lower values are a slower rate -- higher is faster.  
;	This project uses increments by NOTCH_UP_VAL of decimal 16 [0x10] for each notch.
;	WORK_NOTCH will vary by more than one increment depending on rate of speed change and notch rate.
;	The variable input to the computation is WORK_SPEED which is an internal value from decimal 0-99.
;	99 divided by 16 gives the 7 integers for notches 1-8.
;	However the WORK_NOTCH register is incremented by 16's and has the range:
;	NOTCH	Decimal	HEX			Bit			Throttle 
;								7654 3210	Speed (typical)
;	0		0		(low idle option is at rest, not moving... )
;	1		0		0x00		0000 0000		0
;	2		16		0x10		0001 0000		12
;	3		32		0x20		0010 0000		24
;	4		48		0x30		0011 0000		36
;	5		64		0x40		0100 0000		48
;	6		80		0x50		0101 0000		60
;	7		96		0x60		0110 0000		72
;	8		112		0x70		0111 0000		84
;	9		128		0x80		1000 0000		96
;	10		144		0x90		1001 0000
;	11		160		0xA0		1010 0000
;	12		176		0xB0		1011 0000
;	13		192		0xC0		1100 0000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CH1SC0_STARTMOVE_END_ACCL
	;======================== current notch is passed to continuous running
	;							via PREV_NOTCH. Any additional acceleration
	;							or deceleration is processed there.
	;========================
	;v6.1 DO fall thru to run continuous 
	BRANCH_TO	CH1SC0_CONTINUOUS_NOTCH	
	END_SOUND

	
	; ------------------------------------------------
	;  CONTINUOUS NOTCH SOUNDS WITH INTER-NOTCH SOUNDS
	; ------------------------------------------------
;-------------------------------
;DIESEL RUN phase
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	;	this is MODIFIED by DISCRETE notch change logic to vary AMPL with NOTCH setting, RPMs also track
	;----------------------------------------------------
;UC1.6.2================
	INITIATE_SOUND	TRIG_MOVING,RUN_WHILE_TRIG	;+ZAP	;T_SPD_RUN
	;+++++++++++++++++++++++++++++++++++++++++++++++++
CH1SC0_CONTINUOUS_NOTCH
	;SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FAND_ON		; But skip if SF33 was previously on
	;	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS	
		
	LOAD_MODIFIER	MTYPE_BLEND,BLEND_CURRENT_CHNL+BLEND_GAIN0+BLEND_FASE0,BLENDG_DSL_ACCEL1,BLENDF_DSL_ACCEL1
	LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0,IMAXP_DIESEL,DITHERP_DIESEL	;for discrete notch volume control	
	;since prev = 10=NOTCH2 and work = 0 or 10 do next transition...

CH1SC0_TEST_LOAD_RUN
	SKIP_ON_TRIGGER	TRIG_TRUE,T_SPD_DECEL1		;check if DECEL
		BRANCH_TO	CH1SC0_LOAD_RUN_TEST_ACCEL
	BRANCH_TO	CH1SC0_LOAD_RUN_IS_DECEL
;---
CH1SC0_LOAD_RUN_TEST_ACCEL
	;should continue to run up to NOTCH3...

	SKIP_ON_TRIGGER	TRIG_TRUE,T_SPD_ACCEL1		;check if ACCEL
		BRANCH_TO	CH1SC0_LOAD_RUN_CADENCE
		
	;ACCCEL1 STAYS ON UNTIL THROTTLE IS DECREASED...
CH1SC0_LOAD_RUN_IS_ACCEL
	LOAD_MODIFIER  MTYPE_BLEND,BLEND_CURRENT_CHNL,BLENDG_DSL_ACCEL0,BLENDF_DSL_ACCEL0
	LOAD_MODIFIER  MTYPE_PITCH,ANALOG_PITCH_MODIFY,MAXP_DIESEL,0x0 	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,+SCALE_INCR_6,SCALE_MAX   ;Increment
		
	;set drift interlock:
	;(drift is interlocked when not decelerating. when accelerating heavy sounds play.
	;interlock is set on bit is =1)(i.e. DRIFTING IS OFF)
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,VARY_DRIFT_MASK7,MERGE_ALL_MASK	;OR=SETtheBit  bit7	NOT drifting  =0

	BRANCH_TO	CH1SC0_LOAD_RUN_CADENCE

CH1SC0_LOAD_RUN_IS_DECEL
	LOAD_MODIFIER  MTYPE_BLEND,BLEND_CURRENT_CHNL,BLENDG_DSL_DECEL0,BLENDF_DSL_DECEL0
	LOAD_MODIFIER  MTYPE_PITCH,ANALOG_PITCH_MODIFY,MAXP_DIESEL,0x0
	MASK_COMPARE 	VARY_LOAD,IMMED_DATA,SCV_LOAD_MIN,COMP_ALL,SKIP_GRTR		;Prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,-SCALE_INCR_2,SCALE_MAX	;revised 3/10/17    ;decrement SCALE	
	;decrement delta after throttle change:.............
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL	
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent underflow
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL	

	;unset drift interlock: (DRIFTING IS ON)
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~VARY_DRIFT_MASK7,MERGE_ALL_MASK	;AND=FLIPtheBit  bit7	drifting  =1

;-----
CH1SC0_LOAD_RUN_CADENCE
	;(drift is interlocked when not decelerating. when accelerating heavy sounds play.
	;interlock is set on bit is =1)(i.e. DRIFTING IS OFF)
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,VARY_DRIFT_MASK7,~VARY_DRIFT_MASK7,SKIP_SAME	;bit7	VARY_DRIFT_MASK7 INTERLOCK ON OR OFF 		
		BRANCH_TO	CH1SC0_SET_LIGHT_DRIFT
	
;;;;;;;;;;;VARY_LOAD		EQU WORK_USER_2	; used to vary scale from outside motor sound loop 	;range 1-15
			;								; used to branch to gain set with constants
	;tune VARY_LOAD with SCV_LOAD_MIN:
;;;;;;;;;;;;;;;	coding extra GOTOs in branch table::::::::::::::::::
	MASK_COMPARE	VARY_LOAD,IMMED_DATA,SCALE_F,COMP_ALL,SKIP_SAME
		BRANCH_TO	CH1SC0_EXAUST_15
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_F
	BRANCH_TO	CH1SC0_RUN_MAIN
		
CH1SC0_EXAUST_15	
	MASK_COMPARE	VARY_LOAD,IMMED_DATA,SCALE_E,COMP_ALL,SKIP_SAME
		BRANCH_TO	CH1SC0_EXAUST_14
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_E
	BRANCH_TO	CH1SC0_RUN_MAIN
		
CH1SC0_EXAUST_14
	MASK_COMPARE	VARY_LOAD,IMMED_DATA,SCALE_D,COMP_ALL,SKIP_SAME
		BRANCH_TO	CH1SC0_EXAUST_13
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_D
	BRANCH_TO	CH1SC0_RUN_MAIN
		
CH1SC0_EXAUST_13
	MASK_COMPARE	VARY_LOAD,IMMED_DATA,SCALE_C,COMP_ALL,SKIP_SAME
		BRANCH_TO	CH1SC0_EXAUST_12
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_C
	BRANCH_TO	CH1SC0_RUN_MAIN
		
CH1SC0_EXAUST_12
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B
	BRANCH_TO	CH1SC0_RUN_MAIN
		
CH1SC0_SET_LIGHT_DRIFT	
	MASK_COMPARE	VARY_LOAD,IMMED_DATA,SCALE_F,COMP_ALL,SKIP_SAME
		BRANCH_TO	CH1SC0_DRIFT_15
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,DRIFT_VOLUME,SCALE_F
	BRANCH_TO	CH1SC0_RUN_MAIN
	
CH1SC0_DRIFT_15
	MASK_COMPARE	VARY_LOAD,IMMED_DATA,SCALE_E,COMP_ALL,SKIP_SAME
		BRANCH_TO	CH1SC0_DRIFT_14
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,DRIFT_VOLUME,SCALE_E
	BRANCH_TO	CH1SC0_RUN_MAIN
		
CH1SC0_DRIFT_14
	MASK_COMPARE	VARY_LOAD,IMMED_DATA,SCALE_D,COMP_ALL,SKIP_SAME
		BRANCH_TO	CH1SC0_DRIFT_13
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,DRIFT_VOLUME,SCALE_D
	BRANCH_TO	CH1SC0_RUN_MAIN
		
CH1SC0_DRIFT_13
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,DRIFT_VOLUME,SCALE_C
	BRANCH_TO	CH1SC0_RUN_MAIN
		
	
;--
CH1SC0_RUN_MAIN

;;;;;;;;;;;;;;;;;;;;;;;;; EVALUATE MOTOR ::::::::::::::::::

CH1SC0_EVAL_LOAD_RUN
;==============================================================
; all normal changes to notch come thru here:		
	;CONTINUE CHECKING NOTCH CHANGES:
;======================================================================
;	Evaluate WORK_NOTCH  
;======================================================================
	;TEST VARIOUS WAYS TO DETERMINE NOTCH CHANGE DIRECTION:
CH1SC0_CURRNOTCH_TEST
	;accelerate?
	MASK_COMPARE	PREV_NOTCH,TARGET_DATA,WORK_NOTCH,COMP_7LSB,SKIP_LESS   ;Skip if   previousNotch < current
		BRANCH_TO	CH1SC0_CURRNOTCH_TEST2		;work is less
	BRANCH_TO	CH1SC0_SETACC
	
CH1SC0_CURRNOTCH_TEST2	;is decelerate?
	MASK_COMPARE	WORK_NOTCH,TARGET_DATA,PREV_NOTCH,COMP_7LSB,SKIP_LESS   ;Skip unless  previousNotch > current
		BRANCH_TO	CH1SC0_CURRNOTCH_TEST3	;prev is equal
	BRANCH_TO	CH1SC0_SETDEC				;decelerate

CH1SC0_CURRNOTCH_TEST3	
	;no change?  PREViousNotch = current
	;work notch may change mid routine to a lesser value...
	MASK_COMPARE	PREV_NOTCH,TARGET_DATA,WORK_NOTCH,COMP_7LSB,SKIP_SAME   ;Skip if   previousNotch < current
		BRANCH_TO	CH1SC0_ERROR	;skipped when =
	;run:
	BRANCH_TO	CH1SC0_SETRUN
	
CH1SC0_ERROR
	;DEBUG: THROTTLE ERROR....
	;PLAY DEBUG1,no_loop,loop_STD	
	BRANCH_TO	CH1SC0_SETRUN

	
	
;=====================BEGIN ACCELERATION TO NEW NOTCH======================================================================	
CH1SC0_SETACC		
	LOAD_MODIFIER MTYPE_BLEND,BLEND_CURRENT_CHNL,BLENDG_DSL_ACCEL0,BLENDF_DSL_ACCEL0	; motor under load
;===================================
CH1SC0_SETACC_START
;============================================================================================	
;	PREV_NOTCH is an integer work register value between 0 and 7 representing the nominal 
;	prior running notch. It is set by the continuous running routine.
;==================================================================
CH1SC0_ACC_TESTN	
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH1,COMP_ALL,SKIP_SAME    ;Skip if Equal
		BRANCH_TO	CH1SC0_ACC_TESTN2
	BRANCH_TO	CH1SC0_ACC1
 
CH1SC0_ACC_TESTN2	
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH2,COMP_ALL,SKIP_SAME    ;Skip if Equal
		BRANCH_TO	CH1SC0_ACC_TESTN3
	BRANCH_TO	CH1SC0_ACC2
	
CH1SC0_ACC_TESTN3	
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH3,COMP_ALL,SKIP_SAME    ;Skip if Equal
		BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS	
	BRANCH_TO	CH1SC0_ACC3
	
;=========================================
CH1SC0_INCR_PREV_NOTCH_ACC	
;--------------
	MASK_COMPARE	PREV_NOTCH,TARGET_DATA,WORK_NOTCH,COMP_7LSB,SKIP_GRTR   ;Skip if  previousNotch >= current NOTCH_VAL
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+PREV_NOTCH,+0x10,MAX_NOTCH_VAL	; Increment Prev Notch value
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS
	

;========================================================================
CH1SC0_ACC1		
;========================================================================
	; will rev to next notch; running will lower to correct notch;
	; run1 sounds will only play when CV132 computes notch1;
	; Skipped when prev >= 1  (current must be > 1 to enter ACC (see compare above in normal))
	; Only plays when manually selected notch 2:

	PLAY INC_12,no_loop,loop_STD	
	BRANCH_TO	CH1SC0_INCR_PREV_NOTCH_ACC
	
CH1SC0_ACC2
	PLAY INC_23,no_loop,loop_STD
	
	;decrement delta after throttle change:.............
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL	
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent underflow
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL	

	BRANCH_TO	CH1SC0_INCR_PREV_NOTCH_ACC
	
CH1SC0_ACC3
	PLAY RUN_3,no_loop,loop_STD
	BRANCH_TO	CH1SC0_INCR_PREV_NOTCH_ACC
	
;================================================
; Decelerate CHANGE IN NOTCH DOWN:
CH1SC0_SETDEC
	; Yes Decelerate:
	LOAD_MODIFIER MTYPE_BLEND,BLEND_CURRENT_CHNL,BLENDG_DSL_DECEL0,BLENDF_DSL_DECEL0	; motor under NO  load

	
CH1SC0_DEC_TESTN
;============================================================================================	
;	PREV_NOTCH is an integer work register value between 0 and 7 representing the nominal 
;	prior running notch. It is set by the continuous running routine.
;==================================================================
CH1SC0_DEC_TESTN1		
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH1,COMP_ALL,SKIP_SAME    ;Skip if Equal
		BRANCH_TO	CH1SC0_DEC_TESTN2	
	BRANCH_TO	CH1SC0_DEC1
	
CH1SC0_DEC_TESTN2	
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH2,COMP_ALL,SKIP_SAME    ;Skip if Equal
		BRANCH_TO	CH1SC0_DEC_TESTN3
	BRANCH_TO	CH1SC0_DEC2
	
CH1SC0_DEC_TESTN3	
	BRANCH_TO	CH1SC0_DEC3
	

CH1SC0_DECR_PREV_NOTCH	
;---------------	
CH1SC0_DECR_PREV_NOTCH_DEC1
	MASK_COMPARE	PREV_NOTCH,TARGET_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME   ;Skip if  previousNotch = 0
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+PREV_NOTCH,-0x10,MAX_NOTCH_VAL	;Decrement Prev Notch value by NOTCH_DOWN_VAL
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS

CH1SC0_DEC3
	PLAY DEC_32,no_loop,loop_STD
	BRANCH_TO	CH1SC0_DECR_PREV_NOTCH
	
CH1SC0_DEC2 

	PLAY DEC_21,no_loop,loop_STD	
	BRANCH_TO	CH1SC0_DECR_PREV_NOTCH	
	
CH1SC0_DEC1 
	PLAY DEC_10,no_loop,loop_STD	
	BRANCH_TO	CH1SC0_DECR_PREV_NOTCH	

	
CH1SC0_DEC_EXIT
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS

;============================================================================================	
; ALL AFTER NOTCH CHANGE COMES THRU HERE:
;=====================================
CH1SC0_SETRUN	
;
;UC1.7================	BEGIN_RUNNING_SOUNDS
	;----------------------------------------------
	; no change in notch or speed:
	;----------------------------------------------
CH1SC0_SETRUN_START
; Will catch any missed change and play proper notch sound:
;======================================================================
CH1SC0_RUN_TESTN
	MASK_COMPARE	PREV_NOTCH,IMMED_DATA,NOTCH1,COMP_ALL,SKIP_SAME    ;Skip if Equal
		BRANCH_TO	CH1SC0_RUN_TESTN2
	BRANCH_TO	CH1SC0_RUN1	   
	
CH1SC0_RUN_TESTN2	
	MASK_COMPARE	PREV_NOTCH,IMMED_DATA,NOTCH2,COMP_ALL,SKIP_SAME    ;Skip if Equal
		BRANCH_TO	CH1SC0_RUN_TESTN3
	BRANCH_TO	CH1SC0_RUN2
	
CH1SC0_RUN_TESTN3	
	BRANCH_TO	CH1SC0_RUN3
	

CH1SC0_RUN1
	PLAY  RUN_1,no_loop,loop_STD
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS
	
CH1SC0_RUN2
	PLAY  RUN_2,no_loop,loop_STD
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS
	
CH1SC0_RUN3
	PLAY RUN_3,no_loop,loop_STD
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS
		
	
;=======================================	
CH1SC0_NEXT_RUN_SOUNDS	
	; all must exit here
	END_SOUND	
	
	; ---------------------------------------------------------
	;     END CONTINUOUS SOUNDS
	; ---------------------------------------------------------	
	
;----------------------------------------------------
;  Decrease to idle		removed 10/2013 used only in NV projects not in DN projects
;----------------------------------------------------
;----------------------------------------------------
;IDLE/not moving Phase
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH12_ primary PERSISTENT (NOT INTERRUPTABLE)
	;	this is MODIFIED by DISCRETE notch change logic to vary AMPL with NOTCH setting, RPMs also track
	;----------------------------------------------------
	INITIATE_SOUND	TRIG_SND_ACTV11,RUN_WHILE_TRIG	;		T_SPD_IDLE,RUN_WHILE_TRIG
	; Blend Parms for drifting?
	LOAD_MODIFIER  MTYPE_BLEND, BLEND_CURRENT_CHNL+BLEND_GAIN0+BLEND_FASE0, BLENDG_DSL_DECEL0, BLENDF_DSL_DECEL0   ; Blend Parms for drifting?
	;original base code sets both blend and pitch here
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,IMAXP_DIESEL,DITHERP_DIESEL
	MASK_COMPARE 	VARY_LOAD,IMMED_DATA,SCALE_MID,COMP_ALL,SKIP_GRTR		;prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,-SCALE_INCR_1,SCALE_MID    ;decrement SCALE

	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_MIN	
	
CH12SC0_RUN_IDLE	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x01,0   ;decrease

	;;;;;;;;;;;;;;;;;;;;;; DRIVE HOLD WHEN DELTA >0..........5/30/19
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME		;test for >0 notch....
		BRANCH_TO	CH1SC0_CONTINUOUS_NOTCH		;play run notch >0

CH12SC0_RUN2_IDLE
	;============== low idle is bit0 value 0
	;				regular idle is bit0 value 1
	;				unless ALCO stumble which is bit5
	;==============
	PLAY	RUN_0,loop_till_init_TRIG,loop_INVERT

	BRANCH_TO	CH12SC0_SKIP_STOPR2
;---
CH12SC0_RUN3_LOWIDLE	;low idle here:

	;;;;;LONG LOOP HERE TO IDLE:

	;WHEN LOW IDLE DISABLED PLAY RUN 1: AT CH12SC0_RUN2_IDLE

	;ELSE PLAY RUN 0
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x01,0   ;decrease
 
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_MIN
	PLAY	RUN_0,loop_till_init_TRIG,loop_INVERT
	;cannot play here due to interrupted event: 8/1/19

	;THIS CODE IS INTERRUPTED (see above)
	BRANCH_TO	CH12SC0_SKIP_STOPR2
	
CH12SC0_SKIP_STOPR2
 	END_SOUND
	
	;END CH1_ ===============================================================

	PAGE
	
	;//////////////////// TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

;---------------------------------------------
;  START CHANNEL 2
;---------------------------------------------
CHNL_02_S0
   CHANNEL_START	2
;---------------------------------------------

   ; Enable Motor
   INITIATE_SOUND    TRIG_SND_ACTV11,NORMAL

   LOAD_MODIFIER     MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK ; Enable Motor

   LOAD_MODIFIER     MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0 ; Reset work register
   LOAD_MODIFIER     MTYPE_SNDCV, SCV_139,31,0 ; Set to ~4.5 minutes
   
   END_SOUND

;---------------------------------------------
;  START CHANNEL 3
;---------------------------------------------
CHNL_03_S0
   CHANNEL_START   3
;---------------------------------------------

;---------------------------------------------
;  START CHANNEL 4 - Series 6 Only
;---------------------------------------------
CHNL_04_S0
   CHANNEL_START   4
;---------------------------------------------

;---------------------------------------------
;  START CHANNEL 5 - Series 7 Only
;---------------------------------------------
CHNL_05_S0
   CHANNEL_START   5
;---------------------------------------------

;---------------------------------------------
;  START CHANNEL 6 - Series 7 Only
;---------------------------------------------
CHNL_06_S0
   CHANNEL_START   6
;---------------------------------------------

;  END OF SCHEME 0		
;---------------------------------------------
   SKEME_END   0
;---------------------------------------------
;  MARK END OF SDL	
;---------------------------------------------
   SKEME_START   CLOSE_SKEME   ;REQUIRED
   SKEME_END   CLOSE_SKEME   ;REQUIRED

;This should run in all assmeblies to confirm limits not exceeded for VERSION assumed
VERSION_CHECKS
	IF	ICNT>MAX_INITIATES
   INITIATE EXCEEDS DEFINED LIMIT FOR THIS VERSION
	 ELSE
	ENDIF
	IF	CCNT> (MAX_CHANNELS+1) 
   CHANNELS USED EXCEEDS DEFINED LIMIT FOR THIS VERSION
	 ELSE
	ENDIF
	IF	SCNT>MAX_SCHEMES
   SCHEMES USED EXCEEDS DEFINED LIMIT FOR THIS VERSION
	 ELSE
	ENDIF  
	ICNT = ICNT+1
	TEMP = MAX_INITIATES-ICNT
	MAX_SCHEMES = MAX_SCHEMES +0
	MAX_INITIATES = MAX_INITIATES +0
	MAX_CHANNELS = MAX_CHANNELS +0

END_SDF   ;REQUIRED
   END   ;REQUIRED
