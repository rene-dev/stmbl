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
L stmbl:heatsink U33
U 1 1 5B2DE6B6
P 4500 3850
F 0 "U33" H 4778 3853 60  0000 L CNN
F 1 "heatsink" H 4778 3747 60  0000 L CNN
F 2 "" H 4500 3850 60  0001 C CNN
F 3 "" H 4500 3850 60  0001 C CNN
F 4 "V PR138/94-M3" H 4500 3850 50  0001 C CNN "Manufacturer No"
F 5 " " H 4500 3850 50  0001 C CNN "Voltage"
F 6 " " H 4500 3850 50  0001 C CNN "Source"
F 7 " " H 4500 3850 50  0001 C CNN "Tolerance"
F 8 "" H 0   0   50  0001 C CNN "Description"
F 9 "" H 0   0   50  0001 C CNN "InternalName"
F 10 "alutronic" H 0   0   50  0001 C CNN "Manufacturer"
	1    4500 3850
	1    0    0    -1  
$EndComp
$Comp
L stmbl:fan U34
U 1 1 5B2DE72A
P 5600 3900
F 0 "U34" H 5828 3953 60  0000 L CNN
F 1 "fan" H 5828 3847 60  0000 L CNN
F 2 "" H 5600 3900 60  0001 C CNN
F 3 "" H 5600 3900 60  0001 C CNN
F 4 "MB45101V2-000U-A99" H 5600 3900 50  0001 C CNN "Manufacturer No"
F 5 "12V" H 5600 3900 50  0001 C CNN "Voltage"
F 6 " " H 5600 3900 50  0001 C CNN "Source"
F 7 " " H 5600 3900 50  0001 C CNN "Tolerance"
F 8 "" H 0   0   50  0001 C CNN "Description"
F 9 "" H 0   0   50  0001 C CNN "InternalName"
F 10 "Sunon" H 0   0   50  0001 C CNN "Manufacturer"
	1    5600 3900
	1    0    0    -1  
$EndComp
$EndSCHEMATC
