EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L RJ45 J1
U 1 1 56E95EDF
P 4600 3000
F 0 "J1" H 4673 3637 60  0000 C CNN
F 1 "RJ45" H 4673 3531 60  0000 C CNN
F 2 "rj45_vertical_mhrjj88nfvs:RJ45_8" H 4600 3000 60  0001 C CNN
F 3 "" H 4600 3000 60  0000 C CNN
	1    4600 3000
	1    0    0    -1  
$EndComp
$Comp
L CONN_4 P1
U 1 1 56E95F7A
P 3800 3700
F 0 "P1" H 3669 3318 50  0000 C CNN
F 1 "CONN_4" H 3669 3410 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MPT-2.54mm_4pol" H 3800 3700 60  0001 C CNN
F 3 "" H 3800 3700 60  0000 C CNN
	1    3800 3700
	-1   0    0    1   
$EndComp
$Comp
L CONN_5 P2
U 1 1 56E95FCC
P 5550 3650
F 0 "P2" H 5677 3710 50  0000 L CNN
F 1 "CONN_5" H 5677 3618 50  0000 L CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_MPT-2.54mm_5pol" H 5550 3650 60  0001 C CNN
F 3 "" H 5550 3650 60  0000 C CNN
	1    5550 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3450 4250 3550
Wire Wire Line
	4250 3550 4150 3550
Wire Wire Line
	4150 3650 4350 3650
Wire Wire Line
	4350 3650 4350 3450
Wire Wire Line
	4150 3750 4450 3750
Wire Wire Line
	4450 3750 4450 3450
Wire Wire Line
	4150 3850 4550 3850
Wire Wire Line
	4550 3850 4550 3450
Wire Wire Line
	4650 3450 4650 3850
Wire Wire Line
	4650 3850 5150 3850
Wire Wire Line
	5150 3750 4750 3750
Wire Wire Line
	4750 3750 4750 3450
Wire Wire Line
	4850 3450 4850 3650
Wire Wire Line
	4850 3650 5150 3650
Wire Wire Line
	5150 3550 4950 3550
Wire Wire Line
	4950 3550 4950 3450
Wire Wire Line
	5150 2650 5150 3450
$EndSCHEMATC
