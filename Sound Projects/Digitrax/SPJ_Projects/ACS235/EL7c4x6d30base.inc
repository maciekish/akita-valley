; 12/2/22 v7.0
;disable low idle	
;project id and author id
;volumes
; moving brake fix

 	;THIS IS EL7c4x6d30base.INC

;------------------------------------------
USER_VARIANT_ID		EQU	1		;CV160 is variant# 
USER_VARIATION_ID	EQU	2		;cv156	Variation with Notching if CV121/SW version is 2 or ABOVE
;spare WORK flag BIT
WKSB_INTERLOCK_BIT  EQU	5		;0= MUTE channels 2/3 for chnl 1 DIESEL STATUP
WKSB_INTERLOCK_MASK EQU	0x20		;bit involved
;additional SDF settings
WKSB_IDLELOCK_BIT  	EQU	6		;0= IDLE interlock
WKSB_IDLELOCK_MASK 	EQU	0x40		;bit involved
WKSB_ACCEL_BIT  	EQU	3		;0= IDLE interlock
WKSB_ACCEL_MASK 	EQU	0x08		;bit involved
;USER_INTLK_BITS MASKS:
VARY_DRIFT_MASK7	EQU	0x80	;MASK just do bit7	Vary auto grade crossing 6/25/16	
STATIC_MAINT_MASK6	EQU	0x40	;MASK just do bit6	STATIC MAINTENANCE SOUNDS INTERLOCK
COUPLER_MASK5		EQU	0x20	;MASK just do bit5	AUTO COUPLER INTERLOCK
CYLCOCK_MASK4		EQU	0x10	;MASK just do bit4	OPEN CYLCOCK INTERLOCK
DYN_CTRL_MASK3		EQU	0x08	;MASK just do bit3	NOT_USED INTERLOCK
AUTO_BRAKE_MASK2	EQU	0x04	;MASK just do bit2	AUTOMATIC BRAKING INTERLOCK
START_MASK1			EQU	0x02	;MASK just do bit1	STARTUP SOUNDS INTERLOCK
COMP_MASK0			EQU	0x01	;MASK just do bit0	COMPRESSOR AND AIR SOUNDS INTERLOCK

;VOLUME SCALE CONSTANTS v2.0
SCALE_F	EQU	0x0F
SCALE_E	EQU	0x0E
SCALE_D	EQU	0x0D
SCALE_C	EQU	0x0C
SCALE_B	EQU	0x0B
SCALE_A	EQU	0x0A
SCALE_9		EQU	0x09
SCALE_8		EQU	0x08
SCALE_7		EQU	0x07
SCALE_6		EQU	0x06
SCALE_5		EQU	0x05
SCALE_4		EQU	0x04
SCALE_3		EQU	0x03
SCALE_2		EQU	0x02
SCALE_1		EQU	0x01

SCALE_INCR_1	EQU 0x01
SCALE_INCR_2	EQU 0x02
SCALE_INCR_3	EQU 0x03
SCALE_INCR_4	EQU 0x04
SCALE_INCR_6	EQU 0x06
SCALE_INCR_8	EQU 0x08

;see SCV_LOAD_MIN for Min value to tune minimum volume 1-5 [5] VARY_LOAD / SCALE
SCALE_MIN		EQU	SCALE_8	;motor scale will vary 01-15
SCALE_DRIFT		EQU	SCALE_B
SCALE_MID		EQU	SCALE_C
SCALE_MAX		EQU SCALE_F
;see 	DRIFT_VOLUME,58,0 			;CV191 Drift Volume		0-64	[20]										1-100	[30]

IMAXG_DIESEL	EQU	0x4F	;MAXG_DIESEL	EQU	0x1C
IMAXP_DIESEL	EQU	0x5E	;0x7E	;MAXP_DIESEL	EQU	0x3E
IMAXG_STEAM		EQU	0x4F	;MAXG_STEAM	EQU	0x1C
IMAXP_STEAM		EQU	0x7E	;0x5E	;0x7E	;0x3E	; MAXP_STEAM	EQU	0x7E		;was 3E- make more explosive!
;notch volume changes with change in working notch [used with volume scales above]
COMP_notchbits	EQU	0x8F			;MASK just compare 3 NOTCH bits in COMPARE xNNN-xxxx
COMP_bit7		EQU	0x7F			;MASK just do bit7	0111 1111
COMP_bit6		EQU	0xBF			;MASK just do bit6	1011 1111
COMP_bit5		EQU	0xDF			;MASK just do bit5	1101 1111
COMP_bit4		EQU	0xEF			;MASK just do bit4	1110 1111
COMP_bit3		EQU	0xF7			;MASK just do bit3	1111 0111
COMP_bit2		EQU	0xFB			;MASK just do bit2	1111 1011
COMP_bit1		EQU	0xFD			;MASK just do bit1	1111 1101
COMP_bit0		EQU	0xFE			;MASK just do bit0	1111 1110
;===================================================================================
;	WORK_NOTCH is computed by the decoder and is continuously changed. 
;	The rate of change is managed by the SCV_NOTCH	 (CV132) Controller Notch Rate where the 
;	default value CV132=127 is mid range. Lower values are a slower rate -- higher is faster.  
;	This project uses increments by NOTCH_UP_VAL of decimal 16 [0x10] for each notch.
;	WORK_NOTCH will vary by more than one increment depending on rate of speed change and notch rate.
;	The variable input to the computation is WORK_SPEED which is an internal value from decimal 0-127-192.
;	128 divided by 16 gives the 7 integers for notches 1-8.
;	However the WORK_NOTCH register is incremented by 16's and has the range:
;	NOTCH	Decimal	HEX			Bit			Throttle 
;								7654 3210	Speed (typical) at notch rate =96
;	0		0		(low idle option is at rest, not moving... )
;	1		0		0x00		0000 0000		1-10
;	2		16		0x10		0001 0000		10-20
;	3		32		0x20		0010 0000		21-22
;	4		48		0x30		0011 0000		23-25
;	5		64		0x40		0100 0000		26-35
;	6		80		0x50		0101 0000		36-38
;	7		96		0x60		0110 0000		39-49
;	8		112		0x70		0111 0000		50-99
;	9		128		0x80		1000 0000		
;	10		144		0x90		1001 0000
;	11		160		0xA0		1010 0000
;	12		176		0xB0		1011 0000
;	13		192		0xC0		1100 0000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
;USE CV_169;INIT_IDLE_TRIM	EQU 0x48	;decimal 4, 16, 32, 64
;SCV_SPEED_MODE			;CV155, 0=AUTO, 1=SEMIAUTO, 2=MANUAL NOTCHES
CV155_AUTO	EQU	0		;AUTOMATIC code
CV155_SEMI	EQU	1		;SEMI-AUTO code 
CV155_MANUAL	EQU	2		;MANUAL code 
CV155_NOSLIP	EQU	0		;00=STD=NO wheel slip on SF27, 01=WHEEL slip with SF27 ON
SLIP_VALUE	EQU	0x28		;spd incrememt when slipping
NO_SLIP_VALUE	EQU	0
MAX_SLIP_VAL	EQU	0xFF 	

;CONSTANTS FOR AIR AND BRAKES:
MIN_AIR_PRESS	EQU	40
MIN_BRAKE_PRESS	EQU	60
	PAGE 



;==================================
;SOUND handles ALLOCATION for Series4 is (MAX 64 handles)
;SOUND handles ALLOCATION for Series6 is (MAX 180 handles) 
; handle 0=unused externally, is SILENCE internal value
;----------------------------------
;   SOUND WAVE FILE HANDLES
;----------------------------------
	cblock   0   ; Start assigning location of  Sound Clips 0, 1, 2, etc.
HNDL_SC0_MUTE			;HNDL 0 ALWAYS internal SILENCE value
;	Arrange so that the first 64 handles fit Series4 and the >64 handles fit
;	Series6 (i.e., optional and disused handles at the end)

HNDL_SC0_STARTUP

HNDL_SC0_INC_01
HNDL_SC0_INC_12
HNDL_SC0_INC_23

HNDL_SC0_RUN_IDLE
HNDL_SC0_RUN_1
HNDL_SC0_RUN_2
HNDL_SC0_RUN_3

HNDL_SC0_DEC_32
HNDL_SC0_DEC_21
HNDL_SC0_DEC_10


HNDL_SC0_SHUTDOWN_BEGIN
HNDL_SC0_SHUTDOWN_END

HNDL_SC0_HORN0_START			;default Horn 00
HNDL_SC0_HORN0_LOOP	
HNDL_SC0_HORN0_SUSTAIN
HNDL_SC0_HORN0_END
HNDL_SC0_HORN0_SHORT

HNDL_SC0_HORN1_START	;1ST ALT Horn 01
HNDL_SC0_HORN1_LOOP			
HNDL_SC0_HORN1_SUSTAIN
HNDL_SC0_HORN1_END
HNDL_SC0_HORN1_SHORT

;BELLS
;Chained or selected bells
HNDL_SC0_BELL1
HNDL_SC0_BELL2

HNDL_SC0_ENGINE_BRAKE_BEGIN
HNDL_SC0_ENGINE_BRAKE_LOOP1	; hard braking plays combinations of loop2 and 1
HNDL_SC0_ENGINE_BRAKE_LOOP2
HNDL_SC0_ENGINE_BRAKE_END

HNDL_SC0_ISOLATION_REVERSE_FWD
HNDL_SC0_ISOLATION_REVERSE_REV
HNDL_SC0_ISOLATION_CENTER

HNDL_SC0_BRAKE_SET
HNDL_SC0_BRAKE_RELEASE
HNDL_SC0_BRAKE_BAIL_OFFOFF
HNDL_SC0_BRAKE_BAIL_OFFON

HNDL_SC0_HBRAKE_BEGIN
HNDL_SC0_HBRAKE_LOOP
HNDL_SC0_HBRAKE_RELEASE
HNDL_SC0_HBRAKE_END

HNDL_SC0_DYN_BRAKE_START
HNDL_SC0_DYN_BRAKE_SUSTAIN
HNDL_SC0_DYN_BRAKE_END

HNDL_SC0_AIRCOMP_START

HNDL_SC0_AIRCOMP_RUN1
HNDL_SC0_AIRCOMP_END

HNDL_SC0_TRACTION_START
HNDL_SC0_TRACTION_RUN
HNDL_SC0_TRACTION_END

HNDL_SC0_AIR_LETOFF
HNDL_SC0_SILENCE	;is used to fill sound during Estop...

;==================================================
; Shared Common Sound Files -- for multiple scheme projects
;==================================================
HNDL_SC0_FAN1_START
HNDL_SC0_FAN1_LOOP
HNDL_SC0_FAN1_END

HNDL_SC0_AIR_DRYER1
HNDL_SC0_AIR_DRYER2
HNDL_SC0_AIR_DRYER3

HNDL_SC0_SANDER_START
HNDL_SC0_SANDER_RUN
HNDL_SC0_SANDER_END
;==================================================
HNDL_SC0_COUPLER_LIFT_RING
HNDL_SC0_COUPLER_CLASH
HNDL_SC0_COUPLER_SLACK
HNDL_SC0_UNCOUPLE_BEGIN
HNDL_SC0_UNCOUPLE_DISCONNECT

HNDL_SC0_DEFECT_ALARM
HNDL_SC0_WARNING_ALARM

HNDL_SC0_ENGINE_DOOR_OPEN
HNDL_SC0_ENGINE_DOOR_CLOSE
HNDL_SC0_SHUTTERS

HNDL_SC0_CAB_DOOR_OPEN
HNDL_SC0_CAB_DOOR_CLOSE

HNDL_SC0_USER_SOUND1
HNDL_SC0_USER_SOUND2

HNDL_SC0_WHEEL_FLANGE1
HNDL_SC0_WHEEL_FLANGE2
;==================================================
; OPTIONAL AUX Sounds
HNDL_SC0_AUX_START
HNDL_SC0_AUX_RUN
HNDL_SC0_AUX_END

HNDL_SC0_ABOUT

HNDL_SC0_DEBUG1

;==================================================
; END OF Shared Common Sound Files -- for multiple scheme projects
; Series 6 has more than 64 sound file handles
; Older decoders/projects must not use handles above here!


;==================================================	

	ENDC	
;-----------------------------------------
;   SYSTEM VARIABLES
;-----------------------------------------
;these NEXT 32 EEPROM locs define CV_121_0->CV_128_3, with CV119 as PAGE#
;first entry maps to SND_PAGED_CVS [CV_121_0]		
;--------------------------------------------------------------
; Previously defined SCVs - listed here for reference convenience
;--------------------------------------------------------------
;	==================================  
;(B) Sound FX defined CV's: 	[decoder CV8=8  reset value in brackets]
;CV3 Momentum acceleration rate, 0-63, 63=max, (uses after braking to start) [50]
;CV4 Momentum deceleration rate, 0-63, 63=max, (uses for braking to stop) [40]
;CV7 Version ID 51=Older decoders, 16=Newer SFX2 decoders
;SCV_RESET (CV8)  Reset Decoder values to factory =129=Digitrax
;SCV_19  Advanced Consist [0]
;SCV_21  Advanced Consist SF0-SF8 [248] 
;SCV_22  Advanced Consist SF3-SF7 [52] 
;SCV_54 (CV54) 	 Road or Switching Mode [64,0,1]
;SCV_57  BEMF Consist + Advanced Consist, 0-15 / 0-15, 15=max (14,14; 15,15) [238]
;SCV_MASTER_VOL (CV58) Master Volume Setting
; 	DEFAULT_GLBL_GAIN	 	Predefined as 0xC0
;CV60  Scheme select CV, 0=Diesel 1=Steam	[0]
;CV64  Logic select CV, 0=Logic 64=Normal	[64]
;SCV_USER_SDF_SUB_ID		;CV105 	[xx] Sub ID is SUB id or project version
;SCV_USER_SDF_FIX_ID		;CV106 	[xx] FIX ID is change id or project release
;CV121 Sound Project SDF Version 0,4,8,16	Read Only Value
;CV122 Product TYpe, 12 = Digitrax SFX sound decoder 	Read Only Value
;CV123 Hardware Version, 64= SDXH166 type decoder	Read Only Value. May vary based on SFX2 decoder loaded into.
;				;CV126= DL config byte
;SNDCV_CONFIGA	;CV129= config byte
;SNDCV_CONFIGB	;CV130= config byte
;SCV_DCONFIG	;CV131= diesel config
;CV132 Engine Notch rate		64-127	[96]
;SCV_NOTCH		;CV132= typ DIESEL NOTCH rate byte [64,127]
;SNDCV_STEAM	;CV133= typ steam CAM config byte, x80=EXT or 1-127=DRIVER dia in inches
				;CV133, SNDCV_STEAM /DRIVER size bit7=1 means EXTERNAL INPUT_0 generates a special CAM code as well
;SCV_STGEAR		;CV134= steam gear ratio trim
;SCV_MUTE_VOL	;CV135= vol level when MUTE action is triggered, e.g. SF8=On[0,35]
				;SCV_135 EQU SCV_MUTE_VOL; Global volume when muted. Default 0
;CV135 vol level when MUTE action is triggered, e.g. SF8=ON	[30]
;SCV_MAIN_PITCH	;CV136
;SCV_137		;CV137
;SCV_138		;SCV138= 
;SCV_DISTANCE_RATE	;SCV139= mask for controlling DISTANCE rate event/trigger [31]
					;SCV_139 EQU SCV_DISTANCE_RATE ; Time value till Gauge Trigger. Default 63 = 9 min
;SCV_FREEFORM		;SCV_140+, here the SCV's are SDF defined
;------------------------------------------
;   LOCATION FOR CV ADDRESSES  
; Start assigning (after std Digitrax) for CV140, CV141, etc.
;------------------------------------------
   cblock   SCV_FREEFORM   ; Start assigning (after std Digitrax) for CV140, CV141, etc.
SCV_PRIME_VOLUME    	;CV140 PRIME mover / Chuff vol  0-64	[40]
SCV_BELL_VOLUME    		;CV141 BELL vol					0-64	[30]
SCV_HORNWHISTLE_VOLUME	;CV142 Horn / Whistle Volume	0-64	[60] 
SCV_AIRFEAT_VOLUME    	;CV143 AIR feature volume, brake release	[42] Air sounds Volume 0-64
SCV_BRAKE_VOLUME		;CV144 ENGINE BRAKES - brake volume 0-64 [48]
SCV_MISC_VOLUME			;CV145 Miscellaneous sounds volume 	0-64 [51]
SCV_BELL_RING_RATE   	;CV146 BELL delay (24mS counts)		1-100	[1]
notDRYER_RATE			;notCV147 Time between DRYER cycles	1-100	[27]
notAIRPUMP_START_RATE  ;;notCV148 Compressor/Airpump Start rate		[30]
notAIR_PRESSURE_LIMIT  ;notCV149 Compressor/Airpump run time		[88]
SCV_HORNWHISTLE_SELECT	;CV150 default [00]	WHEN CV150 BIT7=1, enable volume modulation for playable horn 1
	;SELECTED CV150:
			;			Decimal	HEX			Bit
			;								7654 3210
			;	1		0		0x00		0000 0000
			;	2		1		0x01		0000 0001
			;	3		2		0x02		0000 0010
			;	4		3		0x03		0000 0011
			;	5		4		0x04		0000 0100
			;	6		5		0x05		0000 0101
			;	7		6		0x06		0000 0110
			;	8		7		0x07		0000 0111
			;	9		8		0x08		0000 1000
			;	PLAYABLE:
			;	1		128		0x80		1000 0000
			;	2		129		0x81		1000 0001
			;	3		130		0x82		1000 0010
			;	4		131		0x83		1000 0011
			;	5		132		0x84		1000 1000
			;	6		133		0x85		1000 0101
			;	7		134		0x86		1000 0110
			;	8		135		0x87		1000 0111
			;	9		136		0x88		1000 1000
			;			Decimal	HEX			Bit
