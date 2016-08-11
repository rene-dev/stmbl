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
LIBS:stm32
LIBS:ksz8081
LIBS:ESD_Protection
LIBS:hr911105_rj45
LIBS:ethernet_bob-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 3
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
L STM32F407VETx U2
U 1 1 57A8F89A
P 5800 3700
F 0 "U2" H 5800 714 50  0000 C CNN
F 1 "STM32F407VETx" H 5800 623 50  0000 C CNN
F 2 "Housings_QFP:LQFP-100_14x14mm_Pitch0.5mm" H 5800 532 50  0000 C CNN
F 3 "" H 5800 3700 50  0000 C CNN
	1    5800 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 6600 6000 6600
Wire Wire Line
	5500 700  6100 700 
Text HLabel 10400 1900 2    60   Input ~ 0
STM_RXDV
Text HLabel 10400 1500 2    60   Input ~ 0
STM_COL
Text HLabel 10400 1400 2    60   Input ~ 0
STM_MDIO
Text HLabel 10400 3900 2    60   Input ~ 0
STM_RXER
Text HLabel 10400 4000 2    60   Input ~ 0
STM_TXEN
Text HLabel 10400 4200 2    60   Input ~ 0
STM_TXD1
Text HLabel 10400 1200 2    60   Input ~ 0
STM_CRS
Text HLabel 10400 2900 2    60   Input ~ 0
STM_RXD2
Text HLabel 10400 3000 2    60   Input ~ 0
STM_RXD3
Text HLabel 1250 3100 0    60   Input ~ 0
STM_TXD3
Text HLabel 10400 5000 2    60   Input ~ 0
STM_RXD0
Text HLabel 10400 5100 2    60   Input ~ 0
STM_RXD1
Text HLabel 10400 4100 2    60   Input ~ 0
STM_TXD0
Wire Wire Line
	10300 1200 10400 1200
Wire Wire Line
	10300 1400 10400 1400
Wire Wire Line
	10300 1500 10400 1500
Wire Wire Line
	10300 1900 10400 1900
Wire Wire Line
	10300 2900 10400 2900
Wire Wire Line
	10300 3000 10400 3000
Wire Wire Line
	10300 3900 10400 3900
Wire Wire Line
	10300 4000 10400 4000
Wire Wire Line
	10300 4100 10400 4100
Wire Wire Line
	10300 4200 10400 4200
Wire Wire Line
	10300 5000 10400 5000
Wire Wire Line
	10300 5100 10400 5100
Text HLabel 10400 4800 2    60   Input ~ 0
STM_TXD2
Wire Wire Line
	10300 4800 10400 4800
$Comp
L USB_OTG P1
U 1 1 57A96FED
P 1300 7350
F 0 "P1" V 1423 7538 50  0000 L CNN
F 1 "USB_OTG" V 1332 7538 50  0000 L CNN
F 2 "Connect:USB_Micro-B" V 1250 7250 50  0001 C CNN
F 3 "" V 1250 7250 50  0000 C CNN
	1    1300 7350
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1250 3100 1300 3100
Text HLabel 10400 4700 2    60   Input ~ 0
STM_MDC
Wire Wire Line
	10300 4700 10400 4700
$EndSCHEMATC
