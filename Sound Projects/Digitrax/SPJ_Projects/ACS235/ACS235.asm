   	 LIST		P=18f242,C=132,T=ON,N=75,R=DEC,ST=OFF,W=1,X=ON ;changed 66 to 80 lines
      TITLE   "single scheme- discrete notching SDF3.0 Decoder Series6"
;----------------------------------------------------------------------------
;PROJECT_ID:	256  3/16/23
;   AMTRK 600 25 kV, 60 Hz AC, Catenary Passenger Engine ACS-64  	
; 	ACS-64.SPJ Version 7.0
;BASE CODE:	EL7c4x6d30 -- Discrete Notching,  4 Channel, SDF Version 2.0
;MEMORY:	2 Mega bytes required
;SDF3.0:	Requires -- 	 #INCLUDE	../SFX3p0_snd_cmd.INC
;						and	 #INCLUDE	../SFX3p0_snd_macs.inc
;DMF:		Requires -- 	SW Ver 8 OR newer
;Author:  John McMasters
;	Custom sound scheme. MAR, 2023 NOT for resale.
;	(C) Copyright John McMasters 2023 used with permission.
;CHANNELS:	4
;SERIES:	6
;CV60=0=DIESEL5 SOUND SCHEME
;----------------------------------------------------------------------------
;    AMTRK 600 Passenger Engine ACS-64 Electric
;Specifications:
;
;ACS-64
;
;Type and origin
;Power type	Electric
;Builder	Siemens Mobility
;Order number	Amtrak: 70
;				SEPTA: 15 (option for 3 additional)
;Build date	Amtrak: 2012–2016
;			SEPTA: 2015–2018
;Configuration:	 • AAR	B-B
;Gauge	1,435 mm (4 ft 81⁄2 in) standard gauge
;Trucks	Siemens model SF4
;Wheel diameter	1,117 mm (43.98 in) (new)
;				1,041 mm (40.98 in) (worn)
;Minimum curve	76 m (249.3 ft)
;Wheelbase	9.9 m (32 ft 5.8 in)	(bogie center distance)
;Length	20,320 mm (66 ft 8 in)
;Width	2,984 mm (9 ft 91⁄2 in)
;Height	3,810 mm (12 ft 6 in) (excluding pantograph)
;Axle load	54,250 lb (24,610 kg)
;Adhesive weight	100%
;Loco weight	215,537 lb (97,766 kg)
;Electric system/s
;	12 kV, 25 Hz AC, Catenary
;	12.5 kV, 60 Hz AC, Catenary
;	25 kV, 60 Hz AC, Catenary
;Current pickup(s)	Pantograph
;Traction motors	3-phase, AC, Fully Suspended, Siemens built (Norwood, Ohio)
;Head end power	1,300 hp (970 kW) 3-phase, 60 Hz, 480 VAC, 1000 kVA
;Transmission	Pinion Hollow Shaft Drive w/ Partially Suspended Gearboxes
;MU working	Yes
;Loco brake	Regenerative braking, NYAB Electro-Pneumatic Cheek Mounted Disk Brakes
;Train brakes	Electro-Pneumatic[1]
;Safety systems	FRA standards
;				ACSES II[2]
;
;Performance figures
;Maximum speed
;	125 mph (201 km/h) Service
;	135 mph (217 km/h) Design[3]
;Power output
;	6,400 kW (8,600 hp) Maximum (Short-Time)
;	5,000 kW (6,700 hp) Continuous
;Tractive effort
;Starting:	320 kN (72,000 lbf)
;Continuous (5,000 kW or 6,700 hp):
;			282 kN (63,000 lbf)@40 mph (64 km/h)
;			89 kN (20,000 lbf)@125 mph (200 km/h)
;Short-time (6,400 kW or 8,600 hp):
;	270 kN (61,000 lbf)@53.5 mph (86 km/h)
;	115 kN (26,000 lbf)@125 mph (200 km/h)
;Factor of adh.	2.99 (33.4%)
;Brakeforce	150 kN (34,000 lbf), 5,000 kW (6,700 hp) Maximum[4]
;Career
;Operators	Amtrak, SEPTA
;Numbers
;    • Amtrak: 600–665, 667–670
;    • SEPTA: 901–915
;Nicknames	Sprinters
;Delivered	2013-2018 [5]
;First run	February 7, 2014 with Amtrak
;
;
;---------------------------------------------------
;	INTRODUCTION
; ----------------------------------------------------------------------------
;  This is the Sound Definition Language (SDL) file for a project that can be
;  used for a 16bit premium 8mb sound decoder for a typical Electric Lovomotive. 
;
;	SLOTS / Channels:
;	1	Prime Mover	--	motor sounds vary: idle blip, load sensing
;	2	--Low Idle -- Optional, Idle run is trimmed, also optional ALCO stumble
;	3	Horn (trigger Auto Bell)	-- Selectable, playable, signal toots	-- Grade crossing horn plays rear horn sounds going backwards	-- Analogue Playable Horn/whistle/horn (horn runs that had the transition to higher pressure/ very loud modes)
;	4	Auto Bell ON/OFF Trigger	-- bells may be chained to play for simulation of rope pulls
;	5	Coupler
;	6	Dynamic Brake Fan	-- Variable Dynamics --  will notch up to run 4 and play dynamic fan, speed varied by throttle setting
;	7	Air Compressor
;	8	Radiator Fan	-- High and low alternate random
;	9	Automatic Brake Emergency
;	10	Automatic Brake
;	11	Independent Brake	-- Brake Interlock	-- will lock motor at standstill until released	-- Integrated automatic braking sounds	-- All brakes are controlled by Fkey so that servo motor decoder braking could be syncronized
;	12	Independent Brake Bail OFF
;	13	Sanding Valve
;	14	Hand Brake
;	15	Cab Door
;	16	Engine Compartment Doors
;	17	Air Dryer
;	18	Air Dryer ON
;	19	Reverser (pos 1)
;	20	Reverser Center 1
;	21	Isolation Switch 1
;	22	Alarm Bell
;	23	Flange Squeal
;	24	Short Air Let OFF 1
;	25	Traction Motor
;	26	Starting Delay
;	27	Manual Notching Logic Auto Low/High Idle
;	28	Smart Start Beep
;	29	Brake Set/Release Automatic 1
;	30	Alerter 1
;	31	
;	32 Shutters Open/Closed
;
;Steam	Slots / Channels
;	1	Chuffs (prime mover)
;	2	Chuffs (Rear Articulated) with secondary trigger
;	3	Whistles
;	4	Bells	-- bells may be chained to play for simulation of rope pulls
;	5	Coupler
;	6	Dynamo (single or dual)
;	7	Air Pump (cross compound)
;	8	Oil Burner Blower
;	9	Injector
;	10	Safety Valve
;	11	Independent Brake
;	12	Brake Set / Release
;	13	Ash Dump
;	14	
;	15	Coal Shoveling / Oil Refill
;	16	
;	17	Power Reverser
;	18	Sander 
;	19	Blowdown
;	20	Air Pump Slow
;	21	Air Horn
;	22	
;	23	Wheel Flange
;	24	Boiler Hiss
;	25	Water Refill
;	26	Dumping
;	27	Sanding Valve
;	28	Coasting (rod clanks)
;	29	Open Cylinders
;	30	Cylinder Cock #1
;	31	Cylinder Cock #2
;	32
;
; ----------------------------------------------------------------------------
;	DESIGN NOTES 
; ----------------------------------------------------------------------------
;SET -- SCV_LOAD_MIN,4,0	;Range Volume Scale to a value between 1-5
;						Drifting volume will diminish to this level.
;
;COLD STARTUP FUNCTIONS:
;	Play Startup Sounds: 
;		COLD START SF4 or SF8 (initial time only, otherwise will mute)
;	The following Function Keys will SKIP STARTUP Sounds and begin with RUN sounds:
;	SF8, SF0, SF1, SF2, SF5, SF11	
;
;USE CASES:
;1	Startup Begin All (hold off all other channels until finished)
;		Prime Mover Sounds
;		Servo Enable
;		Muting
;		OpSW Reset
;		Shutdown
;		Servo Disable
;		EStop (silent until restart)
;		Restart (warm or cold)
;		Increaase to Run
;		Idle Trim
;		Startmove
;		Moving (vary with load and notch)
;		Cadence (vary load)
;		Exaust (vary load)
;		Evaluate Motor
;		Run Main
;		Set Acceleration
;		Set Deceleration
;		Run
;2	Horn SF2
;		Whistle SF2
;3	Brake SF10
;		Toots
;4	Bell
;5	Dynamics
;		Hand Brake
;6	Increase Load
;		Decrease Load
;7	Decelerate Brake sounds
;8	Air Compressor
;9	Coupler events
;		Coupler
;		Wheel Flange sound
;		UnCoupler
;10	Distance Event
;		Grade Crossing
;11	Seconday Mover Sounds (Steam Drift, Traction Drift)
;12	Idle (not moving)
;		ALCO Stumble
;		AUX and HEP
;13	Setup Random Sounds (scatter)
;		Cab Chatter
;		User Sounds
;		Air Letoff (SF19)
;		Notch Set (SF21-SF28)
;		Trim Notch and Idle
;		Wheel Flange sound2
;14	Fan
;15	DRYER
;16	Load Default CVs
;
;
;Limits:
;	One SPJ project SDF of multiple schemes can run up to 64Kbytes of code.
;	A single scheme has a 4Kbyte GOTO instruction branch range. (e.g. 2K 2-byte or 4K 4-byte instructions)
;	That means that a BRANCH_TO an address beyond the range limit will not execute...Use BRANCH_EXT for 4-byte instructions 
;
;Changes v7.0:
;	1) Remap function keys as noted above
;	2) Fix Compressor on eStop 2/21/22
;
;Function Key Mapping:
;This version (v7) includes new sections for mapping functions to default to other manufactures F0-F28 keys. 
;Or you can edit the TRIG include file, re-assemble and make your own custom function mapping.
;
;HOW FUNCTION KEY MAPPING WORKS
;Mapping functions to default to other manufactures F0-F28 keys:
;Mapping is done by using symbolic constants to define the hex value for each function key. 
;For example TRIG_WHISTLES_ON is a value of 0x09. 
;The hex value 0x09 is the physical decoder function key (F2 in this case). 
;This is the same as the standard include for TRIG_SF2	 EQU  0x09    ;byte 1, bit1.  
;And the new symbolic constant is TRIG_WHISTLES_ON	 EQU  0x09    ;2-F2: byte 1, bit1. 
;The new base code uses the remapped symbolic name for all functions. 
;So any change to the physical assignment is masked.
;Whereas TRIG_SF4	 EQU 0x0B now has a symbolic value of TRIG_OPENCYLS_ON EQU 0x0B ;4-F4: CYL COCKS
;One of the re-mapped values is TRIG_SAFETY_ON	 EQU  0x24 ;25-F25: BLOWOFF
;This means that when F25 is toggled the SAFETY BLOWOFF sound function will play.
;I have annotated eac EQU statement with the physical decoder button number 
;for example: ;2-F2: byte 1, bit1
;See SR7TrigV7.INC for F-Key values. The ASM code has the INCLUDE for this file name.
;
;
;Use the following CVs to disable optional features:
;CV185 bit values to disable features1:					0-128	[0]
;		bit0 disable  HEP Aux Gen =0 AUX GEN	=1			=1		
;		bit1 disable hard automatic braking 			=2
;		bit2 disable start up sounds					=4
;		bit3 disable Fans								=8
;		bit4 disable toots on direction change			=16
;		bit5 disable grade crossing random play 		=32
;		bit6 disable bell								=64
;		bit7 fuel prime once for Gen1 diesels OR multiple start warning bells Gen2	=128
;CV186 bit values to disable additional features2 		0-128	[0]
;		bit0 enable Low Idle=1		Disabled=0	=1=Enabled
;		bit1 disable Chain bells 						1-4	=2
;		bit2 disable STOKER change to SHOVEL			=4
;		bit3 disable AUTO VARY NOTCH					=8
;		bit4 disable Shaft Driven Compressor 			=16
;		bit5 disable ALCO Idle Stumble=32 Disable=0		=32
;		bit6 disable Auto REAR horn signals: 0=Off, 64=On	=64
;		bit7 disable Dual Prime Movers		 0=Off 128=Dual On
;CV187 bit values to disable additional features3 	0-128 [0]
;		bit0 disable Short Opt Horn/whistle	 Enabled=0,   =1=disabled
;		bit1 disable DEMO ALL HornS/whistles Enabled=on=0,=2=disabled
;		bit2 disable Bell on Idle Exit				=4
;		bit3 disable Motor Lock on F3				=8
;		bit4 disable Start up Muted					=16
;		bit5 disable Dynamic BRAKES					=32
;		bit6 disable 								=64
;		bit7 disable Auto Wheel Slip				=128
;CV188	BIT values to disable additional features4	0-254 [0] 
;		bit0 disable Traction Motor 				=1 Disabled
;		bit1 disable 2								=2
;		bit2 disable 3								=4
;		bit3 disable 4								=8
;		bit4 disable 5								=16
;		bit5 disable 6								=32
;		bit6 disable 7								=64
;		bit7 disable 8								=128
;
;----------------------------------
;NOTES:
; notch volume routine is improved when the volume scale is set by
;	other external events (deceleration, start from standing, acceleration, etc.)

