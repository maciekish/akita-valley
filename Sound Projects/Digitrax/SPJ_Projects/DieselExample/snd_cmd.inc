;This information is copyrighted all rights reserved. No commercial use permitted without written permission of copyright holder

        STITLE  	"SOUND PROCESSOR DEFINITION FILE for DIGITRAX scalable sound engine TECHNOLOGY.  (C) 2004,2005,2006 Digitrax Inc."

	PAGE
;SOUND PROCESSOR DEFINITION FILE for DIGITRAX scalable sound processor  TECHNOLOGY.  (C) 2004,2005,2006 Digitrax Inc.

;This contained information is used to compile/assemble a text "Sound Definition Language" (SDL) source into Digitrax proprietary 
; BINARY executable <name>.SDF file suitable to be downloaded and executed by DIGITRAX products based on Scalable Sound Processor (SSP) technology. 
; Binary data files may be interpreted in a processor or machine that emulates and interprets the command binary form defined here 
; to provide correct sound results, or may be executed on a micro-coded processor that decodes and execute these instructions
; as "native" instructions.

;-------------------
TRUE    	EQU     1 		;important bit defs!
FALSE   	EQU     0


;---THESE ARE MINIMUM Assumed values------
;for the SDH104 and similar compatible Digitrax first generation 1.xx products with minimum ram size. Follow-on second and further 
;generation product will expand the minimum capabilies defined for playback, and not limited by the SDF "language".

VERSION_1	EQU	0x10		;ver 1.00
MAX_CHANNELS	EQU	3		;limited by CPU spd  [SECOND generation has 6 channels/voices]
MAX_INITIATES	EQU	64		;limited by RAM array (*3 bytes/INIT), TOTAL all chnls in SCHEME
MAX_SCHEMES	EQU	32		;defined in OPC
MAX_SCHEME_LEN	EQU	0xFFF		;12bit /4K BYTE adr range- INITIATE offset PACKING limit
MAX_SDF_LEN	EQU	0xFFFF		;16 bit SDF size

MIN_WAV_LENGTH	EQU	64		;if this or below (Z=no HNDL) will skip fragment.
MAX_WAV_LENGTH	EQU	0x1FFFF

WAV_SAMPLE_RATE EQU	11025		;standard windows .WAV rate, [SECOND generation can interpolate/MIX faster rates]
WAV_SAMPLE_SIZE	EQU	8		;minimum BITS/sample,[SECOND generation can MIX 8,10,12,16 bits/sample downloads]
MONO		EQU	TRUE		;single channel data,[SECOND generation can unpack/MIX 2+ CHANNEL/STEREO data]			


;ASSUMPTIONS of SDF file and SOUND PROCESSOR:
;-------------------------------------------

;1) CHANNELS are in order, starting with 1, THIs code will assign from 1 to N, irrespective of OPC arg
;2) INITIATORS in a channel are in order of PRIORITY. EARLIER entry is higher priority
;3) LAST .WAV scanned with a handle number will be used
;4) FIRST scheme match in active TYPE/ SDF will be used. No SCHEME match-> NO sound
;5) ERROR in SOUND PROGRAM info etc will be given as beeps at POR, if volume >0
;6) SDF and WAV files can be written in any order up to FLASH mem limit and last version of WAV handle or SDF will be used.
;7) SDF File ends logically with SCHEME =0x1F= CLOSE_SCHEME value

; This Digitrax source file will cross-assemble a binary file with the MACRO capability of any compatible Macroassembler.
;In particular, this source code is compatible and tested with the Microchip Inc. MPASM assembler running on the 
; MPLAB IDE 7.20 development environment, available from www.microchip.com.
	PAGE
;MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
;

;2 BYTE OPCODE  command byte/bits  DEFINITIONS

CMD_MARK_VERSION	EQU	0x82		;define 8 bit VERSION code
CMD_START_CHNL		EQU	0x81		;flag following CMDS are grouped in a discrete CHNL#	
CMD_END_SND		EQU	0x00		;END this chain
CMD_PLAY_SND		EQU	0x40		;Play sound HANDLE, bump to NEXT CMD in chain
CMD_INITIATE_SND	EQU	0x90		;trig INITIATE code for a CMD chain
CMD_BLIP_TRIGGER	EQU	0xB1		;make TRIGGER event
CMD_DELAY_SND		EQU	0xB4		;delay/silence this chnl for ARG*24mS
CMD_SKIP_ON_TRIGGER	EQU	0x04		;TEST trigger state
CMD_GOTO_SND		EQU	0xC0		;continue chain at another CMD offset

