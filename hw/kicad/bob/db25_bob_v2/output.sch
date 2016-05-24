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
$Descr A4 11693 8268
encoding utf-8
Sheet 4 5
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
L NCV7608 U18
U 1 1 57449D3B
P 4100 2600
F 0 "U18" H 4100 3737 60  0000 C CNN
F 1 "NCV7608" H 4100 3631 60  0000 C CNN
F 2 "SMD_Packages:SOIC-28" H 4100 2600 60  0001 C CNN
F 3 "" H 4100 2600 60  0000 C CNN
	1    4100 2600
	1    0    0    -1  
$EndComp
$Comp
L +24V #PWR026
U 1 1 57449F65
P 3100 3150
F 0 "#PWR026" H 3100 3000 50  0001 C CNN
F 1 "+24V" H 3115 3323 50  0000 C CNN
F 2 "" H 3100 3150 50  0000 C CNN
F 3 "" H 3100 3150 50  0000 C CNN
	1    3100 3150
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-db25_bob_v2 #PWR027
U 1 1 57449FA2
P 3100 3300
AR Path="/57449FA2" Ref="#PWR027"  Part="1" 
AR Path="/5744969F/57449FA2" Ref="#PWR027"  Part="1" 
F 0 "#PWR027" H 3100 3050 50  0001 C CNN
F 1 "GND" H 3105 3127 50  0000 C CNN
F 2 "" H 3100 3300 50  0000 C CNN
F 3 "" H 3100 3300 50  0000 C CNN
	1    3100 3300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V-RESCUE-db25_bob_v2 #PWR028
U 1 1 57449FDD
P 3250 3050
AR Path="/57449FDD" Ref="#PWR028"  Part="1" 
AR Path="/5744969F/57449FDD" Ref="#PWR028"  Part="1" 
F 0 "#PWR028" H 3250 2900 50  0001 C CNN
F 1 "+3.3V" H 3265 3223 50  0000 C CNN
F 2 "" H 3250 3050 50  0000 C CNN
F 3 "" H 3250 3050 50  0000 C CNN
	1    3250 3050
	1    0    0    -1  
$EndComp
Text GLabel 3250 2700 0    60   Input ~ 0
EN
Text GLabel 3250 2200 0    60   Input ~ 0
CSB
Text GLabel 3250 2300 0    60   Input ~ 0
SCLK
Text GLabel 3250 2400 0    60   Input ~ 0
SI1
Text GLabel 3250 2500 0    60   Input ~ 0
SO1
Text GLabel 3250 1750 0    60   Input ~ 0
PWM1
Text GLabel 3250 1850 0    60   Input ~ 0
PWM2
Text GLabel 3250 1950 0    60   Input ~ 0
PWM3
Text GLabel 3250 2050 0    60   Input ~ 0
PWM4
$Comp
L +24V #PWR029
U 1 1 5744A372
P 5050 1700
F 0 "#PWR029" H 5050 1550 50  0001 C CNN
F 1 "+24V" H 5065 1873 50  0000 C CNN
F 2 "" H 5050 1700 50  0000 C CNN
F 3 "" H 5050 1700 50  0000 C CNN
	1    5050 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3050 3350 3050
Wire Wire Line
	3100 3150 3350 3150
Wire Wire Line
	3100 3300 3100 3250
Wire Wire Line
	3100 3250 3350 3250
Wire Wire Line
	3250 1750 3350 1750
Wire Wire Line
	3250 1850 3350 1850
Wire Wire Line
	3250 1950 3350 1950
Wire Wire Line
	3250 2050 3350 2050
Wire Wire Line
	3250 2200 3350 2200
Wire Wire Line
	3250 2300 3350 2300
Wire Wire Line
	3250 2400 3350 2400
Wire Wire Line
	3250 2500 3350 2500
Wire Wire Line
	3250 2700 3350 2700
Wire Wire Line
	5050 1700 5050 3150
Wire Wire Line
	5050 3150 4850 3150
Wire Wire Line
	4850 2950 5050 2950
Connection ~ 5050 2950
Wire Wire Line
	4850 2750 5050 2750
Connection ~ 5050 2750
Wire Wire Line
	4850 2550 5050 2550
Connection ~ 5050 2550
Wire Wire Line
	4850 2350 5050 2350
Connection ~ 5050 2350
Wire Wire Line
	4850 2150 5050 2150
