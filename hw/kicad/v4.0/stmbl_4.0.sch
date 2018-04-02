EESchema Schematic File Version 4
LIBS:stmbl_4.0-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 14
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
S 5500 2300 600  500 
U 5659094D
F0 "hv" 60
F1 "hv.sch" 60
F2 "TX" I L 5500 2600 60 
F3 "RX" I L 5500 2500 60 
$EndSheet
$Sheet
S 4300 2300 650  500 
U 56590966
F0 "ctrl" 60
F1 "ctrl.sch" 60
F2 "TX" I R 4950 2600 60 
F3 "RX" I R 4950 2500 60 
$EndSheet
Wire Wire Line
	4950 2500 5500 2500
Wire Wire Line
	4950 2600 5500 2600
$Comp
L stmbl1:non_plated U33
U 1 1 5AC37C2D
P 4600 4100
F 0 "U33" H 4600 3750 60  0000 C CNN
F 1 "non_plated" H 4650 4350 60  0000 C CNN
F 2 "stmbl:non_plated_1mm" H 4600 4100 60  0001 C CNN
F 3 "" H 4600 4100 60  0001 C CNN
	1    4600 4100
	1    0    0    -1  
$EndComp
$Comp
L stmbl1:non_plated U35
U 1 1 5AC37C5C
P 5200 4100
F 0 "U35" H 5200 3750 60  0000 C CNN
F 1 "non_plated" H 5250 4350 60  0000 C CNN
F 2 "stmbl:non_plated_1mm" H 5200 4100 60  0001 C CNN
F 3 "" H 5200 4100 60  0001 C CNN
	1    5200 4100
	1    0    0    -1  
$EndComp
$Comp
L stmbl1:non_plated U34
U 1 1 5AC37CB2
P 4600 4800
F 0 "U34" H 4600 4450 60  0000 C CNN
F 1 "non_plated" H 4650 5050 60  0000 C CNN
F 2 "stmbl:non_plated_1mm" H 4600 4800 60  0001 C CNN
F 3 "" H 4600 4800 60  0001 C CNN
	1    4600 4800
	1    0    0    -1  
$EndComp
$Comp
L stmbl1:non_plated U36
U 1 1 5AC37CB8
P 5200 4800
F 0 "U36" H 5200 4450 60  0000 C CNN
F 1 "non_plated" H 5250 5050 60  0000 C CNN
F 2 "stmbl:non_plated_1mm" H 5200 4800 60  0001 C CNN
F 3 "" H 5200 4800 60  0001 C CNN
	1    5200 4800
	1    0    0    -1  
$EndComp
$Comp
L stmbl1:non_plated U37
U 1 1 5AC37D5D
P 5750 4100
F 0 "U37" H 5750 3750 60  0000 C CNN
F 1 "non_plated" H 5800 4350 60  0000 C CNN
F 2 "stmbl:non_plated_1mm" H 5750 4100 60  0001 C CNN
F 3 "" H 5750 4100 60  0001 C CNN
	1    5750 4100
	1    0    0    -1  
$EndComp
$Comp
L stmbl1:non_plated U39
U 1 1 5AC37D64
P 5750 4800
F 0 "U39" H 5750 4450 60  0000 C CNN
F 1 "non_plated" H 5800 5050 60  0000 C CNN
F 2 "stmbl:non_plated_1mm" H 5750 4800 60  0001 C CNN
F 3 "" H 5750 4800 60  0001 C CNN
	1    5750 4800
	1    0    0    -1  
$EndComp
$EndSCHEMATC