;CMD_CALL_SND		EQU	0xA0		;RESERVED-DO Not use version 1.XX
;CMD_REPEAT_SND		EQU	0xB0		;RESERVED-DO Not use version 1.XX
;CMD_PLAY_SND_LAST	EQU	0x60		;play sound HANDLE and END this chain


; 4 BYTE OPCODE  command byte/bits DEFINITIONS


CMD_LOAD_MODIFY		EQU	0xE0		;LOAD task, load up to 3 bytes of ARGS at DEST coded
CMD_SKEME_START		EQU	0xF1		;BEGIN a SCHEME subsection of SDF file, 64KB length coded 
CMD_PLAY_EXPANDED	EQU	0xF2		;long EXPANDED version of PLAY_SOUND cmd
CMD_MASK_COMPARE	EQU	0xF8		;TASK to compare values under mask


;MASKS for following CROSS-ASSEMBLER or MACRO assembler

CMD_MARK_VERSION_MASK	EQU	0xFF		;all 8 bits define 8 bit VERSION code CMD
CMD_END_SND_MASK	EQU	 0xFF		 ;MASK for CMD decoding, 1 bits are active in OPC
CMD_PLAY_SND_MASK	EQU	 0xE0
CMD_PLAY_SND_LAST_MASK	EQU	 0xE0
CMD_START_CHNL_MASK	EQU	 0xFF	
CMD_INITIATE_SND_MASK	EQU	 0xF0
CMD_CALL_SND_MASK	EQU	 0xF0
CMD_REPEAT_SND_MASK	EQU	 0xFF
CMD_BLIP_TRIGGER_MASK 	EQU	 0xFF
CMD_DELAY_SND_MASK	EQU	 0xFC
CMD_SKIP_ON_TRIGGER_MASK EQU	 0xFC
CMD_GOTO_SND_MASK	EQU	 0xF0

CMD_LOAD_MODIFY_MASK	EQU	 0xF0
CMD_SKEME_START_MASK	EQU	 0xFF
CMD_PLAY_EXPANDED_MASK	EQU	 0xFF
CMD_MASK_COMPARE_MASK	EQU	 0xF8
CLOSE_SKEME		EQU	0x1F	;this SCHEME number stops ALL SCHEME evaluation

;=========================================================
;TTTTTTTTTTTTTTTTTTTTTT

;TRIGGER values
;sound TRIGGER PREMPT control bits

; SPECIAL trigs:	POL,0,0,0-0,s,s,s 	0-7 are special trigs
; FN trigger format:	POL,0,0,B-B,b,b,b   	BB 1 of 3 bytes 01,10,11 (24 fns), bbb is bit adr#


NORMAL		EQU	0		;(complete .WAV frag/preempt higher/trigger true)

ZAP		EQU	0x01		;value for ZAP on PREMPT
RUN_WHILE_TRIG	EQU	0x02		;mask/binary values
NO_PREEMPT_TRIG	EQU	0x04
NOT_TRIG	EQU	0x80		;12/13/04, in TRIG argument now

;3 ls bits of INITIATE CMD 1ST byte
CMD_ZAP_BIT		EQU	0	;1=END immed if PREMPTED, 0=finish current .wav frag before END
RUN_WHILE_TRIG_BIT	EQU	1	;1=persistent while TRIGGER is valid, 0=run once
NO_PREEMPT_TRIG_BIT	EQU	2

NOT_TRIG_BIT		EQU	7	;POL bit in TRIG byte 1=reverse POL

;----------
;TRIGGER initiate CODES 

TRIG_NEVER	EQU	0x00		;0 is INACTIVE entry (NULL,not a valid INITIATE coding)
TRIG_MODE_CHNG	EQU	0x01		;
TRIG_MATH	EQU	0x02		;math result trigger
TRIG_DISTANCE	EQU	0x03		;have new increment of distance, use for FUEL low alarm, maintenance ETC

TRIG_SPD_INC	EQU	0x04		;have SPD increase
TRIG_SPD_DEC	EQU	0x05		;have SPD decrease
TRIG_CAM	EQU	0x06		;TRIG on CAM event, either H/W input or AUTO-chuff,
TRIG_NOT_TRIG	EQU	0x07		;brk/loop SPECIAL

