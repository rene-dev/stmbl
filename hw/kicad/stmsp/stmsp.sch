EESchema Schematic File Version 2
LIBS:stmsp-rescue
LIBS:transistors
LIBS:stm32
LIBS:conn
LIBS:power
LIBS:74xgxx
LIBS:74xx
LIBS:ac-dc
LIBS:actel
LIBS:adc-dac
LIBS:Altera
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:brooktre
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:device
LIBS:digital-audio
LIBS:diode
LIBS:display
LIBS:dsp
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:ftdi
LIBS:gennum
LIBS:graphic
LIBS:hc11
LIBS:intel
LIBS:interface
LIBS:ir
LIBS:Lattice
LIBS:linear
LIBS:logo
LIBS:maxim
LIBS:memory
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic32mcu
LIBS:microchip
LIBS:microcontrollers
LIBS:motor_drivers
LIBS:motorola
LIBS:msp430
LIBS:nordicsemi
LIBS:nxp_armmcu
LIBS:onsemi
LIBS:opto
LIBS:Oscillators
LIBS:philips
LIBS:Power_Management
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:relays
LIBS:rfcom
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:stm8
LIBS:supertex
LIBS:switches
LIBS:texas
LIBS:transf
LIBS:ttl_ieee
LIBS:valves
LIBS:video
LIBS:Xicor
LIBS:xilinx
LIBS:Zilog
LIBS:stmbl
LIBS:stmsp-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 7
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
S 8250 3900 1350 800 
U 5915CECC
F0 "f3" 60
F1 "f303.sch" 60
$EndSheet
$Sheet
S 5550 2600 1000 850 
U 5918AB0C
F0 "drv" 60
F1 "drv.sch" 60
F2 "SHUNT_A" I R 6550 2700 60 
F3 "SHUNT_B" I R 6550 2800 60 
F4 "BEMF_2A" I R 6550 3350 60 
F5 "BEMF_1A" I R 6550 3250 60 
F6 "DCLINK" I R 6550 3150 60 
F7 "IN1A" I L 5550 2800 60 
F8 "IN2A" I L 5550 2900 60 
F9 "IN1B" I L 5550 3200 60 
F10 "IN2B" I L 5550 3300 60 
F11 "ENB" I L 5550 3100 60 
F12 "ENA" I L 5550 2700 60 
F13 "SHUNT_LOW" I R 6550 2900 60 
$EndSheet
$Sheet
S 7950 2650 1150 700 
U 5918AB94
F0 "supply" 60
F1 "supply.sch" 60
$EndSheet
$Sheet
S 6300 5350 550  450 
U 5918AD6D
F0 "sensor" 60
F1 "sensor.sch" 60
F2 "CSQ" I R 6850 5500 60 
F3 "SCK" I R 6850 5600 60 
F4 "DATA" I R 6850 5700 60 
$EndSheet
$Sheet
S 8300 5100 700  600 
U 5918ADEA
F0 "cmd" 60
F1 "cmd.sch" 60
F2 "DATA0" I L 8300 5300 60 
F3 "EN0" I L 8300 5200 60 
F4 "A0" I R 9000 5200 60 
F5 "B0" I R 9000 5300 60 
F6 "DATA1" I L 8300 5600 60 
F7 "EN1" I L 8300 5500 60 
F8 "A1" I R 9000 5500 60 
F9 "B1" I R 9000 5600 60 
$EndSheet
$Sheet
S 6150 4100 900  400 
U 5918C2F4
F0 "io" 60
F1 "io.sch" 60
$EndSheet
$Comp
L CONN_02X07 P2
U 1 1 5923DCA2
P 3800 4850
F 0 "P2" H 3800 5250 50  0000 C CNN
F 1 "CONN_02X07" V 3800 4850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x07" H 3800 3650 50  0001 C CNN
F 3 "" H 3800 3650 50  0000 C CNN
	1    3800 4850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