Connection ~ 5050 2150
Wire Wire Line
	4850 1950 5050 1950
Connection ~ 5050 1950
Wire Wire Line
	4850 1750 5050 1750
Connection ~ 5050 1750
Wire Wire Line
	4850 1850 5250 1850
Wire Wire Line
	4850 2050 5250 2050
Wire Wire Line
	4850 2250 5250 2250
Wire Wire Line
	4850 2450 5250 2450
Wire Wire Line
	4850 2650 5250 2650
Wire Wire Line
	4850 2850 5250 2850
Wire Wire Line
	4850 3050 5250 3050
Wire Wire Line
	4850 3250 5250 3250
Text GLabel 5250 1850 2    60   Input ~ 0
OUT1
Text GLabel 5250 2050 2    60   Input ~ 0
OUT2
Text GLabel 5250 2250 2    60   Input ~ 0
OUT3
Text GLabel 5250 2450 2    60   Input ~ 0
OUT4
Text GLabel 5250 2650 2    60   Input ~ 0
OUT5
Text GLabel 5250 2850 2    60   Input ~ 0
OUT6
Text GLabel 5250 3050 2    60   Input ~ 0
OUT7
Text GLabel 5250 3250 2    60   Input ~ 0
OUT8
$Comp
L CONN_01X08 P5
U 1 1 5744A6DD
P 6400 2900
F 0 "P5" H 6350 3350 50  0000 L CNN
F 1 "CONN_01X08" H 6150 2400 50  0000 L CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 6400 2900 50  0001 C CNN
F 3 "" H 6400 2900 50  0000 C CNN
	1    6400 2900
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 P7
U 1 1 5744A7A9
P 6900 2900
F 0 "P7" H 6900 2450 50  0000 C CNN
F 1 "CONN_01X08" H 6900 3400 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 6900 2900 50  0001 C CNN
F 3 "" H 6900 2900 50  0000 C CNN
	1    6900 2900
	-1   0    0    1   
$EndComp
Text GLabel 6100 2550 0    60   Input ~ 0
OUT1
Text GLabel 6100 2650 0    60   Input ~ 0
OUT2
Text GLabel 6100 2750 0    60   Input ~ 0
OUT3
Text GLabel 6100 2850 0    60   Input ~ 0
OUT4
Text GLabel 6100 2950 0    60   Input ~ 0
OUT5
Text GLabel 6100 3050 0    60   Input ~ 0
OUT6
Text GLabel 6100 3150 0    60   Input ~ 0
OUT7
Text GLabel 6100 3250 0    60   Input ~ 0
OUT8
Wire Wire Line
	6100 2550 6200 2550
Wire Wire Line
	6100 2650 6200 2650
Wire Wire Line
	6100 2750 6200 2750
Wire Wire Line
	6100 2850 6200 2850
Wire Wire Line
	6100 2950 6200 2950
Wire Wire Line
	6100 3050 6200 3050
Wire Wire Line
	6100 3150 6200 3150
Wire Wire Line
	6100 3250 6200 3250
Wire Wire Line
	7100 2550 7200 2550
Wire Wire Line
	7100 2650 7200 2650
Wire Wire Line
	7100 2750 7200 2750
Wire Wire Line
	7100 2850 7200 2850
Wire Wire Line
	7100 2950 7200 2950
Wire Wire Line
	7100 3050 7200 3050
Wire Wire Line
	7100 3150 7200 3150
Wire Wire Line
	7100 3250 7200 3250
Wire Wire Line
	7200 2550 7200 3350
$Comp
L GND-RESCUE-db25_bob_v2 #PWR030
U 1 1 5744AAA7
P 7200 3350
AR Path="/5744AAA7" Ref="#PWR030"  Part="1" 
AR Path="/5744969F/5744AAA7" Ref="#PWR030"  Part="1" 
F 0 "#PWR030" H 7200 3100 50  0001 C CNN
F 1 "GND" H 7205 3177 50  0000 C CNN
F 2 "" H 7200 3350 50  0000 C CNN
F 3 "" H 7200 3350 50  0000 C CNN
	1    7200 3350
	1    0    0    -1  
$EndComp
$Comp
L NCV7608 U19
U 1 1 5744AEC6
P 4100 4750
F 0 "U19" H 4100 5887 60  0000 C CNN
F 1 "NCV7608" H 4100 5781 60  0000 C CNN
F 2 "SMD_Packages:SOIC-28" H 4100 4750 60  0001 C CNN
F 3 "" H 4100 4750 60  0000 C CNN
	1    4100 4750
	1    0    0    -1  