;-------
;F1-F28 and F0 map into 3 BYTES of static trig state/fn state bits

;these triggers preCODED for fast TRIG bit adr   POL,0,BBB,bbb:   BB=byte,bbb=bit adr
;Mapped to internal allocations

MOVE_BIT 	EQU	6		;moving BIT
MVFN_MASK	EQU	0x1F		;direct FN bits


TRIG_SF1	EQU	0x08		;F1 change event
TRIG_SF2	EQU	0x09		;byte 1, bit1
TRIG_SF3	EQU	0x0A
TRIG_SF4	EQU	0x0B

TRIG_SF0	EQU	0x0C		;headlight/F0 state
TRIG_DIRNOW_CHNG EQU	0x0D		;have DIR_NOW changed
TRIG_MOVING	EQU	0x0E		;SPD = Non-Zero
TRIG_SND_ACTV11	EQU	0x0F		;DECODER is SPD addressed within CV11 time

;----------
TRIG_SF5	EQU	0x10		;byte 2,bit0
TRIG_SF6	EQU	0x11
TRIG_SF7	EQU	0x12
TRIG_SF8	EQU	0x13

TRIG_SF13	EQU	0x14		;BYTE 2, bit 4, 
TRIG_SF14	EQU	0x15
TRIG_SF15	EQU	0x16
TRIG_SF16	EQU	0x17

;---------
TRIG_SF9	EQU	0x18		;byte 3 bit 0
TRIG_SF10	EQU	0x19
TRIG_SF11	EQU	0x1A
TRIG_SF12	EQU	0x1B

TRIG_SF17	EQU	0x1C		;byte 3, bit 4
TRIG_SF18	EQU	0x1D
TRIG_SF19	EQU	0x1E
TRIG_SF20	EQU	0x1F

;

TRIG_SF21	EQU	0x20		;expanded FUNCTIONS
TRIG_SF22	EQU	0x21
TRIG_SF23	EQU	0x22
TRIG_SF24	EQU	0x23

TRIG_SF25	EQU	0x24
TRIG_SF26	EQU	0x25
TRIG_SF27	EQU	0x26
TRIG_SF28	EQU	0x27

;
;----------------FIRST 8 bsc regs reserved
TRIG_BSC0	EQU	0x28		;expanded FUNCTIONS
TRIG_BSC1	EQU	0x29
TRIG_BSC2	EQU	0x2A
TRIG_BSC3	EQU	0x2B

TRIG_BSC4	EQU	0x2C
TRIG_BSC5	EQU	0x2D
TRIG_BSC6	EQU	0x2E
TRIG_BSC7	EQU	0x2F

;------------------
;these trig lines both REPORT the external inputs to decoder, and CAN also be SET/RESET by SDF trigger commands
; and can be read back by TRANSPONDING as external ALARMS etc

TRIG_IN_BASE	EQU	0x50			;

TRIG_IN_0	EQU	TRIG_IN_BASE+0			;input 0 trig, CAM input if not assigned to STEAM chuf
TRIG_IN_1	EQU	TRIG_IN_BASE+1			;
TRIG_IN_2	EQU	TRIG_IN_BASE+2			;
TRIG_IN_3	EQU	TRIG_IN_BASE+3			;

TRIG_IN_4	EQU	TRIG_IN_BASE+4			;
TRIG_IN_5	EQU	TRIG_IN_BASE+5			;
TRIG_IN_6	EQU	TRIG_IN_BASE+6			;
TRIG_IN_7	EQU	TRIG_IN_BASE+7			;

TRIG_NOTCH_CHNG	EQU	TRIG_IN_7+1			;when notch changes
TRIG_TIME_16PPS	EQU	TRIG_IN_7+2			;16 per sec/64mS rate
TRIG_FACTORY_CVRESET EQU  TRIG_IN_7+3			;have CV8=8/9 request, USER definable CV values
TRIG_OPSPROGWR_CV	  EQU	TRIG_IN_7+4			;have OPSPROG action

;--------------- SCATTER TRIGGER codes, ON/OFF phase per SCATTER task
SCAT_TRIG_BASE	EQU	0x60

