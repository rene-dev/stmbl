#OPT = -O1

HWVERSION = v4

F1_ADDRESS = 0x08000000
F3_ADDRESS = 0x08004000
F4_ADDRESS = 0x08010000

#build f4 firmware
f4: force_look
	$(MAKE) -C f4/ ADDRESS=$(F4_ADDRESS)

#build f4 bootloader
f4_boot: force_look
	$(MAKE) -C f4_boot/ APP_ADDRESS=$(F4_ADDRESS)

#flash f4 bootloader using stlink
f4_boot_flash: force_look
	$(MAKE) -C f4_boot/ APP_ADDRESS=$(F4_ADDRESS) flash

#flash f4 bootloader using df-util
f4_boot_btburn: force_look
	$(MAKE) -C f4_boot/ APP_ADDRESS=$(F4_ADDRESS) btburn

#flash f4 firmware using stlink
f4_flash: force_look
	$(MAKE) -C f4/ ADDRESS=$(F4_ADDRESS) flash

#flash f4 firmware using df-util
f4_btburn: force_look
	$(MAKE) -C f4/ ADDRESS=$(F4_ADDRESS) btburn

#flash f4 firmware + bootloader using df-util
f4_all_btburn: force_look
	$(MAKE) -C f4/ ADDRESS=$(F4_ADDRESS) all_btburn

f4_all_flash: force_look
	$(MAKE) -C f4/ ADDRESS=$(F4_ADDRESS) all_flash

#build f3 firmware
f3: force_look
	$(MAKE) -C f3/ ADDRESS=$(F3_ADDRESS)

#build f3 bootloader
f3_boot: force_look
	$(MAKE) -C f3_boot/ APP_ADDRESS=$(F3_ADDRESS)

#flash f3 bootloader using stlink
f3_boot_flash: force_look
	$(MAKE) -C f3_boot/ APP_ADDRESS=$(F3_ADDRESS) flash 

#flash f3 bootloader using df-util
f3_boot_btburn: force_look
	$(MAKE) -C f3_boot/ APP_ADDRESS=$(F3_ADDRESS) btburn

#flash f3 firmware using stlink
f3_flash: force_look
	$(MAKE) -C f3/ ADDRESS=$(F3_ADDRESS) flash

#flash f3 firmware using df-util
f3_btburn: force_look
	$(MAKE) -C f3/ ADDRESS=$(F3_ADDRESS) btburn

f3_all_btburn: force_look
	$(MAKE) -C f3/ ADDRESS=$(F3_ADDRESS) all_btburn

f3_all_flash: force_look
	$(MAKE) -C f3/ ADDRESS=$(F3_ADDRESS) all_flash

#build f1 firmware
f1: force_look
	$(MAKE) -C f1/  ADDRESS=$(F1_ADDRESS)

#flash f1 firmware for V3 hardware using stlink
f1_flash: force_look
	$(MAKE) -C f1/  ADDRESS=$(F1_ADDRESS) flash

all: f4
	#$(MAKE) -C f4/ ADDRESS=$(F4_ADDRESS)
	#$(MAKE) -C f3/ ADDRESS=$(F3_ADDRESS)
	#$(MAKE) -C f1/
	#$(MAKE) -C f4_boot/
	#$(MAKE) -C f3_boot/



# Target: clean project
#
clean: force_look
	@echo Cleaning project:
	rm -rf build
	#@$(MAKE) -C f4_boot/ clean
	#@$(MAKE) -C f3_boot/ clean
	@$(MAKE) -C f4/ clean
	@$(MAKE) -C f3/ clean
	#@$(MAKE) -C f1/ clean

force_look:
	@true