$EndComp
$Comp
L +24V #PWR031
U 1 1 5744AECC
P 3100 5300
F 0 "#PWR031" H 3100 5150 50  0001 C CNN
F 1 "+24V" H 3115 5473 50  0000 C CNN
F 2 "" H 3100 5300 50  0000 C CNN
F 3 "" H 3100 5300 50  0000 C CNN
	1    3100 5300
	1    0    0    -1  
$EndComp
$Comp
L GND-RESCUE-db25_bob_v2 #PWR032
U 1 1 5744AED2
P 3100 5450
AR Path="/5744AED2" Ref="#PWR032"  Part="1" 
AR Path="/5744969F/5744AED2" Ref="#PWR032"  Part="1" 
F 0 "#PWR032" H 3100 5200 50  0001 C CNN
F 1 "GND" H 3105 5277 50  0000 C CNN
F 2 "" H 3100 5450 50  0000 C CNN
F 3 "" H 3100 5450 50  0000 C CNN
	1    3100 5450
	1    0    0    -1  
$EndComp
$Comp
L +3.3V-RESCUE-db25_bob_v2 #PWR033
U 1 1 5744AED8
P 3250 5200
AR Path="/5744AED8" Ref="#PWR033"  Part="1" 
AR Path="/5744969F/5744AED8" Ref="#PWR033"  Part="1" 
F 0 "#PWR033" H 3250 5050 50  0001 C CNN
F 1 "+3.3V" H 3265 5373 50  0000 C CNN
F 2 "" H 3250 5200 50  0000 C CNN
F 3 "" H 3250 5200 50  0000 C CNN
	1    3250 5200
	1    0    0    -1  
$EndComp
Text GLabel 3250 4850 0    60   Input ~ 0
EN
Text GLabel 3250 4350 0    60   Input ~ 0
CSB
Text GLabel 3250 4450 0    60   Input ~ 0
SCLK
Text GLabel 3250 4650 0    60   Input ~ 0
SO2
Text GLabel 3250 4550 0    60   Input ~ 0
SO1
Text GLabel 3250 3900 0    60   Input ~ 0
PWM5
$Comp
L +24V #PWR034
U 1 1 5744AEE7
P 5050 3850
F 0 "#PWR034" H 5050 3700 50  0001 C CNN
F 1 "+24V" H 5065 4023 50  0000 C CNN
F 2 "" H 5050 3850 50  0000 C CNN
F 3 "" H 5050 3850 50  0000 C CNN
	1    5050 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 5200 3350 5200
Wire Wire Line
	3100 5300 3350 5300
Wire Wire Line
	3100 5450 3100 5400
Wire Wire Line
	3100 5400 3350 5400
Wire Wire Line
	3250 3900 3350 3900
Wire Wire Line
	3250 4000 3350 4000
Wire Wire Line
	3250 4100 3350 4100
Wire Wire Line
	3250 4200 3350 4200
Wire Wire Line
	3250 4350 3350 4350
Wire Wire Line
	3250 4450 3350 4450
Wire Wire Line
	3250 4550 3350 4550
Wire Wire Line
	3250 4650 3350 4650
Wire Wire Line
	3250 4850 3350 4850
Wire Wire Line
	5050 3850 5050 5300
Wire Wire Line
	5050 5300 4850 5300
Wire Wire Line
	4850 5100 5050 5100
Connection ~ 5050 5100
Wire Wire Line
	4850 4900 5050 4900
Connection ~ 5050 4900
Wire Wire Line
	4850 4700 5050 4700
Connection ~ 5050 4700
Wire Wire Line
	4850 4500 5050 4500
Connection ~ 5050 4500
Wire Wire Line
	4850 4300 5050 4300
Connection ~ 5050 4300
Wire Wire Line
	4850 4100 5050 4100
Connection ~ 5050 4100
Wire Wire Line
	4850 3900 5050 3900
Connection ~ 5050 3900
Wire Wire Line
	4850 4000 5250 4000
Wire Wire Line
	4850 4200 5250 4200
Wire Wire Line
	4850 4400 5250 4400
Wire Wire Line
	4850 4600 5250 4600
Wire Wire Line
	4850 4800 5250 4800
Wire Wire Line
	4850 5000 5250 5000
Wire Wire Line
	4850 5200 5250 5200
