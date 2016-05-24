EESchema Schematic File Version 2
LIBS:db25_bob_v2-rescue
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
LIBS:stm32
LIBS:adm4857
LIBS:ncv7608
LIBS:db25_bob_v2-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 2 5
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
L STM32F303CBTx U1
U 1 1 5742F979
P 8400 4600
F 0 "U1" H 8400 6671 50  0000 C CNN
F 1 "STM32F303CBTx" H 8400 6579 50  0000 C CNN
F 2 "Housings_QFP:LQFP-48_7x7mm_Pitch0.5mm" H 8400 6487 50  0001 C CNN
F 3 "" H 8400 4600 50  0000 C CNN
	1    8400 4600
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 P1
U 1 1 5744B58F
P 2000 9050
F 0 "P1" H 1919 8475 50  0000 C CNN
F 1 "CONN_01X08" H 1919 8566 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 2000 9050 50  0001 C CNN
F 3 "" H 2000 9050 50  0000 C CNN
	1    2000 9050
	-1   0    0    1   
$EndComp
Wire Wire Line
	2200 8700 2300 8700
Wire Wire Line
	2200 8800 2300 8800
Wire Wire Line
	2200 8900 2300 8900
Wire Wire Line
	2200 9000 2300 9000
Wire Wire Line
	2200 9100 2300 9100
Wire Wire Line
	2200 9200 2300 9200
Wire Wire Line
	2200 9300 2300 9300
Wire Wire Line
	2200 9400 2300 9400
Text GLabel 2300 8700 2    60   Input ~ 0
IN1
Text GLabel 2300 8800 2    60   Input ~ 0
IN2
Text GLabel 2300 8900 2    60   Input ~ 0
IN3
Text GLabel 2300 9000 2    60   Input ~ 0
IN4
Text GLabel 2300 9100 2    60   Input ~ 0
IN5
Text GLabel 2300 9200 2    60   Input ~ 0
IN6
Text GLabel 2300 9300 2    60   Input ~ 0
IN7
Text GLabel 2300 9400 2    60   Input ~ 0
IN8
$Comp
L CONN_01X08 P3
U 1 1 5744B70D
P 2900 9050
F 0 "P3" H 2819 8475 50  0000 C CNN
F 1 "CONN_01X08" H 2819 8566 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 2900 9050 50  0001 C CNN
F 3 "" H 2900 9050 50  0000 C CNN
	1    2900 9050
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 8700 3200 8700
Wire Wire Line
	3100 8800 3200 8800
Wire Wire Line
	3100 8900 3200 8900
Wire Wire Line
	3100 9000 3200 9000
Wire Wire Line
	3100 9100 3200 9100
Wire Wire Line
	3100 9200 3200 9200
Wire Wire Line
	3100 9300 3200 9300
Wire Wire Line
	3100 9400 3200 9400
Text GLabel 3200 8700 2    60   Input ~ 0
IN9
Text GLabel 3200 8800 2    60   Input ~ 0
IN10
Text GLabel 3200 8900 2    60   Input ~ 0
IN11
Text GLabel 3200 9000 2    60   Input ~ 0
IN12
Text GLabel 3200 9100 2    60   Input ~ 0
IN13
Text GLabel 3200 9200 2    60   Input ~ 0
IN14
Text GLabel 3200 9300 2    60   Input ~ 0
IN15
Text GLabel 3200 9400 2    60   Input ~ 0
IN16
Text GLabel 4100 9350 2    60   Input ~ 0
IN2
Text GLabel 4100 9450 2    60   Input ~ 0
IN3
Text GLabel 4100 9550 2    60   Input ~ 0
IN4
Text GLabel 4100 9650 2    60   Input ~ 0
IN5
Text GLabel 4100 9750 2    60   Input ~ 0
IN6
Text GLabel 4100 9850 2    60   Input ~ 0
IN7
Text GLabel 4100 9950 2    60   Input ~ 0
IN8
Text GLabel 4100 10050 2    60   Input ~ 0
IN9
Text GLabel 4100 10150 2    60   Input ~ 0
IN10
Text GLabel 4100 10250 2    60   Input ~ 0
IN11
Text GLabel 4100 10350 2    60   Input ~ 0
IN12
Text GLabel 4100 10450 2    60   Input ~ 0
IN13
Text GLabel 4100 10550 2    60   Input ~ 0
IN14
Text GLabel 4100 10650 2    60   Input ~ 0
IN15
$Comp
L D D2
U 1 1 5744B850
P 5400 7600
F 0 "D2" V 5354 7678 50  0000 L CNN
F 1 "D" V 5445 7678 50  0000 L CNN
F 2 "Diodes_SMD:MiniMELF_Standard" H 5400 7600 50  0001 C CNN
F 3 "" H 5400 7600 50  0000 C CNN
	1    5400 7600
	0    1    1    0   
$EndComp
$Comp
L D D1
U 1 1 5744B8A9
P 5400 7300
F 0 "D1" V 5354 7378 50  0000 L CNN
F 1 "D" V 5445 7378 50  0000 L CNN
F 2 "Diodes_SMD:MiniMELF_Standard" H 5400 7300 50  0001 C CNN
F 3 "" H 5400 7300 50  0000 C CNN
	1    5400 7300
	0    1    1    0   
