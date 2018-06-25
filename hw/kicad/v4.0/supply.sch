EESchema Schematic File Version 4
LIBS:stmbl_4.0-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 14
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
L stmbl:zldo1117 U2
U 1 1 566074A9
P 2000 5150
F 0 "U2" H 2150 4954 60  0000 C CNN
F 1 "zldo1117" H 2000 5350 60  0000 C CNN
F 2 "stmbl:SOT-223" H 2000 5150 60  0001 C CNN
F 3 "" H 2000 5150 60  0000 C CNN
F 4 "" H 2000 5150 60  0001 C CNN "InternalName"
F 5 "ZLDO1117G33TA" H 2000 5150 60  0001 C CNN "Manufacturer No"
F 6 "" H 2000 5150 60  0001 C CNN "Voltage"
F 7 "" H 2000 5150 60  0001 C CNN "Source"
F 8 "" H 2000 5150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Diodes Incorporated" H 0   0   50  0001 C CNN "Manufacturer"
	1    2000 5150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C18
U 1 1 566075DC
P 2700 5350
F 0 "C18" H 2725 5450 50  0000 L CNN
F 1 "2.2µ" H 2725 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 2738 5200 30  0001 C CNN
F 3 "" H 2700 5350 60  0000 C CNN
F 4 "35V" H 2700 5350 60  0001 C CNN "Voltage"
F 5 "X5R" H 2700 5350 60  0001 C CNN "Tolerance"
F 6 "" H 0   0   50  0001 C CNN "Description"
F 7 "" H 0   0   50  0001 C CNN "InternalName"
F 8 "" H 0   0   50  0001 C CNN "Manufacturer No"
F 9 "" H 0   0   50  0001 C CNN "Source"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    2700 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C8
U 1 1 566076FA
P 1500 5350
F 0 "C8" H 1525 5450 50  0000 L CNN
F 1 "2.2µ" H 1525 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 1538 5200 30  0001 C CNN
F 3 "" H 1500 5350 60  0000 C CNN
F 4 "35V" H 1500 5350 60  0001 C CNN "Voltage"
F 5 "X5R" H 1500 5350 60  0001 C CNN "Tolerance"
F 6 "" H 0   0   50  0001 C CNN "Description"
F 7 "" H 0   0   50  0001 C CNN "InternalName"
F 8 "" H 0   0   50  0001 C CNN "Manufacturer No"
F 9 "" H 0   0   50  0001 C CNN "Source"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    1500 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+5V #PWR072
U 1 1 5660786F
P 1200 5000
F 0 "#PWR072" H 1200 4850 50  0001 C CNN
F 1 "+5V" H 1200 5140 50  0000 C CNN
F 2 "" H 1200 5000 60  0000 C CNN
F 3 "" H 1200 5000 60  0000 C CNN
	1    1200 5000
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+3.3V #PWR073
U 1 1 566078DE
P 2950 5000
F 0 "#PWR073" H 2950 4850 50  0001 C CNN
F 1 "+3.3V" H 2950 5140 50  0000 C CNN
F 2 "" H 2950 5000 60  0000 C CNN
F 3 "" H 2950 5000 60  0000 C CNN
	1    2950 5000
	1    0    0    -1  
$EndComp
$Comp
L stmbl:CONN_01X02 P3
U 1 1 566CDEAC
P 800 3000
F 0 "P3" H 800 3150 50  0000 C CNN
F 1 "CONN_01X02" V 900 3000 50  0000 C CNN
F 2 "stmbl:RM3.5_1x2" H 800 3000 60  0001 C CNN
F 3 "" H 800 3000 60  0000 C CNN
F 4 "" H 800 3000 60  0001 C CNN "InternalName"
F 5 "1844210" H 800 3000 60  0001 C CNN "Manufacturer No"
F 6 "" H 800 3000 60  0001 C CNN "Voltage"
F 7 "" H 800 3000 60  0001 C CNN "Source"
F 8 "" H 800 3000 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Phoenix" H 0   0   50  0001 C CNN "Manufacturer"
	1    800  3000
	-1   0    0    -1  
$EndComp
$Comp
L stmbl:GND #PWR074
U 1 1 57744A84
P 1200 5650
F 0 "#PWR074" H 1200 5400 50  0001 C CNN
F 1 "GND" H 1200 5500 50  0000 C CNN
F 2 "" H 1200 5650 60  0000 C CNN
F 3 "" H 1200 5650 60  0000 C CNN
	1    1200 5650
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C110
U 1 1 57AD279A
P 8200 5350
F 0 "C110" H 8225 5450 50  0000 L CNN
F 1 "10µ" H 8225 5250 50  0000 L CNN
F 2 "stmbl:C_0805" H 8238 5200 30  0001 C CNN
F 3 "" H 8200 5350 60  0000 C CNN
F 4 "25V" H 8200 5350 60  0001 C CNN "Voltage"
F 5 "" H 8200 5350 60  0001 C CNN "InternalName"
F 6 "" H 8200 5350 60  0001 C CNN "Manufacturer No"
F 7 "" H 8200 5350 60  0001 C CNN "Source"
F 8 "X5R" H 8200 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    8200 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R97
U 1 1 57AD27A0
P 6950 5100
F 0 "R97" V 7030 5100 50  0000 C CNN
F 1 "51k" V 6950 5100 50  0000 C CNN
F 2 "stmbl:R_0603" V 6880 5100 30  0001 C CNN
F 3 "" H 6950 5100 30  0000 C CNN
F 4 "" H 6950 5100 30  0001 C CNN "InternalName"
F 5 "" H 6950 5100 30  0001 C CNN "Manufacturer No"
F 6 "" H 6950 5100 30  0001 C CNN "Voltage"
F 7 "" H 6950 5100 30  0001 C CNN "Source"
F 8 "1%" H 6950 5100 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6950 5100
	0    1    1    0   
$EndComp
$Comp
L stmbl:R R59
U 1 1 57AD27A6
P 6700 5350
F 0 "R59" V 6780 5350 50  0000 C CNN
F 1 "3.9k" V 6700 5350 50  0000 C CNN
F 2 "stmbl:R_0603" V 6630 5350 30  0001 C CNN
F 3 "" H 6700 5350 30  0000 C CNN
F 4 "" H 6700 5350 30  0001 C CNN "InternalName"
F 5 "" H 6700 5350 30  0001 C CNN "Manufacturer No"
F 6 "" H 6700 5350 30  0001 C CNN "Voltage"
F 7 "" H 6700 5350 30  0001 C CNN "Source"
F 8 "1%" H 6700 5350 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6700 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:D_Schottky D12
U 1 1 57AD27AC
P 7200 5350
F 0 "D12" H 7200 5450 50  0000 C CNN
F 1 "SS34A" H 7200 5250 50  0000 C CNN
F 2 "stmbl:SMA_Standard" H 7200 5350 60  0001 C CNN
F 3 "" H 7200 5350 60  0000 C CNN
F 4 "" H 7200 5350 60  0001 C CNN "InternalName"
F 5 "SS34A" H 7200 5350 60  0001 C CNN "Manufacturer No"
F 6 "" H 7200 5350 60  0001 C CNN "Voltage"
F 7 "" H 7200 5350 60  0001 C CNN "Source"
F 8 "" H 7200 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Multicomp" H 0   0   50  0001 C CNN "Manufacturer"
	1    7200 5350
	0    1    1    0   
$EndComp
$Comp
L stmbl:C C106
U 1 1 57AD27B2
P 6850 4800
F 0 "C106" H 6875 4900 50  0000 L CNN
F 1 "100n" H 6875 4700 50  0000 L CNN
F 2 "stmbl:C_0603" H 6888 4650 30  0001 C CNN
F 3 "" H 6850 4800 60  0000 C CNN
F 4 "" H 6850 4800 60  0001 C CNN "InternalName"
F 5 "" H 6850 4800 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 6850 4800 60  0001 C CNN "Voltage"
F 7 "" H 6850 4800 60  0001 C CNN "Source"
F 8 "X5R" H 6850 4800 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6850 4800
	0    -1   -1   0   
$EndComp
$Comp
L stmbl:INDUCTOR L6
U 1 1 57AD27B8
P 7600 4950
F 0 "L6" V 7550 4950 50  0000 C CNN
F 1 "10µH 1A" V 7700 4950 50  0000 C CNN
F 2 "stmbl:SMD_INDUCTOR_32x25" H 7600 4950 60  0001 C CNN
F 3 "" H 7600 4950 60  0000 C CNN
F 4 "" H 7600 4950 60  0001 C CNN "InternalName"
F 5 "LQH32PN100MNCL" H 7600 4950 60  0001 C CNN "Manufacturer No"
F 6 "" H 7600 4950 60  0001 C CNN "Voltage"
F 7 "" H 7600 4950 60  0001 C CNN "Source"
F 8 "" H 7600 4950 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Murata" H 0   0   50  0001 C CNN "Manufacturer"
	1    7600 4950
	0    -1   -1   0   
