   LIST   P=18f242,C=132,T=ON,R=DEC,ST=OFF
   TITLE  DIESELEXAMPLE Author: Fred Miller MMR
VERNO	EQU	0x20		;ver 2.00 
;----------------------------------------------------------------------------
;   This Sound Definition Langauge (SDL) was generated
;   by the SPJHelper Design Tool on 4/24/14
;----------------------------------------------------------------------------
;     FUNCTION KEYS DEFINED:   
;    F1 Bell
;    F2 Horn/Whistle
;    F7 Crossing Signal
;    F8 Mute ON/OFF
;----------------------------------
;   SOUND WAVE FILE HANDLES
;----------------------------------
   cblock   0   ; Start assigning location of  Sound Clips 0, 1, 2, etc.
HNDL_MUTE   ; Internal SILENCE value = 0
Diesel_idle
Diesel_idle_run
Diesel_run
Diesel_run_idle
Diesel_turnon
Diesel_horn_begin
Diesel_horn_cont
Diesel_horn_end
Diesel_bell
Diesel_popoff
Diesel_pump_start
Diesel_pump_cont
Diesel_pump_end
Diesel_MilePost
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
SCV_140   ;CV140  Power Unit Volume [64]
SCV_141   ;CV141  Horn/Whistle Volume [64]
SCV_142   ;CV142  Bell Volume [64]
SCV_143   ;CV143  Air Popoff Cycle Time [9]
SCV_144   ;CV144  Timed Sound Volume [64]
SCV_145   ;CV145  Compressor Cycle Time [18]
SCV_146   ;CV146  Compressor Run Time [3]
SCV_147   ;CV147  Compressor Volume [64]
SCV_148   ;CV148  MilePost Volume [64]
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
 
   INITIATE_SOUND TRIG_SND_ACTV11,NORMAL+NO_PREEMPT_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 0,0
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   PLAY HNDL_MUTE,no_loop,loop_STD
   PLAY Diesel_turnon,no_loop,loop_STD
   PLAY Diesel_idle,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_SND_ACTV11,NOT_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   PLAY Diesel_idle,no_loop,loop_STD
   PLAY HNDL_MUTE,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND T_SPD_IDLEXIT,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   PLAY Diesel_idle_run,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND T_SPD_ACCEL1,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 4,2
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   PLAY Diesel_run,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND T_SPD_DECEL1,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 9,7
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   PLAY Diesel_run,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND T_SPD_RUN,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 6,5
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   PLAY Diesel_run,loop_till_init_TRIG,loop_INVERT
   END_SOUND
 
   INITIATE_SOUND TRIG_MOVING,NOT_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   PLAY Diesel_run_idle,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_SND_ACTV11,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 9,7
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   PLAY Diesel_idle,loop_till_init_TRIG,loop_INVERT
   END_SOUND
;---------------------------------------------
;  START CHANNEL 2
;---------------------------------------------
CHNL_02_S0
   CHANNEL_START	2
;---------------------------------------------
 
   INITIATE_SOUND TRIG_SF8,NORMAL
   LOAD_MODIFIER MTYPE_WORK_INDIRECT,WORK_GLBL_GAIN,SCV_MUTE_VOL,MERGE_ALL_MASK  ;Set MUTE Volume
   END_SOUND
 
   INITIATE_SOUND TRIG_SF8,NOT_TRIG
   LOAD_MODIFIER MTYPE_WORK_IMMED,WORK_GLBL_GAIN,DEFAULT_GLBL_GAIN,MERGE_ALL_MASK   ;set Mute OFF (Std Volume)
   END_SOUND
 
   INITIATE_SOUND TRIG_SF2,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_141,SCALE_F  ;Set Volume
   PLAY Diesel_horn_begin,no_loop,loop_STD
   PLAY Diesel_horn_cont,loop_till_init_TRIG,loop_INVERT
   PLAY Diesel_horn_end,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_SND_ACTV11,NORMAL
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK    ;Enable Motor Run
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,SCV_145,SINTEN_LOW+WORK_SPEED   ;set timer
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set to ~4.5 minutes
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set to ~4.5 minutes
   END_SOUND
 
   INITIATE_SOUND TRIG_SCAT0,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_144,SCALE_F  ;Set Volume
   PLAY Diesel_popoff,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,SCV_143,SINTEN_LOW+WORK_SPEED   ;set timer
   END_SOUND
 
   INITIATE_SOUND TRIG_SF7,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_141,SCALE_F  ;Set Volume
   PLAY Diesel_horn_begin,no_loop,loop_STD
   PLAY Diesel_horn_cont,no_loop,loop_STD
   PLAY Diesel_horn_end,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,8,DELAY_GLOBAL
   PLAY Diesel_horn_begin,no_loop,loop_STD
   PLAY Diesel_horn_cont,no_loop,loop_STD
   PLAY Diesel_horn_end,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,8,DELAY_GLOBAL
   PLAY Diesel_horn_begin,no_loop,loop_STD
   PLAY Diesel_horn_end,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,8,DELAY_GLOBAL
   PLAY Diesel_horn_begin,no_loop,loop_STD
   PLAY Diesel_horn_cont,no_loop,loop_STD
   PLAY Diesel_horn_end,no_loop,loop_STD
   END_SOUND
;---------------------------------------------
;  START CHANNEL 3
;---------------------------------------------
CHNL_03_S0
   CHANNEL_START   3
;---------------------------------------------
 
   INITIATE_SOUND TRIG_DISTANCE,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_148,SCALE_F  ;Set Volume
   PLAY Diesel_MilePost,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_SF1,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_142,SCALE_F  ;Set Volume
   PLAY Diesel_bell,no_loop,loop_STD
   END_SOUND
 
   INITIATE_SOUND TRIG_SCAT1,NORMAL
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,SCV_146,SINTEN_LOW+WORK_SPEED   ;set timer
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_147,SCALE_F  ;Set Volume
   PLAY Diesel_pump_start,no_loop,loop_STD
   PLAY Diesel_pump_cont,loop_till_init_TRIG,loop_INVERT
   PLAY Diesel_pump_end,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,SCV_145,SINTEN_LOW+WORK_SPEED   ;set timer
   END_SOUND
 
   INITIATE_SOUND TRIG_FACTORY_CVRESET,NORMAL
   LOAD_MODIFIER MTYPE_SNDCV, SCV_132,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_135,0,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_140,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_141,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_142,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_143,9,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_144,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_145,18,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_146,3,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_147,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_148,64,0    ;Set value
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
