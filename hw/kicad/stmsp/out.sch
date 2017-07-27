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
Sheet 8 10
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
L R R34
U 1 1 5940D06F
P 5200 2700
AR Path="/5940CCE8/5940D06F" Ref="R34"  Part="1" 
AR Path="/59418122/5940D06F" Ref="R39"  Part="1" 
F 0 "R34" V 5280 2700 50  0000 C CNN
F 1 "1k" V 5200 2700 50  0000 C CNN
F 2 "stmbl:R_0603" V 5130 2700 50  0001 C CNN
F 3 "" H 5200 2700 50  0000 C CNN
	1    5200 2700
	1    0    0    -1  
$EndComp
$Comp
L R R35
U 1 1 5940D076
P 5450 2450
AR Path="/5940CCE8/5940D076" Ref="R35"  Part="1" 
AR Path="/59418122/5940D076" Ref="R40"  Part="1" 
F 0 "R35" V 5530 2450 50  0000 C CNN
F 1 "120" V 5450 2450 50  0000 C CNN
F 2 "stmbl:R_0603" V 5380 2450 50  0001 C CNN
F 3 "" H 5450 2450 50  0000 C CNN
	1    5450 2450
	0    1    1    0   
$EndComp
$Comp
L Q_NJFET_GSD Q1
U 1 1 5940D07D
P 4900 2450
AR Path="/5940CCE8/5940D07D" Ref="Q1"  Part="1" 
AR Path="/59418122/5940D07D" Ref="Q2"  Part="1" 
F 0 "Q1" H 5200 2500 50  0000 R CNN
F 1 "Q_NJFET_GSD" H 5550 2400 50  0000 R CNN
F 2 "stmbl:SOT-23" H 5100 2550 50  0001 C CNN
F 3 "" H 4900 2450 50  0000 C CNN
	1    4900 2450
	-1   0    0    -1  
$EndComp
$Comp
L D D4
U 1 1 5940D084
P 4800 1900
AR Path="/5940CCE8/5940D084" Ref="D4"  Part="1" 
AR Path="/59418122/5940D084" Ref="D6"  Part="1" 
F 0 "D4" H 4800 2000 50  0000 C CNN
F 1 "200V 1A" H 4800 1800 50  0000 C CNN
F 2 "stmbl:SOD-123" H 4800 1900 50  0001 C CNN
F 3 "" H 4800 1900 50  0000 C CNN
	1    4800 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	4800 2050 4800 2250
Wire Wire Line
	5100 2450 5300 2450
Wire Wire Line
	5200 2550 5200 2450
Connection ~ 5200 2450
Wire Wire Line
	5200 2950 5200 2850
Wire Wire Line
	4800 2650 4800 3050
$Comp
L VPP #PWR065
U 1 1 5940D092
P 4800 1650
AR Path="/5940CCE8/5940D092" Ref="#PWR065"  Part="1" 
AR Path="/59418122/5940D092" Ref="#PWR068"  Part="1" 
F 0 "#PWR068" H 4800 1500 50  0001 C CNN
F 1 "VPP" H 4800 1800 50  0000 C CNN
F 2 "" H 4800 1650 50  0000 C CNN
F 3 "" H 4800 1650 50  0000 C CNN
	1    4800 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2150 4800 2150
Connection ~ 4800 2150
Wire Wire Line
	5600 2450 5700 2450
Connection ~ 4800 2950
$Comp
L GND #PWR066
U 1 1 5940D09C
P 4800 3050
AR Path="/5940CCE8/5940D09C" Ref="#PWR066"  Part="1" 
AR Path="/59418122/5940D09C" Ref="#PWR069"  Part="1" 
F 0 "#PWR069" H 4800 2800 50  0001 C CNN
F 1 "GND" H 4800 2900 50  0000 C CNN
F 2 "" H 4800 3050 50  0000 C CNN
F 3 "" H 4800 3050 50  0000 C CNN
	1    4800 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 1650 4800 1750
Text HLabel 5700 2450 2    60   Input ~ 0
O
Text HLabel 4700 2150 0    60   Input ~ 0
OUT
Wire Wire Line
	4800 2950 5200 2950
$EndSCHEMATC