$EndComp
$Comp
L stmbl:GND #PWR075
U 1 1 57AD27D2
P 5000 5700
F 0 "#PWR075" H 5000 5450 50  0001 C CNN
F 1 "GND" H 5000 5550 50  0000 C CNN
F 2 "" H 5000 5700 60  0000 C CNN
F 3 "" H 5000 5700 60  0000 C CNN
	1    5000 5700
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+24V #PWR076
U 1 1 57AD27DC
P 5000 4700
F 0 "#PWR076" H 5000 4550 50  0001 C CNN
F 1 "+24V" H 5000 4840 50  0000 C CNN
F 2 "" H 5000 4700 60  0000 C CNN
F 3 "" H 5000 4700 60  0000 C CNN
	1    5000 4700
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C109
U 1 1 57AD3DA2
P 8450 3550
F 0 "C109" H 8475 3650 50  0000 L CNN
F 1 "10µ" H 8475 3450 50  0000 L CNN
F 2 "stmbl:C_0805" H 8488 3400 30  0001 C CNN
F 3 "" H 8450 3550 60  0000 C CNN
F 4 "25V" H 8450 3550 60  0001 C CNN "Voltage"
F 5 "" H 8450 3550 60  0001 C CNN "InternalName"
F 6 "" H 8450 3550 60  0001 C CNN "Manufacturer No"
F 7 "" H 8450 3550 60  0001 C CNN "Source"
F 8 "X5R" H 8450 3550 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    8450 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R60
U 1 1 57AD3DA8
P 6950 3300
F 0 "R60" V 7030 3300 50  0000 C CNN
F 1 "51k" V 6950 3300 50  0000 C CNN
F 2 "stmbl:R_0603" V 6880 3300 30  0001 C CNN
F 3 "" H 6950 3300 30  0000 C CNN
F 4 "" H 6950 3300 30  0001 C CNN "InternalName"
F 5 "" H 6950 3300 30  0001 C CNN "Manufacturer No"
F 6 "" H 6950 3300 30  0001 C CNN "Voltage"
F 7 "" H 6950 3300 30  0001 C CNN "Source"
F 8 "1%" H 6950 3300 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6950 3300
	0    1    1    0   
$EndComp
$Comp
L stmbl:R R35
U 1 1 57AD3DAE
P 6700 3550
F 0 "R35" V 6780 3550 50  0000 C CNN
F 1 "10k" V 6700 3550 50  0000 C CNN
F 2 "stmbl:R_0603" V 6630 3550 30  0001 C CNN
F 3 "" H 6700 3550 30  0000 C CNN
F 4 "" H 6700 3550 30  0001 C CNN "InternalName"
F 5 "" H 6700 3550 30  0001 C CNN "Manufacturer No"
F 6 "" H 6700 3550 30  0001 C CNN "Voltage"
F 7 "" H 6700 3550 30  0001 C CNN "Source"
F 8 "1%" H 6700 3550 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6700 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:D_Schottky D9
U 1 1 57AD3DB4
P 7200 3550
F 0 "D9" H 7200 3650 50  0000 C CNN
F 1 "SS34A" H 7200 3450 50  0000 C CNN
F 2 "stmbl:SMA_Standard" H 7200 3550 60  0001 C CNN
F 3 "" H 7200 3550 60  0000 C CNN
F 4 "" H 7200 3550 60  0001 C CNN "InternalName"
F 5 "SS34A" H 7200 3550 60  0001 C CNN "Manufacturer No"
F 6 "" H 7200 3550 60  0001 C CNN "Voltage"
F 7 "" H 7200 3550 60  0001 C CNN "Source"
F 8 "" H 7200 3550 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Multicomp" H 0   0   50  0001 C CNN "Manufacturer"
	1    7200 3550
	0    1    1    0   
$EndComp
$Comp
L stmbl:C C105
U 1 1 57AD3DBA
P 6850 3000
F 0 "C105" H 6875 3100 50  0000 L CNN
F 1 "100n" H 6875 2900 50  0000 L CNN
F 2 "stmbl:C_0603" H 6888 2850 30  0001 C CNN
F 3 "" H 6850 3000 60  0000 C CNN
F 4 "" H 6850 3000 60  0001 C CNN "InternalName"
F 5 "" H 6850 3000 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 6850 3000 60  0001 C CNN "Voltage"
F 7 "" H 6850 3000 60  0001 C CNN "Source"
F 8 "X5R" H 6850 3000 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6850 3000
	0    -1   -1   0   
$EndComp
$Comp
L stmbl:INDUCTOR L5
U 1 1 57AD3DC0
P 7600 3150
F 0 "L5" V 7550 3150 50  0000 C CNN
F 1 "4.7µH 1.5A" V 7700 3150 50  0000 C CNN
F 2 "stmbl:SMD_INDUCTOR_32x25" H 7600 3150 60  0001 C CNN
F 3 "" H 7600 3150 60  0000 C CNN
F 4 "" H 7600 3150 60  0001 C CNN "InternalName"
F 5 "LQH32PN4R7NNC" H 7600 3150 60  0001 C CNN "Manufacturer No"
F 6 "" H 7600 3150 60  0001 C CNN "Voltage"
F 7 "" H 7600 3150 60  0001 C CNN "Source"
F 8 "" H 7600 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Murata" H 0   0   50  0001 C CNN "Manufacturer"
	1    7600 3150
	0    -1   -1   0   
$EndComp
$Comp
L stmbl:ACT4088 U13
U 1 1 57AD3DD4
P 6200 3150
F 0 "U13" H 6050 3400 60  0000 C CNN
F 1 "ACT4088" H 6200 2900 60  0000 C CNN
F 2 "stmbl:SOT-23-6" H 6200 3150 60  0001 C CNN
F 3 "" H 6200 3150 60  0000 C CNN
F 4 "" H 6200 3150 60  0001 C CNN "InternalName"
F 5 "ACT4088US-T" H 6200 3150 60  0001 C CNN "Manufacturer No"
F 6 "" H 6200 3150 60  0001 C CNN "Voltage"
F 7 "" H 6200 3150 60  0001 C CNN "Source"
F 8 "" H 6200 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Active-Semi" H 0   0   50  0001 C CNN "Manufacturer"
	1    6200 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:GND #PWR077
U 1 1 57AD3DDA
P 5050 3900
F 0 "#PWR077" H 5050 3650 50  0001 C CNN
F 1 "GND" H 5050 3750 50  0000 C CNN
F 2 "" H 5050 3900 60  0000 C CNN
F 3 "" H 5050 3900 60  0000 C CNN
	1    5050 3900
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+24V #PWR078
U 1 1 57AD3DE4
P 5050 2900
F 0 "#PWR078" H 5050 2750 50  0001 C CNN
F 1 "+24V" H 5050 3040 50  0000 C CNN
F 2 "" H 5050 2900 60  0000 C CNN
F 3 "" H 5050 2900 60  0000 C CNN
	1    5050 2900
	1    0    0    -1  
$EndComp
NoConn ~ 5800 3150
$Comp
L stmbl:+5V #PWR079
U 1 1 57AD3DF9
P 8000 3050
F 0 "#PWR079" H 8000 2900 50  0001 C CNN
F 1 "+5V" H 8000 3190 50  0000 C CNN
F 2 "" H 8000 3050 50  0000 C CNN
F 3 "" H 8000 3050 50  0000 C CNN
	1    8000 3050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+12V #PWR080