TRIG_SCAT0	EQU	SCAT_TRIG_BASE+0		;scatter CHNL0, phase A sel NOT_TRIG_BIT
TRIG_SCAT1	EQU	SCAT_TRIG_BASE+1
TRIG_SCAT2	EQU	SCAT_TRIG_BASE+2
TRIG_SCAT3	EQU	SCAT_TRIG_BASE+3
TRIG_SCAT4	EQU	SCAT_TRIG_BASE+4		;has visible WORK register
TRIG_SCAT5	EQU	SCAT_TRIG_BASE+5		;has visible WORK register
TRIG_SCAT6	EQU	SCAT_TRIG_BASE+6		;has visible WORK register
TRIG_SCAT7	EQU	SCAT_TRIG_BASE+7		;has visible WORK register


;===============================
; encoded TRIGGER values related to SPEED_STATE code

T_SS_BASE	EQU	0x70		;0x20			;base TRIG code for this STATE logic
TSPD		EQU	T_SS_BASE		;base TRIG code for this STATE logic

T_SPD_MUTE	EQU	TSPD+0
T_SPD_TURNON	EQU	TSPD+1
T_SPD_IDLE	EQU	TSPD+2
T_SPD_ACCEL1	EQU	TSPD+3

T_SPD_ACC_CHNG	EQU	TSPD+4		;1st GEN parallel/series chng
T_SPD_ACCEL2	EQU	TSPD+5
T_SPD_IDLEXIT	EQU	TSPD+6		;leaving IDLE state
T_SPD_RUN	EQU	TSPD+7

T_SPD_DECEL1	EQU	TSPD+8
T_SPD_DEC_CHNG	EQU	TSPD+9
T_SPD_DECEL2	EQU	TSPD+10
;T_SPD_DIR_CHNG	EQU	TSPD+11

T_SPD_DEC_IDLE	EQU	TSPD+11
T_SPD_TURNOFF	EQU	TSPD+12
T_SPD_DEC_SP1	EQU	TSPD+13
T_SPD_DEC_SP2	EQU	TSPD+14
T_SPD_DIR_CHNG	EQU	TSPD+15

;============================BREAK/LOOP logic during a .WAV playback==================

;WAVBRK modify flags

loop_STD	EQU	0x00		;default is NO loop logic modification
loop_GLOBAL	EQU	0x10		;assert to loop and UP 1 level...
loop_INVERT	EQU	0x08		;invert/FALSE POL of loop to generate WAV BREAK condition

loop_GLOBAL_BIT  	EQU	4	;bit # assert to loop and UP 1 level...
loop_INVERT_BIT		EQU	3	;bit # invert (NOT/FALSE) Polarity of loop condition/evaluation

;--------- 32 loop event codes
loop_MASK	EQU	0x1F		;mask for VALID BRK rng-5bits

;disable loop- just run/playback .WAV file from SOUND flash or other source
no_loop		EQU	0		;RUN to completeion of WAV segmenT

;--------------
;codes that imply LOOPING of current HANDLE WAV seg till loop WAV BREAK CONDITON met


loop_till_NEVER		EQU	TRIG_NEVER		;same as "no_loop", RUN to completion of WAV segmenT


loop_till_cam		EQU	TRIG_CAM		;loop on CAM event, either H/W input or AUTO-chuff,
loop_till_init_TRIG	EQU	TRIG_NOT_TRIG		;loop until INITIATE TRIGGER condition NOT met

;FUNCTION change events
 

loop_till_F1	EQU	TRIG_SF1		;F1 change event
loop_till_F2	EQU	TRIG_SF2
loop_till_F3	EQU	TRIG_SF3
loop_till_F4	EQU	TRIG_SF4

loop_till_F0		EQU	TRIG_SF0		;headlight
loop_till_DIRNOW_CHNG 	EQU	TRIG_DIRNOW_CHNG	;have DIR_NOW changed
loop_till_MOVING	EQU	TRIG_MOVING		;SPD = Non-Zero
loop_till_SND_ACTV11 	EQU	TRIG_SND_ACTV11		;DECODER is addressed within CV11 time


loop_till_F5	EQU	TRIG_SF5
loop_till_F6	EQU	TRIG_SF6
loop_till_F7	EQU	TRIG_SF7
loop_till_F8	EQU	TRIG_SF8

loop_till_F9	EQU	TRIG_SF9
loop_till_F10	EQU	TRIG_SF10
loop_till_F11	EQU	TRIG_SF11
loop_till_F12	EQU	TRIG_SF12

