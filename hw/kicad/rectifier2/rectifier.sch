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
LIBS:relays
LIBS:rectifier-cache
EELAYER 26 0
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
L CONN_01X02 P3
U 1 1 58E4043D
P 8750 2700
F 0 "P3" H 8669 2425 50  0000 C CNN
F 1 "CONN_01X02" H 8669 2516 50  0000 C CNN
F 2 "stmbl:RM5.08_1x2" H 6200 2150 50  0001 C CNN
F 3 "" H 6200 2150 50  0001 C CNN
	1    8750 2700
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X02 P2
U 1 1 58E40510
P 3600 1400
F 0 "P2" H 3519 1125 50  0000 C CNN
F 1 "CONN_01X02" H 3519 1216 50  0000 C CNN
F 2 "stmbl:akl182-2" H 1050 850 50  0001 C CNN
F 3 "" H 1050 850 50  0001 C CNN
	1    3600 1400
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X03 P1
U 1 1 58E4055C
P 1850 2900
F 0 "P1" H 1769 2575 50  0000 C CNN
F 1 "CONN_01X03" H 1769 2666 50  0000 C CNN
F 2 "stmbl:RM5.08_1x3" H -1100 -2550 50  0001 C CNN
F 3 "" H -1100 -2550 50  0001 C CNN
	1    1850 2900
	-1   0    0    1   
$EndComp
$Comp
L CP C1
U 1 1 58E40702
P 7750 2700
F 0 "C1" H 7868 2746 50  0000 L CNN
F 1 "CP" H 7868 2655 50  0000 L CNN
F 2 "stmbl:C_Radial_D26_L45_P10" H 5688 750 50  0001 C CNN
F 3 "" H 5650 900 50  0001 C CNN
	1    7750 2700
	1    0    0    -1  
$EndComp
$Comp
L CP C2
U 1 1 58E4073F
P 8150 2700
F 0 "C2" H 8268 2746 50  0000 L CNN
F 1 "CP" H 8268 2655 50  0000 L CNN
F 2 "stmbl:C_Radial_D26_L45_P10" H 5488 850 50  0001 C CNN
F 3 "" H 5450 1000 50  0001 C CNN
	1    8150 2700
	1    0    0    -1  
$EndComp
$Comp
L FINDER-40.52 RL1
U 1 1 58E405BE
P 3600 2500
F 0 "RL1" H 4430 2546 50  0000 L CNN
F 1 "FINDER-40.52" H 4430 2455 50  0000 L CNN
F 2 "Relays_THT:Relay_DPDT_Schrack-RT2_RM5mm" H 1300 450 50  0001 C CNN
F 3 "" H 1300 450 50  0001 C CNN
	1    3600 2500
	0    1    1    0   
$EndComp
$Comp
L FINDER-40.52 RL2
U 1 1 58E406EB
P 4650 1400
F 0 "RL2" V 4100 1350 50  0000 L CNN
F 1 "FINDER-40.52" V 4200 1150 50  0000 L CNN
F 2 "Relays_THT:Relay_DPDT_Schrack-RT2_RM5mm" H 550 -1300 50  0001 C CNN
F 3 "" H 550 -1300 50  0001 C CNN
	1    4650 1400
	0    1    1    0   
$EndComp
$Comp
L Fuse F1
U 1 1 58E4072E
P 2950 2700
F 0 "F1" V 2753 2700 50  0000 C CNN
F 1 "Fuse" V 2844 2700 50  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuseholder5x20_horiz_SemiClosed_Casing10x25mm" V 380 250 50  0001 C CNN
F 3 "" H 450 250 50  0001 C CNN
	1    2950 2700
	0    1    1    0   
$EndComp
$Comp
L Earth_Protective #PWR01
U 1 1 58E41BF3
P 2150 3100
F 0 "#PWR01" H -250 1400 50  0001 C CNN
F 1 "Earth_Protective" H -50 1500 50  0001 C CNN
F 2 "" H -500 1550 50  0001 C CNN
F 3 "" H -500 1550 50  0001 C CNN
	1    2150 3100
	1    0    0    -1  
$EndComp
$Comp
L Thermistor_NTC TH1
U 1 1 58E4136A
P 4650 2800
F 0 "TH1" V 4360 2800 50  0000 C CNN
F 1 "Thermistor_NTC" V 4451 2800 50  0000 C CNN
F 2 "Varistors:RV_Disc_D21.5_W6.1_P7.5" H -100 1050 50  0001 C CNN
F 3 "" H -100 1050 50  0001 C CNN
	1    4650 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	2050 3000 2150 3000
Wire Wire Line
	2150 3000 2150 3100
Wire Wire Line
	2050 2900 2650 2900
Wire Wire Line
	2650 2900 2650 3100
Wire Wire Line
	2650 3100 3300 3100
Wire Wire Line
	2050 2800 2650 2800
Wire Wire Line
	2650 2800 2650 2700
Wire Wire Line
	2650 2700 2800 2700
Wire Wire Line
	3100 2700 3300 2700
Wire Wire Line
	3900 3200 7000 3200
Wire Wire Line
	3900 2800 4500 2800
Wire Wire Line
	4800 2800 5750 2800
Wire Wire Line
	5750 2800 5750 2700
Wire Wire Line
	5750 2700 6400 2700
Wire Wire Line
	7000 3200 7000 2700
Wire Wire Line
	6700 2400 6700 2350
Wire Wire Line
	6700 2350 8550 2350
Wire Wire Line
	8550 2350 8550 2650
Wire Wire Line
	6700 3000 6700 3050
Wire Wire Line
	6700 3050 8550 3050
Wire Wire Line
	8550 3050 8550 2750
Wire Wire Line
	8150 2550 8150 2350
Connection ~ 8150 2350
Wire Wire Line
	7750 2550 7750 2350
Connection ~ 7750 2350
Wire Wire Line
	7750 2850 7750 3050
Connection ~ 7750 3050
Wire Wire Line
	8150 2850 8150 3050
Connection ~ 8150 3050
Wire Wire Line
	3550 1600 3200 1600
Wire Wire Line
	3200 1600 3200 2300
Wire Wire Line
	3200 2300 3300 2300
Wire Wire Line
	3650 1600 4000 1600
Wire Wire Line
	4000 1600 4000 2300
Wire Wire Line
	4000 2300 3900 2300
Wire Wire Line
	4200 1600 4200 2800
Wire Wire Line
	4200 2000 4350 2000
Connection ~ 4200 2800
Wire Wire Line
	4200 1600 4350 1600
Connection ~ 4200 2000
Wire Wire Line
	4950 1700 5200 1700
Wire Wire Line
	5200 1200 5200 2800
Connection ~ 5200 2800
Wire Wire Line
	4950 2100 5200 2100
Connection ~ 5200 2100
Wire Wire Line
	4950 1200 5200 1200
Connection ~ 5200 1700
Wire Wire Line
	4100 1200 4100 3200
Connection ~ 4100 3200
Wire Wire Line
	4350 1200 4100 1200
$Comp
L D_Bridge_-A+A D1
U 1 1 58E41948
P 6700 2700
F 0 "D1" V 6746 2359 50  0000 R CNN
F 1 "D_Bridge_-A+A" V 6655 2359 50  0000 R CNN
F 2 "stmbl:dfb_bridge" H 0   0   50  0001 C CNN
F 3 "" H 0   0   50  0001 C CNN
	1    6700 2700
	0    -1   -1   0   
$EndComp
$EndSCHEMATC
