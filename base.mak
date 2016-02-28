# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q     := @
NULL     := 2>/dev/null
endif

# Link: create ELF output file from object files
#
$(TARGET).elf: $(OBJECTS) $(LDSCRIPT)
	@echo Linking: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) $(OBJECTS) $(LDFLAGS) --output $(basename $@).tmp

	@echo Post-processing: $@
	$(Q)$(POSTLD) $(basename $@).tmp $@

# Create extended listing file from ELF output file
#
%.lss: %.elf
	@echo Creating Extended Listing: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file
#
%.sym: %.elf
	@echo Creating Symbol Table: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(NM) -n $< > $@

# Create final output files from ELF output file.
#
%.hex: %.elf
	@echo Creating hex file: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(OBJCOPY) -O ihex $< $@

# Create binary output file from ELF output file.
#
%.bin: %.elf
	@echo Creating bin file: $@
	@$(MKDIR) -p $(dir $@)
	$(Q)$(OBJCOPY) -O binary --gap-fill 0xFF $< $@

# Compile: create object files from C source files
#
$(OBJDIR)/%.o : %.c
	@echo Compiling C: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 

$(OBJDIR)/%.o : %.cpp
	@echo Compiling C++: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $(GENDEPFLAGS) $< -o $@ 

# Assemble: create object files from assembler source files
#
$(OBJDIR)/%.o : %.s
	@echo Assembling: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@
