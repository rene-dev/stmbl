#OPT = -O1

HWVERSION = v4


#build f4 firmware
f4: force_look
	$(MAKE) -C f4/ 

#build f4 bootloader
f4_boot:
	$(MAKE) -C f4_boot/ 

#flash f4 bootloader using stlink
f4_boot_flash:
	$(MAKE) -C bootloader/ flash

#flash f4 bootloader using df-util
f4_boot_btburn:
	$(MAKE) -C bootloader/ btburn

#flash f4 firmware using stlink
f4_flash:
	$(MAKE) -C f4/ flash

#flash f4 firmware using df-util
f4_btburn:
	$(MAKE) -C f4/ btburn

#flash f4 firmware + bootloader using df-util
f4_all_btburn:
	$(MAKE) -C f4/ all_btburn

f4_all_flash:
	$(MAKE) -C f4/ all_flash

#build f3 firmware
f3:
	$(MAKE) -C f3/ 

#build f3 bootloader
f3_boot:
	$(MAKE) -C f3_boot/ 

#flash f3 bootloader using stlink
f3_boot_flash:
	$(MAKE) -C f3_boot/ flash

#flash f3 bootloader using df-util
f3_boot_btburn:
	$(MAKE) -C f3_boot/ btburn

#flash f3 firmware using stlink
f3_flash:
	$(MAKE) -C f3/ flash

#flash f3 firmware using df-util
f3_btburn:
	$(MAKE) -C f3/ btburn

f3_all_btburn:
	$(MAKE) -C f3/ all_btburn

f3_all_flash:
	$(MAKE) -C f3/ all_flash

#build f1 firmware
f1: force_look
	$(MAKE) -C f1/ 

#flash f1 firmware for V3 hardware using stlink
f1_flash:
	$(MAKE) -C f1/ flash

all:
	$(MAKE) -C f4/
	$(MAKE) -C f3/
	#$(MAKE) -C f1/
	#$(MAKE) -C f4_boot/
	#$(MAKE) -C f3_boot/



# Target: clean project
#
clean:
	@echo Cleaning project:
	rm -rf build
	#@$(MAKE) -C f4_boot/ clean
	#@$(MAKE) -C f3_boot/ clean
	@$(MAKE) -C f4/ clean
	@$(MAKE) -C f3/ clean
	#@$(MAKE) -C f1/ clean

force_look:
	@true