Wire Wire Line
	4850 5400 5250 5400
$Comp
L CONN_01X08 P6
U 1 1 5744AF1A
P 6400 5050
F 0 "P6" H 6350 5500 50  0000 L CNN
F 1 "CONN_01X08" H 6150 4550 50  0000 L CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 6400 5050 50  0001 C CNN
F 3 "" H 6400 5050 50  0000 C CNN
	1    6400 5050
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X08 P8
U 1 1 5744AF20
P 6900 5050
F 0 "P8" H 6900 4600 50  0000 C CNN
F 1 "CONN_01X08" H 6900 5550 50  0000 C CNN
F 2 "Terminal_Blocks:TerminalBlock_Pheonix_PT-3.5mm_8pol" H 6900 5050 50  0001 C CNN
F 3 "" H 6900 5050 50  0000 C CNN
	1    6900 5050
	-1   0    0    1   
$EndComp
Wire Wire Line
	6100 4700 6200 4700
Wire Wire Line
	6100 4800 6200 4800
Wire Wire Line
	6100 4900 6200 4900
Wire Wire Line
	6100 5000 6200 5000
Wire Wire Line
	6100 5100 6200 5100
Wire Wire Line
	6100 5200 6200 5200
Wire Wire Line
	6100 5300 6200 5300
Wire Wire Line
	6100 5400 6200 5400
Wire Wire Line
	7100 4700 7200 4700
Wire Wire Line
	7100 4800 7200 4800
Wire Wire Line
	7100 4900 7200 4900
Wire Wire Line
	7100 5000 7200 5000
Wire Wire Line
	7100 5100 7200 5100
Wire Wire Line
	7100 5200 7200 5200
Wire Wire Line
	7100 5300 7200 5300
Wire Wire Line
	7100 5400 7200 5400
Wire Wire Line
	7200 4700 7200 5500
$Comp
L GND-RESCUE-db25_bob_v2 #PWR035
U 1 1 5744AF3F
P 7200 5500
AR Path="/5744AF3F" Ref="#PWR035"  Part="1" 
AR Path="/5744969F/5744AF3F" Ref="#PWR035"  Part="1" 
F 0 "#PWR035" H 7200 5250 50  0001 C CNN
F 1 "GND" H 7205 5327 50  0000 C CNN
F 2 "" H 7200 5500 50  0000 C CNN
F 3 "" H 7200 5500 50  0000 C CNN
	1    7200 5500
	1    0    0    -1  
$EndComp
Text GLabel 6100 4700 0    60   Input ~ 0
OUT9
Text GLabel 6100 4800 0    60   Input ~ 0
OUT10
Text GLabel 6100 4900 0    60   Input ~ 0
OUT11
Text GLabel 6100 5000 0    60   Input ~ 0
OUT12
Text GLabel 6100 5100 0    60   Input ~ 0
OUT13
Text GLabel 6100 5200 0    60   Input ~ 0
OUT14
Text GLabel 6100 5300 0    60   Input ~ 0
OUT15
Text GLabel 6100 5400 0    60   Input ~ 0
OUT16
Text GLabel 5250 4000 2    60   Input ~ 0
OUT9
Text GLabel 5250 4200 2    60   Input ~ 0
OUT10
Text GLabel 5250 4400 2    60   Input ~ 0
OUT11
Text GLabel 5250 4600 2    60   Input ~ 0
OUT12
Text GLabel 5250 4800 2    60   Input ~ 0
OUT13
Text GLabel 5250 5000 2    60   Input ~ 0
OUT14
Text GLabel 5250 5200 2    60   Input ~ 0
OUT15
Text GLabel 5250 5400 2    60   Input ~ 0
OUT16
Text GLabel 3250 4000 0    60   Input ~ 0
PWM6
Text GLabel 3250 4200 0    60   Input ~ 0
PWM8
Text GLabel 3250 4100 0    60   Input ~ 0
PWM7
Connection ~ 7200 2650
Connection ~ 7200 2750
Connection ~ 7200 2850
Connection ~ 7200 2950
Connection ~ 7200 3050
Connection ~ 7200 3150
Connection ~ 7200 3250
Connection ~ 7200 4800
Connection ~ 7200 4900
Connection ~ 7200 5000
Connection ~ 7200 5100
Connection ~ 7200 5200
Connection ~ 7200 5300
Connection ~ 7200 5400
$EndSCHEMATC
