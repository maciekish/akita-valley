   LIST   P=18f242,C=132,T=ON,R=DEC,ST=OFF
   TITLE  JRE235 Author: Maciej Swic
VERNO	EQU	0x20		;ver 2.00 
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
HNDL_MUTE   ; Internal SILENCE value = 0
E235_startup
E235_idle
E235_idle_run_1
E235_run_1
E235_run_2
E235_run_3
E235_run_1_idle
E235_shutdown
E233_horn_in
E233_horn_loop
E233_horn_out
E235_brake_release
E235_pantograph_up
E235_pantograph_down
   ENDC
;-----------------------------------------
;   INCLUDED DIGITRAX PROPRIETARY FILES
;-----------------------------------------
#INCLUDE   SERIES6_Snd_cmd.INC
#INCLUDE   SERIES6_snd16_macs.INC
;-----------------------------------------
;   SYSTEM VARIABLES
;-----------------------------------------
AuthorCode   EQU   0x86   ; AUTHOR code Maciej Swic (CV 252)
SubID   EQU   5  ; Project #5 - Designed using SPJHelper
;------------------------------------------
;   LOCATION FOR CV ADDRESSES
;------------------------------------------
   cblock   SCV_FREEFORM   ; Start assigning (after std Digitrax) for CV140, CV141, etc.
SCV_140   ;CV140  Power Unit Volume [64]
SCV_141   ;CV141  Horn Volume [64]
SCV_142   ;CV142  Brake Release Volume [64]
SCV_143   ;CV143  Pantograph Volume [64]
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
NOTCH0        EQU 0x00
NOTCH1        EQU	0x10  ; Gives a ~33% split at a Notch Rate of 64 (CV132)
NOTCH2        EQU	0x20
NOTCH3        EQU	0x50
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
 
   ; Startup Sound
   INITIATE_SOUND TRIG_SND_ACTV11,NORMAL+NO_PREEMPT_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 0,0
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_143,SCALE_F  ;Set Volume
   PLAY HNDL_MUTE,no_loop,loop_STD
   PLAY E235_pantograph_up,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   DELAY_SOUND DELAY_THIS,40,DELAY_GLOBAL
   PLAY E235_startup,no_loop,loop_STD
   PLAY E235_idle,no_loop,loop_STD
   END_SOUND

   ; Shutdown Sound
   INITIATE_SOUND TRIG_SND_ACTV11,NOT_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   PLAY E235_idle,no_loop,loop_STD
   PLAY E235_shutdown,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_143,SCALE_F  ;Set Volume
   PLAY E235_pantograph_down,no_loop,loop_STD
   END_SOUND
 
   ; Idle to Run Sound
   INITIATE_SOUND T_SPD_IDLEXIT,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   PLAY E235_idle_run_1,no_loop,loop_STD
   END_SOUND

   ; Acceleration Sound
   INITIATE_SOUND T_SPD_ACCEL1,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 4,2
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH1,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH1
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH2,COMP_7LSB,SKIP_GRTR  ; Skip next if WORK_NOTCH > NOTCH2
   PLAY E235_run_1 ,no_loop,loop_STD
   
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH1,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH1
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH3,COMP_7LSB,SKIP_GRTR  ; Skip next if WORK_NOTCH > NOTCH3
   PLAY E235_run_2,no_loop,loop_STD
   
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH3,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH3
   PLAY E235_run_3,no_loop,loop_STD
   
   END_SOUND
 
   ; Deceleration Sound
   INITIATE_SOUND T_SPD_DECEL1,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 9,7
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch

   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH1,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH1
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH2,COMP_7LSB,SKIP_GRTR  ; Skip next if WORK_NOTCH > NOTCH2
   PLAY E235_run_1 ,no_loop,loop_STD
   
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH1,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH1
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH3,COMP_7LSB,SKIP_GRTR  ; Skip next if WORK_NOTCH > NOTCH3
   PLAY E235_run_2,no_loop,loop_STD
   
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH3,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH3
   PLAY E235_run_3,no_loop,loop_STD
   
   END_SOUND

   ; Run Sound
   INITIATE_SOUND T_SPD_RUN,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 6,5
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch

   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH1,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH1
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH2,COMP_7LSB,SKIP_GRTR  ; Skip next if WORK_NOTCH > NOTCH2
   PLAY E235_run_1,loop_till_init_TRIG,loop_INVERT
   
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH1,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH1
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH3,COMP_7LSB,SKIP_GRTR  ; Skip next if WORK_NOTCH > NOTCH3
   PLAY E235_run_2,loop_till_init_TRIG,loop_INVERT
   
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH3,COMP_7LSB,SKIP_LESS  ; Skip next if WORK_NOTCH < NOTCH3
   PLAY E235_run_3,loop_till_init_TRIG,loop_INVERT
   
   END_SOUND
 
   ; Run to Idle Sound
   INITIATE_SOUND TRIG_MOVING,NOT_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH0,COMP_7LSB,SKIP_SAME
   PLAY E235_run_1_idle,no_loop,loop_STD
   END_SOUND

   ; Idle Sound
   INITIATE_SOUND TRIG_SND_ACTV11,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 9,7
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_140,SCALE_F  ;Set Volume
   LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   MASK_COMPARE WORK_NOTCH,IMMED_DATA,NOTCH1,COMP_7LSB,SKIP_GRTR
   PLAY E235_idle,loop_till_init_TRIG,loop_INVERT
   END_SOUND
;---------------------------------------------
;  START CHANNEL 2
;---------------------------------------------
CHNL_02_S0
   CHANNEL_START	2
;---------------------------------------------
 
   INITIATE_SOUND TRIG_SND_ACTV11,NORMAL
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK    ;Enable Motor Run
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set to ~4.5 minutes
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,31,0    ;Set to ~4.5 minutes
   END_SOUND

   INITIATE_SOUND TRIG_SF2,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_141,SCALE_F  ;Set Volume
   PLAY E233_horn_in,no_loop,loop_STD
   PLAY E233_horn_loop,loop_till_init_TRIG,loop_INVERT
   PLAY E233_horn_out,no_loop,loop_STD
   END_SOUND
;---------------------------------------------
;  START CHANNEL 3
;---------------------------------------------
CHNL_03_S0
   CHANNEL_START   3
;---------------------------------------------

;---------------------------------------------
;  START CHANNEL 4	'Series 6 Premium Only
;---------------------------------------------
CHNL_04_S0
   CHANNEL_START   4
;---------------------------------------------

   ; Brake Release
   INITIATE_SOUND T_SPD_IDLEXIT,ZAP
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_142,SCALE_F  ;Set Volume
   MASK_COMPARE WORK_STATUS_BITS,IMMED_DATA,WKSB_RUN_MASK,COMP_7LSB,SKIP_SAME  ; Skip next if motor is NOT running
   PLAY E235_brake_release,no_loop,loop_STD
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