$EndComp
$Comp
L +3.3V-RESCUE-db25_bob_v2 #PWR01
U 1 1 5744B8D3
P 5400 7100
F 0 "#PWR01" H 5400 6950 50  0001 C CNN
F 1 "+3.3V" H 5415 7273 50  0000 C CNN
F 2 "" H 5400 7100 50  0000 C CNN
F 3 "" H 5400 7100 50  0000 C CNN
	1    5400 7100
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-db25_bob_v2 #PWR02
U 1 1 5744B8F3
P 5400 7850
F 0 "#PWR02" H 5400 7600 50  0001 C CNN
F 1 "GND" H 5405 7677 50  0000 C CNN
F 2 "" H 5400 7850 50  0000 C CNN
F 3 "" H 5400 7850 50  0000 C CNN
	1    5400 7850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 7850 5400 7750
Wire Wire Line
	5400 7150 5400 7100
Wire Wire Line
	5050 7050 5050 7150
Connection ~ 5400 7450
Text GLabel 5500 7450 2    60   Input ~ 0
IN1_ADC
Text GLabel 5050 7050 1    60   Input ~ 0
IN1
$Comp
L R-RESCUE-db25_bob_v2 R1
U 1 1 5744DA8C
P 5050 7300
F 0 "R1" H 4980 7254 50  0000 R CNN
F 1 "R" H 4980 7345 50  0000 R CNN
F 2 "Resistors_SMD:R_1206" V 4980 7300 50  0001 C CNN
F 3 "" H 5050 7300 50  0000 C CNN
	1    5050 7300
	-1   0    0    1   
$EndComp
$Comp
L R-RESCUE-db25_bob_v2 R2
U 1 1 5744DAEF
P 5050 7650
F 0 "R2" H 5120 7696 50  0000 L CNN
F 1 "R" H 5120 7605 50  0000 L CNN
F 2 "Resistors_SMD:R_1206" V 4980 7650 50  0001 C CNN
F 3 "" H 5050 7650 50  0000 C CNN
	1    5050 7650
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-db25_bob_v2 #PWR03
U 1 1 5744DB3E
P 5050 7850
F 0 "#PWR03" H 5050 7600 50  0001 C CNN
F 1 "GND" H 5055 7677 50  0000 C CNN
F 2 "" H 5050 7850 50  0000 C CNN
F 3 "" H 5050 7850 50  0000 C CNN
	1    5050 7850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 7500 5050 7450
Wire Wire Line
	5050 7450 5500 7450
$Comp
L C-RESCUE-db25_bob_v2 C9
U 1 1 5744DBE9
P 5250 7600
F 0 "C9" H 5365 7646 50  0000 L CNN
F 1 "C" H 5365 7555 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805" H 5288 7450 50  0001 C CNN
F 3 "" H 5250 7600 50  0000 C CNN
	1    5250 7600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 7800 5050 7850
Wire Wire Line
	5050 7800 5250 7800
Wire Wire Line
	5250 7800 5250 7750
$Comp
L CONN_01X08 P2
U 1 1 5744DE95
P 2000 10100
F 0 "P2" H 1919 9525 50  0000 C CNN
F 1 "CONN_01X08" H 1919 9616 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 2000 10100 50  0001 C CNN
F 3 "" H 2000 10100 50  0000 C CNN
	1    2000 10100
	-1   0    0    1   
$EndComp
Wire Wire Line
	2200 9750 2300 9750
Wire Wire Line
	2200 9850 2300 9850
Wire Wire Line
	2300 9950 2200 9950
Wire Wire Line
	2300 10050 2200 10050
Wire Wire Line
	2300 10150 2200 10150
Wire Wire Line
	2300 10250 2200 10250
Wire Wire Line
	2300 10350 2200 10350
Wire Wire Line
	2300 10450 2200 10450
$Comp
L CONN_01X08 P4
U 1 1 5744DF29
P 2900 10100
F 0 "P4" H 2819 9525 50  0000 C CNN
F 1 "CONN_01X08" H 2819 9616 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 2900 10100 50  0001 C CNN
F 3 "" H 2900 10100 50  0000 C CNN
	1    2900 10100
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 9750 3200 9750
Wire Wire Line
	3100 9850 3200 9850
Wire Wire Line
	3200 9950 3100 9950
Wire Wire Line
	3200 10050 3100 10050
Wire Wire Line
	3200 10150 3100 10150
Wire Wire Line
	3200 10250 3100 10250
Wire Wire Line
	3200 10350 3100 10350
Wire Wire Line
	3200 10450 3100 10450
$Comp
L GND-RESCUE-db25_bob_v2 #PWR04
U 1 1 5744DF84
P 2300 10500
F 0 "#PWR04" H 2300 10250 50  0001 C CNN
F 1 "GND" H 2305 10327 50  0000 C CNN
F 2 "" H 2300 10500 50  0000 C CNN
F 3 "" H 2300 10500 50  0000 C CNN
	1    2300 10500
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-db25_bob_v2 #PWR05
U 1 1 5744DFD1
P 3200 10500
F 0 "#PWR05" H 3200 10250 50  0001 C CNN
F 1 "GND" H 3205 10327 50  0000 C CNN
F 2 "" H 3200 10500 50  0000 C CNN
F 3 "" H 3200 10500 50  0000 C CNN
	1    3200 10500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 9750 2300 10500
Wire Wire Line
	3200 9750 3200 10500
Connection ~ 2300 9850
Connection ~ 2300 9950
Connection ~ 2300 10050
Connection ~ 2300 10150
Connection ~ 2300 10250
Connection ~ 2300 10350
Connection ~ 2300 10450
Connection ~ 3200 10450
Connection ~ 3200 10350
Connection ~ 3200 10250
Connection ~ 3200 10150
Connection ~ 3200 10050
Connection ~ 3200 9950
Connection ~ 3200 9850
Connection ~ 5250 7450
$EndSCHEMATC