;ONLY scatter CHNLS 4-7 [also WORK visibles] can be used for TIMED loop break conditions
loop_till_SCAT4		EQU	0x1C		;scatter CHNL4, phase A/B selected by loop_INVERT_BIT
loop_till_SCAT5 	EQU	0x1D		
loop_till_SCAT6 	EQU	0x1E
loop_till_SCAT7 	EQU	0x1F



;----------------
DELAY_GLOBAL	EQU	0x01
DELAY_CV	EQU	0x80			;delay fron SND_CV range
DELAY_THIS	EQU	0x00			;immed data

;----------
;for TEST trigger OPC
TRIG_TRUE	EQU	0x01
TRIG_FALSE	EQU	0x00
ABOVE		EQU	0x02		;this bit forces eval above on SPD state trigs

SK_SENS_BIT	EQU	0
ABOVE_BIT	EQU	1

;=====================================================================
;load MODIFY COMMAND arg/VALUES- allows USER maximal configurability/customization in SDF files
;4byte CMD 	1110-mmmm, ARG1,ARG2,ARG3

;---------------------
;modifier control REG command types
;mmmm target embedded in LOAD MODIFY CMD to direct what following 3 ARG bytes will be loaded TO

MTYPE_TIME	EQU	0	;MODIFY timer
MTYPE_GAIN	EQU	1	;setup a GAIN modify task for current SOUND chain
MTYPE_PITCH	EQU	2	;setup a PITCH modify task for current SOUND chain
MTYPE_BLEND	EQU	3	;setup BLEND logic task

;undefined NOW
MTYPE_SCATTER	EQU	4	;preset a SCATTER channel/task
MTYPE_SNDCV	EQU	5	;modify under MASK SNDCV,ARG1=SNDCV#,ARG2=DATA,ARG3=1bit=chng mask
MTYPE_WORK_IMMED  EQU	6	;modify WORK reg immed data
MTYPE_WORK_INDIRECT EQU	7 	;modify WORK reg indirect from SCV [selectable sound CV] or other WORK reg

;---------------
;values for MATH functions on WORK regs

FMATH_LODE	EQU	0x00	;load DATA,,MATH_FLAG SET shows whole WORK is ZERO- DEFAULT task
FMATH_AND	EQU	0x20	;use to CLR bit(s),MATH_FLAG SET shows whole WORK is ZERO
FMATH_OR	EQU	0x40	;use to SET  bit(S), MATH FLAG SET shows whole WORK is 1's
FMATH_XOR	EQU	0x60	;use to flip bit(s), MATH FLAG SET if all flipped bits are now 0 

;FMATH_ADD	EQU	0x80	;add signed value
FMATH_INTEGRATE	EQU	0xA0	;add signed value to WORK,MATH_FLAG SET if over/underflo,or clamp LIMIT 		
FMATH_TEST_ZERO	EQU	0xC0	; see if WORK under MASK is ZERO, if ZERO set MATH bit

;-----1111----------------
;FIRST modify ARG- COMMAND
GCMD_NONE	EQU		0x00		;NO gain MODIFY task RUN
GCMD_MASK	EQU		0xE0		;3 ms bits 1st CMD decode

