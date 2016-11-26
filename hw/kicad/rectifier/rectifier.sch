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
P 7200 3100
F 0 "D3" H 7200 3150 70  0000 C CNN
F 1 "BRIDGE" H 7200 3050 70  0000 C CNN
F 2 "bridge:dfb_bridge" H 7200 3100 60  0001 C CNN
F 3 "" H 7200 3100 60  0000 C CNN
	1    7200 3100
	1    0    0    -1  
$EndComp
$Comp
L CP1 C1
U 1 1 562625AE
P 4750 5250
F 0 "C1" H 4800 5350 50  0000 L CNN
F 1 "CP1" H 4800 5150 50  0000 L CNN
F 2 "Capacitors_ThroughHole:C_Radial_D35_L51_P10" H 4750 5250 60  0001 C CNN
F 3 "" H 4750 5250 60  0000 C CNN
	1    4750 5250
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 56262678
P 5750 5350
F 0 "P3" V 5700 5350 40  0000 C CNN
F 1 "CONN_2" V 5800 5350 40  0000 C CNN
F 2 "custom_footprints:WE313-2" H 5750 5350 60  0001 C CNN
F 3 "" H 5750 5350 60  0000 C CNN
	1    5750 5350
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 562626ED
P 1150 3250
F 0 "P2" V 1100 3250 40  0000 C CNN
F 1 "CONN_2" V 1200 3250 40  0000 C CNN
F 2 "custom_footprints:WE313-2" H 1150 3250 60  0001 C CNN
F 3 "" H 1150 3250 60  0000 C CNN
	1    1150 3250
	-1   0    0    1   
$EndComp
$Comp
L CONN_3 K1
U 1 1 562627B8
P 1150 1900
F 0 "K1" V 1100 1900 50  0000 C CNN
F 1 "CONN_3" V 1200 1900 40  0000 C CNN
F 2 "custom_footprints:WE313-3" H 1150 1900 60  0001 C CNN
F 3 "" H 1150 1900 60  0000 C CNN
	1    1150 1900
	-1   0    0    1   
$EndComp
$Comp
L FUSE F1
U 1 1 5626282B
P 1900 1900
F 0 "F1" H 2000 1950 40  0000 C CNN
F 1 "FUSE" H 1800 1850 40  0000 C CNN
F 2 "Fuse_Holders_and_Fuses:Fuseholder5x20_horiz_SemiClosed_Casing10x25mm" H 1900 1900 60  0001 C CNN
F 3 "" H 1900 1900 60  0000 C CNN
	1    1900 1900
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 56262BA3
P 2650 5300
F 0 "R2" V 2730 5300 40  0000 C CNN
F 1 "R" V 2657 5301 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Vertical_RM7.5mm" V 2580 5300 30  0001 C CNN
F 3 "" H 2650 5300 30  0000 C CNN
	1    2650 5300
	-1   0    0    1   
$EndComp
$Comp
L DIODE D2
U 1 1 56262D54
P 2350 3150
F 0 "D2" H 2350 3250 40  0000 C CNN
F 1 "DIODE" H 2350 3050 40  0000 C CNN
F 2 "Diodes_ThroughHole:Diode_DO-41_SOD81_Horizontal_RM10" H 2350 3150 60  0001 C CNN
F 3 "" H 2350 3150 60  0000 C CNN
	1    2350 3150
	0    -1   -1   0   
$EndComp
$Comp
L LED D1
U 1 1 56262E24
P 1850 2850
F 0 "D1" H 1850 2950 50  0000 C CNN
F 1 "LED" H 1850 2750 50  0000 C CNN
F 2 "LEDs:LED-3MM" H 1850 2850 60  0001 C CNN
F 3 "" H 1850 2850 60  0000 C CNN
	1    1850 2850
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 56263013
P 2100 3100
F 0 "R1" V 2180 3100 40  0000 C CNN
F 1 "R" V 2107 3101 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 2030 3100 30  0001 C CNN
F 3 "" H 2100 3100 30  0000 C CNN
	1    2100 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2750 2800 2750
Connection ~ 1650 2850
Wire Wire Line
	1500 3350 2800 3350
Wire Wire Line
	2050 2850 2100 2850
Connection ~ 2100 3350
Connection ~ 2350 3350
Wire Wire Line
	2350 2950 2350 2750
Connection ~ 2350 2750
$Comp
L CONN_2 P1
U 1 1 5626358D
P 1150 2900
F 0 "P1" V 1100 2900 40  0000 C CNN
F 1 "CONN_2" V 1200 2900 40  0000 C CNN
F 2 "custom_footprints:WE313-2" H 1150 2900 60  0001 C CNN
F 3 "" H 1150 2900 60  0000 C CNN
	1    1150 2900
	-1   0    0    1   
$EndComp
Wire Wire Line
	1500 2800 1650 2800
Connection ~ 1650 2800
Text GLabel 3250 3450 3    60   Input ~ 0
AC_fused
Text GLabel 2250 1900 2    60   Input ~ 0
AC_fused
Text GLabel 1500 2100 3    60   Input ~ 0
PE
Text GLabel 1750 1750 2    60   Input ~ 0
Neutral
Wire Wire Line
	1500 2100 1500 2000
Wire Wire Line
	1600 1750 1750 1750
Wire Wire Line
	1500 1900 1650 1900
Wire Wire Line
	2150 1900 2250 1900
Wire Wire Line
	3250 3450 3250 3350
