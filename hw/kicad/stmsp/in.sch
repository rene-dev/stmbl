EESchema Schematic File Version 2
LIBS:stmbl
LIBS:transistors
LIBS:stm32
LIBS:conn
LIBS:power
LIBS:74xgxx
LIBS:74xx
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:Altera
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:brooktre
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:intel
LIBS:interface
LIBS:ir
LIBS:Lattice
LIBS:linear
LIBS:logo
LIBS:maxim
LIBS:memory
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:microchip
LIBS:microcontrollers
LIBS:motor_drivers
LIBS:motorola
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:opto
LIBS:Oscillators
LIBS:philips
LIBS:Power_Management
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:Xicor
LIBS:stmsp-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L R R31
U 1 1 59409106
P 4400 2200
AR Path="/594079D4/59409106" Ref="R31"  Part="1" 
AR Path="/5940FC3C/59409106" Ref="R36"  Part="1" 
F 0 "R36" V 4480 2200 50  0000 C CNN
F 1 "10k" V 4400 2200 50  0000 C CNN
F 2 "stmbl:R_0603" V 4330 2200 50  0001 C CNN
F 3 "" H 4400 2200 50  0000 C CNN
	1    4400 2200
	1    0    0    -1  
$EndComp
$Comp
L R R32
U 1 1 5940910D
P 4400 2700
AR Path="/594079D4/5940910D" Ref="R32"  Part="1" 
AR Path="/5940FC3C/5940910D" Ref="R37"  Part="1" 
F 0 "R37" V 4480 2700 50  0000 C CNN
F 1 "1.5k" V 4400 2700 50  0000 C CNN
F 2 "stmbl:R_0603" V 4330 2700 50  0001 C CNN
F 3 "" H 4400 2700 50  0000 C CNN
	1    4400 2700
	1    0    0    -1  
$EndComp
$Comp
L C C31
U 1 1 5940911B
P 4650 2700
AR Path="/594079D4/5940911B" Ref="C31"  Part="1" 
AR Path="/5940FC3C/5940911B" Ref="C32"  Part="1" 
F 0 "C32" H 4675 2800 50  0000 L CNN
F 1 "1n" H 4675 2600 50  0000 L CNN
F 2 "stmbl:C_0603" H 4688 2550 50  0001 C CNN
F 3 "" H 4650 2700 50  0000 C CNN
	1    4650 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2350 4400 2550
Wire Wire Line
	4400 2450 4750 2450
Connection ~ 4400 2450
Wire Wire Line
	4400 2850 4400 2950
Wire Wire Line
	4400 2950 4650 2950
Wire Wire Line
	4650 2850 4650 3050
$Comp
L GND #PWR064
U 1 1 59409131
P 4650 3050
AR Path="/594079D4/59409131" Ref="#PWR064"  Part="1" 
AR Path="/5940FC3C/59409131" Ref="#PWR067"  Part="1" 
F 0 "#PWR067" H 4650 2800 50  0001 C CNN
F 1 "GND" H 4650 2900 50  0000 C CNN
F 2 "" H 4650 3050 50  0000 C CNN
F 3 "" H 4650 3050 50  0000 C CNN
	1    4650 3050
	1    0    0    -1  
$EndComp
Connection ~ 4650 2950
Wire Wire Line
	4400 2050 4400 1950
Wire Wire Line
	4400 1950 4300 1950
Text HLabel 4300 1950 0    60   Input ~ 0
IN
Text HLabel 4750 2450 2    60   Input ~ 0
I
Wire Wire Line
	4650 2550 4650 2450
Connection ~ 4650 2450
Text Notes 5000 2150 0    60   ~ 0
3.3 / 1.5V * (10k + 1.5k) = 25.3V
$EndSCHEMATC
