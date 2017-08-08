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
LIBS:mesabob-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 9
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
L CONN_02X13 P1
U 1 1 5923363F
P 5850 2100
F 0 "P1" H 5850 2915 50  0000 C CNN
F 1 "CONN_02X13" H 5850 2824 50  0000 C CNN
F 2 "Connect:IDC_Header_Straight_26pins" H 5850 950 50  0001 C CNN
F 3 "" H 5850 950 50  0000 C CNN
	1    5850 2100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 59233773
P 6200 1900
F 0 "#PWR01" H 6200 1650 50  0001 C CNN
F 1 "GND" V 6205 1772 50  0000 R CNN
F 2 "" H 6200 1900 50  0000 C CNN
F 3 "" H 6200 1900 50  0000 C CNN
	1    6200 1900
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR03
U 1 1 592338B3
P 6200 2300
F 0 "#PWR03" H 6200 2150 50  0001 C CNN
F 1 "+5V" V 6215 2428 50  0000 L CNN
F 2 "" H 6200 2300 50  0000 C CNN
F 3 "" H 6200 2300 50  0000 C CNN
	1    6200 2300
	0    1    1    0   
$EndComp
Text HLabel 5500 1500 0    60   Input ~ 0
rx0
Text HLabel 5500 1700 0    60   Input ~ 0
tx0
Text HLabel 5500 1900 0    60   Input ~ 0
rx4
Text HLabel 5500 2100 0    60   Input ~ 0
rx6
Text HLabel 5500 2300 0    60   Input ~ 0
tx4
Text HLabel 5500 2500 0    60   Input ~ 0
tx6
Text HLabel 6200 1600 2    60   Input ~ 0
rx3
Text HLabel 6200 1800 2    60   Input ~ 0
tx3
Text HLabel 5500 1600 0    60   Input ~ 0
rx2
Text HLabel 5500 1800 0    60   Input ~ 0
tx2
Text HLabel 5500 2000 0    60   Input ~ 0
rx5
Text HLabel 5500 2200 0    60   Input ~ 0
rx7
Text HLabel 5500 2400 0    60   Input ~ 0
tx5
Text HLabel 5500 2600 0    60   Input ~ 0
tx7
Text HLabel 6200 1500 2    60   Input ~ 0
rx1
Text HLabel 6200 1700 2    60   Input ~ 0
tx1
$Sheet
S 7000 950  750  550 
U 59238838
F0 "port" 60
F1 "port.sch" 60
F2 "tx" I L 7000 1150 60 
F3 "rx" I L 7000 1250 60 
$EndSheet
$Sheet
S 7000 1700 750  550 
U 592390E3
F0 "sheet592390DF" 60
F1 "port.sch" 60
F2 "tx" I L 7000 1900 60 
F3 "rx" I L 7000 2000 60 
$EndSheet
$Sheet
S 7000 2450 750  550 
U 592391A3
F0 "sheet5923919F" 60
F1 "port.sch" 60
F2 "tx" I L 7000 2650 60 
F3 "rx" I L 7000 2750 60 
$EndSheet
$Sheet
S 7000 3200 750  550 
U 5923942C
F0 "sheet59239428" 60
F1 "port.sch" 60
F2 "tx" I L 7000 3400 60 
F3 "rx" I L 7000 3500 60 
$EndSheet
$Sheet
S 8250 950  750  550 
U 59239C28
F0 "sheet59239C21" 60
F1 "port.sch" 60
F2 "tx" I L 8250 1150 60 
F3 "rx" I L 8250 1250 60 
$EndSheet
$Sheet
S 8250 1700 750  550 
U 59239C32
F0 "sheet59239C22" 60
F1 "port.sch" 60
F2 "tx" I L 8250 1900 60 
F3 "rx" I L 8250 2000 60 
$EndSheet
$Sheet
S 8250 2450 750  550 
U 59239C3C
F0 "sheet59239C23" 60
F1 "port.sch" 60
F2 "tx" I L 8250 2650 60 
F3 "rx" I L 8250 2750 60 
$EndSheet
$Sheet
S 8250 3200 750  550 
U 59239C46
F0 "sheet59239C24" 60
F1 "port.sch" 60
F2 "tx" I L 8250 3400 60 
F3 "rx" I L 8250 3500 60 
$EndSheet
Text HLabel 6900 1250 0    60   Input ~ 0
rx0
Text HLabel 6900 1150 0    60   Input ~ 0
tx0
Text HLabel 6900 2000 0    60   Input ~ 0
rx1
Text HLabel 6900 1900 0    60   Input ~ 0
tx1
Text HLabel 6900 2750 0    60   Input ~ 0
rx2
Text HLabel 6900 2650 0    60   Input ~ 0
tx2
Text HLabel 6900 3400 0    60   Input ~ 0
tx3
Text HLabel 6900 3500 0    60   Input ~ 0
rx3
Text HLabel 8150 1250 0    60   Input ~ 0
rx4
Text HLabel 8150 1150 0    60   Input ~ 0
tx4
Text HLabel 8150 2000 0    60   Input ~ 0
rx5
Text HLabel 8150 1900 0    60   Input ~ 0
tx5
Text HLabel 8150 2750 0    60   Input ~ 0
rx6
Text HLabel 8150 2650 0    60   Input ~ 0
tx6
Text HLabel 8150 3400 0    60   Input ~ 0
tx7
Text HLabel 8150 3500 0    60   Input ~ 0
rx7
Wire Wire Line
	6900 1150 7000 1150