U 1 1 57AD61BC
P 8000 4850
F 0 "#PWR080" H 8000 4700 50  0001 C CNN
F 1 "+12V" H 8000 4990 50  0000 C CNN
F 2 "" H 8000 4850 50  0000 C CNN
F 3 "" H 8000 4850 50  0000 C CNN
	1    8000 4850
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C96
U 1 1 57AF3925
P 1200 5350
F 0 "C96" H 1225 5450 50  0000 L CNN
F 1 "100n" H 1225 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 1238 5200 30  0001 C CNN
F 3 "" H 1200 5350 60  0000 C CNN
F 4 "" H 1200 5350 60  0001 C CNN "InternalName"
F 5 "" H 1200 5350 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 1200 5350 60  0001 C CNN "Voltage"
F 7 "" H 1200 5350 60  0001 C CNN "Source"
F 8 "X5R" H 1200 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    1200 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C108
U 1 1 57AF3EB7
P 7700 5350
F 0 "C108" H 7725 5450 50  0000 L CNN
F 1 "100n" H 7725 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 7738 5200 30  0001 C CNN
F 3 "" H 7700 5350 60  0000 C CNN
F 4 "" H 7700 5350 60  0001 C CNN "InternalName"
F 5 "" H 7700 5350 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 7700 5350 60  0001 C CNN "Voltage"
F 7 "" H 7700 5350 60  0001 C CNN "Source"
F 8 "X5R" H 7700 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7700 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C107
U 1 1 57AF42AD
P 7700 3550
F 0 "C107" H 7725 3650 50  0000 L CNN
F 1 "100n" H 7725 3450 50  0000 L CNN
F 2 "stmbl:C_0603" H 7738 3400 30  0001 C CNN
F 3 "" H 7700 3550 60  0000 C CNN
F 4 "" H 7700 3550 60  0001 C CNN "InternalName"
F 5 "" H 7700 3550 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 7700 3550 60  0001 C CNN "Voltage"
F 7 "" H 7700 3550 60  0001 C CNN "Source"
F 8 "X5R" H 7700 3550 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7700 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C103
U 1 1 57AF47AC
P 5050 3550
F 0 "C103" H 5075 3650 50  0000 L CNN
F 1 "100n" H 5075 3450 50  0000 L CNN
F 2 "stmbl:C_0603" H 5088 3400 30  0001 C CNN
F 3 "" H 5050 3550 60  0000 C CNN
F 4 "" H 5050 3550 60  0001 C CNN "InternalName"
F 5 "" H 5050 3550 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 5050 3550 60  0001 C CNN "Voltage"
F 7 "" H 5050 3550 60  0001 C CNN "Source"
F 8 "X5R" H 5050 3550 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    5050 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C104
U 1 1 57AF48FA
P 5000 5350
F 0 "C104" H 5025 5450 50  0000 L CNN
F 1 "100n" H 5025 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 5038 5200 30  0001 C CNN
F 3 "" H 5000 5350 60  0000 C CNN
F 4 "" H 5000 5350 60  0001 C CNN "InternalName"
F 5 "" H 5000 5350 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 5000 5350 60  0001 C CNN "Voltage"
F 7 "" H 5000 5350 60  0001 C CNN "Source"
F 8 "X5R" H 5000 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    5000 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C99
U 1 1 57AF53FF
P 2450 5350
F 0 "C99" H 2475 5450 50  0000 L CNN
F 1 "100n" H 2475 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 2488 5200 30  0001 C CNN
F 3 "" H 2450 5350 60  0000 C CNN
F 4 "" H 2450 5350 60  0001 C CNN "InternalName"
F 5 "" H 2450 5350 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 2450 5350 60  0001 C CNN "Voltage"
F 7 "" H 2450 5350 60  0001 C CNN "Source"
F 8 "X5R" H 2450 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    2450 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C111
U 1 1 57AF630D
P 8200 3550
F 0 "C111" H 8225 3650 50  0000 L CNN
F 1 "10µ" H 8225 3450 50  0000 L CNN
F 2 "stmbl:C_0805" H 8238 3400 30  0001 C CNN
F 3 "" H 8200 3550 60  0000 C CNN
F 4 "25V" H 8200 3550 60  0001 C CNN "Voltage"
F 5 "" H 8200 3550 60  0001 C CNN "InternalName"
F 6 "" H 8200 3550 60  0001 C CNN "Manufacturer No"
F 7 "" H 8200 3550 60  0001 C CNN "Source"
F 8 "X5R" H 8200 3550 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    8200 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:GND #PWR081
U 1 1 57B218AB
P 2500 3550
F 0 "#PWR081" H 2500 3300 50  0001 C CNN
F 1 "GND" H 2500 3400 50  0000 C CNN
F 2 "" H 2500 3550 60  0000 C CNN
F 3 "" H 2500 3550 60  0000 C CNN
	1    2500 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C101
U 1 1 57B21BDA
P 2500 3150
F 0 "C101" H 2525 3250 50  0000 L CNN
F 1 "2.2µ" H 2525 3050 50  0000 L CNN
F 2 "stmbl:C_0603" H 2538 3000 30  0001 C CNN
F 3 "" H 2500 3150 60  0000 C CNN
F 4 "" H 2500 3150 60  0001 C CNN "InternalName"
F 5 "" H 2500 3150 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 2500 3150 60  0001 C CNN "Voltage"
F 7 "" H 2500 3150 60  0001 C CNN "Source"
F 8 "X5R" H 2500 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    2500 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C102
U 1 1 57B21C48
P 5300 5350
F 0 "C102" H 5325 5450 50  0000 L CNN
F 1 "2.2µ" H 5325 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 5338 5200 30  0001 C CNN
F 3 "" H 5300 5350 60  0000 C CNN
F 4 "" H 5300 5350 60  0001 C CNN "InternalName"
F 5 "" H 5300 5350 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 5300 5350 60  0001 C CNN "Voltage"
F 7 "" H 5300 5350 60  0001 C CNN "Source"
F 8 "X5R" H 5300 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    5300 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C123
U 1 1 57D63C06
P 8450 2050
F 0 "C123" H 8475 2150 50  0000 L CNN
F 1 "10µ" H 8475 1950 50  0000 L CNN
F 2 "stmbl:C_0805" H 8488 1900 30  0001 C CNN
F 3 "" H 8450 2050 60  0000 C CNN
F 4 "25V" H 8450 2050 60  0001 C CNN "Voltage"
F 5 "" H 8450 2050 60  0001 C CNN "InternalName"
F 6 "" H 8450 2050 60  0001 C CNN "Manufacturer No"
F 7 "" H 8450 2050 60  0001 C CNN "Source"
F 8 "X5R" H 8450 2050 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    8450 2050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R133
U 1 1 57D63C0C
P 6950 1800
F 0 "R133" V 7030 1800 50  0000 C CNN
F 1 "51k" V 6950 1800 50  0000 C CNN
F 2 "stmbl:R_0603" V 6880 1800 30  0001 C CNN
F 3 "" H 6950 1800 30  0000 C CNN
F 4 "" H 6950 1800 30  0001 C CNN "InternalName"
F 5 "" H 6950 1800 30  0001 C CNN "Manufacturer No"
F 6 "" H 6950 1800 30  0001 C CNN "Voltage"
F 7 "" H 6950 1800 30  0001 C CNN "Source"
F 8 "1%" H 6950 1800 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6950 1800
	0    1    1    0   
$EndComp
$Comp
L stmbl:D_Schottky D13
U 1 1 57D63C18
P 7200 2050
F 0 "D13" H 7200 2150 50  0000 C CNN
F 1 "SS34A" H 7200 1950 50  0000 C CNN
F 2 "stmbl:SMA_Standard" H 7200 2050 60  0001 C CNN
F 3 "" H 7200 2050 60  0000 C CNN
F 4 "" H 7200 2050 60  0001 C CNN "InternalName"
F 5 "SS34A" H 7200 2050 60  0001 C CNN "Manufacturer No"
F 6 "" H 7200 2050 60  0001 C CNN "Voltage"
F 7 "" H 7200 2050 60  0001 C CNN "Source"
F 8 "" H 7200 2050 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Multicomp" H 0   0   50  0001 C CNN "Manufacturer"
	1    7200 2050
	0    1    1    0   
$EndComp
$Comp
L stmbl:C C121
U 1 1 57D63C1E
P 6850 1500
F 0 "C121" H 6875 1600 50  0000 L CNN
F 1 "100n" H 6875 1400 50  0000 L CNN
F 2 "stmbl:C_0603" H 6888 1350 30  0001 C CNN
F 3 "" H 6850 1500 60  0000 C CNN
F 4 "" H 6850 1500 60  0001 C CNN "InternalName"
F 5 "" H 6850 1500 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 6850 1500 60  0001 C CNN "Voltage"
F 7 "" H 6850 1500 60  0001 C CNN "Source"
F 8 "X5R" H 6850 1500 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6850 1500
	0    -1   -1   0   
$EndComp
$Comp
L stmbl:INDUCTOR L12
U 1 1 57D63C24
P 7600 1650
F 0 "L12" V 7550 1650 50  0000 C CNN
F 1 "4.7µH 1.5A" V 7700 1650 50  0000 C CNN
F 2 "stmbl:SMD_INDUCTOR_32x25" H 7600 1650 60  0001 C CNN
F 3 "" H 7600 1650 60  0000 C CNN
F 4 "" H 7600 1650 60  0001 C CNN "InternalName"
F 5 "LQH32PN4R7NNC" H 7600 1650 60  0001 C CNN "Manufacturer No"
F 6 "" H 7600 1650 60  0001 C CNN "Voltage"
F 7 "" H 7600 1650 60  0001 C CNN "Source"
F 8 "" H 7600 1650 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Murata" H 0   0   50  0001 C CNN "Manufacturer"
	1    7600 1650
	0    -1   -1   0   
$EndComp
$Comp
L stmbl:ACT4088 U28
U 1 1 57D63C38
P 6200 1650
F 0 "U28" H 6050 1900 60  0000 C CNN
F 1 "ACT4088" H 6200 1400 60  0000 C CNN
F 2 "stmbl:SOT-23-6" H 6200 1650 60  0001 C CNN
F 3 "" H 6200 1650 60  0000 C CNN
F 4 "" H 6200 1650 60  0001 C CNN "InternalName"
F 5 "ACT4088US-T" H 6200 1650 60  0001 C CNN "Manufacturer No"
F 6 "" H 6200 1650 60  0001 C CNN "Voltage"
F 7 "" H 6200 1650 60  0001 C CNN "Source"
F 8 "" H 6200 1650 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Active-Semi" H 0   0   50  0001 C CNN "Manufacturer"
	1    6200 1650
	1    0    0    -1  
