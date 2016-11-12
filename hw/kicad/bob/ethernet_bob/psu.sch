EESchema Schematic File Version 2
LIBS:ethernet_bob-rescue
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
LIBS:ksz8081
LIBS:ESD_Protection
LIBS:hr911105_rj45
LIBS:stm32
LIBS:crystal_smd
LIBS:usblc
LIBS:ethernet_bob-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 6
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
L GND #PWR018
U 1 1 57FEA6D9
P 6300 3500
F 0 "#PWR018" H 6300 3250 50  0001 C CNN
F 1 "GND" H 6305 3327 50  0000 C CNN
F 2 "" H 6300 3500 50  0000 C CNN
F 3 "" H 6300 3500 50  0000 C CNN
	1    6300 3500
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR019
U 1 1 57FEA6EF
P 6700 3100
F 0 "#PWR019" H 6700 2950 50  0001 C CNN
F 1 "+3.3V" H 6715 3273 50  0000 C CNN
F 2 "" H 6700 3100 50  0000 C CNN
F 3 "" H 6700 3100 50  0000 C CNN
	1    6700 3100
	1    0    0    -1  
$EndComp
$Comp
L +5VP #PWR020
U 1 1 57FEA707
P 5900 3100
F 0 "#PWR020" H 5900 2950 50  0001 C CNN
F 1 "+5VP" H 5915 3273 50  0000 C CNN
F 2 "" H 5900 3100 50  0000 C CNN
F 3 "" H 5900 3100 50  0000 C CNN
	1    5900 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6300 3500 6300 3400
Wire Wire Line
	5900 3100 6000 3100
Wire Wire Line
	6600 3100 6700 3100
$Comp
L C C20
U 1 1 57FEA8F8
P 5950 3250
F 0 "C20" H 6065 3296 50  0000 L CNN
F 1 "C" H 6065 3205 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5988 3100 50  0001 C CNN
F 3 "" H 5950 3250 50  0000 C CNN
	1    5950 3250
	1    0    0    -1  
$EndComp
$Comp
L C C21
U 1 1 57FEA978
P 6650 3250
F 0 "C21" H 6765 3296 50  0000 L CNN
F 1 "C" H 6765 3205 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6688 3100 50  0001 C CNN
F 3 "" H 6650 3250 50  0000 C CNN
	1    6650 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 3400 6650 3400
Connection ~ 6300 3400
Connection ~ 6650 3100
Connection ~ 5950 3100
$Comp
L NCP1117ST33T3G U3
U 1 1 57FEECE5
P 6300 3100
F 0 "U3" H 6300 3367 50  0000 C CNN
F 1 "NCP1117ST33T3G" H 6300 2400 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-223" H 6350 2850 50  0001 L CNN
F 3 "" H 6300 3100 50  0000 C CNN
	1    6300 3100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