Wire Wire Line
	6900 1250 7000 1250
Wire Wire Line
	6900 1900 7000 1900
Wire Wire Line
	6900 2000 7000 2000
Wire Wire Line
	6900 2650 7000 2650
Wire Wire Line
	6900 2750 7000 2750
Wire Wire Line
	6900 3400 7000 3400
Wire Wire Line
	6900 3500 7000 3500
Wire Wire Line
	8150 3500 8250 3500
Wire Wire Line
	8250 3400 8150 3400
Wire Wire Line
	8150 2750 8250 2750
Wire Wire Line
	8250 2650 8150 2650
Wire Wire Line
	8150 2000 8250 2000
Wire Wire Line
	8250 1900 8150 1900
Wire Wire Line
	8150 1250 8250 1250
Wire Wire Line
	8250 1150 8150 1150
Wire Wire Line
	6100 1900 6200 1900
Wire Wire Line
	6100 2000 6150 2000
Wire Wire Line
	6150 1900 6150 2200
Connection ~ 6150 1900
Wire Wire Line
	6150 2100 6100 2100
Connection ~ 6150 2000
Wire Wire Line
	6150 2200 6100 2200
Connection ~ 6150 2100
Wire Wire Line
	6100 2300 6200 2300
Wire Wire Line
	6100 2400 6150 2400
Wire Wire Line
	6150 2300 6150 2600
Connection ~ 6150 2300
Wire Wire Line
	6150 2500 6100 2500
Connection ~ 6150 2400
Wire Wire Line
	6150 2600 6100 2600
Connection ~ 6150 2500
Wire Wire Line
	5500 1500 5600 1500
Wire Wire Line
	5500 1600 5600 1600
Wire Wire Line
	5500 1700 5600 1700
Wire Wire Line
	5500 1800 5600 1800
Wire Wire Line
	5600 1900 5500 1900
Wire Wire Line
	5500 2000 5600 2000
Wire Wire Line
	5600 2100 5500 2100
Wire Wire Line
	5500 2200 5600 2200
Wire Wire Line
	5600 2300 5500 2300
Wire Wire Line
	5500 2400 5600 2400
Wire Wire Line
	5600 2500 5500 2500
Wire Wire Line
	5500 2600 5600 2600
Wire Wire Line
	6200 1500 6100 1500
Wire Wire Line
	6100 1600 6200 1600
Wire Wire Line
	6200 1700 6100 1700
Wire Wire Line
	6100 1800 6200 1800
$EndSCHEMATC
