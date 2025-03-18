   LIST   P=18f242,C=132,T=ON,R=DEC,ST=OFF
   TITLE  STEAMEXAMPLE16 Author: Fred Miller
VERNO	EQU	0x20		;ver 2.00 
;----------------------------------------------------------------------------
;   This Sound Definition Langauge (SDL) was generated
;   by the SPJHelper Design Tool on 4/23/14
;----------------------------------------------------------------------------
;     FUNCTION KEYS DEFINED:   
;    F1 Bell
;    F2 Whistle
;    F6 Water Alarm Reset
;    F7 Crossing Signal
;    F8 Mute ON/OFF
;----------------------------------
;   SOUND WAVE FILE HANDLES
;----------------------------------
   cblock   0   ; Start assigning location of  Sound Clips 0, 1, 2, etc.
HNDL_MUTE   ; Internal SILENCE value = 0
Steam_Chuff1
Steam_Chuff2
Steam_Chuff3
Steam_Chuff4
Steam_airpump
Steam_blow_start
Steam_blow_run
Steam_blow_end
Steam_Bell
Steam_Whistle_start
Steam_Whistle_run
Steam_Whistle_end
Steam_Water_start
Steam_Water_run
Steam_Water_end
Steam_Boiler
Steam_LowWater
   ENDC
;-----------------------------------------
;   INCLUDED DIGITRAX PROPRIETARY FILES
;-----------------------------------------
#INCLUDE   SERIES6_Snd_cmd.INC
#INCLUDE   SERIES6_snd16_macs.INC
;-----------------------------------------
;   SYSTEM VARIABLES
;-----------------------------------------
AuthorCode   EQU   0xFC   ; AUTHOR code Fred's Carbarn (CV 252)
SubID   EQU   5  ; Project #5 - Designed using SPJHelper
;------------------------------------------
;   LOCATION FOR CV ADDRESSES
;------------------------------------------
   cblock   SCV_FREEFORM   ; Start assigning (after std Digitrax) for CV140, CV141, etc.
SCV_140   ;CV140  Chuff Volume [64]
SCV_141   ;CV141  Compressor Cycle Time [12]
SCV_142   ;CV142  Compressor Run Time [5]
SCV_143   ;CV143  Compressor Volume [64]
SCV_144   ;CV144  Water Pump Cycle Time [9]
SCV_145   ;CV145  Water Pump Volume [64]
SCV_146   ;CV146  Bell Volume [64]
SCV_147   ;CV147  Whistle Volume [64]
SCV_148   ;CV148  Blow Down Volume [64]
SCV_149   ;CV149  Boiler Volume [64]
SCV_150   ;CV150  Water Alarm Volume [64]
SCV_151   ;CV151  Horn/Whistle Volume [64]
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
;------------------------------------------------------------------
;   LOCAL VARIABLES (Work Registers and Work_User Memory Registers)                                       
;------------------------------------------------------------------
WORK_SPEED	  EQU	0x00
WORK_NOTCH	  EQU	0x01
WORK_SERVO	  EQU	0x02
WORK_MVOLTS	  EQU	0x03
WORK_USER_LINES	  EQU	0x05
WORK_TIMEBASE	  EQU	0x06
WORK_STATUS_BITS  EQU	0x07
WORK_GLBL_GAIN	  EQU	0x08
WORK_GAIN_TRIM	  EQU	0x09
WORK_PITCH_TRIM	  EQU	0x0A
WORK_SPEED_DELTA  EQU	0x0B
WORK_SCATTER4	  EQU	0x10
WORK_SCATTER5	  EQU	0x11
WORK_SCATTER6	  EQU	0x12
WORK_SCATTER7	  EQU	0x13
WORK_ACHNL_7F	  EQU	0x14
WORK_ACHNL_7E	  EQU	0x15
WORK_SKAT_FAST	  EQU	0x16
WORK_SKAT_SLOW	  EQU	0x17
WORK_DISTANCE	  EQU	0x18
WORK_PEAK_SPD	  EQU	0x19
WORK_USER_0	  EQU	0x1A
WORK_USER_1	  EQU	0x1B
WORK_USER_2	  EQU	0x1C
WORK_USER_3	  EQU	0x1D
WORK_USER_4	  EQU	0x1E
WORK_USER_5	  EQU	0x1F
;========================================================================================
; START SCHEME 0                   
;========================================================================================
   ORG   0									
   SKEME_START   0   ; defines the start of scheme 0
   SDL_VERSION VERSION_1   ; Required by SDL
;---------------------------------------------
;  START CHANNEL   1 
;---------------------------------------------
CHNL_01_S0
   CHANNEL_START   1