SCV_COUPLER_THRESHOLD	;CV151 speed limit FOR AUTOMATIC COUPLER/BRAKE on DIR chng. about 1/2MPH per unit	[32] Coupler speed threshold
SCV_USER_SDF_ID			;CV152 User ID defines USER code for this SDF [xx]
SCV_PROJECT_ID			;CV153 Identifies Project within author ID where nnn nnn is serial 1-254 [000]  
SCV_BRAKE_RATE			;CV154 Delay brake apply 1-100	[14] 
SCV_SPEED_MODE			;CV155 Notching mode 0=AUTO, 1=SEMIAUTO, 2=MANUAL NOTCHES [01]
SCV_HORNWHISTLE_DELAY	;CV156 Delay for HORN/WHISTLE start in 27mS VALUES 	[0] 
SCV_157_SEL_BELL        ;CV157 bell selection   [00] 
	;BELL SELECTED CV157:
			;			Decimal	HEX			Bit
			;								7654 3210
			;	1		0		0x00		0000 0000
			;	2		1		0x01		0000 0001
			;	3		2		0x02		0000 0010
			;	4		3		0x03		0000 0011
			;	5		4		0x04		0000 0100
SCV_SPARE158			;CV158 is DIGITRAX RESERVED
SCV_LOAD_MIN			;CV159 Min value to tune minimum volume 5-8 [7]
SCV_USER_SDF_SUB_ID		;CV160 Sub ID is SUB id or project version (MAYBE NOT USED) (USE CV106 INSTEAD) 	[xx]
;	Extended Custom CV values: These are not easily modified by the user as yet (Use SoundLoader to manually override)
SCV_INIT_IDLE_TRIM 		;CV161 Initial Idle Trim 			4-64	[48]
SCV_INJECTOR_VOLUME		;CV162 Water Injector Volume		0-64	[25]
UNUSED163		;CV163 Safety Run Time				0-64	[32]
UNUSED164	;CV164 Level in the guage/ Water Pump Cycle Time				0-64	 [19]
UNUSED165 	;CV165 add fuel to firebox at 150lbs/ Stoker/oil pump run time				0-254	[20]	
UNUSED166		;CV166 Cutoff Speed for wheel slip chuffs		4-9	[9]
SCV_CUTOFF_SPD_AUTO_OPNCYL	;CV167 Cutoff Speed for Auto Cyl. blow down			0-64	[7]
SCV_STOKER_VOLUME			;CV168 Stoker Volume								0-64	[32]
SCV_CUTOFF_SPEED_HIGH		;CV169 Cutoff Speed for high speed chuffs		40-99	[48]	
SCV_CUTOFF_SPEED_LOW		;CV170 Cutoff Speed LOW	skip brake sounds		4-19	[13]
SCV_AUXGEN_VOLUME			;CV171 AUX GEN VOLUME							0-64	[40]
SCV_FAN_VOLUME				;CV172 FAN MOTOR VOLUME							0-64	[30]
SCV_SAFTEY_VOLUME			;CV173 STEAM blowdown volume					0-64	[50]
SCV_DRYER_VOLUME			;CV174 AIR feature volume, DRYER Air sounds Vol 0-64	[49] 
SCV_COMPRESSOR_PUMP_VOLUME	;CV175 Air compressor volume					0-64	[40]
SCV_TRACTION_MOTOR_VOLUME 	;CV176 TRACTION MOTOR VOLUME				0-64	[60]			
SCV_COUPLER_VOLUME			;CV177 Coupler / Slack Volume					0-64	[32]
SCV_BOILER_VOLUME			;CV178 Steam Boiler Volume						0-64	[40]
SCV_BRAKE_DYN_VOLUME		;CV179 Dynamic brake /Dynamo volume				0-64	[55]
SCV_BLOWDOWN_VOLUME 		;CV180 Steam Blowdown/Safety vol				0-64	[59]
SCV_POWER_REVERSER_VOLUME	;CV181 Power Reverser volume					0-64	[40]
SCV_LETOFF_VOLUME			;CV182 brake release Air sounds Volume 		0-64 [32] 
SCV_WATER_COAL_FILL_VOLUME	;CV183 Water fill/Coal fill Volume				0-64	[54]
SCV_STEAM_COCK_THRESH  		;CV184 Steam cock blow down threshold 			1-64	[24]
SCV_FEATURE_DISABLE		;CV185 BIT values to disable features1:
;		bit0 disable HEP diesel =0 AUX GEN			=1
;		bit1 disable hard automatic braking 		=2
;		bit2 disable startup sounds					=4
;		bit3 disable Fans							=8
;		bit4 disable toots on direction change		=16
;		bit5 disable grade crossing random play 	=32
;		bit6 disable bell							=64
;		bit7 fuel prime once for Gen1 diesels OR multiple start warning bells Gen2	=128
;		bit7 steam disable SF11 Open Cyl Cocks		=128	
SCV_FEATURE2_DISABLE	;CV186	0-254 [2]  BIT values to disable additional features2 
;		bit0 disable Diesel Low Idle=1 Enabled=0	=1
;		bit1 disable Chain bells 1-4 				=2
;		bit2 disable STOKER change to SHOVEL		=4
;		bit3 disable AUTO VARY NOTCH				=8
;		bit4 disable Shaft Driven Compressor 		=16
;		bit5 disable ALCO Idle Stumble=32 Disable=0	=32
;		bit6 disable Auto REAR WHISTLE signals: 0=Off, On=64	
;		bit7 disable Dual Prime Movers			0 = Off =128 = Dual On
SCV_FEATURE3_DISABLE	;CV187	0-254 [2]  BIT values to disable additional features3 
;		bit0 disable Short Opt Horn/whistle	 Enabled=0	=1
;		bit1 disable DEMO ALL HornS/whistles Enabled=on=0 =2=disabled
;		bit2 disable Bell on Idle Exit				=4;
;		bit3 disable Motor Lock on BRAKE				=8
;		bit4 disable Startup Muted					=16
;		bit5 disable Dynamic BRAKES					=32
;		bit6 disable 								=64
;		bit7 disable Auto Wheel Slip				=128
SCV_FEATURE4_DISABLE	;CV188	0-254 [0]  BIT values to disable additional features4
;		bit0 disable Traction Motor 		= 1 Disabled
;		bit1 disable HILO HORN						=2
;		bit2 disable 3								=4;
;		bit3 disable 4								=8
;		bit4 disable 5								=16
;		bit5 disable 6								=32
;		bit6 disable 7								=64
;		bit7 disable 8								=128
SCV_ALARM_VOLUME				;CV189 Water Alarm Volume			0-64	[45]
notSTART_SLIP_SPEED			;CV190 Minimum Speed Slip			0-32	[5]
notSTOKER_OIL_PUMP_RUN_TIME 	;CV191 Stoker/oil pump run time				0-254	[20]										1-100	[30]
notDRIFT_VOLUME				;CV191 Drift Volume					0-64	[20]			
SCV_CUTOFF_HORN_SPEED			;CV192 Cutoff Speed for LONG HORN	4-9	[9]
; extended definitions for indexing:
SCV_SOUND_PROJ_TYPE ;CV193	Sound Project TYPE 	0-255	[0]
;Possible Values are :
;	: Index	 Decimal	HEX			Bit
;	:								7654 3210
;	:	1		0		0x00		0000 0000		Steam Coal
;	:	2		1		0x01		0000 0001		Steam Oil
;	:	3		2		0x02		0000 0010		Steam Coal  Articulated
;	:	4		3		0x03		0000 0011		Steam Oil Articulated	
;	:	5		4		0x04		0000 0100		Steam Geared	
;	:	9		8		0x08		0000 1000		Steam non-fired	
;	:	81		80		0x50		0101 0000		Gas Carbody / Doodlebug
;	:	127		128		0x80		1000 0000		Diesel
;	:	145		144		0x90		1001 0000		Diesel Carbody	
;	:	193		192		0xC0		1100 0000		Diesel Dual Motor (startup Linked)	
;	:	225		224		0xE0		1110 0000		Diesel Dual Motor (startup un-Linked)
;	:	241		240		0xF0		1111 0000		Diesel Boxcab
;	:	137		136		0x88		1000 1000		Traction
SCV_SOUND_PROJ_MEMREQ ;CV194 Sound Project Memory Required [1=512kb, 2=1024kb, 3=8mb, 4=4mb, 8=8mb, 16=16mb].
SCV_SOUND_PROJ_JMRIDEF ;CV195 Sound Project JMRI Definition (0-255)
SCV_196 ;RESERVED 
SCV_197 ;RESERVED 
SCV_198 ;RESERVED 
SCV_199 ;RESERVED 
SCV_200 ;RESERVED 
SCV_201 ;RESERVED (121)
SCV_202 ;NON_SOUND_JMRIDEF ; JMRI Definition (0-255) 
SCV_203 ;Decoder Memory Available (read only) [1=512kb, 2=1024kb, 3=8mb, 4=4mb, 8=8mb, 16=16mb].
SCV_TEST ;CV204 TEST (99) 

	ENDC
 
;----------------------------------------
;PREDEFINED lo CV's with .7=1
SCV_CV1		equ	(1-1)
SCV_CV3		equ	(3-1)
SCV_CV4		equ	(4-1)
SCV_CV8		equ	(8-1)

SCV_CV19	equ	(19-1)
SCV_CV21	equ	(21-1)
SCV_CV22	equ	(22-1)
SCV_CV23	equ	(23-1)
SCV_CV24	equ	(24-1)
SCV_CV29	equ	(29-1)
SCV_CV49	equ	(49-1)
SCV_CV54	equ	(54-1)
SCV_CV58	equ	(58-1)
SCV_CV60	equ	(60-1)
SCV_CV64	equ	(64-1)
;test values
SCV_CV105	equ	(105-1)
SCV_CV106	equ	(106-1)
SCV_CV121	equ	(121-1)
SCV_CV122	equ	(122-1)
SCV_CV123	equ	(123-1)
SCV_CV153	equ	(153-1)
SCV_CV160	equ	(160-1)

;SCHEME intents
SCHEME_TYPE_DIESEL	EQU	0	;	

	PAGE
	
;-----------------------------------------
;   SYSTEM VARIABLES
;-----------------------------------------
AuthorCode   EQU   0x19   ; AUTHOR code John McMasters (ID CV=25)
;------------------------------------------------------------------
;   LOCAL VARIABLES (Work Registers and Work_User Memory Registers)                                       
;------------------------------------------------------------------
;WORK_SPEED	  	  EQU	0x00
;WORK_NOTCH	  	  EQU	0x01
;WORK_SERVO	   	  EQU	0x02
;WORK_MVOLTS	  EQU	0x03
;WORK_USER_LINES  EQU	0x05
;WORK_TIMEBASE	  EQU	0x06
;WORK_STATUS_BITS EQU	0x07
;WORK_GLBL_GAIN	  EQU	0x08
;WORK_GAIN_TRIM	  EQU	0x09
;WORK_PITCH_TRIM  EQU	0x0A
;WORK_SPEED_DELTA EQU	0x0B
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
;----------------------------------------
;   LOCAL VARIABLES (Work_User Memory Registers)                                       
;----------------------------------------
COUNT_LOOP		EQU WORK_USER_0 ; used to count n times
PREV_NOTCH		EQU WORK_USER_1 ; used to remember previous Notch	0x00 - 0x70 [run notches 0-8] [increments of 0x10 16decimal]

VARY_LOAD		EQU WORK_USER_2	; used to vary scale from outside motor sound loop 	;range 1-15
AIR_PRESS		EQU WORK_USER_3 ; used to save current air or brake pressure
								;from 90 PSI = Pounds per Square Inch to 140 PSI
;								; used to branch to gain set with constants 
USER_INTLK_BITS	EQU WORK_USER_4	; used for simple On/Off State Machines 
	;================== TABLE OF COMP BITS INVOLVED ==============
	;=== COMP_bit7 =VARY_DRIFT_MASK7=		EQU	0x80	;MASK just do bit7	Vary auto grade crossing 6/25/16	
	;=== COMP_bit6 =STATIC_MAINT_MASK6= 	EQU	0x40	;MASK just do bit6	STATIC MAINTENANCE SOUNDS INTERLOCK
	;=== COMP_bit5 =COUPLER_MASK5=			EQU	0x20	;MASK just do bit5	AUTO COUPLER INTERLOCK
	;=== COMP_bit4 =CYLCOCK_MASK4=			EQU	0x10	;MASK just do bit4	OPEN CYLCOCK INTERLOCK
	;=== COMP_bit3 =DYN_CTRL_MASK3=			EQU	0x08	;MASK just do bit3	NOT_USED INTERLOCK
	;=== COMP_bit2 =AUTO_BRAKE_MASK2=		EQU	0x04	;MASK just do bit2	AUTOMATIC BRAKING INTERLOCK
	;=== COMP_bit1 =START_MASK1=			EQU	0x02	;MASK just do bit1	STARTUP SOUNDS INTERLOCK
	;=== COMP_bit0 =COMP_MASK0=				EQU	0x01	;MASK just do bit0	COMPRESSOR AND AIR SOUNDS INTERLOCK
	;example MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,CYLCOCK_MASK4,~CYLCOCK_MASK4,SKIP_SAME	;TEST BIT4 FOR TRUE WHEN SET=ON / SKIP_LESSSKIP_GRTR  ;		
	;example LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,CYLCOCK_MASK4,MERGE_ALL_MASK	;OR=SETtheBit bit4	OPEN CYLCOCK INTERLOCK
	;example LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~CYLCOCK_MASK4,MERGE_ALL_MASK	;AND=FLIPtheBit bit4	CLOSE CYLCOCK INTERLOCK
	;OR=SETtheBit 
	;AND=FLIPtheBit
	;example interlock is set on bit is =1) (i.e. DRIFTING IS OFF)
	;example LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,VARY_DRIFT_MASK7,MERGE_ALL_MASK	;OR=SETtheBit  bit7	NOT drifting  =0
	;example unset drift interlock: (DRIFTING IS ON)
	;example LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~VARY_DRIFT_MASK7,MERGE_ALL_MASK	;AND=FLIPtheBit  bit7	drifting  ON
	
	PAGE
	
	
;================================================
;Start Scheme 0 DIESEL CV60
;================================================
	ORG	0
;PRIORITIZED initiate LIST and SOUND tasks
;DIESEL RUN scheme 0
	SKEME_START	SCHEME_TYPE_DIESEL
	SDL_VERSION VERSION_2
;------------------------------------------------
;Start Channel 1	
;------------------------------------------------
CH1SC0_BEGIN
	CHANNEL_START	1		;typically PRIME mover channel
;UC1.0================
; ====================================================================
;	1) StartSound routines use default global gain set to 
;		zero and only reset on SF8, SF0, SF1, SF26, or SF29
;	UNLESS:
;CV187 bit values to disable additional features3 	
;		bit4 disable Startup Muted					=16
; ====================================================================
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;     INITIALIZE SETTINGS	
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	; ---------------------------------------------------------
;UC1.1================
;--------------------------------
;THIS SEQUENCE starts the diesel when POR and ADDRESSED (OR DC power ON)
;     INITIALIZE SETTINGS AND PLAY "STARTUP"                              
; ---------------------------------------------------------
;UC1.3================
	INITIATE_SOUND	TRIG_SND_ACTV11,NORMAL+NO_PREEMPT_TRIG	;run ONCE no BRK before CV11 begins IDLE/RUN!!
	LOAD_MODIFIER 	MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,0,0    ;Set value initial
	
CH1SC0_RESELECT
	;bit4 disable CV187 bit4 =0 startup, operator must toggle SF8 to begin...
	MASK_COMPARE 	SCV_FEATURE3_DISABLE,IMMED_DATA,0x00,COMP_bit4,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		GENERATE_TRIGGER  TRIG_SMUTE_ON			;be sure MUTE releases
	
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING					;test MUTE/Moving
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND
	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON			
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND	
		
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_LESS ;when moving
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND
		
CH1SC0_START_MUTE
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,0x01,MERGE_ALL_MASK  ; full MUTE
	;16) 1/31/17 Interlock:	STATUS:	
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+USER_INTLK_BITS,0,0		;CLEAR ALL 8 BITS ;Disable Compressor Run	;Enable Maint Sounds Play

	;=============== LOOP STARTS HERE::::::::
CH1SC0_START1
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,CYLCOCK_MASK4,MERGE_ALL_MASK	;OR=SETtheBit bit4	OPEN CYLCOCK INTERLOCK
	;buffer MUTE action
	PLAY	 HNDL_SC0_MUTE,no_loop,loop_STD		 

	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING					;test MUTE/Moving
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND

	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON						;test MUTE/SF0=ON
		BRANCH_TO	CH1SC0_RESTART_UNMUTE

	;MAY STILL BE MUTE HERE
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_ELIGHTS_ON	; But skip if SF5 is OFF
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
		
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_SMUTE_ON						;test MUTE/SF8=ON, with RESELECT
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
		
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_ESTOP_ON	
	    BRANCH_TO	CH1SC0_RESTART_UNMUTE

	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON						;test MUTE/SF0=ON
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_BELLS_ON						;test MUTE/SF1=ON
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_HORNS_ON						;test MUTE/SF2=ON
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_COUPLE_ON						;test MUTE/SF3=ON
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_DYNBRK_ON						;test MUTE/SF4=ON
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_DITCHL_ON						;test MUTE/SF6=ON
		BRANCH_TO	CH1SC0_RESTART_UNMUTE

	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_LESS ;when moving
		BRANCH_TO	CH1SC0_RESTART_UNMUTE

	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING					;test MUTE/Moving
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
	
	;=================LOOP HERE==================================
	;else no sounds
	BRANCH_TO	CH1SC0_START1	;KEEP LOOPING TILL A F-KEY IS PRESSED
	;=================LOOP HERE==================================
	
CH1SC0_RESTART_UNMUTE								;SF8 OFF-UNMUTE!
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK	;load WORK GLOBAL_GAIN,ends MUTE
	;COULD STILL BE MUTE HERE (SEE ABOVE)
	LOAD_MODIFIER	MTYPE_BLEND,BLEND_CURRENT_CHNL+BLEND_GAIN0+BLEND_FASE0,0,0	;blend immed
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+WORK_PEAK_SPD,00,0	;clear WORK_PEAK_SPD	
		
	MASK_COMPARE WORK_STATUS_BITS,IMMED_DATA,0x0,0xFE,SKIP_SAME	;0	254
    	BRANCH_TO	CH1SC0_SKIP_STARTSOUND
	 
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,1,COMP_ALL,SKIP_LESS ;when moving
	    BRANCH_TO	CH1SC0_SKIP_STARTSOUND
		