Text GLabel 3350 2650 1    60   Input ~ 0
AC_switched
Wire Wire Line
	3350 2750 3350 2650
Wire Wire Line
	1900 5600 2650 5600
Wire Wire Line
	2650 5600 2650 5550
Wire Wire Line
	2350 5000 2650 5000
Wire Wire Line
	2650 5000 2650 5050
Text GLabel 2500 4900 1    60   Input ~ 0
AC_switched
Wire Wire Line
	2500 5000 2500 4900
Connection ~ 2500 5000
Text GLabel 2450 5700 3    60   Input ~ 0
AC_ntc
Wire Wire Line
	2450 5700 2450 5600
Connection ~ 2450 5600
Text GLabel 1450 4850 0    60   Input ~ 0
AC_ntc
Text GLabel 7000 2250 0    60   Input ~ 0
AC_ntc
Wire Wire Line
	7000 2250 7200 2250
Wire Wire Line
	7000 3950 7200 3950
Text GLabel 6200 3100 0    60   Input ~ 0
DC-
Text GLabel 8150 3100 2    60   Input ~ 0
DC+
Wire Wire Line
	6200 3100 6350 3100
Wire Wire Line
	8050 3100 8150 3100
Wire Wire Line
	4500 5450 5400 5450
Wire Wire Line
	4500 5050 5400 5050
Wire Wire Line
	5400 5050 5400 5250
Connection ~ 5050 5450
Connection ~ 5050 5050
Text GLabel 4500 5050 0    60   Input ~ 0
DC+
Text GLabel 4500 5450 0    60   Input ~ 0
DC-
Connection ~ 4750 5050
Connection ~ 4750 5450
Wire Wire Line
	1500 3000 1500 3150
Wire Wire Line
	1650 2750 1650 3700
$Comp
L CONN_2 P4
U 1 1 56266A2B
P 1150 3600
F 0 "P4" V 1100 3600 40  0000 C CNN
F 1 "CONN_2" V 1200 3600 40  0000 C CNN
F 2 "Socket_Strips:Socket_Strip_Straight_1x02" H 1150 3600 60  0001 C CNN
F 3 "" H 1150 3600 60  0000 C CNN
	1    1150 3600
	-1   0    0    1   
$EndComp
Wire Wire Line
	1500 3500 1600 3500
Wire Wire Line
	1600 3500 1600 3350
Connection ~ 1600 3350
Wire Wire Line
	1650 3700 1500 3700
$Comp
L R R3
U 1 1 5626609F
P 4650 5950
F 0 "R3" V 4730 5950 40  0000 C CNN
F 1 "R" V 4657 5951 40  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 4580 5950 30  0001 C CNN
F 3 "" H 4650 5950 30  0000 C CNN
	1    4650 5950
	1    0    0    -1  
$EndComp
Text GLabel 4500 6200 0    60   Input ~ 0
DC-
Text GLabel 4500 5700 0    60   Input ~ 0
DC+
Wire Wire Line
	4500 6200 4650 6200
Wire Wire Line
	4500 5700 4650 5700
$Comp
L CONN_1 P5
U 1 1 56267627
P 1500 1450
F 0 "P5" H 1580 1450 40  0000 L CNN
F 1 "CONN_1" H 1500 1505 30  0001 C CNN
F 2 "Wire_Pads:SolderWirePad_single_2-5mmDrill" H 1500 1450 60  0001 C CNN
F 3 "" H 1500 1450 60  0000 C CNN
	1    1500 1450
	1    0    0    -1  
$EndComp
Text GLabel 1250 1450 0    60   Input ~ 0
PE
Wire Wire Line
	1250 1450 1350 1450
Wire Wire Line
	1500 1800 1600 1800
Wire Wire Line
	1600 1800 1600 1750
$Comp
L FINDER-40.52 RL2
U 1 1 56268E4F
P 3000 3050
F 0 "RL2" H 2650 3300 40  0000 C CNN
F 1 "FINDER-40.52" V 3845 3055 40  0000 C CNN
F 2 "Relays_ThroughHole:Relay_DPDT_Schrack-RT2_RM5mm" H 3000 3050 60  0001 C CNN
F 3 "" H 3000 3050 60  0000 C CNN
	1    3000 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 2750 3700 2700
Connection ~ 3350 2700
$Comp
L FINDER-40.52 RL1
U 1 1 562691A0
P 1650 5300
F 0 "RL1" H 1300 5550 40  0000 C CNN
F 1 "FINDER-40.52" V 2495 5305 40  0000 C CNN
F 2 "Relays_ThroughHole:Relay_DPDT_Schrack-RT2_RM5mm" H 1650 5300 60  0001 C CNN
F 3 "" H 1650 5300 60  0000 C CNN
	1    1650 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 5000 1450 4850
Wire Wire Line
	1450 5750 1450 5600
Connection ~ 2250 5600
Wire Wire Line
	2000 5000 2000 4950
Wire Wire Line
	2000 4950 2400 4950
Wire Wire Line
	2400 4950 2400 5000
Connection ~ 2400 5000
Text GLabel 3600 3450 3    60   Input ~ 0
Neutral
Wire Wire Line
	3600 3450 3600 3350
Text GLabel 3700 2700 1    60   Input ~ 0
Neutral_Switched
Text GLabel 1450 5750 0    60   Input ~ 0
Neutral
Text GLabel 7000 3950 0    60   Input ~ 0
Neutral_Switched
$EndSCHEMATC