;---------------------------------------------
 
   INITIATE_SOUND TRIG_SF8,NORMAL
   LOAD_MODIFIER MTYPE_WORK_INDIRECT,WORK_GLBL_GAIN,SCV_MUTE_VOL,MERGE_ALL_MASK  ;Set MUTE Volume
   END_SOUND
 
   INITIATE_SOUND TRIG_SF8,NOT_TRIG
   LOAD_MODIFIER MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK   ;set Mute OFF (Std Volume)
   END_SOUND
 
   INITIATE_SOUND TRIG_MOVING,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_USER_0,1,0    ;Set value
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 6,5
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   PLAY Steam_Chuff1,loop_till_cam,loop_STD
   PLAY Steam_Chuff2,loop_till_cam,loop_STD
   PLAY Steam_Chuff3,loop_till_cam,loop_STD
   PLAY Steam_Chuff4,loop_till_cam,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_MOVING,NOT_TRIG
   MASK_COMPARE WORK_USER_0,IMMED_DATA,1,COMP_7LSB,SKIP_SAME    ;Skip if Equal
   BRANCH_TO TAG0
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_148,SCALE_F  ;Set Volume
   PLAY Steam_blow_start,no_loop,loop_STD
   PLAY Steam_blow_run,no_loop,loop_STD
   PLAY Steam_blow_end,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_USER_0,0,0    ;Set value
TAG0
   END_SOUND
;---------------------------------------------
;  START CHANNEL 2
;---------------------------------------------
CHNL_02_S0
   CHANNEL_START	2
;---------------------------------------------
 
   INITIATE_SOUND TRIG_SND_ACTV11,NORMAL
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK    ;Enable Motor Run
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,SCV_141,SINTEN_LOW+WORK_SPEED   ;set timer
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,SCV_144,SINTEN_LOW+WORK_SPEED   ;set timer
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set to ~4.5 minutes
   END_SOUND
 
   INITIATE_SOUND TRIG_SCAT1,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_145,SCALE_F  ;Set Volume
   PLAY Steam_Water_start,no_loop,loop_STD
   PLAY Steam_Water_run,no_loop,loop_STD
   PLAY Steam_Water_run,no_loop,loop_STD
   PLAY Steam_Water_run,no_loop,loop_STD
   PLAY Steam_Water_run,no_loop,loop_STD
   PLAY Steam_Water_end,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,SCV_144,SINTEN_LOW+WORK_SPEED   ;set timer
   END_SOUND
 
   INITIATE_SOUND TRIG_SF2,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_147,SCALE_F  ;Set Volume
   PLAY Steam_Whistle_start,no_loop,loop_STD
   PLAY Steam_Whistle_run,loop_till_init_TRIG,loop_INVERT
   PLAY Steam_Whistle_end,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_SF7,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_151,SCALE_F  ;Set Volume
   PLAY Steam_Water_start,no_loop,loop_STD
   PLAY Steam_Whistle_run,no_loop,loop_STD
   PLAY Steam_Whistle_end,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,8,DELAY_GLOBAL
   PLAY Steam_Water_start,no_loop,loop_STD
   PLAY Steam_Whistle_run,no_loop,loop_STD
   PLAY Steam_Whistle_end,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,8,DELAY_GLOBAL
   PLAY Steam_Water_start,no_loop,loop_STD
   PLAY Steam_Whistle_end,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,8,DELAY_GLOBAL
   PLAY Steam_Water_start,no_loop,loop_STD
   PLAY Steam_Whistle_run,no_loop,loop_STD
   PLAY Steam_Whistle_end,no_loop,loop_STD
   END_SOUND
;---------------------------------------------
;  START CHANNEL 3
;---------------------------------------------
CHNL_03_S0
   CHANNEL_START   3
;---------------------------------------------
 
   INITIATE_SOUND TRIG_SF1,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_146,SCALE_F  ;Set Volume
   PLAY Steam_Bell,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_SF6,NORMAL
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set to ~4.5 minutes
   END_SOUND
 
   INITIATE_SOUND TRIG_SCAT0,NORMAL
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,SCV_142,SINTEN_LOW+WORK_SPEED   ;set timer
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_143,SCALE_F  ;Set Volume
   PLAY Steam_airpump,loop_till_init_TRIG,loop_INVERT
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,SCV_141,SINTEN_LOW+WORK_SPEED   ;set timer
   END_SOUND
;---------------------------------------------
;  START CHANNEL 4	'Series 6 Premium Only
;---------------------------------------------
CHNL_04_S0
   CHANNEL_START   4
;---------------------------------------------
 
   INITIATE_SOUND TRIG_DISTANCE,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_150,SCALE_F  ;Set Volume
   PLAY Steam_LowWater,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,0,0     ;Set to Continuous Trigger
   END_SOUND
 
   INITIATE_SOUND TRIG_SND_ACTV11,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_149,SCALE_F  ;Set Volume
   PLAY Steam_Boiler,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_FACTORY_CVRESET,NORMAL
   LOAD_MODIFIER MTYPE_SNDCV, SCV_132,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_133,127,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_134,32,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_135,0,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_140,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_141,12,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_142,5,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_143,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_144,9,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_145,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_146,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_147,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_148,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_149,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_150,64,0    ;Set value
   END_SOUND
;  END OF SCHEME 0		
;---------------------------------------------
   SKEME_END   0
;---------------------------------------------
;  MARK END OF SDL	
;---------------------------------------------
   SKEME_START   CLOSE_SKEME   ;REQUIRED
   SKEME_END   CLOSE_SKEME   ;REQUIRED
END_SDF   ;REQUIRED
   END   ;REQUIRED
