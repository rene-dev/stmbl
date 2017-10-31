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
LIBS:stmbl
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 3
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 1750 2300 1500 1050
U 59F49F2C
F0 "stepper" 60
F1 "stepper.sch" 60
F2 "~CS" I L 1750 2550 60 
F3 "CK" I L 1750 2650 60 
F4 "SDO" I R 3250 2950 60 
F5 "SDI" I L 1750 2750 60 
F6 "~STBY" I L 1750 2850 60 
$EndSheet
$Sheet
S 1800 3600 1450 850 
U 59F4C6C6
F0 "stepper2" 60
F1 "stepper.sch" 60
F2 "~CS" I L 1800 3850 60 
F3 "CK" I L 1800 3950 60 
F4 "SDO" I R 3250 4000 60 
F5 "SDI" I L 1800 4050 60 
F6 "~STBY" I L 1800 4150 60 
$EndSheet
Wire Wire Line
	3250 2950 3400 2950
Wire Wire Line
	3400 2950 3400 3450
Wire Wire Line
	3400 3450 1550 3450
Wire Wire Line
	1550 3450 1550 4050
Wire Wire Line
	1550 4050 1800 4050
Wire Wire Line
	1750 2550 1250 2550
Wire Wire Line
	1250 2550 1250 3850
Wire Wire Line
	1250 3850 1800 3850
Wire Wire Line
	1750 2850 1300 2850
Wire Wire Line
	1300 2850 1300 4150
Wire Wire Line
	1300 4150 1800 4150
Wire Wire Line
	1750 2650 1350 2650
Wire Wire Line
	1350 2650 1350 3950
Wire Wire Line
	1350 3950 1800 3950
$EndSCHEMATC