CH1SC0_STARTER2
	;=========================
	;test PRIMARY decoder STATUS:	
	;================== TABLE OF COMP BITS INVOLVED ==============
	;=== COMP_bit7 =MATH=		EQU	0x7F			;MASK just do bit7
	;=== COMP_bit6 =IDLELOCK=	EQU	0xBF			;MASK just do bit6
	;=== COMP_bit5 =INTERLOCK=	EQU	0xDF			;MASK just do bit5
	;=== COMP_bit4 =SPDDELTA=	EQU	0xEF			;MASK just do bit4
	;=== COMP_bit3 =ACEL=		EQU	0xF7			;MASK just do bit3
	;=== COMP_bit2 =RUN=		EQU	0xFB			;MASK just do bit2
	;=== COMP_bit1 =DIRNOW=		EQU	0xFD			;MASK just do bit1
	;=== COMP_bit0 =ANALOG=		EQU	0xFE			;MASK just do bit0
;-
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,0x00,~WKSB_ANALOG_MASK,SKIP_SAME ;see if ANALOG=0/DCC 
CH1SC0_DC_START
		BRANCH_TO	CH1SC0_SERVO_ENABLE	;ANALOG status bit=1, is DC -> NO START sequence
;--	otherwise
	 SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON						;test MUTE/SF0=ON
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND
		
CH1SC0_DCC_START
;	skip start sounds when moving or headlight is already on -- to prevent issues when electrical contact is lost
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B	;use VOL control
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK  ;STD full volume 192	

	; Check STARTUP SOUNDS INTERLOCK if Disabled
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,START_MASK1,~START_MASK1,SKIP_SAME	;TEST BIT1 FOR TRUE WHEN SET=ON	
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND
		
	MASK_COMPARE	0x7,IMMED_DATA,0x0,0xFE,SKIP_SAME		;test bit 1 WKSB_ANALOG_BIT  of status bits 0x7
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND
			
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON	; But skip if SF0 was previously on
		BRANCH_TO  CH1SC0_SKIP_STARTSOUND

	;	WKSB_RUN_BIT	EQU	2		;0= stop motor PWM, non-primemover sounds RUN
;******************************************************** UNLESS SF0 IS ON THE MOTOR WILL NOT RUN UNTIL AFTER ALL THE STARTUP SOUNDS ******************************
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+WORK_STATUS_BITS,~WKSB_RUN_MASK,MERGE_ALL_MASK	;CLR the MOTOR run bit to disable	
	
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING	
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND 	
		
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH1SC0_NOT_MOVING_YET
	BRANCH_TO	CH1SC0_SKIP_STARTSOUND
	
CH1SC0_NOT_MOVING_YET
;--
;======================================================
CH1SC0_STARTSOUND
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B	;use VOL control

	;	WKSB_RUN_BIT	EQU	2		;0= stop motor PWM, non-primemover sounds RUN
 	;	bit2 disable startup sounds
	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x01,COMP_bit2,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND 			;Skipped when ARG1 = ARG2	
	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON	; But skip if SF0 was previously on
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND
		
	MASK_COMPARE WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR ;when moving
		BRANCH_TO	CH1SC0_START_ALARM
	BRANCH_TO	CH1SC0_SKIP_STARTSOUND
	
	;;;;;;;;;;;;;;;;; BEGIN STARTUP SOUNDS :::::::::::::::::::	
CH1SC0_START_ALARM	
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B

	;cold start
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~START_MASK1,MERGE_ALL_MASK	;AND=FLIPtheBit bit1 STARTUP SOUNDS INTERLOCK enabled		  

	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_DITCHL_ON						;test MUTE/SF6=ON
		BRANCH_TO	CH1SC0_COLD_START

	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B
	;see if INTERLOCK ON	
	;	PLAY HNDL_SC0_ABOUT,no_loop,loop_STD	;DEBUG2
	; Check STARTUP SOUNDS INTERLOCK if Disabled

CH1SC0_COLD_START
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_MOVING	; when moving there must have been a short out
		PLAY	HNDL_SC0_STARTUP,no_loop,loop_STD	

CH1SC0_FINISH_START

	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B	

	; Check STARTUP SOUNDS INTERLOCK if Disabled
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,START_MASK1,~START_MASK1,SKIP_SAME	;TEST BIT1 FOR TRUE WHEN SET=ON 	
		BRANCH_TO	CH1SC0_FINISH3
	BRANCH_TO	CH1SC0_SKIP_STARTSOUND		 ;interlock is 00, SKIP	
	
CH1SC0_FINISH3
	;Disable Start Sounds Play AGAIN
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,START_MASK1,MERGE_ALL_MASK	;OR=SETtheBit bit1 STARTUP SOUNDS INTERLOCK to DISable =1
	;Allow Compressor Run
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COMP_MASK0,MERGE_ALL_MASK	;OR=SETtheBit BIT0 the COMPRESSOR run bit to enable
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON	; But skip if SF0 was previously on
		BRANCH_TO	CH1SC0_SKIP_STARTSOUND
	
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_SMUTE_ON	; But skip if SF8 was previously MUTED
		;MAX GAIN:	
		LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK  ;STD full volume 192	

	;============== low idle is bit0 value 0
	;				regular idle is bit0 value 1
	;==============

	PLAY	HNDL_SC0_RUN_IDLE,no_loop,loop_STD
	;Allow Compressor Run
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COMP_MASK0,MERGE_ALL_MASK	;OR=SETtheBit BIT0 the COMPRESSOR run bit to enable

CH1SC0_PLAY_LOWIDLE
	;PLAY HNDL_SC0_DEBUG1,no_loop,loop_STD	

	;fix trans low to run1 1/25/18
	MASK_COMPARE 	WORK_PEAK_SPD,IMMED_DATA,0x00,COMP_ALL,SKIP_SAME 	;if PK_SPD=0, no MOVEMENT,no BRAKE/COUPLE!
		LOAD_MODIFIER 	MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,0,0    ;Set value initial

;***************
CH1SC0_SKIP_STARTSOUND
;***************
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,STATIC_MAINT_MASK6,MERGE_ALL_MASK	;OR=SETtheBit bit6	STATIC MAINTENANCE SOUNDS INTERLOCK disabled	
	LOAD_MODIFIER 	MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,0,0    ;Set value initial

;---
CH1SC0_SERVO_ENABLE
	;Notch1=0 is initial default...
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH1,0	;initial value is 0
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,NOTCH1,0	;initial value is 0
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_PEAK_SPD,0x00,0	;initial value is 0

	;this allows the MOTOR to run once PRIME mover is STARTED and audible, [unless defeated by user setup CV]		
	;Allow Compressor Run
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COMP_MASK0,MERGE_ALL_MASK	;OR=SETtheBit BIT0 COMPRESSOR run bit to enable
	GENERATE_TRIGGER  TRIG_FANS_ON	

	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,START_MASK1,MERGE_ALL_MASK	;OR=SETtheBit bit1 STARTUP SOUNDS INTERLOCK disabled	
	;set PRIMARY decoder STATUS:
	;================== TABLE OF COMP BITS INVOLVED ==============
	;=== COMP_bit7 =MATH=		EQU	0x7F			;MASK just do bit7
	;=== COMP_bit6 =IDLELOCK=	EQU	0xBF			;MASK just do bit6
	;=== COMP_bit5 =INTERLOCK=	EQU	0xDF			;MASK just do bit5
	;=== COMP_bit4 =SPDDELTA=	EQU	0xEF			;MASK just do bit4
	;=== COMP_bit3 =ACEL=		EQU	0xF7			;MASK just do bit3
	;=== COMP_bit2 =RUN=		EQU	0xFB			;MASK just do bit2
	;=== COMP_bit1 =DIRNOW=		EQU	0xFD			;MASK just do bit1
	;=== COMP_bit0 =ANALOG=		EQU	0xFE			;MASK just do bit0	
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK	;set the MOTOR run bit to enable
	
CH1SC0_CH2_3_ENABLE	;CH 2 and 3 ENABLE HERE:
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_INTERLOCK_MASK,MERGE_ALL_MASK	;set the INTERLOCK bit to enable CH2/3

CH1SC0_NOTCH_MODE_CHECK
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+WORK_STATUS_BITS,~WKSB_SPDDELTA_MASK,MERGE_ALL_MASK	;assume AUTO mode	

	MASK_COMPARE 	SCV_SPEED_MODE,IMMED_DATA,CV155_MANUAL,COMP_ALL,SKIP_SAME	;CV155, 0=AUTO, 1=SEMIAUTO, 2=MANUAL NOTCHES
		BRANCH_TO	CH1SC0_BEGIN_IDLE			;NOT manual mode!	;Skipped when ARG1 = ARG2
		
;CV155 is MANUAL mode
CH1SC0_MANUAL_NOTCHES
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_SPDDELTA_MASK,MERGE_ALL_MASK	;set MANUAL,remove SPD control 

;---
CH1SC0_BEGIN_IDLE	
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,0,0	;clr SPD DELTA HISTORY
	END_SOUND
 
;================================================================
;UC1.7================

	
; ============================ SF8 RELOCATED HERE ==================================
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	;	SF8 Off, set REG volume - TURN ALL SOUNDS ON 
	; ---------------------------------------------------------
;UC1.1================
   	INITIATE_SOUND	TRIG_SMUTE_ON,NOT_TRIG
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_BELLS_ON	
		BRANCH_TO	CH1SC0_RESTART_UNMUTE
	END_SOUND 	
		
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	;	SF8 OFF, set mute/cv volume - MUTE ALL SOUNDS
	;	ROUTINE WAS CHANNEL1 IN ver4.  
	; --------------------------------------------------
;UC1.2================
  	INITIATE_SOUND	TRIG_SMUTE_ON,NORMAL
	;USE SF8 TO PLAY STARTUP SOUNDS WHEN SERVO MOTOR IS DISABLED:
CH1SC0_MUTE
	;CHECK TO ESTOP AND WITH BRAKES
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_IBRAKES_ON		
		BRANCH_TO	CH1SC0_MUTE_ON0
 	BRANCH_TO	CH1SC0_ESTOP
;-
CH1SC0_MUTE_ON0	;1st time =192, 2nd time=91, 3rd time=32, 4th time=0
	;Disable Compressor Run
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~COMP_MASK0,MERGE_ALL_MASK	;AND=FLIPtheBit bit0 INTERLOCK to DISable =1	
	
	MASK_COMPARE 	WORK_GLBL_GAIN,IMMED_DATA,0x01,COMP_ALL,SKIP_SAME	;When SF8 is toggled twice halve volume:
		BRANCH_TO	CH1SC0_MUTE_ON1	;>0
	BRANCH_TO	CH1SC0_MUTE_ON5	;=0
	
CH1SC0_MUTE_ON1	;>0
	MASK_COMPARE 	WORK_GLBL_GAIN,TARGET_DATA,SCV_MUTE_VOL,COMP_ALL,SKIP_GRTR	;
		BRANCH_TO	CH1SC0_MUTE_ON4	;<32 >=1
	MASK_COMPARE 	WORK_GLBL_GAIN,IMMED_DATA,0x60,COMP_ALL,SKIP_LESS
		BRANCH_TO	CH1SC0_MUTE_ON3	
	 
CH1SC0_MUTE_ON2	;<SCV_MUTE_VOL
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,0x60,MERGE_ALL_MASK  ;half MUTE
	END_SOUND
	
CH1SC0_MUTE_ON3	;<SCV_MUTE_VOL
	LOAD_MODIFIER	MTYPE_WORK_INDIRECT,WORK_GLBL_GAIN,SCV_MUTE_VOL,MERGE_ALL_MASK   ;MUTE volume from SCV135-fixed location!	
	END_SOUND
	
CH1SC0_MUTE_ON4	;>= SCV_MUTE_VOL
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,0x01,MERGE_ALL_MASK  ; full MUTE
	END_SOUND
	
CH1SC0_MUTE_ON5 
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK  ;STD full volume 192
	END_SOUND
	
	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	;	have OPSWR,be sure NOTCH mode is evaluated without RESTART
	;----------------------------------------------------
;UC1.4================
	INITIATE_SOUND	TRIG_OPSPROGWR_CV,NORMAL
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK	;load WORK GLOBAL_GAIN,ends MUTE
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_C
	;error got reset here:
	PLAY	HNDL_SC0_ABOUT,no_loop,loop_STD
	BRANCH_TO	CH1SC0_NOTCH_MODE_CHECK
	END_SOUND	

	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	;	DIESEL shutdown when not addressed any more after CV11 timeout
	;----------------------------------------------------
;UC1.5================
	INITIATE_SOUND	TRIG_SND_ACTV11,NOT_TRIG	
CH1SC0_ENDR1
	MASK_COMPARE 	VARY_LOAD,IMMED_DATA,SCV_LOAD_MIN,COMP_ALL,SKIP_GRTR		;prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,-SCALE_INCR_1,SCALE_MAX	;revised 3/10/17   

	PLAY	HNDL_SC0_RUN_1,no_loop,loop_STD	
	;Disable Compressor Run
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~COMP_MASK0,MERGE_ALL_MASK	;AND=FLIPtheBit bit0 COMPRESSOR AND AIR SOUNDS INTERLOCK to DISable =1
	
	PLAY	HNDL_SC0_SHUTDOWN_BEGIN,no_loop,loop_STD

 	PLAY	HNDL_SC0_SHUTDOWN_END,no_loop,loop_STD
	
CH1SC0_SERVO_DISABLE
	;Disable Compressor Run
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~COMP_MASK0,MERGE_ALL_MASK	;AND=FLIPtheBit bit0 COMPRESSOR AND AIR SOUNDS INTERLOCK to DISable =1
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+WORK_STATUS_BITS,~WKSB_RUN_MASK,MERGE_ALL_MASK	;CLR the BOILER run bit to enable

;---
CH1SC0_MUTE_ACTV11
	BRANCH_TO	CH1SC0_MUTE_ON0			;zero VOLUME after TURNOFF
	END_SOUND	
	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	; EStop Shutdown	(disables motor movement until OFF) when SF15 is on:
	;----------------------------------------------------
   	INITIATE_SOUND	TRIG_ESTOP_ON,NOT_TRIG	;i.e. TOGGLED OFF
CH1SC0_RESTART
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK  ;STD full volume
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_B
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_LIGHTS_ON	; But skip if SF0 was previously off
		BRANCH_TO	CH1SC0_SKIP_RESTART

	;	WKSB_RUN_BIT	EQU	2		;0= stop motor PWM, non-primemover sounds RUN
;****** UNLESS SOME F-KEY IS ON THE SERVO MOTOR WILL NOT RUN UNTIL AFTER ALL THE STARTUP SOUNDS ******************************
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK	;CLR the MOTOR run bit to ENsable	
	
	; Check STARTUP SOUNDS INTERLOCK if Disabled
	;MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,START_MASK1,~START_MASK1,SKIP_SAME	;TEST BIT1 FOR TRUE WHEN SET=ON 	
	;	BRANCH_TO	CH1SC0_RESTART3
	;BRANCH_TO	CH1SC0_SKIP_RESTART
	
CH1SC0_SKIP_RESTART	
	;v5.02 fix restart interlock 7/20/17
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,START_MASK1,MERGE_ALL_MASK	;OR=SETtheBit bit1 STARTUP SOUNDS INTERLOCK Disabled	
	;YES BIG JUMP HERE:
	BRANCH_TO	CH1SC0_RESTART_UNMUTE
	END_SOUND
	
	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH1_ primary PERSISTENT (NOT INTERRUPTABLE)
	; EStop Shutdown	(disables motor movement until OFF) when SF15 is on:
	;----------------------------------------------------
 	INITIATE_SOUND	TRIG_ESTOP_ON,NORMAL
CH1SC0_ESTOP

	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,0x00,0	; set Manual Notch value
	MASK_COMPARE 	VARY_LOAD,IMMED_DATA,SCALE_MID,COMP_ALL,SKIP_GRTR	;SKIP_was >30
		BRANCH_TO	CH1SC0_ESTOP2
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,-SCALE_INCR_1,SCV_LOAD_MIN    ;decrement SCALE
CH1SC0_ESTOP2
	MASK_COMPARE 	VARY_LOAD,IMMED_DATA,SCV_LOAD_MIN,COMP_ALL,SKIP_GRTR		;prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,-SCALE_INCR_1,SCALE_MAX	;revised 3/10/17    

	;decrement SCALE	
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_MID	
	;MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR		; Check if stopped
	;	BRANCH_TO	CH1SC0_ESTOP3
	;BRANCH_TO	CH1SC0_ESTOPPED
CH1SC0_ESTOP3
CH1SC0_ESTOPPED
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,~WKSB_INTERLOCK_MASK,MERGE_ALL_MASK	;set the INTERLOCK bit to DISable CH2/3
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~START_MASK1,MERGE_ALL_MASK	;AND=FLIPtheBit bit1 STARTUP SOUNDS INTERLOCK enabled	
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COUPLER_MASK5,MERGE_ALL_MASK	;OR=SETtheBit bit5


	;SERVO_DISABLE
	;******************************************************** UNLESS SF0 IS ON THE MOTOR WILL NOT RUN UNTIL AFTER ALL THE STARTUP SOUNDS ******************************
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+WORK_STATUS_BITS,~WKSB_RUN_MASK,MERGE_ALL_MASK	;CLR the MOTOR run bit to disable	
	;Disable Compressor Run 
	GENERATE_TRIGGER  TRIG_COMPR_ON		;	
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~COMP_MASK0,MERGE_ALL_MASK	;AND=FLIPtheBit bit1 STARTUP SOUNDS INTERLOCK to DISable =1	
	PLAY 	HNDL_SC0_SHUTDOWN_BEGIN,no_loop,loop_STD
	PLAY 	HNDL_SC0_SHUTDOWN_END,no_loop,loop_STD	
	
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_B
	PLAY	HNDL_SC0_BRAKE_SET,no_loop,loop_STD	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x13,0   ;decrease

	;LONG LOOP HERE FILLS SOUNDS SO THAT UNIT APPEARS TO BE SHUTDOWN:
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_ESTOP_ON		; But skip if F14 was previously off
		LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,0x01,MERGE_ALL_MASK  ; full MUTE
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_ESTOP_ON		; But skip if F14 was previously off
		PLAY 	HNDL_SC0_SILENCE,loop_till_ESTOP_OFF,loop_INVERT
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK	;load WORK GLOBAL_GAIN,ends MUTE
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_B
	
	;PLAY	HNDL_SC0_BRAKE_RELEASE,no_loop,loop_STD
	;play isolation/reverser:
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_POWER_REVERSER_VOLUME,SCALE_A
	PLAY	HNDL_SC0_ISOLATION_CENTER,no_loop,loop_STD		
