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
Sheet 1 5
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
S 8300 2100 1500 950 
U 5742F569
F0 "stm32" 60
F1 "stm32.sch" 60
F2 "MOSI_stm" I L 8300 2300 60 
F3 "MISO_stm" I L 8300 2400 60 
F4 "SCK_stm" I L 8300 2500 60 
$EndSheet
$Sheet
S 8400 3750 1300 1000
U 5742FAA4
F0 "sserial" 60
F1 "sserial.sch" 60
F2 "ESTOP" I L 8400 3850 60 
F3 "GNDstm" I L 8400 3950 60 
F4 "SSerialOUTstm" I L 8400 4050 60 
F5 "SSerialINstm" I L 8400 4150 60 
F6 "VCCstm" I L 8400 4250 60 
$EndSheet
$Sheet
S 6550 2100 1500 1300
U 5744969F
F0 "output" 60
F1 "output.sch" 60
F2 "MOSI_out" I R 8050 2300 60 
F3 "MISO_out" I R 8050 2400 60 
F4 "SCK_out" I R 8050 2500 60 
$EndSheet
$Sheet
S 4050 1700 1250 550 
U 5744F206
F0 "led" 60
F1 "led.sch" 60
$EndSheet
Wire Wire Line
	8050 2300 8300 2300
Wire Wire Line
	8050 2400 8300 2400
Wire Wire Line
	8050 2500 8300 2500
$EndSCHEMATC
