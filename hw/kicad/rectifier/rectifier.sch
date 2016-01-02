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
LIBS:74xgxx
LIBS:ac-dc
LIBS:brooktre
LIBS:cmos_ieee
LIBS:dc-dc
LIBS:elec-unifil
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:logo
LIBS:microchip1
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:msp430
LIBS:nxp_armmcu
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:special
LIBS:stm8
LIBS:stm32
LIBS:supertex
LIBS:transf
LIBS:ttl_ieee
LIBS:video
LIBS:rectifier-cache
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
L BRIDGE D3
U 1 1 56262529
P 4800 4600
F 0 "D3" H 4800 4650 70  0000 C CNN
F 1 "BRIDGE" H 4800 4550 70  0000 C CNN
F 2 "bridge:dfb_bridge" H 4800 4600 60  0001 C CNN
F 3 "" H 4800 4600 60  0000 C CNN
	1    4800 4600
	1    0    0    -1  
$EndComp
$Comp
L CP1 C1
U 1 1 562625AE
P 8000 3850
F 0 "C1" H 8050 3950 50  0000 L CNN
F 1 "CP1" H 8050 3750 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D35_L51_P10" H 8000 3850 60  0001 C CNN
F 3 "" H 8000 3850 60  0000 C CNN
	1    8000 3850
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 56262678
P 9000 3950
F 0 "P3" V 8950 3950 40  0000 C CNN
F 1 "CONN_2" V 9050 3950 40  0000 C CNN
F 2 "custom_footprints:WE313-2" H 9000 3950 60  0001 C CNN
F 3 "" H 9000 3950 60  0000 C CNN
	1    9000 3950
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 562626ED
P 1500 2500
F 0 "P2" V 1450 2500 40  0000 C CNN
F 1 "CONN_2" V 1550 2500 40  0000 C CNN
F 2 "custom_footprints:WE313-2" H 1500 2500 60  0001 C CNN
F 3 "" H 1500 2500 60  0000 C CNN
	1    1500 2500
	-1   0    0    1   
$EndComp
$Comp
L CONN_3 K1
U 1 1 562627B8
P 1500 1350
F 0 "K1" V 1450 1350 50  0000 C CNN
F 1 "CONN_3" V 1550 1350 40  0000 C CNN
F 2 "custom_footprints:WE313-3" H 1500 1350 60  0001 C CNN
F 3 "" H 1500 1350 60  0000 C CNN
	1    1500 1350
	-1   0    0    1   
$EndComp
$Comp
L FUSE F1
U 1 1 5626282B
P 2250 1350
F 0 "F1" H 2350 1400 40  0000 C CNN
F 1 "FUSE" H 2150 1300 40  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuseholder5x20_horiz_SemiClosed_Casing10x25mm" H 2250 1350 60  0001 C CNN
F 3 "" H 2250 1350 60  0000 C CNN
	1    2250 1350
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 56262BA3
P 2650 3900
F 0 "R2" V 2730 3900 40  0000 C CNN
F 1 "R" V 2657 3901 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM7.5mm" V 2580 3900 30  0001 C CNN
F 3 "" H 2650 3900 30  0000 C CNN
	1    2650 3900
	-1   0    0    1   
$EndComp
$Comp
L DIODE D2
U 1 1 56262D54
P 2700 2400
F 0 "D2" H 2700 2500 40  0000 C CNN
F 1 "DIODE" H 2700 2300 40  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_DO-41_SOD81_Horizontal_RM10" H 2700 2400 60  0001 C CNN
F 3 "" H 2700 2400 60  0000 C CNN
	1    2700 2400
	0    -1   -1   0   
$EndComp
$Comp
L LED D1
U 1 1 56262E24
P 2200 2100
F 0 "D1" H 2200 2200 50  0000 C CNN
F 1 "LED" H 2200 2000 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 2200 2100 60  0001 C CNN
F 3 "" H 2200 2100 60  0000 C CNN
	1    2200 2100
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 56263013
P 2450 2350
F 0 "R1" V 2530 2350 40  0000 C CNN
F 1 "R" V 2457 2351 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 2380 2350 30  0001 C CNN
F 3 "" H 2450 2350 30  0000 C CNN
	1    2450 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 2000 3150 2000
Connection ~ 2000 2100
Wire Wire Line
	1850 2600 3150 2600
Wire Wire Line
	2400 2100 2450 2100
Connection ~ 2450 2600
Connection ~ 2700 2600
Wire Wire Line
	2700 2200 2700 2000
Connection ~ 2700 2000
$Comp
L CONN_2 P1
U 1 1 5626358D
P 1500 2150
F 0 "P1" V 1450 2150 40  0000 C CNN
F 1 "CONN_2" V 1550 2150 40  0000 C CNN
F 2 "custom_footprints:WE313-2" H 1500 2150 60  0001 C CNN
F 3 "" H 1500 2150 60  0000 C CNN
	1    1500 2150
	-1   0    0    1   
$EndComp
Wire Wire Line
	1850 2050 2000 2050
Connection ~ 2000 2050
Text GLabel 3600 2700 3    60   Input ~ 0
AC_fused
Text GLabel 2600 1350 2    60   Input ~ 0
AC_fused
Text GLabel 1850 1550 3    60   Input ~ 0
PE
Text GLabel 2100 1200 2    60   Input ~ 0
Neutral
Wire Wire Line
	1850 1550 1850 1450