$EndComp
$Comp
L stmbl:GND #PWR082
U 1 1 57D63C3E
P 5050 2400
F 0 "#PWR082" H 5050 2150 50  0001 C CNN
F 1 "GND" H 5050 2250 50  0000 C CNN
F 2 "" H 5050 2400 60  0000 C CNN
F 3 "" H 5050 2400 60  0000 C CNN
	1    5050 2400
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+24V #PWR083
U 1 1 57D63C47
P 5050 1400
F 0 "#PWR083" H 5050 1250 50  0001 C CNN
F 1 "+24V" H 5050 1540 50  0000 C CNN
F 2 "" H 5050 1400 60  0000 C CNN
F 3 "" H 5050 1400 60  0000 C CNN
	1    5050 1400
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C122
U 1 1 57D63C68
P 7700 2050
F 0 "C122" H 7725 2150 50  0000 L CNN
F 1 "100n" H 7725 1950 50  0000 L CNN
F 2 "stmbl:C_0603" H 7738 1900 30  0001 C CNN
F 3 "" H 7700 2050 60  0000 C CNN
F 4 "" H 7700 2050 60  0001 C CNN "InternalName"
F 5 "" H 7700 2050 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 7700 2050 60  0001 C CNN "Voltage"
F 7 "" H 7700 2050 60  0001 C CNN "Source"
F 8 "X5R" H 7700 2050 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7700 2050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C120
U 1 1 57D63C71
P 5050 2050
F 0 "C120" H 5075 2150 50  0000 L CNN
F 1 "100n" H 5075 1950 50  0000 L CNN
F 2 "stmbl:C_0603" H 5088 1900 30  0001 C CNN
F 3 "" H 5050 2050 60  0000 C CNN
F 4 "" H 5050 2050 60  0001 C CNN "InternalName"
F 5 "" H 5050 2050 60  0001 C CNN "Manufacturer No"
F 6 "50V" H 5050 2050 60  0001 C CNN "Voltage"
F 7 "" H 5050 2050 60  0001 C CNN "Source"
F 8 "X5R" H 5050 2050 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    5050 2050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C124
U 1 1 57D63C7A
P 8200 2050
F 0 "C124" H 8225 2150 50  0000 L CNN
F 1 "10µ" H 8225 1950 50  0000 L CNN
F 2 "stmbl:C_0805" H 8238 1900 30  0001 C CNN
F 3 "" H 8200 2050 60  0000 C CNN
F 4 "25V" H 8200 2050 60  0001 C CNN "Voltage"
F 5 "" H 8200 2050 60  0001 C CNN "InternalName"
F 6 "" H 8200 2050 60  0001 C CNN "Manufacturer No"
F 7 "" H 8200 2050 60  0001 C CNN "Source"
F 8 "X5R" H 8200 2050 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    8200 2050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+5F #PWR084
U 1 1 57D6418F
P 8000 1550
F 0 "#PWR084" H 8000 1400 50  0001 C CNN
F 1 "+5F" H 8000 1690 50  0000 C CNN
F 2 "" H 8000 1550 50  0000 C CNN
F 3 "" H 8000 1550 50  0000 C CNN
	1    8000 1550
	1    0    0    -1  
$EndComp
Text HLabel 5700 1650 0    60   Input ~ 0
EN_5V
$Comp
L stmbl:ACT4088 U27
U 1 1 57AD27CC
P 6200 4950
F 0 "U27" H 6050 5200 60  0000 C CNN
F 1 "ACT4088" H 6200 4700 60  0000 C CNN
F 2 "stmbl:SOT-23-6" H 6200 4950 60  0001 C CNN
F 3 "" H 6200 4950 60  0000 C CNN
F 4 "ACT4088US-T" H 6200 4950 50  0001 C CNN "Manufacturer No"
F 5 "" H 6200 4950 50  0001 C CNN "Voltage"
F 6 "" H 6200 4950 50  0001 C CNN "Source"
F 7 "" H 6200 4950 60  0001 C CNN "InternalName"
F 8 "" H 6200 4950 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Active-Semi" H 0   0   50  0001 C CNN "Manufacturer"
	1    6200 4950
	1    0    0    -1  
$EndComp
Text Notes 8150 4850 0    60   ~ 0
FB and FAN
Text Notes 8150 3050 0    60   ~ 0
CPU, IO and analog
Text Notes 8250 1550 0    60   ~ 0
FB and RS485
$Comp
L stmbl:Jumper_NO_Small JP4
U 1 1 57D95B7C
P 9350 1800
F 0 "JP4" H 9350 1880 50  0000 C CNN
F 1 "Jumper_NO_Small" H 9360 1740 50  0001 C CNN
F 2 "stmbl:SOLDER_JUMPER" H 9350 1800 50  0001 C CNN
F 3 "" H 9350 1800 50  0000 C CNN
F 4 "" H 9350 1800 50  0001 C CNN "InternalName"
F 5 "" H 9350 1800 50  0001 C CNN "Manufacturer No"
F 6 "" H 9350 1800 50  0001 C CNN "Voltage"
F 7 "" H 9350 1800 50  0001 C CNN "Source"
F 8 "" H 9350 1800 50  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    9350 1800
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+5F #PWR085
U 1 1 57D95C13
P 9150 1700
F 0 "#PWR085" H 9150 1550 50  0001 C CNN
F 1 "+5F" H 9150 1840 50  0000 C CNN
F 2 "" H 9150 1700 50  0000 C CNN
F 3 "" H 9150 1700 50  0000 C CNN
	1    9150 1700
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+5V #PWR086
U 1 1 57D95CA2
P 9550 1700
F 0 "#PWR086" H 9550 1550 50  0001 C CNN
F 1 "+5V" H 9550 1840 50  0000 C CNN
F 2 "" H 9550 1700 50  0000 C CNN
F 3 "" H 9550 1700 50  0000 C CNN
	1    9550 1700
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R146
U 1 1 5812447C
P 7450 5100
F 0 "R146" V 7530 5100 50  0000 C CNN
F 1 "3.9k" V 7450 5100 50  0000 C CNN
F 2 "stmbl:R_0603" V 7380 5100 30  0001 C CNN
F 3 "" H 7450 5100 30  0000 C CNN
F 4 "" H 7450 5100 30  0001 C CNN "InternalName"
F 5 "" H 7450 5100 30  0001 C CNN "Manufacturer No"
F 6 "" H 7450 5100 30  0001 C CNN "Voltage"
F 7 "" H 7450 5100 30  0001 C CNN "Source"
F 8 "1%" H 7450 5100 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7450 5100
	0    1    1    0   
$EndComp
$Comp
L stmbl:R R145
U 1 1 581247B5
P 7450 3300
F 0 "R145" V 7530 3300 50  0000 C CNN
F 1 "1k" V 7450 3300 50  0000 C CNN
F 2 "stmbl:R_0603" V 7380 3300 30  0001 C CNN
F 3 "" H 7450 3300 30  0000 C CNN
F 4 "" H 7450 3300 30  0001 C CNN "InternalName"
F 5 "" H 7450 3300 30  0001 C CNN "Manufacturer No"
F 6 "" H 7450 3300 30  0001 C CNN "Voltage"
F 7 "" H 7450 3300 30  0001 C CNN "Source"
F 8 "1%" H 7450 3300 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7450 3300
	0    1    1    0   
$EndComp
$Comp
L stmbl:R R144
U 1 1 581254D2
P 7450 1800
F 0 "R144" V 7530 1800 50  0000 C CNN
F 1 "3.9k" V 7450 1800 50  0000 C CNN
F 2 "stmbl:R_0603" V 7380 1800 30  0001 C CNN
F 3 "" H 7450 1800 30  0000 C CNN
F 4 "" H 7450 1800 30  0001 C CNN "InternalName"
F 5 "" H 7450 1800 30  0001 C CNN "Manufacturer No"
F 6 "" H 7450 1800 30  0001 C CNN "Voltage"
F 7 "" H 7450 1800 30  0001 C CNN "Source"
F 8 "1%" H 7450 1800 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7450 1800
	0    1    1    0   
