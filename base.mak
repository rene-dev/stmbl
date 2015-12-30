# Link: create ELF output file from object files
#
$(TARGET).elf: $(OBJECTS) $(LDSCRIPT)
	@echo
	@echo Linking: $@
	@$(MKDIR) -p $(dir $@)
	$(CC) $(OBJECTS) $(LDFLAGS) --output $(basename $@).tmp

	@echo
	@echo Post-processing: $@
	$(POSTLD) $(basename $@).tmp $@

# Create extended listing file from ELF output file
#
%.lss: %.elf
	@echo
	@echo Creating Extended Listing: $@
	@$(MKDIR) -p $(dir $@)
	$(OBJDUMP) -h -S -z $< > $@

# Create a symbol table from ELF output file
#
%.sym: %.elf
	@echo
	@echo Creating Symbol Table: $@
	@$(MKDIR) -p $(dir $@)
	$(NM) -n $< > $@

# Create final output files from ELF output file.
#
%.hex: %.elf
	@echo
	@echo Creating hex file: $@
	@$(MKDIR) -p $(dir $@)
	$(OBJCOPY) -O ihex $< $@

# Create binary output file from ELF output file.
#
%.bin: %.elf
	@echo
	@echo Creating bin file: $@
	@$(MKDIR) -p $(dir $@)
	$(OBJCOPY) -O binary --gap-fill 0xFF $< $@

# Compile: create object files from C source files
#
$(OBJDIR)/%.o : %.c
	@echo
	@echo Compiling C: $<
	@$(MKDIR) -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 

$(OBJDIR)/%.o : %.cpp
	@echo
	@echo Compiling C++: $<
	@$(MKDIR) -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $(GENDEPFLAGS) $< -o $@ 

# Assemble: create object files from assembler source files
#
$(OBJDIR)/%.o : %.s
	@echo
	@echo Assembling: $<
	@$(MKDIR) -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@