;
		 																 
 
																							  
;----------------------------------
;     FUNCTION KEYS DEFINED:   
;----------------------------------
;(A) Function Key Usage: 
;F0- Lights  [disables start-up sounds when on during initialization]
;F1- Bell (Bells1-4 may be chained)
;F2- Horn/Whistle	CV150 selects Horn/Whistle type	(Playable when bit7 of CV150 is on)	-- Shorter sounds when SF3 ON
;F3- Coupler / Coupler disconnect	[Auto-coupler/brake set by  CV151 max spd]
;		Lift ring and connect plays with SF3 when SF3 is ON
;		Long train consist brake sounds with SF19 ON
;		Steam Wheel Slip on start-up (Enable) /auto coupler
;		(disables toots on SF3) [Leave on for auto toots]
;F4- Dynamic brake fans when moving		
;		Otherwise: Handbrake sequence	[Handbrake while stopped] 
;		Handbrake loop will PLAY until SF14 Off
;		Must be toggled off after start-up (SF10+SF11) or else handbrake will 
;		keep unit from moving...
;F5- Emergency Lights / Short Horn / Reserved for light function (turn on rear light, etc.) / Cold start when on
;		Will play shorter horn on SF2
;F6- Ditchlights (Front -- flashing)
;F7- Wheel Flange (continuous when moving) [Disables bell on SF1]
;F8- Prime Mover ON / Mute control/Start-up	[SF8 to OFF is muted to CV volume, 
;		SF8 ON, set REG volume]
;		On the initial push of SF8 the sound comes up. 
;		A secondary push raises the sound again to normal levels.
;		EStop  on SF8 On with SF10 on shut-down	(disables motor movement until OFF)
;F9- UNCOUPLE / Disconnect
;F10- BRAKE TO STOP -- Usage:
;			Independent Brake/Train Brake	[Leave SF3 OFF for auto brakes] 
;			[SF10 on will enable servo motor stop.]
;			Toggle OFF plays directional toots WHEN SF3 ON
;		1) Slow servo motor to stop using CV3,CV4 momentum values (where
;			larger value (50) takes longer to slow/accelerate)
;		2) Lock motor at standstill to simulate engine brake set. Servo
;			will not resume until SF10 is toggled off.
;		3) While brake is set toggle SF3 on to begin uncoupling. SF3 will play 
;			lift ring noise in preparation for disconnect. 
;		4) Releasing the brake and advancing the throttle will play coupler
;			disconnect when SF3 is already on.
;		Brake rate is determined by BEMF and CV3,4
;	 	Auto Coupler coupler clank and BRAKE if SF3 is ON,  is dir chng and last PK spd below e.g. CV151
;			[Auto-coupler/brake set by  CV151 max spd] SF3 ON for un-coupler
;			[Leave SF3 on for auto coupler]	
;			EStop  on SF8 On with SF10 on Shutdown	(disables servo motor movement until OFF)
;			Toggle OFF plays directional toots WHEN F11 OFF
; 	SF10 OFF -- Release Brake [not SF10 Apply else Release] 
;		Use for start-up -- SF10 on + F11 on to cold start (release F11 to end fuel prime)
;F11- High Fan / Steam=Greaser
;		Allow SUSTAIN horn on SF2
;F12- Coupler / Coupler disconnect	[Autocoupler/brake set by  CV151 max spd]
;		Lift ring and connect plays with SF3 when F11 is ON
;		Long train consist brake sounds with SF19 ON
;		Steam Wheel Slip on start-up (Enable) /auto coupler
;		(disables toots on SF3) [Leave on for auto toots]
;F13- Ditchlights (Rear -- flashing)
;F14- Hand Brake
;F15- EStop when moving and shut-down	(disables motor movement until OFF)
;F16- Air Dryer ON Shutdown
;F17- Brake Set / Release Automatic
;F18- Sanding Valve / Water Fill
;F19- Short Air Let Off	
;F20- Compressor / Air features disable   / Coal Fill  
;		SF20 ON =disable AIR sound/FEATURES, trig BLOWOFF/HNDL_FAST_DRYER1 then compressor ONE time
;		[SF20 OFF enables pop-off, DRYER and compressor]
;F21- Straight to manual notch1
;F22- Engine Compartment Sounds
;F23- Grade crossing Air-horn/Whistle sequence	[Bell stays on until SF23 off]
;F24- Reverser Center
;F25- Enables AUX generator (Disables Auto Notch Vary)
;		Pressing SF30 on the throttle once HEP has already been 
;		engaged will allow the prime mover to come down a little. 
;F26- Manual Notching Up / Run8
;		Leave on for increased load;
;		Leave SF26 OFF to use run trim option: [will increment 
;		notches on acceleration events; 
;		SF27 will decrement run trim.
;F27- Notch down	-- Leave ON for decreased load;
;		Toggle SF27 ON Is notch down for EVENT, 
;			integrate -VE in SPEED_DELTA;
;		SF27 will decrement run trim (i.e., use to select lower trim value).
;		Steam=Wheel-slip, when CV155 is not 0 and SF3 ON
;		Manual Notching Down / Coast
;F28- Straight to manual notch8
;============================ virtual functions: =====================
;F29- FORCE STARTUP SOUNDS / Load / Global Gain
;F30- Enables AUX generator (Disables Auto Notch Vary)
;		Pressing SF30 on the throttle once HEP has already been 
;		engaged will allow the prime mover to come down a little. 
;F34- Cab Chatter / Shutters	--	 Cab Sounds
;
;	==================================  
;Note: Many sound levels have been reduced so that horn/whistle is much louder.
;	Global Gain in the project has been maximized to help. Some low volume 
;	levels may need increases on your model.
;
;	==================================  
;(B) Sound FX defined CV's: 	[decoder CV8=8  reset value in brackets]
;CV3	  Momentum acceleration rate, 0-63, 63=max, (uses after braking to start) [50]
;CV4	  Momentum deceleration rate, 0-63, 63=max, (uses for braking to stop) [40]
;CV7 Version ID 51=Older decoders, 16=Newer SFX2 decoders
;SCV_RESET (CV8)  Reset Decoder values to factory =129=Digitrax
;SCV_19  Advanced Consist [0]
;SCV_21  Advanced Consist SF0-SF8 [248] 
;SCV_22  Advanced Consist SF3-SF7 [52] 
;SCV_54 (Cv60) 	 Road or Switching Mode [64,0,1]
;SCV_57  BEMF Consist + Advanced Consist, 0-15 / 0-15, 15=max (14,14; 15,15) [238]
;CV58  Master Volume, 0-15, 0= max, (SF8 used for Mute) [11] 
; 	DEFAULT_GLBL_GAIN	 	Predefined as 0xC0
;CV60  Scheme select CV, 0=Electric 1=Steam	[0]
;CV64  Logic select CV, 0=Aux Logic output 64=Normal LED outputs	[64]
;SCV_USER_SDF_SUB_ID		;CV105 	[xx] Sub ID is SUB id or project version
;SCV_USER_SDF_FIX_ID		;CV106 	[xx] FIX ID is change id or project release
;CV121 Sound Project SDF Version 0,4,8,16	Read Only Value
;CV122 Product TYpe, 12 = Digitrax SFX sound decoder 	Read Only Value
;CV123 Hardware Version, 64= SDXH166 type decoder	Read Only Value. May vary based on SFX2 decoder loaded into.
;				;CV126= DL config byte
;SNDCV_CONFIGA	;CV129= config byte
;SNDCV_CONFIGB	;CV130= config byte
;SCV_DCONFIG	;CV131= Electric config
;CV132 Engine Notch rate		64-192	[164]
;CV133 Steam Chuff/CAM config, 128=>EXT cam, 1-127=>DRIVER dia in inches [60]
;CV134 Steam gear ratio trim, 32=100% ratio,	[41]
;CV135 vol level when MUTE action is triggered, e.g. SF8=ON	[30]
;SCV_MAIN_PITCH	;CV136
;SCV_137		;CV137
;SCV_138		;SCV138= 
;CV139 Set to trigger distance event	0-64	[30]
;User configurable SoundCV's
;SCV_FREEFORM		;SCV_140+, here the SCV's are SDF defined:
;CV140 PRIME mover Electric/chuff-exhaust vol 0-64	[50]
;CV141 Bell Volume							0-64	[36]
;CV142 WHISTLE/HORN vol						0-64	[42]
;CV143 AIR feature volume, brake release Air sounds Volume 0-64 [49]
;CV144 Engine brake volume 0-64	[55]
;CV145 Miscellaneous volume 0-64	[51] 
;CV146 Delay (24mS counts) between Bells 1-100	[01] 
;notCV147 Time between DRYER cycles Users normally reset DRYER rate to 10 or more[27] 
;notCV148 Time between Air-pump/Compressor cycles	[30] 
;notCV149 Air-pump/Compressor run time				[52] 
;CV150 Horn/Whistle Selector,0=std ,  1/7 =Alternate, +128 for Playable volume [0]
;			HORN SELECTED CV150:
;						Decimal	HEX			Bit
;											7654 3210
;				1		0		0x00		0000 0000
;				2		1		0x01		0000 0001
;				3		2		0x02		0000 0010
;				4		3		0x03		0000 0011
;				5		4		0x04		0000 0100
;				6		5		0x05		0000 0101
;				7		6		0x06		0000 0110
;				PLAYABLE:
;				1		128		0x80		1000 0000
;				2		129		0x81		1000 0001
;				3		130		0x82		1000 0010
;				4		131		0x83		1000 0011
;				5		132		0x84		1000 0100
;						Decimal	HEX			Bit
;CV151 speed limit FOR AUTOMATIC COUPLER/BRAKE on DIR chng. about 1/2MPH per unit
;CV152 defines USER code for this SDF
;CV153 is SUB id
;CV154 Delay brake apply 1-100/ speed at which brake sounds begin to play [12] 
;CV155 Notching mode 0=AUTO, 1=SEMIAUTO, 2=MANUAL NOTCHES	[01] /STEAM Slip Mode: 0=AUTO, 1=SEMIAUTO, 2=MANUAL 						[01]
;CV156 delay for HORN start in 27mS VALUES
;CV157 is FOR bell selection 
	;BELL SELECTED CV157:
			;			Decimal	HEX			Bit
			;								7654 3210
			;	1		0		0x00		0000 0000
			;	2		1		0x01		0000 0001
			;	3		2		0x02		0000 0010
			;	4		3		0x03		0000 0011
			;	5		4		0x04		0000 0100
;CV158 	DIGITRAX RESERVED
;CV159  Min value to tune minimum volume 5-8 [7]
;CV160  VARIANT id or project version	0-254	[000]
;	Extended Custom CV values: These are not easily modified by the user as yet (Use SoundLoader to manually override)
;CV161 Initial Idle Trim 							4-64	[48]
	;INIT_IDLE_TRIM	EQU 0x48	;decimal 4, 16, 32, 64
;CV162		Air Letoff Volume						0-64	[25]
;notCV163	Safety Run Time/ how often to check boiler pressure		0-64	[32]
;notCV164  Level in the guage/ Water Pump Cycle Time, percent full	0-64	 [19]
;notCV165	add fuel to firebox at 150lbs/ Stoker/oil pump run time	0-254	[20]	
;notCV166	Cut-off Speed for wheel slip chuffs				4-9	[9]
;notCV167 	Cut-off Speed for Auto Cyl. blow down			0-64	[7]
;CV168  Sander Volume									0-64	[32]
;CV169	Cutoff Speed for high speed chuffs				40-99	[48]
;CV170	Cut-off Speed LOW	skips brake sounds			4-39	[12]
;CV171	AUX GEN VOLUME									0-64	[40]
;CV172	FAN MOTOR VOLUME								0-64	[30]
;CV173  STEAM blow-down volume							0-64	[50]
;CV174 	AIR DRYER Air sounds Volume 					0-64	[49] 
;CV175	Air compressor volume							0-64	[40]
;CV176	TRACTION MOTOR VOLUME							0-64	[60]			
;CV177	Coupler Slack Volume							0-64	[31]
;CV178	Wheel Flange Volume								0-64	[40]
;CV179  Dynamic brake / Dynamo volume 					0-64	[55]
;CV180	Steam Blow-down/Safety vol						0-64	[59]
;CV181	Power Reverser volume							0-64	[40]
;CV182 	brake release Air sounds Volume 				0-64 	[32] 
;CV183	Water fill/Coal fill Volume						0-64	[54]
;CV184	Steam cock blow down threshold 					1-64	[24]
;CV185 bit values to disable features1:					0-128	[0]
;		bit0 disable  HEP Aux Gen =0 AUX GEN	=1			=1		
;		bit1 disable hard automatic braking 			=2
;		bit2 disable start-up sounds					=4
;		bit3 disable Fans								=8
;		bit4 disable toots on direction change			=16
;		bit5 disable grade crossing random play 		=32
;		bit6 disable bell								=64
;		bit7 fuel prime once for Gen1 diesels OR multiple start warning bells Gen2	=128
;CV186 bit values to disable additional features2 		0-128	[0]
;		bit0 enable Low Idle=1 Disabled=0	=1=Enabled
;		bit1 disable Chain bells 1-4 					=2
;		bit2 disable STOKER change to SHOVEL			=4
;		bit3 disable AUTO VARY NOTCH					=8
;		bit4 disable Shaft Driven Compressor 			=16
;		bit5 disable ALCO Idle Stumble=32 Disable=0		=32
;		bit6 disable Auto REAR horn signals: 0=Off, 64=On	=64
;		bit7 disable Dual Prime Movers		0=Off 128=Dual On
;CV187 bit values to disable additional features3 	0-128 [0]
;		bit0 disable Short Opt Horn/whistle	Enabled=0, =1=disabled
;		bit1 disable DEMO ALL HornS/whistles Enabled=on=0, =2=disabled
;		bit2 disable Bell on Idle Exit				=4;
;		bit3 disable Motor Lock on SF10				=8
;		bit4 disable start-up Muted					=16
;		bit5 disable Dynamic BRAKES					=32
;		bit6 disable 								=64
;		bit7 disable Auto Wheel Slip				=128
;CV188 bit values to disable additional features4	0-128 [0]
;		bit0 disable 1								=1
;		bit0 disable 2								=2
;		bit2 disable 3								=4;
;		bit3 disable 4								=8
;		bit4 disable 5								=16
;		bit5 disable 6								=32
;		bit6 disable 7								=64
;		bit7 disable 8								=128
;CV189 Water Alarm Volume			0-64	[45]
;notCV190 Cut-off Speed for high speed chuffs				40-99	[48]	
;notCV191 Stoker/oil pump run time		0-254	[20]	
;notCV192 Drift Volume					0-64	[20]										1-100	[30]
;notCV193 Cut-off Speed for LONG HORN	4-9	[9]
;notCV193 Sound Project TYPE
;Possible Values are :
;	:			Decimal	HEX			Bit
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
;notCV194 Sound Project Memory Required [1=512kb, 2=1024kb, 3=8mb, 4=4mb, 8=8mb, 16=16mb].
;notCV195 Sound Project JMRI Definition(0-255)
;CV200 RESERVED
;CV201 RESERVED
;CV202 RESERVED
;notCV203 Decoder Memory Available (read only) [1=512kb, 2=1024kb, 3=8mb, 4=4mb, 8=8mb, 16=16mb].
	
;==================================================
;----------------------------------
;macros to ASSEMBLE  SDF
;----------------------------------
VERNO	SET 	VERSION_3	;SDF ver 3.00
	NOLIST
	 #INCLUDE	../SFX3p0_snd_cmd.INC
	 #INCLUDE	../SFX3p0_snd_macs.inc
    LIST
CUSTOM_SDF			EQU	0xFB	;cv152	AUTHOR code John McMasters (CV 251)
USER_SDF_SUB_ID		set	70		;cv105  AND CV160	;Version 6 where 60=v6.1
USER_SDF_FIX_ID		set	0		;cv106  AND CV160	;Version 7.0 where 0=v6.1
PROJECT_ID			EQU	256		;cv153	000-254	<ENTER ID HERE>	
;
;REVISED FROM SCVs 1/5/21
;<ENTER HERE> Modify these constants for your custom model 
DRIFT_VOLUME			EQU 55	;Drift Volume		0-64	[20]										1-100	[30]
START_SLIP_SPEED		EQU 3	;Minimum Speed to Slip 	0-32	[3]
DRYER_RATE				EQU 47	;Time between DRYER cycles [12]
AIRPUMP_START_RATE		EQU 30	;Time between Compressor cycles [30]
AIR_PRESSURE_LIMIT		EQU 88	;COMPRESSOR DURATION



 	 #INCLUDE	EL7TrigV7.INC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;PRIME MOVER CONSTANTS:
;Idle speed - 315 rotations per minute (rpm)
MIN_RPM		EQU	315	
;645 series has a maximum engine speed of between 900 and 950
MAX_RPM		EQU	900

 	 #INCLUDE	EL7c4x6d30base.INC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	 ;;;;;;;;;;;;FOLLOW WITH VERY LAST CV RESET CODE HERE:
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV3,31,0 ;CV3	  Momentum acceleration rate, 0-63, 63=max, (uses after braking to start) [50]
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV4,28,0 ;CV4	  Momentum deceleration rate, 0-63, 63=max, (uses for braking to stop) [40]
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV23,50,0;SCV_23  Acceleration Mass 0-255 [50] 
	 ;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV24,50,0;SCV_24  Deceleration Mass 0-255 [50] 
	 ;cutback22:LOAD_MODIFIER	MTYPE_SNDCV,SCV_USER_SDF_ID,AuthorCode,0	;CV152 defines USER code for this SDF
	;cutback22:LOAD_MODIFIER	MTYPE_SNDCV,SCV_PROJECT_ID,PROJECT_ID,0		;CV153 [0]  Identifies Project within author ID where nnn is serial 1-255
	;cutback22:LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV160,USER_SDF_SUB_ID,0 	;CV160 is SUB id
	;cutback22:LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV106,PROJECT_ID,0		;CV106 [0]  Identifies Project within author ID where nnn is serial 1-255
	;cutback22:LOAD_MODIFIER	MTYPE_SNDCV,SCV_CV153,PROJECT_ID,0		;CV153 [0]  Identifies Project within author ID where nnn is serial 1-255

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;X LOAD_MODIFIER	MTYPE_SNDCV,SCV_FEATURE_DISABLE,128,0		;CV185 is bit values to disable features1:
;				bit0 disable  HEP Aux Gen =1  AUX GEN	=0
;				bit1 disable hard automatic braking 	=2
;				bit2 disable startup sounds				=4
;				bit3 disable Fans						=8
;				bit4 disable toots on direction change	=16
;				bit5 disable grade crossing random play =32
;				bit6 disable bell						=64
;				bit7 fuel prime once for Gen1 diesels OR multiple start warning bells Gen2	=128		
	;X LOAD_MODIFIER	MTYPE_SNDCV,SCV_FEATURE2_DISABLE,2,0	;CV186 is bit values to disable additional features2 
;				bit0 disable Low Idle=1 Enabled=0	=0=Enabled
;				bit0 disable STEAM BLOWER=1 BOILER=0		=1
;				bit1 disable Chain bells 1-4 				=2
;				bit2 disable STOKER change to SHOVEL		=4
;				bit3 disable AUTO VARY NOTCH				=8
;				bit4 disable Shaft Driven Compressor 		=16
;				bit5 disable ALCO Idle Stumble=32 Disable=0	=32
;				bit6 disable Auto REAR HORN/WHISTLE signals: 00=Off 	=64
;				bit7 disable Duel Prime Movers		0 = Off =128 = Duel On
	;X LOAD_MODIFIER	MTYPE_SNDCV,SCV_FEATURE3_DISABLE,0,0	;CV187 is bit values to disable additional features3 
;				bit0 disable Short Horn/whistle				Enabled=0	=1 
;				bit1 disable DEMO ALL HornS/whistles		Enabled=2	=0 Disabled
;				bit2 disable Bell on Idle Exit							=4	
;				bit3 disable Motor Lock on SF10							=8
;				bit4 disable Startup MUTED								=16 Disabled
;				bit5 disable Dynamic BRAKES								=32
;				bit6 disable Run High									=64
;				bit7 disable Auto Wheel Slip							=128
	 ;;;;;;3/26/18;;;;;;;;;;;;;;;;;;;;;;;;;;;;;MUST BE VERY LAST CODE IN FINAL CHANNEL;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;CV189 Water Alarm Volume							0-64	[45]
	;CV190 Cutoff Speed for high speed chuffs			40-99	[48]	
	;CV191 Drift Volume									0-64	[20]							
	;CV192 Cutoff Speed for LONG HORN					4-9	[9]
	;CV193 Sound Project TYPE
	;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_SOUND_PROJ_TYPE,128,0 	;CV193 Sound Project TYPE	0-255	[128]										1-100	[30]
;Possible Values are :
;	:			Decimal	HEX			Bit
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
	;CV194 Sound Project Memory Required [1=512kb, 2=1024kb, 3=8mb, 4=4mb, 8=8mb, 16=16mb].
	;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_SOUND_PROJ_MEMREQ,1,0 	;CV194 Sound Project Memory Required	0-255	[128]										1-100	[30]
	;CV195 Sound Project JMRI Definition
	;cutback20:LOAD_MODIFIER	MTYPE_SNDCV,SCV_SOUND_PROJ_JMRIDEF,59,0 	;CV195 Sound Project JMRI Definition	0-255	[128]										1-100	[30]
	;CV_196 ;RESERVED 
	;CV_197 ;RESERVED 
	;CV_198 ;RESERVED 
	;CV_199 ;RESERVED 
	;CV_200 ;RESERVED 
	;CV_201 ;RESERVED (221)	
	;CV_202 ;NON_SOUND_JMRIDEF ; JMRI Definition (0-255) 
	;CV_203 ;Decoder Memory Available (read only) [1=512kb, 2=1024kb, 3=8mb, 4=4mb, 8=8mb, 16=16mb].
	END_SOUND
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;------------------------------------------------
; End of Scheme 0
;------------------------------------------------
;===============================================================================
;------------------------------------------------
; End of SDL Code
;------------------------------------------------
END_SDL
	SKEME_START	CLOSE_SKEME		;START of "CLOSE_SKEME" is SDL end
	SKEME_END	CLOSE_SKEME

;This should run in all assmeblies to confirm limits not exceeded for VERSION assumed
VERSION_CHECKS
	IF	ICNT>MAX_INITIATES
   INITIATE EXCEEDS DEFINED LIMIT FOR THIS VERSION
	 ELSE
	ENDIF
;	IF	CCNT> (MAX_CHANNELS+1) 
;   CHANNELS USED EXCEEDS DEFINED LIMIT FOR THIS VERSION
;	 ELSE
;	ENDIF
	IF	SCNT>MAX_SCHEMES
   SCHEMES USED EXCEEDS DEFINED LIMIT FOR THIS VERSION
	 ELSE
	ENDIF  
	ICNT = ICNT+1
	TEMP = MAX_INITIATES-ICNT
	MAX_SCHEMES = MAX_SCHEMES +0
	MAX_INITIATES = MAX_INITIATES +0
	MAX_CHANNELS = MAX_CHANNELS +0

END_SDF
	END		;==================================