$EndComp
Text Notes 9100 2750 0    60   ~ 0
0.81V * (51k + 3.9k + 10k) / 10k = 5.25V
Text Notes 9100 2900 0    60   ~ 0
0.81V * (51k + 3.9k + 3.9k) / 3.9k = 12.21V
$Comp
L stmbl:D_Schottky D22
U 1 1 581AE161
P 1750 3150
F 0 "D22" H 1750 3250 50  0000 C CNN
F 1 "SMAJ24A TVS 24V" H 1750 3050 50  0000 C CNN
F 2 "stmbl:SMA_Standard" H 1750 3150 60  0001 C CNN
F 3 "" H 1750 3150 60  0000 C CNN
F 4 "" H 1750 3150 60  0001 C CNN "InternalName"
F 5 "SMAJ24A-TR" H 1750 3150 60  0001 C CNN "Manufacturer No"
F 6 "" H 1750 3150 60  0001 C CNN "Voltage"
F 7 "" H 1750 3150 60  0001 C CNN "Source"
F 8 "" H 1750 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "STMicroelectronics" H 0   0   50  0001 C CNN "Manufacturer"
	1    1750 3150
	0    1    1    0   
$EndComp
$Comp
L stmbl:+24V #PWR087
U 1 1 581B5518
P 4600 2300
F 0 "#PWR087" H 4600 2150 50  0001 C CNN
F 1 "+24V" H 4600 2440 50  0000 C CNN
F 2 "" H 4600 2300 60  0000 C CNN
F 3 "" H 4600 2300 60  0000 C CNN
	1    4600 2300
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R149
U 1 1 5817A2EE
P 1400 3200
F 0 "R149" V 1500 3100 50  0000 L CNN
F 1 "3.9k" V 1400 3150 50  0000 L CNN
F 2 "stmbl:R_0603" V 1330 3200 30  0001 C CNN
F 3 "" H 1400 3200 30  0000 C CNN
F 4 "" H 1400 3200 30  0001 C CNN "InternalName"
F 5 "" H 1400 3200 30  0001 C CNN "Manufacturer No"
F 6 "" H 1400 3200 30  0001 C CNN "Voltage"
F 7 "" H 1400 3200 30  0001 C CNN "Source"
F 8 "1%" H 1400 3200 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    1400 3200
	1    0    0    -1  
$EndComp
$Comp
L stmbl:Led_Small D24
U 1 1 5817A2F5
P 1400 2850
F 0 "D24" H 1350 2975 50  0000 L CNN
F 1 "green" H 1225 2750 50  0000 L CNN
F 2 "stmbl:LED-0805-SIDE" V 1400 2850 50  0001 C CNN
F 3 "" V 1400 2850 50  0000 C CNN
F 4 "" V 1400 2850 50  0001 C CNN "InternalName"
F 5 "LTST-S220KGKT" V 1400 2850 50  0001 C CNN "Manufacturer No"
F 6 "" V 1400 2850 50  0001 C CNN "Voltage"
F 7 "" V 1400 2850 50  0001 C CNN "Source"
F 8 "" V 1400 2850 50  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "Lite-On" H 0   0   50  0001 C CNN "Manufacturer"
	1    1400 2850
	0    -1   -1   0   
$EndComp
$Comp
L stmbl:C C136
U 1 1 5824ACDB
P 7950 5350
F 0 "C136" H 7975 5450 50  0000 L CNN
F 1 "2.2µ" H 7975 5250 50  0000 L CNN
F 2 "stmbl:C_0603" H 7988 5200 30  0001 C CNN
F 3 "" H 7950 5350 60  0000 C CNN
F 4 "35V" H 7950 5350 60  0001 C CNN "Voltage"
F 5 "" H 7950 5350 60  0001 C CNN "InternalName"
F 6 "" H 7950 5350 60  0001 C CNN "Manufacturer No"
F 7 "" H 7950 5350 60  0001 C CNN "Source"
F 8 "X5R" H 7950 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7950 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C135
U 1 1 5824C702
P 7950 3550
F 0 "C135" H 7975 3650 50  0000 L CNN
F 1 "2.2µ" H 7975 3450 50  0000 L CNN
F 2 "stmbl:C_0603" H 7988 3400 30  0001 C CNN
F 3 "" H 7950 3550 60  0000 C CNN
F 4 "35V" H 7950 3550 60  0001 C CNN "Voltage"
F 5 "" H 7950 3550 60  0001 C CNN "InternalName"
F 6 "" H 7950 3550 60  0001 C CNN "Manufacturer No"
F 7 "" H 7950 3550 60  0001 C CNN "Source"
F 8 "X5R" H 7950 3550 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7950 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C134
U 1 1 58250581
P 7950 2050
F 0 "C134" H 7975 2150 50  0000 L CNN
F 1 "2.2µ" H 7975 1950 50  0000 L CNN
F 2 "stmbl:C_0603" H 7988 1900 30  0001 C CNN
F 3 "" H 7950 2050 60  0000 C CNN
F 4 "35V" H 7950 2050 60  0001 C CNN "Voltage"
F 5 "" H 7950 2050 60  0001 C CNN "InternalName"
F 6 "" H 7950 2050 60  0001 C CNN "Manufacturer No"
F 7 "" H 7950 2050 60  0001 C CNN "Source"
F 8 "X5R" H 7950 2050 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    7950 2050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R132
U 1 1 57D63C12
P 6700 2050
F 0 "R132" V 6780 2050 50  0000 C CNN
F 1 "10k" V 6700 2050 50  0000 C CNN
F 2 "stmbl:R_0603" V 6630 2050 30  0001 C CNN
F 3 "" H 6700 2050 30  0000 C CNN
F 4 "" H 6700 2050 30  0001 C CNN "InternalName"
F 5 "" H 6700 2050 30  0001 C CNN "Manufacturer No"
F 6 "" H 6700 2050 30  0001 C CNN "Voltage"
F 7 "" H 6700 2050 30  0001 C CNN "Source"
F 8 "1%" H 6700 2050 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    6700 2050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C132
U 1 1 582791CF
P 8450 5350
F 0 "C132" H 8475 5450 50  0000 L CNN
F 1 "10µ" H 8475 5250 50  0000 L CNN
F 2 "stmbl:C_0805" H 8488 5200 30  0001 C CNN
F 3 "" H 8450 5350 60  0000 C CNN
F 4 "25V" H 8450 5350 60  0001 C CNN "Voltage"
F 5 "" H 8450 5350 60  0001 C CNN "InternalName"
F 6 "" H 8450 5350 60  0001 C CNN "Manufacturer No"
F 7 "" H 8450 5350 60  0001 C CNN "Source"
F 8 "X5R" H 8450 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    8450 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C131
U 1 1 5828EFB0
P 2950 5350
F 0 "C131" H 2975 5450 50  0000 L CNN
F 1 "10µ" H 2975 5250 50  0000 L CNN
F 2 "stmbl:C_0805" H 2988 5200 30  0001 C CNN
F 3 "" H 2950 5350 60  0000 C CNN
F 4 "25V" H 2950 5350 60  0001 C CNN "Voltage"
F 5 "" H 2950 5350 60  0001 C CNN "InternalName"
F 6 "" H 2950 5350 60  0001 C CNN "Manufacturer No"
F 7 "" H 2950 5350 60  0001 C CNN "Source"
F 8 "X5R" H 2950 5350 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    2950 5350
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C137
U 1 1 5829DD53
P 5300 3550
F 0 "C137" H 5325 3650 50  0000 L CNN
F 1 "2.2µ" H 5325 3450 50  0000 L CNN
F 2 "stmbl:C_0603" H 5338 3400 30  0001 C CNN
F 3 "" H 5300 3550 60  0000 C CNN
F 4 "" H 5300 3550 60  0001 C CNN "InternalName"
F 5 "" H 5300 3550 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 5300 3550 60  0001 C CNN "Voltage"
F 7 "" H 5300 3550 60  0001 C CNN "Source"
F 8 "X5R" H 5300 3550 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    5300 3550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C133
U 1 1 5829DE03
P 5300 2050
F 0 "C133" H 5325 2150 50  0000 L CNN
F 1 "2.2µ" H 5325 1950 50  0000 L CNN
F 2 "stmbl:C_0603" H 5338 1900 30  0001 C CNN
F 3 "" H 5300 2050 60  0000 C CNN
F 4 "" H 5300 2050 60  0001 C CNN "InternalName"
F 5 "" H 5300 2050 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 5300 2050 60  0001 C CNN "Voltage"
F 7 "" H 5300 2050 60  0001 C CNN "Source"
F 8 "X5R" H 5300 2050 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    5300 2050
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C62
U 1 1 583297B2
P 2750 3150
F 0 "C62" H 2775 3250 50  0000 L CNN
F 1 "2.2µ" H 2775 3050 50  0000 L CNN
F 2 "stmbl:C_0603" H 2788 3000 30  0001 C CNN
F 3 "" H 2750 3150 60  0000 C CNN
F 4 "" H 2750 3150 60  0001 C CNN "InternalName"
F 5 "" H 2750 3150 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 2750 3150 60  0001 C CNN "Voltage"
F 7 "" H 2750 3150 60  0001 C CNN "Source"
F 8 "X5R" H 2750 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    2750 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C139
U 1 1 5832CFE2
P 3000 3150
F 0 "C139" H 3025 3250 50  0000 L CNN
F 1 "2.2µ" H 3025 3050 50  0000 L CNN
F 2 "stmbl:C_0603" H 3038 3000 30  0001 C CNN
F 3 "" H 3000 3150 60  0000 C CNN
F 4 "" H 3000 3150 60  0001 C CNN "InternalName"
F 5 "" H 3000 3150 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 3000 3150 60  0001 C CNN "Voltage"
F 7 "" H 3000 3150 60  0001 C CNN "Source"
F 8 "X5R" H 3000 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    3000 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C140
U 1 1 5832D0A0
P 3250 3150
F 0 "C140" H 3275 3250 50  0000 L CNN
F 1 "2.2µ" H 3275 3050 50  0000 L CNN
F 2 "stmbl:C_0603" H 3288 3000 30  0001 C CNN
F 3 "" H 3250 3150 60  0000 C CNN
F 4 "" H 3250 3150 60  0001 C CNN "InternalName"
F 5 "" H 3250 3150 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 3250 3150 60  0001 C CNN "Voltage"
F 7 "" H 3250 3150 60  0001 C CNN "Source"
F 8 "X5R" H 3250 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    3250 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C142
U 1 1 5833FCEB
P 3500 3150
F 0 "C142" H 3525 3250 50  0000 L CNN
F 1 "2.2µ" H 3525 3050 50  0000 L CNN
F 2 "stmbl:C_0603" H 3538 3000 30  0001 C CNN
F 3 "" H 3500 3150 60  0000 C CNN
F 4 "" H 3500 3150 60  0001 C CNN "InternalName"
F 5 "" H 3500 3150 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 3500 3150 60  0001 C CNN "Voltage"
F 7 "" H 3500 3150 60  0001 C CNN "Source"
F 8 "X5R" H 3500 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    3500 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:JUMPER3_NC JP6
U 1 1 5832AA58
P 10450 1650
F 0 "JP6" H 10500 1550 50  0000 L CNN
F 1 "JUMPER3_NC" H 10450 1750 50  0000 C BNN
F 2 "stmbl:SOLDER_JUMPER_3_NC" H 10450 1650 50  0001 C CNN
F 3 "" H 10450 1650 50  0000 C CNN
F 4 "" H 10450 1650 50  0001 C CNN "InternalName"
F 5 "" H 10450 1650 50  0001 C CNN "Manufacturer No"
F 6 "" H 10450 1650 50  0001 C CNN "Voltage"
F 7 "" H 10450 1650 50  0001 C CNN "Source"
F 8 "" H 10450 1650 50  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    10450 1650
	1    0    0    1   