;-
CH1SC0_ESTOPPED_QUIET	;loops here until SF15 OFF OR BRAKE IS RELEASED
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,0x01,MERGE_ALL_MASK  ; full MUTE
	PLAY 	HNDL_SC0_SILENCE,loop_till_IBRAKES_OFF,loop_INVERT		
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_ESTOP_ON		; But skip if SF15 was previously off
		BRANCH_TO	CH1SC0_ESTOPPED_QUIET	
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK	;load WORK GLOBAL_GAIN,ends MUTE
		
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

	;CHECK HEP HERE:
	;aux or hep is running:
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_AXGEN_ON; But branch if SF30/25 was previously on
		BRANCH_TO	CH1SC0_CONTINUOUS_NOTCH	
	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_RUN8_ON; But branch if SF28 was previously on
		BRANCH_TO	CH1SC0_CONTINUOUS_NOTCH		;END_SOUND	
	
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_MID
	PLAY HNDL_SC0_INC_01,no_loop,loop_STD	; 

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
;---	
	;Increase LOAD
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_TBRAKE_ON		; But skip if SF26 was previously oFF
		BRANCH_TO	CH1SC0_LOAD_RUN_IS_ACCEL
		
	;Decrease LOAD
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_NOTCHDN_ON		; But skip if SF27 was previously oFF
		BRANCH_TO	CH1SC0_LOAD_RUN_IS_DECEL
		
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
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_NOTCHDN_ON		; But skip if SF27 was previously oFF
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,+SCALE_INCR_4,SCALE_MAX   ;Increment	
		
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
	;PLAY HNDL_SC0_DEBUG1,no_loop,loop_STD	
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

	PLAY HNDL_SC0_INC_12,no_loop,loop_STD	
	BRANCH_TO	CH1SC0_INCR_PREV_NOTCH_ACC
	
CH1SC0_ACC2
	PLAY HNDL_SC0_INC_23,no_loop,loop_STD
	
	;decrement delta after throttle change:.............
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL	
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent underflow
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL	

	BRANCH_TO	CH1SC0_INCR_PREV_NOTCH_ACC
	
CH1SC0_ACC3
	PLAY HNDL_SC0_RUN_3,no_loop,loop_STD
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
	PLAY HNDL_SC0_DEC_32,no_loop,loop_STD
	BRANCH_TO	CH1SC0_DECR_PREV_NOTCH
	
CH1SC0_DEC2 

	PLAY HNDL_SC0_DEC_21,no_loop,loop_STD	
	BRANCH_TO	CH1SC0_DECR_PREV_NOTCH	
	
CH1SC0_DEC1 
	PLAY HNDL_SC0_DEC_10,no_loop,loop_STD	
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
	PLAY  HNDL_SC0_RUN_1,no_loop,loop_STD
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS
	
CH1SC0_RUN2
	PLAY  HNDL_SC0_RUN_2,no_loop,loop_STD
	BRANCH_TO	CH1SC0_NEXT_RUN_SOUNDS
	
CH1SC0_RUN3
	PLAY HNDL_SC0_RUN_3,no_loop,loop_STD
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

	MASK_COMPARE 	SCV_FEATURE2_DISABLE,IMMED_DATA,0x01,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;		
		BRANCH_TO	CH12SC0_RUN3_LOWIDLE

CH12SC0_RUN2_IDLE
	;============== low idle is bit0 value 0
	;				regular idle is bit0 value 1
	;				unless ALCO stumble which is bit5
	;==============
	PLAY	HNDL_SC0_RUN_IDLE,loop_till_init_TRIG,loop_INVERT

	BRANCH_TO	CH12SC0_SKIP_STOPR2
;---
CH12SC0_RUN3_LOWIDLE	;low idle here:

	;;;;;LONG LOOP HERE TO IDLE:

	;WHEN LOW IDLE DISABLED PLAY RUN 1: AT CH12SC0_RUN2_IDLE

	;ELSE PLAY RUN 0
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x01,0   ;decrease
 
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_PRIME_VOLUME,SCALE_MIN
	PLAY	HNDL_SC0_RUN_IDLE,loop_till_init_TRIG,loop_INVERT
	;cannot play here due to interrupted event: 8/1/19

	;THIS CODE IS INTERRUPTED (see above)
	BRANCH_TO	CH12SC0_SKIP_STOPR2
	
CH12SC0_SKIP_STOPR2
 	END_SOUND
	
	;END CH1_ ===============================================================

	PAGE
	
	;//////////////////// TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

;------------------------------------------------
;Start Channel 2		whistle/horn
;------------------------------------------------
CH12SC0_BEGIN
	CHANNEL_START	2	;typically TRANSIENT channel for QUEUED non-persistent sounds
;----------------------------------------------------
;---------RUN ONCE!  always LOAD SCATTER tasks when locomotive becomes ACTIVE! (hi priority-no sound)
;	NOTE: SCATTER EVENTS MUST BE INTERLOCKED IN HANDLERS TO PREVENT PLAY AFTER DISABLE.
;----------------------------------------------------
	INITIATE_SOUND	TRIG_SND_ACTV11,NORMAL		;run ONCE only
;------------------------------------------------
 ;added 8/13/06 to holdoff until START is complete
;----------------------------------------------------
CH12SC0_INTERLOCK_HOLD
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		BRANCH_TO	CH12SC0_INTERLOCK_HOLD	;interlock is 00, freeze		

	;---------------------------------------------
	;  Setup random sounds -- scatter		
	;---------------------------------------------		 	;keep scatter channels straight:
	;TRIG_SCAT0	
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,DRYER_RATE,SINTEN_LOW+WORK_SPEED 	;by 47
	;TRIG_SCAT2	FAN1
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL2,0x61,SINTEN_MID+WORK_SPEED		;FAN1 $test6$
	;TRIG_SCAT3	FAN1
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL3,0x51,SINTEN_HIGH+WORK_SPEED	;FAN1  $test6$

	;TRIG_SCAT1	COMPRESSOR
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,AIRPUMP_START_RATE,SINTEN_LOW+WORK_SPEED	;BY 30	
	;TRIG_SCAT4	DRYER
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_SAWTOOTH+SCAT_CHNL4,0x02,SINTEN_OFF 	;diesel Scatter ABT 4SECS

	;  load other timers here:
	;TRIG_SCAT5	TRIM NOTCH AND IDLE
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL5,0x72,SINTEN_MID+WORK_SPEED	;BY CV195
	
	END_SOUND	
	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH2_ high priority/whistle/horn PERSISTENT (NOT INTERRUPTABLE)
	; SF2  horn/ PLAYABLE HORN selection
	;----------------------------------------------------
;UC2.1================
CH2SC0_F2_HORN_STARTER
	INITIATE_SOUND	TRIG_HORNS_ON,NORMAL	;+NO_PREEMPT_TRIG		;BE sure not INTERRUPTED buy HI prioriry  
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x16,0   ;decrease
;-----NEW 8/15/07
;we delay here by SCV156=20 to allow a SHORT blow with SF2 actuation,
; without sustain BURST COMMON to ALL horn types
;if KEY press graeter than CV156 delay then will be
; normal user controllable LONG blow
;if KEY press graeter than CV156 delay then will be normal user controllable LONG blow

CH2SC0_F2_HORN_DELAY	
	DELAY_SOUND		DELAY_CV,SCV_HORNWHISTLE_DELAY,DELAY_GLOBAL
	   
;	===========DO NOT SET GAIN PRIOR TO DELAY ==== LOSS OF GAIN ISSUE 3/30/17
;============================
;-----COMMON horn select and play section
	;HORN SELECTED CV150:
			;			Decimal	HEX			Bit
			;								7654 3210
			;	1		0		0x00		0000 0000
			;	2		1		0x01		0000 0001
			;	3		2		0x02		0000 0010
			;	4		3		0x03		0000 0011
			;	5		4		0x04		0000 0100
			;	6		5		0x05		0000 0101
			;	7		6		0x06		0000 0110
			;	8		7		0x07		0000 0111
			;	9		8		0x08		0000 1000
			;	10		9		0x09		0000 1001
			;	11		10		0x10		0000 1010
			;	12		11		0x11		0000 1011
			;	PLAYABLE:
			;	1		128		0x80		1000 0000
			;	2		129		0x81		1000 0001
			;	3		130		0x82		1000 0010
			;	4		131		0x83		1000 0011
			;	5		132		0x84		1000 0100
			;	6		133		0x85		1000 0101
			;	7		134		0x86		1000 0110
			;	8		135		0x87		1000 0111
			;	9		136		0x88		1000 1000
			;	10		137		0x89		1000 1001
			;			Decimal	HEX		Bit 7654 3210
;============================
;-----COMMON horn select and play section
;----------------------------------------------------
CH2SC0_F2_HORN_SELECT	
;HORN choices 0-1
;-----
;CV150= xxxx-x00X
CH2SC0_F2_HORN_00X
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO		CH2SC0_F2_HORN_001
;----
;CV150=00 DEFAULT HORN0 -- 0x00
CH2SC0_F2_HORN_000
	;==========================SEE if PLAYABLE horn scv  (scv150) BIT7=1
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_HORNWHISTLE_VOLUME,SCALE_F
	;option non-playable when SF11 on:
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FANS_ON		;SUSTAIN HORN ON
		BRANCH_TO	CH2SC0_F2_HORN_000_START
	
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit7,SKIP_SAME	
	; If CV150 BIT7=1, enable volume modulation for playable horn 1
		LOAD_MODIFIER	MTYPE_GAIN,ANALOG_GAIN_MODIFY+WORK_ACHNL_7F,SCV_HORNWHISTLE_VOLUME,SUMG+SCALE_F	;HORN vol	

CH2SC0_F2_HORN_000_START
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_COUPLE_ON	;see if SHORT HORN ON 
		BRANCH_TO	CH2SC0_F2_HORN_000_SHORT
	PLAY		HNDL_SC0_HORN0_START,no_loop,loop_STD
	
CH2SC0_F2_HORN_000_LONG
	;longer when moving:
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_HORN0_LOOP,no_loop,loop_STD

CH2SC0_F2_HORN_000_LONGER
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_HORNS_ON		;see if HORN OFF, just ATTACK/SUSTAIN (SHORT) BLOW!!
		BRANCH_TO	CH2SC0_F2_HORN_000_HOLD
	BRANCH_TO	CH2SC0_F2_HORN_000_DECAY
		
CH2SC0_F2_HORN_000_SHORT
	PLAY	HNDL_SC0_HORN0_SHORT,no_loop,loop_STD
	BRANCH_TO	CH2SC0_F2_HORN_000_EXIT
	
CH2SC0_F2_HORN_000_HOLD
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_HORNS_ON		;see if HORN OFF, just ATTACK/SUSTAIN (SHORT) BLOW!!
		PLAY	HNDL_SC0_HORN0_LOOP,loop_till_HORNS_OFF,loop_INVERT			
		
CH2SC0_F2_HORN_000_DECAY
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FANS_ON	;see if SUSTAIN HORN ON 
		PLAY	HNDL_SC0_HORN0_SUSTAIN,no_loop,loop_STD	
		
	PLAY		HNDL_SC0_HORN0_END,no_loop,loop_STD
		
	MASK_COMPARE 	SCV_FEATURE4_DISABLE,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH2SC0_F2_HORN_001
		
CH2SC0_F2_HORN_000_EXIT
	END_SOUND	;Play HILO Horn when bit1 is on; bypass disabled HiLo sounds

;CV150=01 SECOND HORN2 -- 0x01
CH2SC0_F2_HORN_001
	;==========================SEE if PLAYABLE horn scv  (scv150) BIT7=1
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_HORNWHISTLE_VOLUME,SCALE_F
	;option non-playable when SF11 on:
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FANS_ON		;SUSTAIN HORN ON
		BRANCH_TO	CH2SC0_F2_HORN_001_START

	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit7,SKIP_SAME	
	; If CV150 BIT7=1, enable volume modulation for playable horn 1
		LOAD_MODIFIER	MTYPE_GAIN,ANALOG_GAIN_MODIFY+WORK_ACHNL_7F,SCV_HORNWHISTLE_VOLUME,SUMG+SCALE_F	;HORN vol
	
CH2SC0_F2_HORN_001_START
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_COUPLE_ON	;see if SHORT HORN ON 
		BRANCH_TO	CH2SC0_F2_HORN_001_SHORT
	PLAY		HNDL_SC0_HORN1_START,no_loop,loop_STD	
	
CH2SC0_F2_HORN_001_LONG	
	;longer when moving:
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_HORN1_LOOP,no_loop,loop_STD

CH2SC0_F2_HORN_001_LONGER	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_HORNS_ON		;see if HORN OFF, just ATTACK/SUSTAIN (SHORT) BLOW!!
		BRANCH_TO	CH2SC0_F2_HORN_001_HOLD
	BRANCH_TO	CH2SC0_F2_HORN_001_DECAY
	
CH2SC0_F2_HORN_001_SHORT
	PLAY	HNDL_SC0_HORN1_SHORT,no_loop,loop_STD
	BRANCH_TO	CH2SC0_F2_HORN_001_EXIT
	
CH2SC0_F2_HORN_001_HOLD
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_HORNS_ON		;see if HORN OFF, just ATTACK/SUSTAIN (SHORT) BLOW!!
		PLAY	HNDL_SC0_HORN1_LOOP,loop_till_HORNS_OFF,loop_INVERT			
	
CH2SC0_F2_HORN_001_DECAY
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FANS_ON	;see if SUSTAIN HORN ON 
		PLAY	HNDL_SC0_HORN1_SUSTAIN,no_loop,loop_STD	

	;SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_HORNS_ON		;see if HORN OFF, just ATTACK/SUSTAIN (SHORT) BLOW!!
	PLAY		HNDL_SC0_HORN1_END,no_loop,loop_STD

CH2SC0_F2_HORN_001_EXIT
	END_SOUND 


	
	 ;;;;;;;;;;;;;;end SF2 horns ;;;;;;;;;;;;;;;;;;;;

	 
