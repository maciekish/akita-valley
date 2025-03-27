   LIST   P=18f242,C=132,T=ON,R=DEC,ST=OFF
   TITLE  CITYSNDSEXAMPLE Author: Fred Miller MMR
VERNO	EQU	0x20		;ver 2.00 
;----------------------------------------------------------------------------
;   This Sound Definition Langauge (SDL) was generated
;   by the SPJHelper Design Tool on 4/24/14
;----------------------------------------------------------------------------
;     FUNCTION KEYS DEFINED:   
;----------------------------------
;   SOUND WAVE FILE HANDLES
;----------------------------------
   cblock   0   ; Start assigning location of  Sound Clips 0, 1, 2, etc.
HNDL_MUTE   ; Internal SILENCE value = 0
Children
Puppy_barking
Crowds
Crows
StreetSounds
Truck_Horn
New_Car
Old_Car
Crossing_Bells
ChurchBell
Clock_Strike
Telephone
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
SCV_140   ;CV140  Voice One Cycle Time [5]
SCV_141   ;CV141  Voice One Volume [64]
SCV_142   ;CV142  Voice Two Cycle Time [8]
SCV_143   ;CV143  Voice Two Volume [64]
SCV_144   ;CV144  Voice Three Cycle Time [11]
SCV_145   ;CV145  Voice Three Volume [64]
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
 
   INITIATE_SOUND TRIG_SND_ACTV11,NORMAL
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,1,0    ;Set timer
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,3,0    ;Set timer
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL2,4,0    ;Set timer
   END_SOUND
 
   INITIATE_SOUND TRIG_SCAT2,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_145,SCALE_F  ;Set Volume
   PLAY ChurchBell,no_loop,loop_STD
   PLAY ChurchBell,no_loop,loop_STD
   PLAY ChurchBell,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,124,DELAY_GLOBAL
   PLAY Crossing_Bells,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,124,DELAY_GLOBAL
   PLAY Clock_Strike,no_loop,loop_STD
   PLAY Clock_Strike,no_loop,loop_STD
   PLAY Clock_Strike,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,124,DELAY_GLOBAL
   PLAY Telephone,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL2,SCV_144,SINTEN_LOW+WORK_SPEED   ;set timer
   END_SOUND
;---------------------------------------------
;  START CHANNEL 2
;---------------------------------------------
CHNL_02_S0
   CHANNEL_START	2
;---------------------------------------------
 
   INITIATE_SOUND TRIG_SCAT1,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_143,SCALE_F  ;Set Volume
   PLAY StreetSounds,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,124,DELAY_GLOBAL
   PLAY Truck_Horn,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,124,DELAY_GLOBAL
   PLAY New_Car,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,124,DELAY_GLOBAL
   PLAY Old_Car,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL1,SCV_142,SINTEN_LOW+WORK_SPEED   ;set timer
   END_SOUND
;---------------------------------------------
;  START CHANNEL 3
;---------------------------------------------
CHNL_03_S0
   CHANNEL_START   3
;---------------------------------------------
 
   INITIATE_SOUND TRIG_SCAT0,NORMAL
   LOAD_MODIFIER MTYPE_GAIN,IMMED_GAIN_MODIFY,SCV_141,SCALE_F  ;Set Volume
   PLAY Children,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,249,DELAY_GLOBAL
   PLAY Puppy_barking,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,249,DELAY_GLOBAL
   PLAY Crowds,no_loop,loop_STD
   DELAY_SOUND DELAY_THIS,249,DELAY_GLOBAL
   PLAY Crows,no_loop,loop_STD
   LOAD_MODIFIER MTYPE_SCATTER,SCAT_CMD_PERIOD+SCAT_CHNL0,SCV_140,SINTEN_LOW+WORK_SPEED   ;set timer
   END_SOUND
 
   INITIATE_SOUND TRIG_FACTORY_CVRESET,NORMAL
   LOAD_MODIFIER MTYPE_SNDCV, SCV_132,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_135,0,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_140,5,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_141,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_142,8,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_143,64,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_144,11,0    ;Set value
   LOAD_MODIFIER MTYPE_SNDCV, SCV_145,64,0    ;Set value
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