$EndComp
$Comp
L stmbl:+5V #PWR088
U 1 1 5832AF71
P 10100 1550
F 0 "#PWR088" H 10100 1400 50  0001 C CNN
F 1 "+5V" H 10100 1690 50  0000 C CNN
F 2 "" H 10100 1550 50  0000 C CNN
F 3 "" H 10100 1550 50  0000 C CNN
	1    10100 1550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+3.3V #PWR089
U 1 1 5832B0AA
P 10800 1550
F 0 "#PWR089" H 10800 1400 50  0001 C CNN
F 1 "+3.3V" H 10800 1690 50  0000 C CNN
F 2 "" H 10800 1550 50  0000 C CNN
F 3 "" H 10800 1550 50  0000 C CNN
	1    10800 1550
	1    0    0    -1  
$EndComp
$Comp
L stmbl:+5VA #PWR090
U 1 1 5832B172
P 10450 1450
F 0 "#PWR090" H 10450 1300 50  0001 C CNN
F 1 "+5VA" H 10450 1590 50  0000 C CNN
F 2 "" H 10450 1450 50  0000 C CNN
F 3 "" H 10450 1450 50  0000 C CNN
	1    10450 1450
	1    0    0    -1  
$EndComp
NoConn ~ 5800 4950
$Comp
L stmbl:CP C143
U 1 1 584F61D7
P 2000 3150
F 0 "C143" H 2025 3250 50  0000 L CNN
F 1 "22µ" H 2025 3050 50  0000 L CNN
F 2 "stmbl:C_Radial_D6.5_L11_P2.5" H 2038 3000 50  0000 C CNN
F 3 "" H 2000 3150 50  0000 C CNN
F 4 "" H 2000 3150 50  0001 C CNN "InternalName"
F 5 "" H 2000 3150 50  0001 C CNN "Manufacturer No"
F 6 "50V" H 2000 3150 50  0001 C CNN "Voltage"
F 7 "" H 2000 3150 50  0001 C CNN "Source"
F 8 "" H 2000 3150 50  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    2000 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R158
U 1 1 5884D28E
P 4600 3150
F 0 "R158" V 4700 3050 50  0000 L CNN
F 1 "10k" V 4600 3100 50  0000 L CNN
F 2 "stmbl:R_0603" V 4530 3150 30  0001 C CNN
F 3 "" H 4600 3150 30  0000 C CNN
F 4 "" H 4600 3150 30  0001 C CNN "InternalName"
F 5 "" H 4600 3150 30  0001 C CNN "Manufacturer No"
F 6 "" H 4600 3150 30  0001 C CNN "Voltage"
F 7 "" H 4600 3150 30  0001 C CNN "Source"
F 8 "1%" H 4600 3150 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    4600 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R155
U 1 1 5884FB7D
P 4000 3150
F 0 "R155" V 4100 3050 50  0000 L CNN
F 1 "10k" V 4000 3100 50  0000 L CNN
F 2 "stmbl:R_0603" V 3930 3150 30  0001 C CNN
F 3 "" H 4000 3150 30  0000 C CNN
F 4 "" H 4000 3150 30  0001 C CNN "InternalName"
F 5 "" H 4000 3150 30  0001 C CNN "Manufacturer No"
F 6 "" H 4000 3150 30  0001 C CNN "Voltage"
F 7 "" H 4000 3150 30  0001 C CNN "Source"
F 8 "1%" H 4000 3150 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    4000 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R156
U 1 1 5884FC85
P 4200 3150
F 0 "R156" V 4300 3050 50  0000 L CNN
F 1 "10k" V 4200 3100 50  0000 L CNN
F 2 "stmbl:R_0603" V 4130 3150 30  0001 C CNN
F 3 "" H 4200 3150 30  0000 C CNN
F 4 "" H 4200 3150 30  0001 C CNN "InternalName"
F 5 "" H 4200 3150 30  0001 C CNN "Manufacturer No"
F 6 "" H 4200 3150 30  0001 C CNN "Voltage"
F 7 "" H 4200 3150 30  0001 C CNN "Source"
F 8 "1%" H 4200 3150 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    4200 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R157
U 1 1 5884FD91
P 4400 3150
F 0 "R157" V 4500 3050 50  0000 L CNN
F 1 "10k" V 4400 3100 50  0000 L CNN
F 2 "stmbl:R_0603" V 4330 3150 30  0001 C CNN
F 3 "" H 4400 3150 30  0000 C CNN
F 4 "" H 4400 3150 30  0001 C CNN "InternalName"
F 5 "" H 4400 3150 30  0001 C CNN "Manufacturer No"
F 6 "" H 4400 3150 30  0001 C CNN "Voltage"
F 7 "" H 4400 3150 30  0001 C CNN "Source"
F 8 "1%" H 4400 3150 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    4400 3150
	1    0    0    -1  
$EndComp
$Comp
L stmbl:R R154
U 1 1 588520BC
P 3000 2650
F 0 "R154" V 3100 2550 50  0000 L CNN
F 1 "22" V 3000 2600 50  0000 L CNN
F 2 "stmbl:R_0603" V 2930 2650 30  0001 C CNN
F 3 "" H 3000 2650 30  0000 C CNN
F 4 "" H 3000 2650 30  0001 C CNN "InternalName"
F 5 "" H 3000 2650 30  0001 C CNN "Manufacturer No"
F 6 "" H 3000 2650 30  0001 C CNN "Voltage"
F 7 "" H 3000 2650 30  0001 C CNN "Source"
F 8 "1%" H 3000 2650 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    3000 2650
	1    0    0    -1  
$EndComp
$Comp
L stmbl:C C144
U 1 1 58856551
P 3750 3150
F 0 "C144" H 3775 3250 50  0000 L CNN
F 1 "2.2µ" H 3775 3050 50  0000 L CNN
F 2 "stmbl:C_0603" H 3788 3000 30  0001 C CNN
F 3 "" H 3750 3150 60  0000 C CNN
F 4 "" H 3750 3150 60  0001 C CNN "InternalName"
F 5 "" H 3750 3150 60  0001 C CNN "Manufacturer No"
F 6 "35V" H 3750 3150 60  0001 C CNN "Voltage"
F 7 "" H 3750 3150 60  0001 C CNN "Source"
F 8 "X5R" H 3750 3150 60  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    3750 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 5600 2700 5500
Wire Wire Line
	1500 5500 1500 5600