;32 analog ARGS 3/21
;TYPE 8 GAIN
IMMED_GAIN_MODIFY   EQU		0x80		;set new GAIN to [CV# 6 bit offset in ARG1] *CV58 , CV120 base
ANALOG_GAIN_MODIFY  EQU		0xA0		;modify INIT GAIN by analog chnl in 5 ls bits
						;AUX1=		AUX2=dither coding


FCMD_NONE	EQU		0x00		;NO PITCH MODIFY task RUN
FCMD_MASK	EQU		0xE0		;3 ms bits 1st CMD decode

;type 8 PITCH
CV_PITCH_MODIFY	    EQU		0x80		;set new PITCH to [CV# 6 bit offset in ARG1] , CV120 base
ANALOG_PITCH_MODIFY EQU		0xA0		;modify INIT PITCH by analog chnl in 5 ls bits
						;AUX1=		AUX2=dither coding

;==================
;32 predefined user visible WORK/STATE regs, use these defined values

;first 16 WORK visible reg address codes.

WORK_SPEED	EQU	0x00		;SOUND target PWM/SPD 
WORK_NOTCH	EQU	0x01		;CHNL# for NOTCH Modified spd info, MODUL0 8
WORK_SERVO	EQU	0x02
WORK_MVOLTS	EQU	0x03

WORK_USER_LINES	EQU	0x05
WORK_TIMEBASE	EQU	0x06		;64mS cntr, ROLL=16 SECS
WORK_STATUS_BITS EQU	0x07		;PRIMARY decoder STATUS bits

WORK_GLBL_GAIN	EQU	0x08		;FULL gain authority
WORK_GAIN_TRIM	EQU	0x09		;signed (bit7) gain trim +/- 25%
WORK_PITCH_TRIM EQU	0x0A		;signed (bit7) pitch trim +/- 25%
WORK_SPEED_DELTA EQU	0x0B		;unsigned SPEED change

;SECOND 16 WORK  visible reg address codes

WORK_SCATTER4	EQU	0x10		;VISIBLE working SCATTER reg
WORK_SCATTER5	EQU	0x11		;VISIBLE working SCATTER reg
WORK_SCATTER6	EQU	0x12		;VISIBLE working SCATTER reg
WORK_SCATTER7	EQU	0x13		;VISIBLE working SCATTER reg

WORK_ACHNL_7F	EQU	0x14		;DT400 VARIABLE whistle CHNL
WORK_ACHNL_7E	EQU	0x15
WORK_SKAT_FAST	EQU	0x16
WORK_SKAT_SLOW	EQU	0x17

WORK_DISTANCE	EQU	0x18
WORK_PEAK_SPD	EQU	0x19
WORK_USER_0	EQU	0x1A			;user  REG
WORK_USER_1	EQU	0x1B			;user  REG

WORK_USER_2	EQU	0x1C			;user  REG
WORK_USER_3	EQU	0x1D			;user  REG
WORK_USER_4	EQU	0x1E			;user  REG
WORK_USER_5	EQU	0x1F			;user  REG


;---------------------
;WORK_STATUS_BITS register bit definitions. data is VOLATILE

WKSB_ANALOG_BIT	EQU	0		;1=is in ANALOG mode [STATUS mirror]
WKSB_ANALOG_MASK EQU	0x01		;bit involved

WKSB_DIRNOW_BIT	EQU	1		;1= rev direction ?[STATUS mirror]
WKSB_DIRNOW_MASK EQU	0x02		;bit involved

WKSB_RUN_BIT	EQU	2		;0= stop motor PWM, non-primemover sounds RUN
WKSB_RUN_MASK EQU	0x04		;bit involved
	
WKSB_ACEL_BIT	EQU	3		;1= is ACCEL state
WKSB_ACEL_MASK EQU	0x08		;bit involved

WKSB_SPDDELTA_BIT  EQU	4		;1= change SPD by unsigned SPD_DELTA work REG only
WKSB_SPDDELTA_MASK EQU	0x10		;bit involved


WKSB_MATH_BIT	EQU	7		;result from last MODIFY math action (can test with
WKSB_MATH_MASK	EQU	0x80		;bit involved

;========================
;SCATTER commands as arg bytes in MTYPE_SCATTER

;is MODIFY SCTR command  -  1110-0100, cccc-XAAA, aaaa-aaaa, bbbb-bbbb, AAA is 1 of 8 scat tasks/chnls


;is SCTR_PERIOD command  -  1110-0100, 0001-PAAA, Srrr-rrrr, IIIw-wwww, 

;AAA is 1 of 8 scat tasks/chnls,  P=WORK scatter POLARITY, 
;S=1 is SOUNDCV src  [0x80=CV141 etc], S=0 rrr-rrrr is RATE in approx 1 sec counts, rate=0 is CNTR hold...
;III=scatter intensity- 000=no SCATTER, w-wwww is a WORK reg# as SCATTER vary data src

;cccc is 16 SCATTER command modes, dddd and eeee are ARG Regs for run modes

SCAT_CMD_PERIOD		EQU	0x20		;command for PERIODIC event, SCALABLE scatter speedup on WORK reg INC
SCAT_CMD_PERIOD_REV	EQU	0x28		;command for PERIODIC event, with SCALABLE scatter slowdn on WORK reg INC

SCAT_CMD_SAWTOOTH	EQU	0x30
SCAT_CMD_SAWTOOTH_REV	EQU	0x38

SCAT_PERIOD_POLARITY_BIT EQU	3		;1= reverse WORK influence
SKATTER_INCREMENT	EQU	8		;phase INCREMENT
	

SCAT_CHNL0		EQU	0x00		;SCATTER task0 
SCAT_CHNL1		EQU	0x01		;SCATTER task1 
SCAT_CHNL2		EQU	0x02		;SCATTER task2 
SCAT_CHNL3		EQU	0x03		;SCATTER task3 
SCAT_CHNL4		EQU	0x04		;SCATTER task4- visible WORK reg
SCAT_CHNL5		EQU	0x05		;SCATTER task5- visible WORK reg
SCAT_CHNL6		EQU	0x06		;SCATTER task6- visible WORK reg
SCAT_CHNL7		EQU	0x07		;SCATTER task7- visible WORK reg


SINTEN_IMMED		EQU	0xE0		;use WORK# as immediate data
SINTEN_HIGH		EQU	0xC0
SINTEN_MID		EQU	0x60		;mid scatter intensity 
SINTEN_LOW		EQU	0x40
SINTEN_MIN		EQU	0x20
SINTEN_OFF		EQU	0x00

;======================
DEFAULT_GLBL_GAIN	EQU	0xC0
MERGE_ALL_MASK		EQU	0		;any 0 bit is involved

;----2222--------------
;2ND modify ARG

SNDCV_SRC	EQU	0x80		;this bit set chngs from IMMED to SND_CV to control EFFECT span
SNDCV_SRC_BIT	EQU	7		;1=ls 7 bits SCV#, 0=ls 7 bits immed arg data

;ls 7 bit ARGS
MAXG_NONE	EQU	0x00		;value of 0 means no scaling, SRC is 100% GAIN
MAXP_NONE	EQU	0x00		;value of 0 means no scaling, SRC is 100% PITCH


;----3333--------------
;3RD modify ARG
;MPUSH		EQU	0x80		;this bit forces LEVEL OVERIDE

DITHER		EQU	0x80		;value to change to DITHER mode
SUMG		EQU	0x40

MIN_DITH	EQU	0x00
LOW_DITH	EQU	0x20
MID_DITH	EQU	0x40
MAX_DITH	EQU	0x60

;MPUSH_BIT	EQU	7

DITHER_BIT	EQU	7
SUM_BIT		EQU	6		;1=add ARG1/2 variation,0=multiply
MG1_BIT		EQU	5		;these code meaning of LO nibble
MG0_BIT		EQU	4


;for CMDS 8/9 ls nibble ARG3 is scaling factor to set CV/IMMED data control SPAN
SCALE_F		EQU	0x0F
SCALE_C		EQU	0x0C
SCALE_8		EQU	0x08
SCALE_6		EQU	0x06
SCALE_5		EQU	0x05
SCALE_4		EQU	0x04
SCALE_2		EQU	0x02

;DITHER ????

DITHERG_WHISTLE	EQU	0x00
DITHERP_WHISTLE	EQU	0x00

DITHERG_DIESEL	EQU	0x00
DITHERP_DIESEL	EQU	0x00

DITHERG_NONE	EQU	0x00
DITHERP_NONE	EQU	0x00

;BLEND logic commnds
; 1110-BLEND, BLEND_CMDS, BLEND_GAIN_ARG, BLEND_FASE_ARG

;BLEND cmds bit encoding
BLEND_TGTMASK	EQU	0xC0		;2 ms bits control Blend target
BLEND_FASEMASK	EQU	0x38		;3  bits FASE_BLEND type, 000=inactive
BLEND_GAINMASK	EQU	0x07		;3 ls bits GAIN_BLEND type, 000=inactive

;MODE0 with ARG=0 is INACTIVE BLEND

BLEND_CURRENT_CHNL	EQU	0x00 & BLEND_TGTMASK
BLEND_ALL		EQU	0x40 & BLEND_TGTMASK

BLEND_GAIN0		EQU	0x00 & BLEND_GAINMASK	;STD logic 0 blend
BLEND_GAIN1		EQU	0x01 & BLEND_GAINMASK	;alt logic 1 blend

BLEND_FASE0		EQU	0x00 & BLEND_FASEMASK	;STD logic 0 blend
BLEND_FASE1		EQU	0x08 & BLEND_FASEMASK	;alt logic 1 blend


BLENDG_DSL_ACCEL0	EQU	0x04		;GAIN rate for loaded DIESEL
BLENDF_DSL_ACCEL0	EQU	0x02		;FASE rate for loaded DIESEL

BLENDG_DSL_ACCEL1	EQU	0x06		;GAIN rate for loaded DIESEL
BLENDF_DSL_ACCEL1	EQU	0x05		;GAIN rate for unloading DIESEL

BLENDG_DSL_DECEL0	EQU	0x09		;GAIN rate for unloading DIESEL
BLENDF_DSL_DECEL0	EQU	0x07		;FASE rate for unloading DIESEL



;COMPARE CMD control bits

TARGET_DATA	EQU	0x00		;second COMPARE ARG is WORK reg or SCV
IMMED_DATA	EQU	0x04		;second COMPARE ARG is IMMEDIATE 8 bit DATA

SKIP_SAME	EQU	0x00
SKIP_RSVD	EQU	0x01		;reserved CONDITION code
SKIP_LESS	EQU	0x02
SKIP_GRTR	EQU	0x03	

COMP_ALL	EQU	0x00		;1 bits= do not include
COMP_7LSB	EQU	0X80


;	CMD_MASK_COMPARE		IMMED_COMP+SKIP_LESS,WORK_SPEED,0x3F,0


;mod for WHISTLE GAIN, chng GAIN/VOL based on ANALOG chnl 
;	LOAD_MODIFIER	MTYPE_GAIN,ANALOG_GAIN_MODIFY+WORK_ACHNL_7F,MAXG_WHISTLE,DITHERG_WHISTLE
;mod for WHISTLE PITCH
;	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_ACHNL_7F,MAXF_WHISTLE,DITHERP_WHISTLE

;mod for DIESEL GAIN
;	LOAD_MODIFIER	MTYPE_GAIN,ANALOG_GAIN_MODIFY+WORK_SPEED,MAXG_DIESEL,DITHERG_DIESEL
;mod for DIESEL PITCH
;	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_GAIN_MODIFY+WORK_SPEED,MAXF_DIESEL,DITHERP_DIESEL


;	LOAD_MODIFIER	MTYPE_GAIN,ANALOG_GAIN_MODIFY+WORK_ACHNL_7F,MAXG_WHISTLE,DITHERG_NONE


;sound COMMAND cv definitions
;THESE first 11 CV"s are FIXED locations used by CODE for fixed meanings/functions
;CV140/SCV_FREEFORM and above are USER/SDF definable

	CBLOCK	0x80		;is CV129

SNDCV_CONFIGA			;CV129= config byte
SNDCV_CONFIGB			;CV130= config byte
SCV_DCONFIG			;CV131= diesel config
SCV_NOTCH			;CV132= typ DIESEL NOTCH rate byte
SNDCV_STEAM			;CV133= typ steam CAM config byte, x80=EXT or 1-127=DRIVER dia in inches
SCV_STGEAR			;CV134= steam gear ratio trim
SCV_MUTE_VOL			;CV135= vol level when MUTE action is triggered, e.g. F8=ON
SCV_MAIN_PITCH			;CV136
SCV_137
SCV_138				;SCV138= 
SCV_DISTANCE_RATE		;SCV139= mask for controlling DISTANCE rate event/trigger
SCV_FREEFORM			;SCV_140, here the SCV's are SDF defined

	ENDC
	
;CV133, SNDCV_STEAM /DRIVER size bit7=1 means EXTERNAL INPUT_0 generates a special CAM code as well

STEAM_CAM_BIT		EQU	7

;=======end of FIXED DATA assignments==============

;=================
;USER choices
;SCAT_AIRCOMP_RATE	EQU	25		;AIR compressor about 2 mins, above 128 is SNDCV foR RATE ARG
;SCAT_DRIER_RATE		EQU	10		;about 10 secs for DRIER
;SCAT_WATERPUMP_RATE	EQU	100
;SCAT_COAL_RATE		EQU	120


;specific DFLT values to scale MAX ranges
MAXG_WHISTLE	EQU	0x6A
MAXP_WHISTLE	EQU	0x07
MAXG_DIESEL	EQU	0x1C
MAXP_DIESEL	EQU	0x3E
MAXG_STEAM	EQU	0x1C
MAXP_STEAM	EQU	0x7E		;was 3E- make more explosive!