;UC2.3================
	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH2_ high priority (PERSISTENT (NOT INTERRUPTABLE)
	;---------DISTANCE indicator -change to MILEPOST message...?
	;	ROUTINE WAS CHANNEL2 IN ver4. 
	;----------------------------------------------------
	INITIATE_SOUND	TRIG_DISTANCE,NORMAL
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING
		END_SOUND	
		
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,+10,COMP_ALL,SKIP_LESS ;compare current speed 
		BRANCH_TO	CH2SC0_AUTO_MOVING
	END_SOUND
	
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH2SC0_AUTO_MOVING
	;BRANCH_TO	CH2SC0_AUTO_MOVING1
	
CH2SC0_AUTO_MOVING1
	END_SOUND
	
CH2SC0_AUTO_MOVING
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_UNCOUPLE_ON				;qualify with SF3 on
		PLAY	HNDL_SC0_COUPLER_SLACK,no_loop,loop_STD	
					
CH13SC0_AUTO_GRADE_CROSSING	
	;-------------------------------------------------
	;  DISTANCE EVENT ! Play Horn Distraction
	;-------------------------------------------------
	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x00,COMP_bit5,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
	;Disable horn on distance event			;Skipped when ARG1 = ARG2
		END_SOUND
	 
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_LIGHTS_ON	;when headlamp dark do not play crossing sound:
		END_SOUND
		
	;TEST SPEED HERE:
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,SCV_CUTOFF_SPEED_LOW,COMP_ALL,SKIP_GRTR		; Check if slowed >12
		END_SOUND
		
CH13SC0_AUTO_GRADE_CROSSING2
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_UNCOUPLE_ON		;When coupler link is raised do not play horn
		BRANCH_TO	CH13SC0_GRADE_CROSS_HORN
	END_SOUND
	
;---------------
;UC2.4================
; CROSSING HORN WHEN AUTOMATIC notching selected in SCV155=0
CH13SC0_GRADE_CROSS_HORN	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_IBRAKES_ON		;braking on, check stopped:
		END_SOUND
		
CH13SC0_XING_HORN_SELECT	

;HORN choices 0-1
CH13SC0_XING_HORN_0XX
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO		CH13SC0_XING_HORN_01X

;DEMO		bit1 disable DEMO ALL HornS/whistles Enabled=on=0 =2=disabled
	MASK_COMPARE 	SCV_FEATURE3_DISABLE,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH13SC0_XING_HORN_000			;Disabled	
;-----
;CV150= xxxx-x00X
CH13SC0_XING_HORN_00X
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO		CH13SC0_XING_HORN_001

;----
;*************************** CV150= 00 DEFAULT HORN
CH13SC0_XING_HORN_000
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_HORNWHISTLE_VOLUME,SCALE_F	;default VOLUME	
	PLAY		HNDL_SC0_HORN0_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN0_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN0_END,no_loop,loop_STD	
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING
		BRANCH_TO	CH13SC0_XING_HORN_000_EXIT
	MASK_COMPARE 	SCV_FEATURE4_DISABLE,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH13SC0_XING_HORN_001		;bypass disabled HiLo sounds:

	DELAY_SOUND	DELAY_THIS,(150/24),DELAY_GLOBAL	
	PLAY		HNDL_SC0_HORN0_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN0_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN0_END,no_loop,loop_STD	
	DELAY_SOUND	DELAY_THIS,(150/24),DELAY_GLOBAL	
	PLAY		HNDL_SC0_HORN0_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN0_END,no_loop,loop_STD	
	DELAY_SOUND	DELAY_THIS,(150/24),DELAY_GLOBAL	
	PLAY		HNDL_SC0_HORN0_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN0_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN0_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN0_END,no_loop,loop_STD
	
CH13SC0_XING_HORN_000_EXIT
	MASK_COMPARE 	SCV_FEATURE3_DISABLE,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH13SC0_XING_HORN_001			;Disabled	
	END_SOUND
	
;*************************** CV150= 01 SECOND HORN
CH13SC0_XING_HORN_001
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_HORNWHISTLE_VOLUME,SCALE_F	;default VOLUME
	PLAY		HNDL_SC0_HORN1_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN1_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN1_END,no_loop,loop_STD	
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING
		BRANCH_TO	CH13SC0_XING_HORN_001_EXIT
	MASK_COMPARE 	SCV_FEATURE4_DISABLE,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		END_SOUND

	DELAY_SOUND	DELAY_THIS,(150/24),DELAY_GLOBAL	
	PLAY		HNDL_SC0_HORN1_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN1_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN1_END,no_loop,loop_STD	
	DELAY_SOUND	DELAY_THIS,(150/24),DELAY_GLOBAL	
	PLAY		HNDL_SC0_HORN1_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN1_END,no_loop,loop_STD	
	DELAY_SOUND	DELAY_THIS,(150/24),DELAY_GLOBAL	
	PLAY		HNDL_SC0_HORN1_START,no_loop,loop_STD
	PLAY		HNDL_SC0_HORN1_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN1_LOOP,no_loop,loop_INVERT
	PLAY		HNDL_SC0_HORN1_END,no_loop,loop_STD
	
CH13SC0_XING_HORN_001_EXIT
	END_SOUND

CH13SC0_XING_HORN_01X
	
;----------------- 
;UC2.5================
  INITIATE_SOUND	TRIG_ALTGCS_ON,NORMAL	;RUN_WHILE_TRIG		;continuous NON repeat version
	 BRANCH_TO	CH13SC0_GRADE_CROSS_HORN
	 
	END_SOUND 		;dummy inst to PAD file FOR NEW len	
	
	
		
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH2_ high priority/whistle/horn (INTERRUPTABLE)
	;	Direction changed
	;	DO coupler clank and BRAKE if SF3 is ON,  is dir chng and last PK spd below e.g. CV151
	;----------------------------------------------------
	INITIATE_SOUND	TRIG_DIRNOW_CHNG,NORMAL
	;play isolation/reverser:
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_B
	PLAY	HNDL_SC0_ISOLATION_REVERSE_FWD,no_loop,loop_STD		
	
	;Load Control works with continuous sounds in voice 1	
	;triggers on reversal	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,SCALE_INCR_1,SCALE_MAX	;revised 5/29/17    ;increment SCALE
;---REDUCE DELTA HERE
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,NOTCH4,COMP_ALL,SKIP_LESS		;prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH3,0	; set Manual Notch3 value
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent underflow
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL	
;---
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COUPLER_MASK5,MERGE_ALL_MASK	;OR=SETtheBit bit5	
	;---------------------------------------------
	;  Direction change and SF3 on DO coupler clank and BRAKE if SF3 is ON,  is dir chng and last PK spd below e.g. CV151                       
	;---------------------------------------------
CH2SC0_DIRECTION_CHANGED
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~AUTO_BRAKE_MASK2,MERGE_ALL_MASK	;AND=FLIPtheBit bit2	AUTOMATIC BRAKING INTERLOCK to disable =1
	MASK_COMPARE 	WORK_PEAK_SPD,IMMED_DATA,0,COMP_ALL,SKIP_SAME 	;if PK_SPD=0, no MOVEMENT,no BRAKE/COUPLE!
		SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_IBRAKES_ON	;qualify with BRAKE ON		;Skipped when ARG1 = ARG2
;-			THEN:
CH2SC0_COUPLER_INACTIVE	;NOTE DOUBLE NESTING IFFS
			BRANCH_TO	CH2SC0_DIR_CLR_PEAK_SPD		;SF3=COUPLER OFF or PK_SPD=0, just reset PKD_SPD at DIR change
				; just reset PKD_SPD at DIR change

;----------------------------------------------------
;at this direction change: SF3 is ON and WORK_PEAK_SPD is not ZERO
;----------------------------------------------------
CH2SC0_COUPLER_TEST
	MASK_COMPARE 	WORK_PEAK_SPD,TARGET_DATA,SCV_COUPLER_THRESHOLD,COMP_ALL,SKIP_LESS ;SKIP_SAME ;SKIP_GRTR  ;
		BRANCH_TO	CH2SC0_DIR_CLR_PEAK_SPD			;Skipped when ARG1 < ARG2		;too FAST, just reset PEAK_SPD	
;----------------------------------------------------
;here WORK_PEAK_SPD is less than the SCV chosen, so BRAKE an COUPLE can fire
;----------------------------------------------------
CH2SC0_AUTO_COUPLER
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_B
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_BRAKE_SET,no_loop,loop_STD				;do PLAY_ENGINE_BRAKES
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x18,0   ;decrease
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_B	;CV166 A6	Coupler Slack Volume				0-64	[32]
	PLAY	HNDL_SC0_COUPLER_CLASH,no_loop,loop_STD			;followed by COUPLER crash at DIRECTION chng
	
CH2SC0_DIR_CLR_PEAK_SPD
	;;play reverser:
	;LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_POWER_REVERSER_VOLUME,SCALE_B
	;PLAY	HNDL_SC0_ISOLATION_REVERSE_REV,no_loop,loop_STD		
	PLAY	HNDL_SC0_ISOLATION_CENTER,no_loop,loop_STD		

	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+WORK_PEAK_SPD,00,0	;clear WORK_PEAK_SPD
	END_SOUND

	
;------------------------------
;	TIMED CYCLE OF DRYER (TIMER SCAT0)
;SCATTER chnL 0 is automatic air DRYER
;----------------------------------------------------

;UC2.6================
	INITIATE_SOUND	TRIG_SCAT0,NORMAL
CH15SC0_PLAY_DRYER	
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,COMP_MASK0,~COMP_MASK0,SKIP_SAME	;bit0	COMPRESSOR AND AIR SOUNDS INTERLOCK ON OR OFF 		
		END_SOUND
	;interlock scatter events here in handler to prevent play after disable:
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		END_SOUND			

	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FANS_ON	;qualify with SF11 OFF
		END_SOUND
		
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_DRYER_VOLUME,SCALE_F
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_DRYER_VOLUME,SCALE_E
	MASK_COMPARE COUNT_LOOP,IMMED_DATA,0,COMP_ALL,SKIP_GRTR    ;PLAY if Equal
		BRANCH_TO	CH15SC0_PLAY_DRYER1
	MASK_COMPARE COUNT_LOOP,IMMED_DATA,1,COMP_ALL,SKIP_GRTR    ;PLAY if Equal
		BRANCH_TO	CH15SC0_PLAY_DRYER2
	MASK_COMPARE COUNT_LOOP,IMMED_DATA,2,COMP_ALL,SKIP_GRTR    ;PLAY if Equal
		BRANCH_TO	CH15SC0_PLAY_DRYER3
		
CH15SC0_PLAY_DRYER1
	PLAY	HNDL_SC0_AIR_DRYER1,no_loop,loop_STD
	
CH15SC0_PLAY_DRYER2
	PLAY	HNDL_SC0_AIR_DRYER2,no_loop,loop_STD
	
CH15SC0_PLAY_DRYER3
	PLAY	HNDL_SC0_AIR_DRYER3,no_loop,loop_STD
			
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+COUNT_LOOP,+0x1,4	; Increment 

	END_SOUND

	
;----------------------------------------------------
;	TIMED CYCLE AIR SOUNDS (TIMER SCAT4) 
;----SCATTER chnL 1 is automatic AIR  DRYER at END of COMPRESSOR cycle on CHNL3
;----------------------------------------------------
	INITIATE_SOUND	TRIG_SCAT4,NOT_TRIG	;B phase of SCATTER4	;NORMAL		;RUN_WHILE_TRIG		
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x12,0   ;decrease
	MASK_COMPARE 	VARY_LOAD,IMMED_DATA,8,COMP_ALL,SKIP_GRTR		;SKIP
		BRANCH_TO	CH15SC0_PLAY_DRYER			
	END_SOUND

;;;;;;;;;;;;;;;;;;BRAKING HERE ;;;;;;;;;;;;;;;;;;;;;;;;;
;-------------------------------------------------
;  FUNCTION KEY SF10 - BRAKE SOUNDS !(INTERRUPTABLE)
;	continuous when moving repeat version
;	moved back to CH3 7/26/17 to make brake to stop work consistently
;-------------------------------------------------
	INITIATE_SOUND TRIG_IBRAKES_ON,NORMAL	;;BRAKE +NO_PREEMPT_TRIG	; Play brake sound when BRAKE is pressed 	;Set AUTOMATIC BRAKING INTERLOCK On =1
	;Set AUTOMATIC BRAKING INTERLOCK On =1
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~AUTO_BRAKE_MASK2,MERGE_ALL_MASK	;AND=FLIPtheBit  bit2	AUTOMATIC BRAKING INTERLOCK to DISable =1

	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		END_SOUND	 ;interlock is 00, freeze	  	;Skipped when ARG1 = ARG2

	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_LETOFF_VOLUME,SCALE_B
	PLAY	HNDL_SC0_BRAKE_BAIL_OFFON,no_loop,loop_STD

	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING		;skip when moving...
		BRANCH_TO	CH3SC0_NO_MOVING_BRAKE
	;BRANCH_TO	CH3SC0_MOVING_BRAKE
	
	;=====================  Sometimes WORK_SPEED does NOT reset to zero when the servo is not
	;						moving. Neither will TRIG_MOVING be any different.
	;						So, care must be taken to not have brake sounds at
	;						standstill...
	;=====================
CH3SC0_MOVING_BRAKE
		
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_B
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_BRAKE_SET,no_loop,loop_STD
									  
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x011,0   ;decrease
 	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_F
	PLAY	HNDL_SC0_ENGINE_BRAKE_BEGIN,no_loop,loop_STD
	PLAY	HNDL_SC0_ENGINE_BRAKE_LOOP1,no_loop,loop_STD
	
CH3SC0_SERVO_LOCK	
	;bit3 disable DO NOT USE ENGINE BRAKE FEATURE -- Operator must slow engine...
	MASK_COMPARE 	SCV_FEATURE3_DISABLE,IMMED_DATA,0x01,COMP_bit3,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH3SC0_BRAKE_TO_STOP			;Disabled	

	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING		;skip when moving...
		BRANCH_TO	CH3SC0_BRAKE_LOCK
	
CH3SC0_BRAKE_LOCK
	;APPLY ENGINE BRAKE HERE:
	PLAY	HNDL_SC0_ENGINE_BRAKE_LOOP1,no_loop,loop_STD
	PLAY	HNDL_SC0_ENGINE_BRAKE_LOOP2,no_loop,loop_STD

	;SERVO_DISABLE
	;********************* THE MOTOR WILL NOT RUN UNTIL AFTER BRAKE IS RELEASED ******************************
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+WORK_STATUS_BITS,~WKSB_RUN_MASK,MERGE_ALL_MASK	;CLR the MOTOR run bit to disable
	;;;;;;;;;;;;;;;WKSB_RUN_BIT	EQU	2		;0= stop motor PWM, non-primemover sounds RUN
;---
CH3SC0_BRAKE_STOPPING	
	PLAY	HNDL_SC0_ENGINE_BRAKE_LOOP1,no_loop,loop_STD
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING		;skip when moving...
		BRANCH_TO	CH3SC0_BRAKE_STOPPED 
	;improve by motor lock and real brake slow down to stop (todo 3/8/17)	johnm
	MASK_COMPARE	WORK_SPEED,TARGET_DATA,0x15,COMP_ALL,SKIP_LESS		; Check if slowed >9
		BRANCH_TO	CH3SC0_BRAKE_STOPPING2
	BRANCH_TO	CH3SC0_BRAKE_STOPPED 
	
CH3SC0_BRAKE_STOPPING2
	MASK_COMPARE	WORK_SPEED,TARGET_DATA,0x14,COMP_ALL,SKIP_GRTR		; Check if slowed >9
		BRANCH_TO	CH3SC0_BRAKE_STOPPED
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING		;skip when moving...
		BRANCH_TO	CH3SC0_BRAKE_STOPPED 
		
CH3SC0_BRAKE_TO_STOP
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x12,0   ;decrease

	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL		; Set normal pitch	
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING	
		BRANCH_TO	CH3SC0_BRAKE_STOPPED 
		
	PLAY	HNDL_SC0_ENGINE_BRAKE_LOOP2,no_loop,loop_INVERT
	PLAY	HNDL_SC0_ENGINE_BRAKE_LOOP1,no_loop,loop_INVERT
		
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_IBRAKES_ON
		BRANCH_TO	CH3SC0_NO_MOVING_BRAKE 
 
	;loop here for long stops
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING	
		BRANCH_TO	CH3SC0_BRAKE_TO_STOP  	;LOOP UNTIL SLOWED TO NEAR STOP..........
		
CH3SC0_BRAKE_STOPPED
	PLAY	HNDL_SC0_ENGINE_BRAKE_END,no_loop,loop_STD
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x15,0   ;decrease

	PLAY	HNDL_SC0_BRAKE_SET,no_loop,loop_STD

CH3SC0_NO_MOVING_BRAKE				  
CH3SC0_NO_SERVO_LOCK
	;Set AUTOMATIC BRAKING INTERLOCK On =1
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~AUTO_BRAKE_MASK2,MERGE_ALL_MASK	;AND=FLIPtheBit bit2	AUTOMATIC BRAKING INTERLOCK to disable =1	

	;let underflow:
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,-SCALE_INCR_4,SCALE_MAX	;revised 3/10/17    ;decrement SCALE
	MASK_COMPARE 	VARY_LOAD,IMMED_DATA,SCV_LOAD_MIN,COMP_ALL,SKIP_GRTR		;SKIP
		BRANCH_TO	CH3SC0_NO_UNDERFLOW_LOCK
CH3SC0_FIX_UNDERFLOW_LOCK
	LOAD_MODIFIER	MTYPE_WORK_IMMED,VARY_LOAD,SCV_LOAD_MIN,MERGE_ALL_MASK	;reset to min load
	
CH3SC0_NO_UNDERFLOW_LOCK
	END_SOUND
	 
	 
	  ;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH3_ braking (NOT INTERRUPTABLE)
	;  FUNCTION KEY SF10 - BRAKE SOUNDS !
	;continuous when moving repeat version
	;-------------------------------------------------
	INITIATE_SOUND TRIG_IBRAKES_ON,NOT_TRIG+NO_PREEMPT_TRIG		; Play brake RELEASE when BRAKE toggled off
	;---------------------------------------------
	; toots sound triggered by BRAKE OFF
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_LETOFF_VOLUME,SCALE_B
	PLAY	HNDL_SC0_BRAKE_BAIL_OFFOFF,no_loop,loop_STD
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x17,0   ;decrease

	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x00,COMP_bit4,SKIP_SAME		;SKIP_LESSSKIP_GRTR  
		BRANCH_TO	CH3SC0_EXIT_TOOT	
	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_UNCOUPLE_ON		; Skip if SF3 is not currently ON -- TOOTS when lift ring is up
		BRANCH_TO	CH3SC0_EXIT_TOOT	
		
	MASK_COMPARE WORK_STATUS_BITS,IMMED_DATA,0x02,~WKSB_DIRNOW_MASK,SKIP_SAME		; Skip the next line if loco is set to forward
		BRANCH_TO	CH3SC0_BACKUP
		
	;play isolation/reverser:
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_POWER_REVERSER_VOLUME,SCALE_B
	PLAY	HNDL_SC0_ISOLATION_REVERSE_REV,no_loop,loop_STD		
	;		TWO_TOOTS
	;---------------------------------------------
	;  Forward is two toots                
	;---------------------------------------------
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_HORNWHISTLE_VOLUME,SCALE_F		;Set control for constant PLAY volume
		;HORN SELECTED CV150:
	;			Decimal	HEX			Bit
	;								7654 3210
	;	1		0		0x00		0000 0000
	;	2		1		0x01		0000 0001
	;	3		2		0x02		0000 0010
	;	4		3		0x03		0000 0011
	;	5		4		0x04		0000 0100
	;		TOOT_SELECT
						
CH13SC0_TOOT_SELECT	
;HORN choices 5-8
	;MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit3,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
	;	BRANCH_TO		CH13SC0_TOOT_1XXX
	;MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit2,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
	;	BRANCH_TO		CH13SC0_TOOT_1XX
		
;HORN choices 0-4
CH13SC0_TOOT_0XX
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO		CH13SC0_TOOT_01X

;DEMO		bit1 disable DEMO ALL HornS/whistles Enabled=on=0 =2=disabled
	MASK_COMPARE 	SCV_FEATURE3_DISABLE,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH13SC0_TOOT_000			;Disabled	

CH13SC0_TOOT_00X
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO		CH13SC0_TOOT_001
		
;DEFAULT HORN:	
CH13SC0_TOOT_000
	PLAY	HNDL_SC0_HORN0_SHORT,no_loop,loop_STD				
	PLAY	HNDL_SC0_HORN0_SHORT,no_loop,loop_STD				
	BRANCH_TO	CH3SC0_EXIT_TOOT
	
CH13SC0_TOOT_001
	PLAY	HNDL_SC0_HORN1_SHORT,no_loop,loop_STD
	PLAY	HNDL_SC0_HORN1_SHORT,no_loop,loop_STD
	BRANCH_TO	CH3SC0_EXIT_TOOT
 
CH13SC0_TOOT_01X

CH3SC0_BACKUP
	;play isolation/reverser:
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_POWER_REVERSER_VOLUME,SCALE_B
	PLAY	HNDL_SC0_ISOLATION_REVERSE_REV,no_loop,loop_STD		

	;---------------------------------------------
	;  Reverse is three toots     SO, PLAY ONE AND BRANCH TO TWO           
	;---------------------------------------------
	GENERATE_TRIGGER  TRIG_SAND_ON		;sander under wheels for startup	
	;		ONE_TOOT
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_HORNWHISTLE_VOLUME,SCALE_F		;Set control for constant PLAY volume
	
	;HORN choices 5-8
	;MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit3,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
	;	BRANCH_TO		CH13SC0_RTOOT_1XXX
	;MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit2,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
	;	BRANCH_TO		CH13SC0_RTOOT_1XX
		
;HORN choices 0-4
CH13SC0_RTOOT_0XX
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO		CH13SC0_RTOOT_01X

;DEMO		bit1 disable DEMO ALL HornS/whistles Enabled=on=0 =2=disabled
	MASK_COMPARE 	SCV_FEATURE3_DISABLE,IMMED_DATA,0x00,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH13SC0_RTOOT_000			;Disabled	

CH13SC0_RTOOT_00X
	MASK_COMPARE 	SCV_HORNWHISTLE_SELECT,IMMED_DATA,0x00,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO		CH13SC0_RTOOT_001
		
;DEFAULT HORN:	
CH13SC0_RTOOT_000
	PLAY	HNDL_SC0_HORN0_SHORT,no_loop,loop_STD				
	BRANCH_TO 	CH13SC0_TOOT_000
	
CH13SC0_RTOOT_001
	PLAY	HNDL_SC0_HORN1_SHORT,no_loop,loop_STD
	BRANCH_TO 	CH13SC0_TOOT_001
 
CH13SC0_RTOOT_01X

CH3SC0_EXIT_TOOT
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_LETOFF_VOLUME,SCALE_A
	PLAY	HNDL_SC0_BRAKE_BAIL_OFFOFF,no_loop,loop_STD	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x18,0   ;decrease
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_LETOFF_VOLUME,SCALE_A
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		END_SOUND	 ;interlock is 00, freeze	  	;Skipped when ARG1 = ARG2

	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,AUTO_BRAKE_MASK2,~AUTO_BRAKE_MASK2,SKIP_SAME		;bit2	AUTOMATIC BRAKING INTERLOCK ON OR OFF 		
		PLAY	HNDL_SC0_BRAKE_RELEASE,no_loop,loop_STD		
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x16,0   ;decrease

	;Set AUTOMATIC BRAKING INTERLOCK On =1
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,~AUTO_BRAKE_MASK2,MERGE_ALL_MASK	;OR=SETtheBit bit2	AUTOMATIC BRAKING INTERLOCK to enable =0	
	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_DYNBRK_ON		; lOOP if SF15 is not currently ON
		BRANCH_TO	CH3SC0_RELEASE_HBRAKE
	BRANCH_TO	CH3SC0_BRAKES_RELEASED
	
CH3SC0_RELEASE_HBRAKE
		PLAY	HNDL_SC0_HBRAKE_LOOP,loop_till_DYNBRK_OFF,loop_INVERT	
		
CH3SC0_BRAKES_RELEASED
	;WKSB_RUN_BIT	EQU	2		;0= stop motor PWM, non-primemover sounds RUN

	;SERVO RELEASE
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK	;set the RUN bit to enable 	
	END_SOUND						  
	
	
;----------------------------------------------------
;---- SF20 is ON =disable AIR sound/FEATURES, trig BLOWOFF/DRYER then compressor ONE time
;----------------------------------------------------
	INITIATE_SOUND	TRIG_FANS_ON,NOT_TRIG		;drive AIR cycle when SF11 goes OFF!
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x11,0   ;decrease
		BRANCH_TO	CH8SC0_AIR_PLAY
	END_SOUND
	
;----------------------------------------------------
;---- SF16 is ON = DRYER
;----------------------------------------------------
	INITIATE_SOUND	TRIG_DRYER_ON,NORMAL		;Set Brakes

	BRANCH_TO CH15SC0_PLAY_DRYER
	END_SOUND

	
;----------------------------------------------------
;---- SF17 is ON = Set Brakes
;----------------------------------------------------
	INITIATE_SOUND	TRIG_BRAKEA_ON,NORMAL		;Set Brakes

CH9SC0_BRAKE_SET
	BRANCH_TO CH7SC0_BRAKE
	END_SOUND
	
;-------------------------------
;SF18 is SANDER
;-------------------------------
	INITIATE_SOUND	TRIG_SAND_ON,NORMAL	;RUN_WHILE_TRIG		;continuous NON repeat version
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_A
		PLAY	HNDL_SC0_SANDER_START,no_loop,loop_STD
		;PLAY	HNDL_SC0_SANDER_RUN,no_loop,loop_STD
		PLAY	HNDL_SC0_SANDER_RUN,loop_till_init_TRIG,loop_INVERT
		;PLAY	HNDL_SC0_SANDER_RUN,no_loop,loop_STD
		PLAY	HNDL_SC0_SANDER_END,no_loop,loop_STD
		MASK_COMPARE	WORK_NOTCH,IMMED_DATA,NOTCH7,COMP_ALL,SKIP_GRTR    ;Skip if Equal
			BRANCH_TO	CH13SC0_SANDER2
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH1,0	; set Manual Notch1 value
CH13SC0_SANDER2
	END_SOUND 
	

;-------------------------------
;SF11 High Fan
;-------------------------------
	INITIATE_SOUND	TRIG_FANS_ON,NORMAL		
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_D							 ; Air volume control
		PLAY	HNDL_SC0_FAN1_START,no_loop,loop_STD		
		PLAY	HNDL_SC0_FAN1_LOOP,loop_till_init_TRIG,loop_INVERT	
		PLAY	HNDL_SC0_FAN1_LOOP,no_loop,loop_INVERT	
		PLAY	HNDL_SC0_FAN1_END,no_loop,loop_STD
	END_SOUND 
	
;-------------------------------
;SF24 Reverser
;-------------------------------
	INITIATE_SOUND	TRIG_REVS_ON,NORMAL		
	
		;play isolation/reverser:
		LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_POWER_REVERSER_VOLUME,SCALE_B
		PLAY	HNDL_SC0_ISOLATION_REVERSE_FWD,no_loop,loop_STD		
	END_SOUND 

;-------------------------------
;SF19 
;-------------------------------
	INITIATE_SOUND	TRIG_LETOFF_ON,NORMAL		
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_LETOFF_VOLUME,SCALE_F
		PLAY	HNDL_SC0_AIR_LETOFF,no_loop,loop_STD
	
	END_SOUND 

; -------------------------------------------------------------
;   FUNCTION KEY SF22 - CAB CHATTER !
; -------------------------------------------------------------
	INITIATE_SOUND TRIG_USERSND_ON,NORMAL			; Play sound when SF22 is pressed
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_C	; Volume set by Misc Volume CV
	PLAY HNDL_SC0_USER_SOUND1,no_loop,loop_STD
	;MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x04,COMP_ALL,SKIP_GRTR		; Check if stopped
	;	BRANCH_TO	CH13SC0_NO_CAB_SOUND
	;BRANCH_TO	CH13SC0_CAB_SOUND

CH13SC0_CAB_SOUND
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_A	; Volume set by Misc Volume CV
	PLAY HNDL_SC0_SHUTTERS,no_loop,loop_STD
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_B	; Volume set by Misc Volume CV
	PLAY HNDL_SC0_SHUTTERS,no_loop,loop_STD
	PLAY HNDL_SC0_USER_SOUND2,no_loop,loop_STD

CH13SC0_NO_CAB_SOUND
	BRANCH_TO	CH15SC0_PLAY_DRYER
	END_SOUND
 

; -------------------------------------------------------------
;   FUNCTION KEY TRIG_USERSND2_ON - USER SOUNDS !
; -------------------------------------------------------------
	INITIATE_SOUND TRIG_USERSND2_ON,NORMAL			; Play sound when TRIG_USERSND2_ON is pressed
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_C	; Volume set by Misc Volume CV
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH13SC0_NO_USER_SOUND
	BRANCH_TO	CH13SC0_USER_SOUND
CH13SC0_USER_SOUND
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_USERSND_ON		; Skip if SF22 is not currently oFF
		END_SOUND
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_A	; Volume set by Misc Volume CV
	PLAY HNDL_SC0_ENGINE_DOOR_CLOSE,no_loop,loop_STD
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_B	; Volume set by Misc Volume CV
	PLAY HNDL_SC0_ENGINE_DOOR_OPEN,no_loop,loop_STD
CH13SC0_NO_USER_SOUND
	BRANCH_TO	CH15SC0_PLAY_DRYER
 	END_SOUND
 	
	
	; ---------------------------------------------------------
	;     REMOVE COUPLER SLACK
	; ---------------------------------------------------------
	INITIATE_SOUND T_SPD_IDLEXIT,NORMAL
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_POWER_REVERSER_VOLUME,SCALE_B
	;;play isolation/reverser:
	;PLAY	HNDL_SC0_ISOLATION_REVERSE_FWD,no_loop,loop_STD		
	PLAY	HNDL_SC0_BRAKE_BAIL_OFFOFF,no_loop,loop_STD
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x05,0   ;decrease
	
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_D	;CV166 A6	Coupler Slack Volume				0-64	[32]	
	  
	MASK_COMPARE	WORK_NOTCH,TARGET_DATA,SCV_CUTOFF_SPEED_LOW,COMP_7LSB,SKIP_GRTR   ;Skip if <	SCV_CUTOFF_SPEED_LOW,13
		BRANCH_TO	CH9SC0_REMOVE_COUPLER_SLACK2
	BRANCH_TO	CH9SC0_REMOVED_COUPLER_SLACK		;SKIP UNLESS HAS ACTUALLY MOVED/MOVING; NOT WHEN SF26 IS TOGGLED	
CH9SC0_REMOVE_COUPLER_SLACK2
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING	;No sound unless moving.
		BRANCH_TO	CH9SC0_REMOVED_COUPLER_SLACK	
		
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x03,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH9SC0_REMOVE_COUPLER_SLACK
	BRANCH_TO	CH9SC0_REMOVED_COUPLER_SLACK
			
CH9SC0_REMOVE_COUPLER_SLACK
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_B	;CV166 A6	Coupler Slack Volume				0-64	[32
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL				; Set coupler varied pitch v4.0
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_COUPLER_SLACK,no_loop,loop_STD	
		
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x04,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH9SC0_REMOVED_COUPLER_SLACK2
	BRANCH_TO	CH9SC0_REMOVED_COUPLER_SLACK
	
CH9SC0_REMOVED_COUPLER_SLACK2
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_UNCOUPLE_ON		; Skip if SF3 is not currently OFF -- lift ring must be down
		END_SOUND
		
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_A	;CV166 A6	Coupler Slack Volume				0-64	[32]
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL				;Set coupler varied pitch v4.0	
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_COUPLER_SLACK,no_loop,loop_STD	
		
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x05,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH9SC0_REMOVED_COUPLER_SLACK3	
	BRANCH_TO	CH9SC0_REMOVED_COUPLER_SLACK	
	
CH9SC0_REMOVED_COUPLER_SLACK3
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_A	;CV166 A6	Coupler Slack Volume				0-64	[32]	
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL				; Set coupler varied pitch v4.0
	
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_COUPLER_SLACK,no_loop,loop_STD	
		
CH9SC0_REMOVED_COUPLER_SLACK
	END_SOUND
	
	
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;Straight to 1 manual notch set:
	INITIATE_SOUND TRIG_RUN1_ON,NORMAL

;SF21- Notch 1
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH1,0	
	END_SOUND
	
	
	;;INITIATE_SOUND TRIG_TBRAKE_ON,NORMAL
;SF26- Notch 6
		;;LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH6,0	
	;;END_SOUND	

	;Straight to 7 manual notch set: SELECT SF27...
	;;INITIATE_SOUND TRIG_NOTCHDN_ON,NORMAL
;SF27- Notch 7
		;;LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH7,0	
	;;END_SOUND	
	
	;Straight to 8 manual notch set: SELECT SF28...
	INITIATE_SOUND TRIG_RUN8_ON,NORMAL
;SF28- Notch 8
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH8,0	
	END_SOUND	
	

	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH2_ high priority/whistle/horn PERSISTENT (NOT INTERRUPTABLE)  
	; SF26 NOTCH-UP
	;----------------------------------------------------
	INITIATE_SOUND	TRIG_NOTCHUP_ON,NORMAL
	;-------------------------------------------------
	;SF27 ON Is now notch down for EVENT, integrate -VE in SPEED_DELTA
	;MANUAL mode is ALREADY evaluated at CV11/START and OPSCV_WR that sets WKSB_SPDDELTA_BIT
	;----------------------------------------------------	
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MAX_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent overflow
	;FIX 1/25/18
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,+0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL
	;reset when long at idle...
	;MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MAX_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent underflow
	;	END_SOUND
		
	END_SOUND

	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH2_ high priority/whistle/horn PERSISTENT (NOT INTERRUPTABLE)  
	; SF27 NOTCH-DOWN
	;----------------------------------------------------
	INITIATE_SOUND	TRIG_NOTCHDN_ON,NORMAL
	;-------------------------------------------------
	;SF27 ON Is now notch down for EVENT, integrate -VE in SPEED_DELTA
	;MANUAL mode is ALREADY evaluated at CV11/START and OPSCV_WR that sets WKSB_SPDDELTA_BIT
	;----------------------------------------------------	
	;PLAY HNDL_SC0_DEBUG1,no_loop,loop_STD	

	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent underflow
	;FIX 1/25/18
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL	;decrement by NOTCH_DOWN_VAL
	;reset when long at idle...
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_SAME		;prevent underflow
		END_SOUND
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+WORK_PEAK_SPD,00,0	;clear WORK_PEAK_SPD	
		
	END_SOUND

	

;---------------------------------------------
;  Decelerate  brake sounds:                  
;---------------------------------------------
	INITIATE_SOUND T_SPD_DECEL1,NORMAL		
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_GRTR		;prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL   ;DEcrement NOTCH_DOWN_VAL	

	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_GRTR		;prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL   ;DEcrement NOTCH_DOWN_VAL	

	;SKIP UNLESS HAS ACTUALLY MOVED/MOVING
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING	
		BRANCH_TO	CH7SC0_NOT_BRAKE
		
	;PLAY HNDL_SC0_DEBUG1,no_loop,loop_STD	
	; Decelerate:
	;SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_IBRAKES_ON	; But skip if BRAKE was previously on -- BRAKE enables auto brake sounds
	;	BRANCH_TO	CH7SC0_NOT_BRAKE	
		
	;when coupler lift ring is up do not play brake squeel:
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_UNCOUPLE_ON	; But skip if SF3 was previously OFF -- SF3 ON enables auto brake sounds
		BRANCH_TO	CH7SC0_NOT_BRAKE	
		
	;MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR		; Check if stopped
	;	BRANCH_TO	CH7SC0_BRAKE2
	;BRANCH_TO	CH7SC0_NOT_BRAKE
	
CH7SC0_BRAKE2
	; Check AUTOMATIC BRAKING INTERLOCK if Disabled
	;PLAY HNDL_SC0_ABOUT,no_loop,loop_STD	;debug2

	;compare current brake cycle count when >0 continue to brake
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,AUTO_BRAKE_MASK2,~AUTO_BRAKE_MASK2,SKIP_SAME	;BIT2 ON OR OFF 
		BRANCH_TO	CH7SC0_TEST_BRAKE		;Skipped when ARG1 = ARG2
	BRANCH_TO	CH7SC0_NOT_BRAKE
	

CH7SC0_TEST_BRAKE
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,SCV_COUPLER_THRESHOLD,COMP_ALL,SKIP_LESS	;28
		BRANCH_TO	CH7SC0_NOT_BRAKE			;Skipped when ARG1 < ARG2	 

;								SPEED THRESHOLD FOR HARD BRAKE 14-19		
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,SCV_BRAKE_RATE,COMP_ALL,SKIP_LESS	;19
		BRANCH_TO	CH7SC0_NOT_BRAKE			;Skipped when ARG1 < ARG2	 

CH7SC0_BRAKE
	MASK_COMPARE 	WORK_SPEED_DELTA,IMMED_DATA,MIN_NOTCH_VAL,COMP_ALL,SKIP_GRTR		;prevent underflow
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,-0x10,MAX_NOTCH_VAL   ;DEcrement NOTCH_DOWN_VAL	
	
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING	
		BRANCH_TO	CH7SC0_NOT_BRAKE

	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_E
	PLAY	HNDL_SC0_ENGINE_BRAKE_BEGIN,no_loop,loop_STD		; light brake sound	
	PLAY	HNDL_SC0_ENGINE_BRAKE_LOOP1,no_loop,loop_STD
CH7SC0_QUICK_BRAKE
	;PLAY HNDL_SC0_DEBUG1,no_loop,loop_STD	
	PLAY	HNDL_SC0_ENGINE_BRAKE_END,no_loop,loop_STD
	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x05,0   ;decrease
	
CH7SC0_NOT_BRAKE
	;Set AUTOMATIC BRAKING INTERLOCK Off =0
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,~AUTO_BRAKE_MASK2,MERGE_ALL_MASK	;OR=SETtheBit  bit2	AUTOMATIC BRAKING INTERLOCK to enable =0

	END_SOUND
	
	;END CH2_==============================================================	
		PAGE
		
		
;------------------------------------------------
;Start Channel 3		
;------------------------------------------------
	CHANNEL_START	3	;typically for PERSISTENT sounds other than PRIME mover
CH3SC0_BEGIN        
		
;--------------------------------------------------------------------
;added 8/13/06 to holdoff until DIESEL START is complete
;--------------------------------------------------------------------
	INITIATE_SOUND	TRIG_SND_ACTV11,NORMAL				;run ONCE only
CH3SC0_INTERLOCK_HOLD
	DELAY_SOUND DELAY_THIS,24,DELAY_GLOBAL
	
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		BRANCH_TO	CH3SC0_INTERLOCK_HOLD	 ;interlock is 00, freeze	  	;Skipped when ARG1 = ARG2
	;DO NOT reset blend, gain, pitch here:
	END_SOUND
	
	
;BEGIN CH3_=============================

	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH3_ (NOT INTERRUPTABLE)
	;   FUNCTION KEY SF31 - BELL, GRADE CROSSING SEQUENCE !
	;	::::::::::::::::::::::::::::::: EVENT MUST PRECEED SF1 BELOW ===============================
	;-------------------------------------------------------
	INITIATE_SOUND TRIG_ALTGCS_ON,RUN_WHILE_TRIG	;PZAP 2/13/17 +NO_PREEMPT_TRIG		; Do not allow bell to be interrupted by other events
	;						bit6 disable bell
	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x01,COMP_bit6,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		END_SOUND				;Skipped when ARG1 = ARG2
	; only one bell routine can be supported. use SF1 below.
	BRANCH_TO	CH4SC0_BELL_ON	
	
	END_SOUND

	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH3_ bell/air/fan/misc (NOT INTERRUPTABLE)
	;   FUNCTION KEY SF1 - BELL     	
	; -------------------------------------------------------------
	INITIATE_SOUND	TRIG_BELLS_ON,RUN_WHILE_TRIG	;PZAP 2/13/17 +NO_PREEMPT_TRIG		;continuous NON repeat version
;-
CH4SC0_BELL_ON	
	;				bit6 disable bell
	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x01,COMP_bit6,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		END_SOUND				;Skipped when ARG1 = ARG2
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BELL_VOLUME,SCALE_D
	
	;BELL SELECTED CV157:
	;			Decimal	HEX			Bit
	;								7654 3210
	;	1		0		0x00		0000 0000
	;	2		1		0x01		0000 0001
	;	3		2		0x02		0000 0010
	;	4		3		0x03		0000 0011
	;	5		4		0x04		0000 0100
	;			Decimal	HEX			Bit
;----


;-----
;CV150= xxxx-x00X
CH4SC0_F1_BELL_00X
	;MASK_COMPARE 	SCV_157_SEL_BELL,IMMED_DATA,0x03,COMP_bit0,SKIP_LESS		;SKIP_LESSSKIP_GRTR  ;
	;	BRANCH_TO	CH4SC0_F1_BELL_003
	;MASK_COMPARE 	SCV_157_SEL_BELL,IMMED_DATA,0x02,COMP_bit0,SKIP_LESS		;SKIP_LESSSKIP_GRTR  ;
	;	BRANCH_TO	CH4SC0_F1_BELL_002
	MASK_COMPARE 	SCV_157_SEL_BELL,IMMED_DATA,0x01,COMP_bit0,SKIP_LESS		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH4SC0_F1_BELL_001
;-
;CV150= 00 DEFAULT BELL
CH4SC0_F1_BELL_000
	PLAY	HNDL_SC0_BELL1,no_loop,loop_STD	
	MASK_COMPARE 	SCV_FEATURE2_DISABLE,IMMED_DATA,0x01,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;		
		BRANCH_TO	CH4SC0_F1_BELL_DELAY	
		
	DELAY_SOUND	DELAY_CV,SCV_BELL_RING_RATE,DELAY_GLOBAL
	;chain bell
	;END_SOUND
	; uncomment to have only 1 bell sound fragments
	
;CV150= 01 SECOND BELL
CH4SC0_F1_BELL_001   

	PLAY	HNDL_SC0_BELL2,no_loop,loop_STD
	MASK_COMPARE 	SCV_FEATURE2_DISABLE,IMMED_DATA,0x01,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;		
		BRANCH_TO	CH4SC0_F1_BELL_DELAY	

	DELAY_SOUND	DELAY_CV,SCV_BELL_RING_RATE,DELAY_GLOBAL	
	
	;END_SOUND
	
;CV150= 02 THIRD BELL
;CH4SC0_F1_BELL_002  

;	PLAY	HNDL_SC0_BELL3,no_loop,loop_STD
;	MASK_COMPARE 	SCV_FEATURE2_DISABLE,IMMED_DATA,0x01,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;		
;		BRANCH_TO	CH4SC0_F1_BELL_DELAY	

;	DELAY_SOUND	DELAY_CV,SCV_BELL_RING_RATE,DELAY_GLOBAL	
	
;	;END_SOUND
	
;CV150= 03 FORTH BELL
;CH4SC0_F1_BELL_003  

;	PLAY	HNDL_SC0_BELL4,no_loop,loop_STD
;	MASK_COMPARE 	SCV_FEATURE2_DISABLE,IMMED_DATA,0x01,COMP_bit1,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;		
;		BRANCH_TO	CH4SC0_F1_BELL_DELAY	

;	DELAY_SOUND	DELAY_CV,SCV_BELL_RING_RATE,DELAY_GLOBAL	
	
;	;END_SOUND
	
	
CH4SC0_F1_BELL_DELAY
	DELAY_SOUND	DELAY_CV,SCV_BELL_RING_RATE,DELAY_GLOBAL	
	
	END_SOUND

	   


; -------------------------------------------------------------
;   FUNCTION KEY SF3 - UNCOUPLE  ! take up slack
; -------------------------------------------------------------
	INITIATE_SOUND TRIG_UNCOUPLE_ON,NOT_TRIG			; Play sound when SF3 is released
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH9SC0_UNCOUPLE
	BRANCH_TO	CH9SC0_BRAKE_RELEASE
		
CH9SC0_UNCOUPLE	
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_E	; Volume set 	
	PLAY	HNDL_SC0_UNCOUPLE_BEGIN,no_loop,loop_STD
	 
CH9SC0_BRAKE_RELEASE
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_LETOFF_VOLUME,SCALE_B

	PLAY	HNDL_SC0_BRAKE_BAIL_OFFOFF,no_loop,loop_STD
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x15,0   ;decrease
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_UNCOUPLE_DISCONNECT,no_loop,loop_STD													

 	END_SOUND
	
	
; -------------------------------------------------------------
;   FUNCTION KEY SF3 - UNCOUPLE  ! lift ring
; -------------------------------------------------------------
	INITIATE_SOUND TRIG_UNCOUPLE_ON,NORMAL			; Play lift ring sound when SF3 ON
	;set COUPLER_INTLK5 interlock:
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~COUPLER_MASK5,MERGE_ALL_MASK	;AND=FLIPtheBit  bit5
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_D	; Volume set 
	PLAY	HNDL_SC0_UNCOUPLE_BEGIN,no_loop,loop_STD
	;SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_UNCOUPLE_DISCONNECT,no_loop,loop_STD 	
		
	END_SOUND
	  
	  
; -------------------------------------------------------------
;   FUNCTION KEY SF3 - COUPLE  ! COUPLE
; -------------------------------------------------------------
	INITIATE_SOUND TRIG_COUPLE_ON,NORMAL			;  sound when SF3 ON
	;set COUPLER_INTLK5 interlock:
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~COUPLER_MASK5,MERGE_ALL_MASK	;AND=FLIPtheBit  bit5
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_D	; Volume set 
	PLAY	HNDL_SC0_COUPLER_LIFT_RING,no_loop,loop_STD
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		PLAY	HNDL_SC0_COUPLER_CLASH,no_loop,loop_STD 	
		
	END_SOUND
	  
; -------------------------------------------------------------
;   FUNCTION KEY SF7 - WHEEL FLANGE  !
; -------------------------------------------------------------
	INITIATE_SOUND TRIG_FLANGES_ON,NOT_TRIG		; Interrupt play
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_LETOFF_VOLUME,SCALE_A	

	;PLAY HNDL_SC0_ABOUT,no_loop,loop_STD	;DEBUG2

	;Set AUTOMATIC BRAKING INTERLOCK On =1
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~AUTO_BRAKE_MASK2,MERGE_ALL_MASK	;AND=FLIPtheBit  bit2	AUTOMATIC BRAKING INTERLOCK to DISable =1
	END_SOUND	
		 
; -------------------------------------------------------------
	INITIATE_SOUND TRIG_FLANGES_ON,NORMAL+ZAP			; Play sound when SF7 is pressed
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_A	; Volume set by Misc Volume CV
CH9SC0_WHEEL_SOUND2

	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		BRANCH_TO	CH9SC0_WHEEL_SOUND
	BRANCH_TO	CH9SC0_NO_WHEEL_SOUND
;-
CH9SC0_WHEEL_SOUND

	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_C	;12/19/17
	PLAY	HNDL_SC0_WHEEL_FLANGE1,loop_till_FLANGES_OFF,loop_STD
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_D	;12/19/17
	PLAY	HNDL_SC0_WHEEL_FLANGE2,no_loop,loop_STD
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL		; Set varied pitch	
	
	PLAY	HNDL_SC0_WHEEL_FLANGE2,loop_till_FLANGES_OFF,loop_INVERT
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_C	;12/19/17
	PLAY	HNDL_SC0_WHEEL_FLANGE1,no_loop,loop_STD                   

	END_SOUND
	 

CH9SC0_NO_WHEEL_SOUND		
	LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_B
	END_SOUND
	
	
;------------------------------------------------
;-----------  PERSISTENT air compressor on TIMER/speed and SF20,*** 11/21 BRK/zap with BELL or DYNAMICS!!
;------------------------------------------------
	INITIATE_SOUND	TRIG_SCAT1,NORMAL		;NORMAL+ZAP		;RUN_WHILE_TRIG	
CH8SC0_AIR_PLAY
	;interlock scatter event handler here to prevent play after disable.
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,COMP_MASK0,~COMP_MASK0,SKIP_SAME	;bit0	COMPRESSOR AND AIR SOUNDS INTERLOCK ON OR OFF 		
		END_SOUND	
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		END_SOUND	 ;interlock is 00, freeze	  	;Skipped when ARG1 = ARG2

   	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_COMPR_ON		;disable
		END_SOUND
			
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		BRANCH_TO	CH8SC0_NO_COMP ;until startup done
 
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COMPRESSOR_PUMP_VOLUME,SCALE_A
	
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COMPRESSOR_PUMP_VOLUME,SCALE_B
	PLAY	HNDL_SC0_AIR_DRYER1,no_loop,loop_STD

CH8SC0_AIR_SEQ
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,AIR_PRESSURE_LIMIT,SINTEN_LOW+WORK_SPEED	
 	PLAY	HNDL_SC0_AIRCOMP_START,no_loop,loop_STD	
	
CH8SC0_AIR_LOOPER

CH8SC0_NOT_SHAFT_DRIVEN
	;play only one sound fragment:
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL
		
	;estop is on:
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_ESTOP_ON		;disable
		END_SOUND
		
	PLAY	HNDL_SC0_AIRCOMP_RUN1,no_loop,loop_INVERT	
	PLAY	HNDL_SC0_AIRCOMP_RUN1,no_loop,loop_INVERT	
	BRANCH_TO	CH8SC0_AIR_LOOP_END

			
CH8SC0_AIR_LOOP_END	
	;loop unitl max pressure:
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,+0x01,80   ;increment
	;max AIR PRESSURE reset
	;MIN_AIR_PRESS	EQU	40
	;AIR_PRESSURE_LIMIT  ;notCV149 Compressor/Airpump run time		[88]
	;AIR_PRESS		EQU WORK_USER_3 ; used to save current air or brake pressure

	MASK_COMPARE AIR_PRESS,IMMED_DATA,AIR_PRESSURE_LIMIT,COMP_ALL,SKIP_GRTR
		BRANCH_TO	CH8SC0_AIR_LOOPER
 	BRANCH_TO	CH8SC0_AIR_PRESS_LIMIT

CH8SC0_AIR_PRESS_LIMIT
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+AIR_PRESS,AIR_PRESSURE_LIMIT,0		;CLEAR 
	
CH8SC0_AIR_PRESS_EXIT
 	PLAY	HNDL_SC0_AIRCOMP_END,no_loop,loop_STD
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_DRYER_VOLUME,SCALE_E

	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,AIRPUMP_START_RATE,SINTEN_LOW+WORK_SPEED	;BY CV146
	
CH8SC0_NO_COMP ;until startup done
	BRANCH_TO	CH15SC0_PLAY_DRYER
	END_SOUND	
	
	
	
;------------------------------------------------
;-----------  PERSISTENT vary load on TIMER/speed 
;	TRIM NOTCH AND IDLE
;------------------------------------------------
	INITIATE_SOUND	TRIG_SCAT5,NORMAL+ZAP		;RUN_WHILE_TRIG			
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x14,0   ;decrease
	MASK_COMPARE COUNT_LOOP,IMMED_DATA,3,COMP_ALL,SKIP_LESS    
		BRANCH_TO	CH13SC0_MOVING_EXIT

	;aux or hep is running:
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_AXGEN_ON; But branch if SF30 was previously on
		END_SOUND	
		
	;estop is on:
	SKIP_ON_TRIGGER TRIG_FALSE,TRIG_ESTOP_ON		;disable
		END_SOUND
		
	;in advanced consist:
	MASK_COMPARE	SCV_CV19,IMMED_DATA,1,COMP_ALL,SKIP_LESS
		END_SOUND	;WHEN IN ADVANCED CONSIST DO NOT AUTO VARY NOTCH SOUNDS...4/3/2015

	MASK_COMPARE 	SCV_FEATURE2_DISABLE,IMMED_DATA,0x01,COMP_bit3,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;		
		END_SOUND	
	;moving at all?:
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR ;compare current speed 
		BRANCH_TO	CH13SC0_MOVING_STOPPED	
CH13SC0_FLANGE_SOUND	
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_A	; Volume set by Misc Volume CV
	PLAY	HNDL_SC0_WHEEL_FLANGE1,no_loop,loop_STD	
	;cutdown 4k:LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_B	; Volume set by Misc Volume CV
	;cutdown 4k:PLAY	HNDL_SC0_WHEEL_FLANGE1,no_loop,loop_STD	

	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_A	; Volume set by Misc Volume CV
	PLAY	HNDL_SC0_WHEEL_FLANGE2,no_loop,loop_STD
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_B	; Volume set by Misc Volume CV
	PLAY	HNDL_SC0_WHEEL_FLANGE2,no_loop,loop_STD
	BRANCH_TO	CH13SC0_MOVING_EXIT

CH13SC0_MOVING_STOPPED
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,0x00,0	; set Manual Notch value
		
CH13SC0_MOVING_EXIT
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL5,SCALE_MAX,SINTEN_MID+WORK_SPEED+WORK_PEAK_SPD	;BY CV195	
	
	END_SOUND
	
	
	;END CH3_==============================================================

		PAGE
	
;------------------------------------------------
;Start Channel 4		
;------------------------------------------------
CH4SC0_BEGIN
	CHANNEL_START	4	;typically TRANSIENT channel for QUEUED non-persistent sounds

;------------------------------------------------
;added 8/13/06 to holdoff until START is complete
;------------------------------------------------
;UC4.0================
	INITIATE_SOUND	TRIG_SND_ACTV11,NORMAL				;run ONCE only
CH4SC0_INTERLOCK_HOLD

	DELAY_SOUND DELAY_THIS,24,DELAY_GLOBAL
	
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		BRANCH_TO	CH4SC0_INTERLOCK_HOLD					;interlock is 00, freeze		;Skipped when ARG1 = ARG2
	;DO NOT reset blend, gain, pitch here:
	END_SOUND
	
	
	
;BEGIN CH4SC0_ ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH4_ (NOT INTERRUPTABLE)

	;----------------- SF4= DYNAMIC BRAKE fans- persistent instaed of 
	;------------------------------------------------
	INITIATE_SOUND	TRIG_DYNBRK_ON,NORMAL
	 
	SKIP_ON_TRIGGER	TRIG_TRUE,TRIG_MOVING
		BRANCH_TO	CH5SC0_HAND_BRAKE
	;		bit5 disable Dynamic BRAKES					=32
	MASK_COMPARE 	SCV_FEATURE3_DISABLE,IMMED_DATA,0x00,COMP_bit5,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		BRANCH_TO	CH5SC0_DYN_DISABLED
		
	;Proper working Dynamic Brakes. PM Spools down to Idle and then rises as Dynamic Brake Fan Runs. 
	; AND The Engine Brakes faster due to Dynamic Brake motor Logic.	11/26/2015
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,0,0	;clr SPD DELTA HISTORY
	;Vary Dynamic Braking on SF4 loop 2/10/15
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,DYN_CTRL_MASK3,~DYN_CTRL_MASK3,SKIP_SAME	;TEST BIT3 FOR TRUE WHEN SET=ON / SKIP_LESSSKIP_GRTR  ;		
		BRANCH_TO	CH5SC0_DYN_NOTCH5	;bit3	DYNAMIC BRAKE CONTROL INTERLOCK	
CH5SC0_DYN_NOTCH5
;	INCREASE THROTTLE TO - Notch 4
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_INTEGRATE+WORK_SPEED_DELTA,+0x10,MAX_NOTCH_VAL   ;increment by 2 notches
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_DYN_VOLUME,SCALE_E		;0X14 OR 0X18 
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL				; Set varied pitch
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,DYN_CTRL_MASK3,MERGE_ALL_MASK	;Set Variable Dynamic Brake sound
	PLAY	HNDL_SC0_DYN_BRAKE_START,no_loop,loop_STD
	 
CH5SC0_PLAY_DYN_NOTCH
	LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL				; Set varied pitch
	;PLAY	HNDL_SC0_DYN_BRAKE_START,no_loop,loop_STD
	PLAY	HNDL_SC0_DYN_BRAKE_SUSTAIN,loop_till_DYNBRK_OFF,loop_INVERT
	PLAY	HNDL_SC0_DYN_BRAKE_SUSTAIN,no_loop,loop_STD
					
CH5SC0_DYN_DISABLED
	PLAY	HNDL_SC0_DYN_BRAKE_END,no_loop,loop_STD 
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,NOTCH1,0	; set Manual Notch value

 	;Reset dynamics to normal
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~DYN_CTRL_MASK3,MERGE_ALL_MASK	;Reset Variable Dynamic Brake sound
	LOAD_MODIFIER 	MTYPE_PITCH,ANALOG_PITCH_MODIFY+0x0,IMAXP_DIESEL,DITHERP_DIESEL	;for discrete notch volume control
	LOAD_MODIFIER  MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_DYN_VOLUME,SCALE_B	

	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x12,0   ;decrease
	END_SOUND
	
		
;------------------------------------------------
; HAND BRAKE -- while stopped
;------------------------------------------------
	INITIATE_SOUND	TRIG_HBRAKE_ON,NORMAL

CH5SC0_HAND_BRAKE
	;should not be on deck pulling brake when loco is in motion:
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_MOVING
		END_SOUND	
		
	;Reset dynamics to normal
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_AND+USER_INTLK_BITS,~DYN_CTRL_MASK3,MERGE_ALL_MASK	;Reset Variable Dynamic Brake sound
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_BRAKE_VOLUME,SCALE_F	
	
	PLAY	HNDL_SC0_HBRAKE_BEGIN,no_loop,loop_STD
	;PLAY	HNDL_SC0_HBRAKE_LOOP,no_loop,loop_INVERT		
	PLAY	HNDL_SC0_HBRAKE_LOOP,loop_till_HBRAKE_OFF,loop_INVERT
	PLAY	HNDL_SC0_HBRAKE_END,no_loop,loop_STD
	
	END_SOUND		
	

	
	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	THIS ROUTINE IS CH4_(NOT INTERRUPTABLE)
	;----------------- SF25= AUX Generator - persistent 
	;------------------------------------------------
	INITIATE_SOUND	TRIG_AXGEN_ON,NORMAL+NO_PREEMPT_TRIG		; Do not allow aux to be interrupted by other events	
	LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK  ;STD full volume 192

	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x01,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;		
		BRANCH_TO	CH12SC0_RUN_AUX
	BRANCH_TO	CH12SC0_RUN_HEP
	
CH12SC0_RUN_AUX
	;Most FP-units for passenger service were built in the early 50's. They came with steam generators. 
	;Starting around 1960 some replaced the steam generators with Cummins diesel HEP units 
	;putting out 440VAc, Others came with Detroit Diesel HEP units. 
	;Later NS OSX units have revised AUX generators.
	PLAY	HNDL_SC0_AUX_START,no_loop,loop_STD
	;		PLAY_AUX_GEN
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_AUXGEN_VOLUME,SCALE_A
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING	;raise voulme if not moving
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_AUXGEN_VOLUME,SCALE_B

	PLAY	HNDL_SC0_AUX_RUN,loop_till_init_TRIG,loop_INVERT
	PLAY	HNDL_SC0_AUX_END,no_loop,loop_STD
	END_SOUND

CH12SC0_RUN_HEP		;Run at Notch 7
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,0x60,0	
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_F							 ; Air volume control
	PLAY	HNDL_SC0_FAN1_LOOP,no_loop,loop_INVERT		
	PLAY	HNDL_SC0_FAN1_LOOP,loop_till_init_TRIG,loop_INVERT	

	END_SOUND
	
	
	

;----------------------------------------
;   TIMED CYCLE OF FAN1 (TIMER SCAT3)
;----------------------------------------
	INITIATE_SOUND TRIG_SCAT3,NORMAL+ZAP		; Allows air sounds to be preempted	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x12,0   ;decrease
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,COMP_MASK0,~COMP_MASK0,SKIP_SAME	;bit0	COMPRESSOR AND AIR SOUNDS INTERLOCK ON OR OFF 		
		END_SOUND
	;interlock scatter event handler here to prevent play after disable.
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		END_SOUND		;Skipped when ARG1 = ARG2
	;disable fans with bit3
	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x01,COMP_bit3,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		END_SOUND 			;Skipped when ARG1 = ARG2
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_AXGEN_ON; But branch if SF30 was previously on
		END_SOUND 
	;SKIP_ON_TRIGGER TRIG_FALSE,TRIG_FAND_ON		;disable
	;	END_SOUND		


CH12SC0_LOW_FAN
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FANS_ON		; Qualify with SF11 OFF
		END_SOUND					; Ends here if SF20 is ON	

	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL3,20+WORK_SPEED,SINTEN_LOW+WORK_SPEED ; run time	
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH4,COMP_7LSB,SKIP_GRTR   ;Skip if 							 ; Air volume control
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_B							 ; Air volume control
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING	;raise voulme if not moving
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_C						 ; Air volume control
	;raise volume if in notch 4 or greater
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH4,COMP_7LSB,SKIP_LESS    ;Skip if 
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_D							 ; Air volume control
	;cutdown 4k:PLAY	HNDL_SC0_FAN1_LOOP,no_loop,loop_STD	
	PLAY	HNDL_SC0_FAN1_LOOP,loop_till_init_TRIG,loop_INVERT	
	;cutdown 4k:PLAY	HNDL_SC0_FAN1_LOOP,no_loop,loop_STD
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL3,44+WORK_SPEED,SINTEN_HIGH  ; Next cycle time
	END_SOUND


;----------------------------------------
;   TIMED CYCLE OF FAN1 (TIMER SCAT2)
;----------------------------------------
	INITIATE_SOUND	TRIG_SCAT2,NORMAL+ZAP	; Allows air sounds to be preempted	
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x11,0   ;decrease
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,COMP_MASK0,~COMP_MASK0,SKIP_SAME	;bit0	COMPRESSOR AND AIR SOUNDS INTERLOCK ON OR OFF 		
		END_SOUND
	;interlock scatter event handler here to prevent play after disable.
	MASK_COMPARE 	WORK_STATUS_BITS,IMMED_DATA,WKSB_INTERLOCK_MASK,~WKSB_INTERLOCK_MASK,SKIP_SAME ;see if INTERLOCK done=0/DCC 
		END_SOUND			;Skipped when ARG1 = ARG2
	
	;disable fans with bit3
	MASK_COMPARE 	SCV_FEATURE_DISABLE,IMMED_DATA,0x01,COMP_bit3,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
		END_SOUND 		;Skipped when ARG1 = ARG2	
	
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_AXGEN_ON; But branch if SF30 was previously on
		END_SOUND 	
	;SKIP_ON_TRIGGER TRIG_FALSE,TRIG_FAND_ON		;disable
	;	END_SOUND
CH14SC0_HIGH_FAN		
	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_FANS_ON		; Qualify with SF11 OFF
		END_SOUND					; Ends here if SF20 is ON	
		
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH4,COMP_ALL,SKIP_GRTR    ;Skip if 
		BRANCH_TO	CH14SC0_LOWHIGH_FAN		
	BRANCH_TO	CH14SC0_PLAY_HIGH_FAN		
	
CH14SC0_LOWHIGH_FAN	 
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_9						 ; Air volume control

	PLAY	HNDL_SC0_FAN1_START,no_loop,loop_STD	
	PLAY	HNDL_SC0_FAN1_LOOP,no_loop,loop_INVERT	
	PLAY	HNDL_SC0_FAN1_LOOP,loop_till_init_TRIG,loop_INVERT	
	PLAY	HNDL_SC0_FAN1_END,no_loop,loop_STD		
	END_SOUND	
	
CH14SC0_PLAY_HIGH_FAN	
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL2,25+WORK_SPEED,SINTEN_LOW+WORK_SPEED ;  run time	
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH4,COMP_ALL,SKIP_GRTR   ;Skip if 	
		BRANCH_TO	CH14SC0_PLAY_HIGH_FAN1
	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_C		; Air volume control	
	
CH14SC0_PLAY_HIGH_FAN1
	SKIP_ON_TRIGGER TRIG_TRUE,TRIG_MOVING	;raise voulme if not moving
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_B						 ; Air volume control
	;raise volume if in notch 4 or greater
	MASK_COMPARE PREV_NOTCH,IMMED_DATA,NOTCH4,COMP_ALL,SKIP_GRTR    ;Skip if 
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_FAN_VOLUME,SCALE_D							 ; Air volume control
	PLAY	HNDL_SC0_FAN1_START,no_loop,loop_STD	
	PLAY	HNDL_SC0_FAN1_LOOP,no_loop,loop_INVERT	
	PLAY	HNDL_SC0_FAN1_LOOP,loop_till_init_TRIG,loop_INVERT	
	PLAY	HNDL_SC0_FAN1_END,no_loop,loop_STD
	LOAD_MODIFIER	MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL2,60+WORK_SPEED,SINTEN_MID+WORK_SPEED  ; Next cycle time	
	
	END_SOUND
	
	;-------------------------------
	;SF12 Disconnect Coupler
	;-------------------------------
	INITIATE_SOUND	TRIG_COUPLERD_ON,NORMAL		

		BRANCH_TO	CH6SC0_PLAY_DISCONNECT
	END_SOUND 

	
	;---------------------------------------------
	;  Accelerate   Play UnCoupler            ::::: New 11/19/2015
	;---------------------------------------------
	INITIATE_SOUND T_SPD_ACCEL1,NORMAL
	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x10,0   ;decrease
	;set drift interlock:
	;(drift is interlocked when not decelerating. when accelerating heavy sounds play.
	;interlock is set on bit is =1)(i.e. DRIFTING IS OFF)
	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,VARY_DRIFT_MASK7,MERGE_ALL_MASK	;OR=SETtheBit  bit7	NOT drifting  =0
	;=============== ACCEL REPEATS MANY TIMES =================
	;  Accelerate   Play UnCoupler            ::::: New 11/19/2015
	MASK_COMPARE	WORK_SPEED,IMMED_DATA,0x01,COMP_ALL,SKIP_GRTR		; Check if stopped
		BRANCH_TO	CH6SC0_PLAY_BLIP2
	END_SOUND
	
CH6SC0_PLAY_BLIP2
	;blip motor here:
 	LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+VARY_LOAD,+SCALE_INCR_2,SCALE_MAX   ;Increment

	MASK_COMPARE WORK_STATUS_BITS,IMMED_DATA,0x02,~WKSB_DIRNOW_MASK,SKIP_SAME		; Skip the next line if loco is set to forward
		END_SOUND
		

	LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_COUPLER_VOLUME,SCALE_E	
		
	MASK_COMPARE 	USER_INTLK_BITS,IMMED_DATA,COUPLER_MASK5,~COUPLER_MASK5,SKIP_SAME	;TEST BIT5
		BRANCH_TO	CH6SC0_PLAY_DISCONNECT
	BRANCH_TO	CH6SC0_PLAYED_DISCONNECT
	

CH6SC0_PLAY_DISCONNECT

	SKIP_ON_TRIGGER	TRIG_FALSE,TRIG_UNCOUPLE_ON	; Skip if SF3 is not currently on -- lift ring must be up  
		BRANCH_TO	CH6SC0_PLAYED_DISCONNECT
	PLAY	HNDL_SC0_UNCOUPLE_DISCONNECT,no_loop,loop_STD
		  
CH6SC0_PLAYED_DISCONNECT		

	LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COUPLER_MASK5,MERGE_ALL_MASK	;OR=SETtheBit bit5	
	
	END_SOUND
		
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		;TRACTION SOUNDS -- CONTINUOUS:
		;----------------------------------------
		;    Start moving from idle                                 
		;----------------------------------------

		INITIATE_SOUND	T_SPD_IDLEXIT,RUN_WHILE_TRIG+ZAP
		MASK_COMPARE 	SCV_FEATURE4_DISABLE,IMMED_DATA,0x00,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
			END_SOUND			;bypass disabled traction sounds:

  		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_TRACTION_MOTOR_VOLUME,SCALE_9	;use VOL control
		PLAY HNDL_SC0_TRACTION_START,no_loop,loop_STD				;Play idle to run wave
;There is no END_SOUND callout here - otherwise it plays this more than once!! 

		;----------------------------------------
		;  Run at speed
		;----------------------------------------
		INITIATE_SOUND	T_SPD_RUN,RUN_WHILE_TRIG+ZAP
		MASK_COMPARE 	SCV_FEATURE4_DISABLE,IMMED_DATA,0x00,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
			END_SOUND			;bypass disabled traction sounds:

		LOAD_MODIFIER	MTYPE_BLEND, BLEND_CURRENT_CHNL+BLEND_GAIN0+BLEND_FASE0,BLENDG_DSL_ACCEL1,BLENDF_DSL_ACCEL1 ;x06,x05
		;LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY+0x18,SCV_TRACTION_MOTOR_VOLUME,DITHER+LOW_DITH+WORK_SPEED
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY+0x5,SCV_TRACTION_MOTOR_VOLUME,DITHER+LOW_DITH+WORK_SPEED
		LOAD_MODIFIER	MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_SPEED,MAXP_DIESEL,DITHERP_DIESEL
		PLAY	HNDL_SC0_TRACTION_RUN,loop_till_init_TRIG,loop_INVERT 
		END_SOUND

		;----------------------------------------
		;   Coming to rest                            
		;----------------------------------------
		INITIATE_SOUND	TRIG_MOVING,NOT_TRIG	
		MASK_COMPARE 	SCV_FEATURE4_DISABLE,IMMED_DATA,0x00,COMP_bit0,SKIP_SAME		;SKIP_LESSSKIP_GRTR  ;
			END_SOUND			;bypass disabled traction sounds:

;		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_TRACTION_MOTOR_VOLUME,SCALE_9

		PLAY HNDL_SC0_TRACTION_END,no_loop,loop_STD 
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x01,0   ;decrease

		END_SOUND




	;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	;	SF20 COMPRESSOR
	;-------------------------------
	INITIATE_SOUND	TRIG_COMPR_ON,NORMAL	;RUN_WHILE_TRIG		;continuous NON repeat version		
		;interlock is set on bit is =1) (i.e. DRIFTING IS OFF)
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,VARY_DRIFT_MASK7,MERGE_ALL_MASK	;OR=SETtheBit  bit7	NOT drifting  =0
		LOAD_MODIFIER	MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_MISC_VOLUME,SCALE_C
		PLAY	HNDL_SC0_AIR_LETOFF,no_loop,loop_STD
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_INTEGRATE+AIR_PRESS,-0x19,0   ;decrease
		LOAD_MODIFIER	MTYPE_WORK_IMMED,FMATH_OR+USER_INTLK_BITS,COUPLER_MASK5,MERGE_ALL_MASK	;OR=SETtheBit bit5	;---------------------------------------------
	;---------------------------------------------
	;  RESET MAX GAIN ET.AL
	;---------------------------------------------
		;LOAD_MODIFIER 	MTYPE_WORK_IMMED,FMATH_LODE+PREV_NOTCH,0,0    ;Set value initial
		LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_SPEED_DELTA,0x00,0	; set Manual Notch value
		;MAX GAIN:
		LOAD_MODIFIER	MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK  ;STD full volume 192
		BRANCH_TO	CH8SC0_AIR_SEQ

	END_SOUND	
		
	
; -------------------------------------------------------------
;   SET DEFAULT CVs WHEN SETTING CV8 TO 8/9
; -------------------------------------------------------------
	; These CV values are set at the moment the SPJ is downloaded to the decoder: CV > 155 CANNOT BE EXTRACTED FROM SDF
 	INITIATE_SOUND	TRIG_FACTORY_CVRESET,NORMAL

CH16SC0_FACTORY_CVSET
;NOTE: REMOVE MOST OF THESE SETTINGS AFTER TESTING::::
;SHOULD test herE for POR SW version higher than 8!!
;skip these if NOT SFX2 capable
	;;;NO NO;;;; 	LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV1,4,0			;CV1 Primary address
	;; fact default is AUTO NOTCHING
	;xLOAD_MODIFIER	MTYPE_SNDCV,SCV_CV3,50,0			;CV3
	;xLOAD_MODIFIER	MTYPE_SNDCV,SCV_CV4,43,0			;CV4
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV8,255,0		;make sure is RO
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV19,0,0  Advanced Consist [0]
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV21,248,0  Advanced Consist SF0-SF8 [248] 
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV22,52,0  Advanced Consist SF3-SF7 [52] 
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV29,7,0		;CV29 REVERSE
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV49,2,0		;CV49 MARS lite
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV57,238,0  BEMF Consist + Advanced Consist, 0-15 / 0-15, 15=max (14,14; 15,15) [238]
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV58,12,0		;CV58 Should be low 1-5
	LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV60,0,0		;CV61 SELECT SCHEME 0=DIESEL
	LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV105,USER_SDF_SUB_ID,0	; Identifies Project VERSION within author ID where nnn is serial 1-255
	LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV106,PROJECT_ID,0		; Identifies Project change within version ID where nnn is serial 1-255
	;LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV121,255,0		;make sure is RO
	LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV153,PROJECT_ID,0		; Identifies Project change within version ID where nnn is serial 1-255
	 ;cutback20:LOAD_MODIFIER MTYPE_SNDCV,SCV_DCONFIG,127,0	;;CV131 127 is diesel config
	;LOAD_MODIFIER MTYPE_SNDCV,SCV_DCONFIG,0,0	;CV131 0 is Steam config 
	 ;cutback20:LOAD_MODIFIER MTYPE_SNDCV,SCV_NOTCH,191,0	;CV132 notch rate 64-128 to avoid missing notches SF26 up. [127]	
	 ;cutback20:LOAD_MODIFIER MTYPE_SNDCV,SNDCV_STEAM,60,0	;CV133= typ steam CAM config byte, x80=EXT or 1-127=DRIVER dia in inches [60]
;				DRIVER size bit7=1 means EXTERNAL INPUT_0 generates a special CAM code as well
	 ;cutback20:LOAD_MODIFIER MTYPE_SNDCV,SCV_STGEAR,41,0	;CV134= steam gear ratio trim [28]
	 ;cutback20:LOAD_MODIFIER MTYPE_SNDCV,SCV_MUTE_VOL,30,0	;CV135= vol level when MUTE action is triggered, e.g. SF8=ON
	;SCV_MAIN_PITCH			;CV136
	 ;cutback20:LOAD_MODIFIER MTYPE_SNDCV,SCV_DISTANCE_RATE,30,0   	 ;cv139 Set to trigger distance event
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_USER_SDF_SUB_ID,USER_SDF_SUB_ID,0 	;CV160 is SUB id
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_FEATURE_DISABLE,128,0		;CV185 is bit values to disable features1:
;				bit0 disable  HEP diesel =1  AUX GEN	=0
;				bit1 disable hard automatic braking 	=2
;				bit2 disable startup sounds				=4
;				bit3 disable Fans						=8
;				bit4 disable toots on direction change	=16
;				bit5 disable grade crossing random play =32
;				bit6 disable bell						=64
;				bit7 fuel prime once for Gen1 diesels OR multiple start warning bells Gen2	=128		
;				bit7 steam disable Open Cyl Cocks	=128		
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_FEATURE2_DISABLE,3,0	;CV186 is bit values to disable additional features2 
;				bit0 disable Diesel Low Idle=1 Enabled=0	=0=Enabled
;				bit0 disable STEAM BLOWER=1 BOILER=0		=1
;				bit1 disable Chain bells 1-4 				=2
;				bit2 disable STOKER change to SHOVEL		=4
;				bit3 disable AUTO VARY NOTCH				=8
;				bit4 disable Shaft Driven Compressor 		=16
;				bit5 disable ALCO Idle Stumble=32 Disable=0	=32
;				bit6 disable Auto REAR HORN/WHISTLE signals: 00=Off 	=64
;				bit7 disable Duel Prime Movers		0 = Off =128 = Duel On
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_FEATURE3_DISABLE,0,0	;CV187 is bit values to disable additional features3 
;				bit0 disable Short Horn/whistle				Enabled=0	=1 
;				bit1 disable DEMO ALL HornS/whistles		Enabled=2	=0 Disabled
;				bit2 disable Bell on Idle Exit							=4	
;				bit3 disable Motor Lock on SF10							=8
;				bit4 disable Startup MUTED								=16 Disabled
;				bit5 disable Dynamic BRAKES								=32
;				bit6 disable Run High									=64
;				bit7 disable Auto Wheel Slip							=128
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_FEATURE4_DISABLE,0,0	;CV188 is bit values to disable additional features4 
;				bit0 disable Traction Motor 		= 1 Disabled
;				bit1 disable HILO HORN				= 2 Disabled
;				bit2 disable 3						= 4 Disabled
;				bit3 disable 4						= 8 Disabled
;				bit4 disable 5						= 16 Disabled
;				bit5 disable 6						= 32 Disabled
;				bit6 disable 7						= 64 Disabled
;				bit7 disable 8						= 64 Disabled

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 	 
; MERGE UNIQUE CV VALUES HERE FOR THIS SPJ ONLY:
	 ;cutback20: #INCLUDE	EL7c4x6d30CVS.INC
;;;;;;;;;;;;;;;;;;;	;;;;; FOLLOW WITH ASM FINAL CODE HERE:
	;END CH3_==============================================================
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;no end sound here (MUST BE IN ASM) END_SOUND         
;end EL7c4x6d30base
											