Wire Wire Line
	2000 5600 2000 5400
Wire Wire Line
	1500 5100 1500 5200
Connection ~ 1500 5100
Wire Wire Line
	2400 5100 2450 5100
Wire Wire Line
	2700 5100 2700 5200
Wire Wire Line
	1200 5000 1200 5100
Connection ~ 2700 5100
Wire Wire Line
	1100 2950 1000 2950
Wire Wire Line
	1100 2650 1100 2950
Wire Wire Line
	1000 3050 1100 3050
Wire Wire Line
	1100 3050 1100 3450
Wire Wire Line
	1200 5100 1500 5100
Wire Wire Line
	1200 5500 1200 5600
Connection ~ 1500 5600
Connection ~ 2000 5600
Wire Wire Line
	6600 4800 6700 4800
Wire Wire Line
	7000 4800 7100 4800
Wire Wire Line
	7200 4950 7200 5200
Wire Wire Line
	8000 5100 8000 4950
Wire Wire Line
	7950 5600 7950 5500
Connection ~ 8000 5100
Wire Wire Line
	6600 5100 6700 5100
Wire Wire Line
	6700 5200 6700 5100
Connection ~ 6700 5100
Wire Wire Line
	6600 4950 7100 4950
Wire Wire Line
	7100 4800 7100 4950
Connection ~ 8000 4950
Wire Wire Line
	5000 5600 5300 5600
Wire Wire Line
	5000 5500 5000 5600
Wire Wire Line
	5700 5100 5800 5100
Connection ~ 5000 5600
Wire Wire Line
	5000 4700 5000 4800
Wire Wire Line
	5000 4800 5300 4800
Connection ~ 7200 4950
Connection ~ 7200 5600
Connection ~ 6700 5600
Wire Wire Line
	6600 3000 6700 3000
Wire Wire Line
	6600 3150 7100 3150
Wire Wire Line
	7200 3150 7200 3400
Wire Wire Line
	8000 3300 8000 3150
Wire Wire Line
	7950 3800 7950 3700
Connection ~ 8000 3300
Wire Wire Line
	6600 3300 6700 3300
Wire Wire Line
	6700 3400 6700 3300
Connection ~ 6700 3300
Connection ~ 7100 3150
Connection ~ 8000 3150
Wire Wire Line
	5050 3700 5050 3800
Connection ~ 5050 3800
Wire Wire Line
	5050 2900 5050 3000
Wire Wire Line
	5050 3000 5300 3000
Connection ~ 7200 3150
Connection ~ 7200 3800
Connection ~ 6700 3800
Wire Wire Line
	5050 3800 5300 3800
Wire Wire Line
	7900 3150 8000 3150
Wire Wire Line
	1200 5600 1500 5600
Connection ~ 1200 5100
Connection ~ 1200 5600
Connection ~ 7700 5100
Wire Wire Line
	7700 5500 7700 5600
Connection ~ 7700 5600
Wire Wire Line
	7700 3700 7700 3800
Connection ~ 7700 3800
Connection ~ 7700 3300
Connection ~ 5050 3000
Connection ~ 5000 4800
Wire Wire Line
	2450 5200 2450 5100
Connection ~ 2450 5100
Wire Wire Line
	2450 5500 2450 5600
Connection ~ 2450 5600
Wire Wire Line
	8200 3800 8200 3700
Connection ~ 7950 3800
Wire Wire Line
	2500 3300 2500 3450
Wire Wire Line
	6600 1500 6700 1500
Wire Wire Line
	7200 1650 7200 1900
Wire Wire Line
	7950 2300 7950 2200
Connection ~ 7950 1800
Wire Wire Line
	6600 1800 6700 1800
Wire Wire Line
	6700 1900 6700 1800
Connection ~ 6700 1800
Wire Wire Line
	6600 1650 7100 1650
Wire Wire Line
	5700 2300 5700 1800
Connection ~ 5050 2300
Wire Wire Line
	5050 1400 5050 1500
Wire Wire Line
	8000 1800 8000 1650
Connection ~ 7200 1650
Connection ~ 7200 2300
Connection ~ 6700 2300
Wire Wire Line
	5050 2300 5300 2300
Wire Wire Line
	7700 2200 7700 2300
Connection ~ 7700 2300
Connection ~ 7700 1800
Connection ~ 5050 1500
Wire Wire Line
	8200 2300 8200 2200
Connection ~ 7950 2300
Wire Wire Line
	5700 1650 5800 1650
Wire Wire Line
	9550 1700 9550 1800
Wire Wire Line
	9550 1800 9450 1800
Wire Wire Line
	9250 1800 9150 1800
Wire Wire Line
	9150 1800 9150 1700
Wire Wire Line
	1750 2650 1750 2900
Wire Wire Line
	1750 3450 1750 3300
Wire Wire Line
	7100 5100 7300 5100
Wire Wire Line
	7600 5100 7700 5100
Wire Wire Line
	7100 3300 7300 3300
Wire Wire Line
	7600 3300 7700 3300
Wire Wire Line
	7100 1800 7300 1800
Wire Wire Line
	7600 1800 7700 1800
Wire Wire Line
	1750 2900 2000 2900
Connection ~ 1750 3450
Connection ~ 1750 2900
Wire Wire Line
	1400 2650 1400 2750
Connection ~ 1400 2650
Wire Wire Line
	1400 3450 1400 3350
Connection ~ 1400 3450
Wire Wire Line
	8450 3800 8450 3700
Connection ~ 8200 3800
Connection ~ 8200 3300
Wire Wire Line
	8200 5600 8200 5500
Connection ~ 7950 5600
Wire Wire Line
	6700 2300 6700 2200
Wire Wire Line
	7200 2300 7200 2200
Wire Wire Line
	7700 1900 7700 1800
Wire Wire Line
	8200 1800 8200 1900
Wire Wire Line
	8450 1800 8450 1900
Connection ~ 8200 1800
Wire Wire Line
	8450 2300 8450 2200
Connection ~ 8200 2300
Wire Wire Line
	5700 1800 5800 1800
Wire Wire Line
	5050 2200 5050 2300
Wire Wire Line
	5050 1500 5300 1500
Connection ~ 8000 1800
Connection ~ 8000 1650
Wire Wire Line
	7950 1900 7950 1800
Wire Wire Line
	7700 3400 7700 3300
Wire Wire Line
	8200 3400 8200 3300
Wire Wire Line
	8450 3300 8450 3400
Wire Wire Line
	7200 3800 7200 3700
Wire Wire Line
	6700 3800 6700 3700
Wire Wire Line
	5800 3300 5700 3300
Wire Wire Line
	5700 3300 5700 3800
Connection ~ 5700 3800
Wire Wire Line
	5700 5100 5700 5600
Connection ~ 5700 5600
Wire Wire Line
	6700 5500 6700 5600
Wire Wire Line
	7200 5500 7200 5600
Wire Wire Line
	8450 5100 8450 5200
Wire Wire Line
	8200 5200 8200 5100
Connection ~ 8200 5100
Wire Wire Line
	8450 5600 8450 5500
Connection ~ 8200 5600
Wire Wire Line
	7700 5200 7700 5100
Wire Wire Line
	7900 1650 8000 1650
Wire Wire Line
	7000 1500 7100 1500
Wire Wire Line
	7100 1500 7100 1650
Connection ~ 7100 1650
Wire Wire Line
	7000 3000 7100 3000
Wire Wire Line
	7100 3000 7100 3150
Wire Wire Line
	7900 4950 8000 4950
Connection ~ 7100 4950
Wire Wire Line
	7950 3400 7950 3300
Connection ~ 7950 3300
Wire Wire Line
	7950 5200 7950 5100
Connection ~ 7950 5100
Wire Wire Line
	2950 5600 2950 5500
Connection ~ 2700 5600
Wire Wire Line
	2950 5000 2950 5100
Connection ~ 2950 5100
Wire Wire Line
	1400 2950 1400 3050
Wire Wire Line
	5300 5500 5300 5600
Connection ~ 5300 5600
Wire Wire Line
	5300 5200 5300 4800
Connection ~ 5300 4800
Wire Wire Line
	5300 3400 5300 3000
Connection ~ 5300 3000
Wire Wire Line
	5300 3700 5300 3800
Connection ~ 5300 3800
Wire Wire Line
	5300 1900 5300 1500
Connection ~ 5300 1500
Wire Wire Line
	5300 2200 5300 2300
Connection ~ 5300 2300
Connection ~ 5700 2300
Wire Wire Line
	3500 3450 3500 3300
Connection ~ 2500 3450
Wire Wire Line
	2750 3300 2750 3450
Connection ~ 2750 3450
Wire Wire Line
	3000 3300 3000 3450
Connection ~ 3000 3450
Wire Wire Line
	3250 3300 3250 3450
Connection ~ 3250 3450
Wire Wire Line
	10450 1450 10450 1550