Wire Wire Line
	1950 1200 2100 1200
Wire Wire Line
	1850 1350 2000 1350
Wire Wire Line
	2500 1350 2600 1350
Wire Wire Line
	3600 2700 3600 2600
Text GLabel 3700 1900 1    60   Input ~ 0
AC_switched
Wire Wire Line
	3700 2000 3700 1900
Wire Wire Line
	1900 4200 2650 4200
Wire Wire Line
	2650 4200 2650 4150
Wire Wire Line
	2350 3600 2650 3600
Wire Wire Line
	2650 3600 2650 3650
Text GLabel 2500 3500 1    60   Input ~ 0
AC_switched
Wire Wire Line
	2500 3600 2500 3500
Connection ~ 2500 3600
Text GLabel 2450 4300 3    60   Input ~ 0
AC_ntc
Wire Wire Line
	2450 4300 2450 4200
Connection ~ 2450 4200
Text GLabel 1450 3450 0    60   Input ~ 0
AC_ntc
Text GLabel 1450 4350 0    60   Input ~ 0
Neutral
Text GLabel 4600 5450 0    60   Input ~ 0
Neutral
Text GLabel 4600 3750 0    60   Input ~ 0
AC_ntc
Wire Wire Line
	4600 3750 4800 3750
Wire Wire Line
	4600 5450 4800 5450
Text GLabel 3800 4600 0    60   Input ~ 0
DC-
Text GLabel 5750 4600 2    60   Input ~ 0
DC+
Wire Wire Line
	3800 4600 3950 4600
Wire Wire Line
	5650 4600 5750 4600
Wire Wire Line
	7750 4050 8650 4050
Wire Wire Line
	7750 3650 8650 3650
Wire Wire Line
	8650 3650 8650 3850
Connection ~ 8300 4050
Connection ~ 8300 3650
Text GLabel 7750 3650 0    60   Input ~ 0
DC+
Text GLabel 7750 4050 0    60   Input ~ 0
DC-
Connection ~ 8000 3650
Connection ~ 8000 4050
Wire Wire Line
	1850 2250 1850 2400
Wire Wire Line
	2000 2000 2000 2950
$Comp
L CONN_2 P4
U 1 1 56266A2B
P 1500 2850
F 0 "P4" V 1450 2850 40  0000 C CNN
F 1 "CONN_2" V 1550 2850 40  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x02" H 1500 2850 60  0001 C CNN
F 3 "" H 1500 2850 60  0000 C CNN
	1    1500 2850
	-1   0    0    1   
$EndComp
Wire Wire Line
	1850 2750 1950 2750
Wire Wire Line
	1950 2750 1950 2600
Connection ~ 1950 2600
Wire Wire Line
	2000 2950 1850 2950
$Comp
L R R3
U 1 1 5626609F
P 8700 4600
F 0 "R3" V 8780 4600 40  0000 C CNN
F 1 "R" V 8707 4601 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 8630 4600 30  0001 C CNN
F 3 "" H 8700 4600 30  0000 C CNN
	1    8700 4600
	1    0    0    -1  
$EndComp
Text GLabel 8550 4850 0    60   Input ~ 0
DC-
Text GLabel 8550 4350 0    60   Input ~ 0
DC+
Wire Wire Line
	8550 4850 8700 4850
Wire Wire Line
	8550 4350 8700 4350
$Comp
L CONN_1 P5
U 1 1 56267627
P 2400 850
F 0 "P5" H 2480 850 40  0000 L CNN
F 1 "CONN_1" H 2400 905 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_2-5mmDrill" H 2400 850 60  0001 C CNN
F 3 "" H 2400 850 60  0000 C CNN
	1    2400 850 
	1    0    0    -1  
$EndComp
Text GLabel 2150 850  0    60   Input ~ 0
PE
Wire Wire Line
	2150 850  2250 850 
Wire Wire Line
	1850 1250 1950 1250
Wire Wire Line
	1950 1250 1950 1200
$Comp
L FINDER-40.52 RL2
U 1 1 56268E4F
P 3350 2300
F 0 "RL2" H 3000 2550 40  0000 C CNN
F 1 "FINDER-40.52" V 4195 2305 40  0000 C CNN
F 2 "Relays_ThroughHole:Relay_DPDT_Schrack-RT2_RM5mm" H 3350 2300 60  0001 C CNN
F 3 "" H 3350 2300 60  0000 C CNN
	1    3350 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2600 3950 2600
Wire Wire Line
	4050 2000 4050 1950
Wire Wire Line
	4050 1950 3700 1950
Connection ~ 3700 1950
$Comp
L FINDER-40.52 RL1
U 1 1 562691A0
P 1650 3900
F 0 "RL1" H 1300 4150 40  0000 C CNN
F 1 "FINDER-40.52" V 2495 3905 40  0000 C CNN
F 2 "Relays_ThroughHole:Relay_DPDT_Schrack-RT2_RM5mm" H 1650 3900 60  0001 C CNN
F 3 "" H 1650 3900 60  0000 C CNN
	1    1650 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 3600 1450 3450
Wire Wire Line
	1450 4350 1450 4200
Connection ~ 2250 4200
Wire Wire Line
	2000 3600 2000 3550
Wire Wire Line
	2000 3550 2400 3550
Wire Wire Line
	2400 3550 2400 3600
Connection ~ 2400 3600
$EndSCHEMATC
