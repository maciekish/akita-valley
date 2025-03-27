   LIST   P=18f242,C=132,T=ON,R=DEC,ST=OFF
   TITLE  JRE235 Author: Maciej Swic
VERNO	EQU	0x20		;ver 2.00 
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
E235_brake
E235_joint
E235_doors_ringer
E235_track_1
E235_track_2
E235_track_3
E235_doors_are_closing
E235_doors_be_careful
E235_doors_actually_closing
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
SCV_140   ;CV140  Motor Volume [64]
SCV_141   ;CV141  Horn Volume [64]
SCV_142   ;CV142  Brake Release Volume [64]
SCV_143   ;CV143  Braking Volume [64]
SCV_144   ;CV144  Pantograph Volume [64]
SCV_147   ;CV147  Joint Volume [64]
SCV_148   ;CV147  Joint Rate [?]
SCV_150   ;CV150  Announement Volume [64]
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
WORK_MOVING      EQU	0x1A ; 1 = Moving, 0 = Not Moving
WORK_BRAKED 	  EQU	0x1B ; 1 = Braked, 0 = Not Braked
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
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,SCV_148,SINTEN_LOW+WORK_SPEED   ; Set joint timer
   LOAD_MODIFIER MTYPE_BLEND, BLEND_CURRENT_CHNL, 0,0
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_144,SCALE_F  ;Set Volume
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
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_144,SCALE_F  ;Set Volume
   PLAY E235_pantograph_down,no_loop,loop_STD
   END_SOUND
 
   ; Idle to Run Sound
   INITIATE_SOUND T_SPD_IDLEXIT,RUN_WHILE_TRIG
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_MOVING     ,1,0    ;Set to moving
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_BRAKED     ,0,0    ;Set to NOT braked
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
   MASK_COMPARE WORK_MOVING       ,IMMED_DATA,0,COMP_7LSB,SKIP_SAME    ; Skip next if NOT moving
   PLAY E235_run_1_idle,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_MOVING     ,0,0    ;Set to NOT moving
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
 
   ; Start Motor
   INITIATE_SOUND TRIG_SND_ACTV11,NORMAL
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_OR+WORK_STATUS_BITS,WKSB_RUN_MASK,MERGE_ALL_MASK    ;Enable Motor Run
   LOAD_MODIFIER MTYPE_WORK_IMMED, FMATH_LODE+WORK_DISTANCE,0,0   ;reset work register
   LOAD_MODIFIER MTYPE_SNDCV, SCV_139,1,0    ;Set to ~4.5 minutes
   END_SOUND

   ; Horn
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

   INITIATE_SOUND TRIG_SF6,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_142,SCALE_F  ;Set Volume
   PLAY E235_doors_ringer,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,16,DELAY_GLOBAL
   PLAY E235_track_3,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,33,DELAY_GLOBAL
   PLAY E235_doors_are_closing,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,29,DELAY_GLOBAL
   PLAY E235_doors_be_careful,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,41,DELAY_GLOBAL
   PLAY E235_doors_actually_closing,no_loop,loop_STD
   END_SOUND
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

   ; Braking
   INITIATE_SOUND TRIG_MOVING,NOT_TRIG
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_143,SCALE_F  ;Set Volume
   MASK_COMPARE WORK_BRAKED,IMMED_DATA,1,COMP_7LSB,SKIP_SAME
   ;SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING ; Skip if not moving
   PLAY E235_brake,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_WORK_IMMED,FMATH_LODE+WORK_BRAKED     ,1,0    ;Set to braked
   END_SOUND

   ; Joint
   ;INITIATE_SOUND TRIG_SCAT0,NORMAL
   ;LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_147,SCALE_F  ;Set Volume
   ;LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   ;SKIP_ON_TRIGGER TRIG_FALSE,TRIG_MOVING ; Skip if not moving
   ;PLAY E235_joint,no_loop,loop_STD
   ;LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,SCV_148,SINTEN_LOW+WORK_SPEED   ;set timer
   ;END_SOUND
   
   ;INITIATE_SOUND TRIG_DISTANCE,NORMAL
   ;LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_147,SCALE_F  ;Set Volume
   ;LOAD_MODIFIER MTYPE_PITCH,ANALOG_PITCH_MODIFY+WORK_NOTCH,MAXP_DIESEL,DITHERP_DIESEL  ;set pitch
   ;PLAY E235_joint,no_loop,loop_STD
   ;END_SOUND

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