Wire Wire Line
	10100 1550 10100 1650
Wire Wire Line
	10100 1650 10200 1650
Wire Wire Line
	10700 1650 10800 1650
Wire Wire Line
	10800 1650 10800 1550
Wire Wire Line
	2000 2400 2000 2900
Connection ~ 2000 2900
Wire Wire Line
	2000 3450 2000 3300
Connection ~ 2000 3450
Wire Wire Line
	3750 3450 3750 3300
Connection ~ 3500 3450
Wire Wire Line
	3000 2800 3000 2900
Wire Wire Line
	3000 2900 3250 2900
Wire Wire Line
	3750 2900 3750 3000
Wire Wire Line
	3500 3000 3500 2900
Connection ~ 3500 2900
Wire Wire Line
	3250 2800 3250 2900
Connection ~ 3250 2900
Connection ~ 3000 2900
Wire Wire Line
	4000 3450 4000 3300
Connection ~ 3750 3450
Wire Wire Line
	4200 3450 4200 3300
Connection ~ 4000 3450
Wire Wire Line
	4400 3450 4400 3300
Connection ~ 4200 3450
Connection ~ 4400 3450
Wire Wire Line
	3000 2500 3000 2400
Wire Wire Line
	2000 2400 2750 2400
Wire Wire Line
	4400 2400 4400 3000
Connection ~ 4400 2400
Wire Wire Line
	4200 3000 4200 2400
Connection ~ 4200 2400
Wire Wire Line
	4000 3000 4000 2400
Connection ~ 4000 2400
Connection ~ 3000 2400
Wire Wire Line
	4600 2300 4600 2400
Wire Wire Line
	4600 3450 4600 3300
Connection ~ 4600 2400
$Comp
L stmbl:R R159
U 1 1 5885E706
P 2750 2650
F 0 "R159" V 2850 2550 50  0000 L CNN
F 1 "22" V 2750 2600 50  0000 L CNN
F 2 "stmbl:R_0603" V 2680 2650 30  0001 C CNN
F 3 "" H 2750 2650 30  0000 C CNN
F 4 "" H 2750 2650 30  0001 C CNN "InternalName"
F 5 "" H 2750 2650 30  0001 C CNN "Manufacturer No"
F 6 "" H 2750 2650 30  0001 C CNN "Voltage"
F 7 "" H 2750 2650 30  0001 C CNN "Source"
F 8 "1%" H 2750 2650 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    2750 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3000 2500 2900
Wire Wire Line
	2500 2900 2750 2900
Wire Wire Line
	2750 2800 2750 2900
Connection ~ 2750 2900
Wire Wire Line
	2750 2400 2750 2500
Connection ~ 2750 2400
$Comp
L stmbl:R R160
U 1 1 5885FDC7
P 3250 2650
F 0 "R160" V 3350 2550 50  0000 L CNN
F 1 "22" V 3250 2600 50  0000 L CNN
F 2 "stmbl:R_0603" V 3180 2650 30  0001 C CNN
F 3 "" H 3250 2650 30  0000 C CNN
F 4 "" H 3250 2650 30  0001 C CNN "InternalName"
F 5 "" H 3250 2650 30  0001 C CNN "Manufacturer No"
F 6 "" H 3250 2650 30  0001 C CNN "Voltage"
F 7 "" H 3250 2650 30  0001 C CNN "Source"
F 8 "1%" H 3250 2650 30  0001 C CNN "Tolerance"
F 9 "" H 0   0   50  0001 C CNN "Description"
F 10 "" H 0   0   50  0001 C CNN "Manufacturer"
	1    3250 2650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 2500 3250 2400
Connection ~ 3250 2400
Wire Wire Line
	1500 5100 1600 5100
Wire Wire Line
	2700 5100 2950 5100
Wire Wire Line
	1500 5600 2000 5600
Wire Wire Line
	2000 5600 2450 5600
Wire Wire Line
	8000 5100 8200 5100
Wire Wire Line
	6700 5100 6800 5100
Wire Wire Line
	8000 4950 8000 4850
Wire Wire Line
	5000 5600 5000 5700
Wire Wire Line
	7200 4950 7300 4950
Wire Wire Line
	7200 5600 7700 5600
Wire Wire Line
	6700 5600 7200 5600
Wire Wire Line
	8000 3300 8200 3300
Wire Wire Line
	6700 3300 6800 3300
Wire Wire Line
	7100 3150 7200 3150
Wire Wire Line
	8000 3150 8000 3050
Wire Wire Line
	5050 3800 5050 3900
Wire Wire Line
	7200 3150 7300 3150
Wire Wire Line
	7200 3800 7700 3800
Wire Wire Line
	6700 3800 7200 3800
Wire Wire Line
	1200 5100 1200 5200
Wire Wire Line
	1200 5600 1200 5650
Wire Wire Line
	7700 5100 7950 5100
Wire Wire Line
	7700 5600 7950 5600
Wire Wire Line
	7700 3800 7950 3800
Wire Wire Line
	7700 3300 7950 3300
Wire Wire Line
	5050 3000 5050 3400
Wire Wire Line
	5000 4800 5000 5200
Wire Wire Line
	2450 5100 2700 5100
Wire Wire Line
	2450 5600 2700 5600
Wire Wire Line
	7950 3800 8200 3800
Wire Wire Line
	7950 1800 8000 1800
Wire Wire Line
	6700 1800 6800 1800
Wire Wire Line
	5050 2300 5050 2400
Wire Wire Line
	7200 1650 7300 1650
Wire Wire Line
	7200 2300 7700 2300
Wire Wire Line
	6700 2300 7200 2300
Wire Wire Line
	7700 2300 7950 2300
Wire Wire Line
	7700 1800 7950 1800
Wire Wire Line
	5050 1500 5050 1900
Wire Wire Line
	7950 2300 8200 2300
Wire Wire Line
	1750 3450 2000 3450
Wire Wire Line
	1750 2900 1750 3000
Wire Wire Line
	1400 2650 1750 2650
Wire Wire Line
	1400 3450 1750 3450
Wire Wire Line
	8200 3800 8450 3800
Wire Wire Line
	8200 3300 8450 3300
Wire Wire Line
	7950 5600 8200 5600
Wire Wire Line
	8200 1800 8450 1800
Wire Wire Line
	8200 2300 8450 2300
Wire Wire Line
	8000 1800 8200 1800
Wire Wire Line
	8000 1650 8000 1550
Wire Wire Line
	5700 3800 6700 3800
Wire Wire Line
	5700 5600 6700 5600
Wire Wire Line
	8200 5100 8450 5100
Wire Wire Line
	8200 5600 8450 5600
Wire Wire Line
	7100 1650 7200 1650
Wire Wire Line
	7100 4950 7200 4950
Wire Wire Line
	7950 3300 8000 3300
Wire Wire Line
	7950 5100 8000 5100
Wire Wire Line
	2700 5600 2950 5600
Wire Wire Line
	2950 5100 2950 5200
Wire Wire Line
	5300 5600 5700 5600
Wire Wire Line
	5300 4800 5800 4800
Wire Wire Line
	5300 3000 5800 3000
Wire Wire Line
	5300 3800 5700 3800
Wire Wire Line
	5300 1500 5800 1500
Wire Wire Line
	5300 2300 5700 2300
Wire Wire Line
	5700 2300 6700 2300
Wire Wire Line
	2500 3450 2500 3550
Wire Wire Line
	2500 3450 2750 3450
Wire Wire Line
	2750 3450 3000 3450
Wire Wire Line
	3000 3450 3250 3450
Wire Wire Line
	3250 3450 3500 3450
Wire Wire Line
	2000 2900 2000 3000
Wire Wire Line
	2000 3450 2500 3450
Wire Wire Line
	3500 3450 3750 3450
Wire Wire Line
	3500 2900 3750 2900
Wire Wire Line
	3250 2900 3500 2900
Wire Wire Line
	3250 2900 3250 3000
Wire Wire Line
	3000 2900 3000 3000
Wire Wire Line
	3750 3450 4000 3450
Wire Wire Line
	4000 3450 4200 3450
Wire Wire Line
	4200 3450 4400 3450
Wire Wire Line
	4400 3450 4600 3450
Wire Wire Line
	4400 2400 4600 2400
Wire Wire Line
	4200 2400 4400 2400
Wire Wire Line
	4000 2400 4200 2400
Wire Wire Line
	3000 2400 3250 2400
Wire Wire Line
	4600 2400 4600 3000
Wire Wire Line
	2750 2900 2750 3000
Wire Wire Line
	2750 2400 3000 2400
Wire Wire Line
	3250 2400 4000 2400
Text Notes 9100 2600 0    60   ~ 0
0.81V * (51k + 1k + 10k) / 10k = 5.02V
Wire Wire Line
	1100 3450 1400 3450
Wire Wire Line
	1100 2650 1400 2650
$EndSCHEMATC
