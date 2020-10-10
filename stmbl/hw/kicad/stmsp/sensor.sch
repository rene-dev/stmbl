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
Sheet 4 10
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
L C C27
U 1 1 5918C487
P 5950 3350
F 0 "C27" H 5975 3450 50  0000 L CNN
F 1 "100n" H 5975 3250 50  0000 L CNN
F 2 "stmbl:C_0603" H 5988 3200 50  0001 C CNN
F 3 "" H 5950 3350 50  0000 C CNN
	1    5950 3350
	1    0    0    -1  
$EndComp
$Comp
L TLE5012 U7
U 1 1 5923BF54
P 5000 3250
F 0 "U7" H 4800 3500 60  0000 C CNN
F 1 "TLE5012" H 5250 3000 60  0000 C CNN
F 2 "stmbl:SOIC-8-N" H 4950 3250 60  0001 C CNN
F 3 "" H 4950 3250 60  0001 C CNN
	1    5000 3250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR033
U 1 1 5923BFBE
P 5450 3350
F 0 "#PWR033" H 5450 3100 50  0001 C CNN
F 1 "GND" H 5450 3200 50  0000 C CNN
F 2 "" H 5450 3350 50  0000 C CNN
F 3 "" H 5450 3350 50  0000 C CNN
	1    5450 3350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5450 3350 5350 3350
Text HLabel 4550 3150 0    60   Input ~ 0
CSQ
Text HLabel 4550 3250 0    60   Input ~ 0
SCK
Text HLabel 4550 3350 0    60   Input ~ 0
DATA
Wire Wire Line
	4550 3350 4650 3350
Wire Wire Line
	4550 3250 4650 3250
Wire Wire Line
	4550 3150 4650 3150
$Comp
L GND #PWR034
U 1 1 5923C005
P 5000 3750
F 0 "#PWR034" H 5000 3500 50  0001 C CNN
F 1 "GND" H 5000 3600 50  0000 C CNN
F 2 "" H 5000 3750 50  0000 C CNN
F 3 "" H 5000 3750 50  0000 C CNN
	1    5000 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 3550 5000 3750
$Comp
L +5V #PWR035
U 1 1 5923C047
P 4650 2550
F 0 "#PWR035" H 4650 2400 50  0001 C CNN
F 1 "+5V" H 4650 2690 50  0000 C CNN
F 2 "" H 4650 2550 50  0000 C CNN
F 3 "" H 4650 2550 50  0000 C CNN
	1    4650 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 2750 5000 2950
$Comp
L +3.3V #PWR036
U 1 1 5923C08F
P 5350 2550
F 0 "#PWR036" H 5350 2400 50  0001 C CNN
F 1 "+3.3V" H 5350 2690 50  0000 C CNN
F 2 "" H 5350 2550 50  0000 C CNN
F 3 "" H 5350 2550 50  0000 C CNN
	1    5350 2550
	1    0    0    -1  
$EndComp
$Comp
L JUMPER3_NC JP2
U 1 1 5923C12B
P 5000 2650
F 0 "JP2" H 5050 2550 50  0000 L CNN
F 1 "JUMPER3_NC" H 5000 2750 50  0000 C BNN
F 2 "stmbl:SOLDER_JUMPER_3_NC" H 5000 2650 50  0001 C CNN
F 3 "" H 5000 2650 50  0000 C CNN
	1    5000 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 2550 4650 2650
Wire Wire Line
	4650 2650 4750 2650
Wire Wire Line
	5250 2650 5350 2650
Wire Wire Line
	5350 2650 5350 2550
Wire Wire Line
	5950 2850 5950 3200
Wire Wire Line
	3350 2850 5950 2850
Connection ~ 5000 2850
Wire Wire Line
	3350 3650 5950 3650
Wire Wire Line
	5950 3650 5950 3500
Connection ~ 5000 3650
$Comp
L TLE5012 U10
U 1 1 5945AC83
P 3350 3250
F 0 "U10" H 3150 3500 60  0000 C CNN
F 1 "TLE5012" H 3600 3000 60  0000 C CNN
F 2 "stmbl:SOIC-8-N" H 3300 3250 60  0001 C CNN
F 3 "" H 3300 3250 60  0001 C CNN
	1    3350 3250
	-1   0    0    -1  
$EndComp
Text HLabel 3800 3150 2    60   Input ~ 0
CSQ
Text HLabel 3800 3250 2    60   Input ~ 0
SCK
Text HLabel 3800 3350 2    60   Input ~ 0
DATA
Wire Wire Line
	3800 3350 3700 3350
Wire Wire Line
	3800 3250 3700 3250
Wire Wire Line
	3800 3150 3700 3150
Wire Wire Line
	3350 3650 3350 3550
Wire Wire Line
	3350 2850 3350 2950
$Comp
L GND #PWR037
U 1 1 5945AE01
P 2900 3350
F 0 "#PWR037" H 2900 3100 50  0001 C CNN
F 1 "GND" H 2900 3200 50  0000 C CNN
F 2 "" H 2900 3350 50  0000 C CNN
F 3 "" H 2900 3350 50  0000 C CNN
	1    2900 3350
	0    1    1    0   
$EndComp
Wire Wire Line
	2900 3350 3000 3350
$EndSCHEMATC
