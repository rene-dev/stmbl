ifneq (,$(findstring HAL, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/hal
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  INCDIRS += build/gen/inc/comps
  INCDIRS += build/gen/inc/src_comps/
  # INCDIRS += build/gen/inc/shared_comps/
  # INCDIRS += build/gen/inc/lib_comps/

  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
  GENSOURCES += build/gen/src/hal_tbl.c

  OBJECTS += $(SRC_COMP_OBJECTS) $(SHARED_COMP_OBJECTS) $(LIB_COMP_OBJECTS)

  #SOURCES += $(SRC_COMPS) $(SHARED_COMPS) $(LIB_COMPS)
endif


SRC_COMP_OBJECTS = $(subst src/comps/,,$(addprefix build/src_comps/,$(addsuffix .o,$(basename $(SRC_COMPS)))))
SHARED_COMP_OBJECTS = $(subst ../shared/src/comps/,,$(addprefix build/shared_comps/,$(addsuffix .o,$(basename $(SHARED_COMPS)))))
LIB_COMP_OBJECTS = $(subst ../../framework/comps/,,$(addprefix build/lib_comps/,$(addsuffix .o,$(basename $(LIB_COMPS)))))
COMPS = $(SRC_COMPS) $(SHARED_COMPS) $(LIB_COMPS)

INCDIRS += $(subst ../shared/src/comps/,,$(addprefix build/gen/inc/shared_comps/,$(dir $(SHARED_COMPS))))
INCDIRS += $(subst ../../framework/comps/,,$(addprefix build/gen/inc/lib_comps/,$(dir $(LIB_COMPS))))

build/gen/src/hal_tbl.c: $(FRAMEWORK_DIR)/tools/create_hal_tbl.py $(COMPS)
	@echo Generating HAL table
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_hal_tbl.py $@ $(COMPS)

$(SRC_COMP_OBJECTS): build/src_comps/%.o: build/gen/inc/src_comps/%_comp.h
$(SHARED_COMP_OBJECTS): build/shared_comps/%.o: build/gen/inc/shared_comps/%_comp.h
$(LIB_COMP_OBJECTS): build/lib_comps/%.o: build/gen/inc/lib_comps/%_comp.h

build/gen/inc/src_comps/%_comp.h: src/comps/%.c
	@echo Generating src H: $<
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_comp_h.py $@ $<

build/gen/inc/shared_comps/%_comp.h: ../shared/src/comps/%.c
	@echo Generating shared H: $<
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_comp_h.py $@ $<

build/gen/inc/lib_comps/%_comp.h: ../../framework/comps/%.c
	@echo Generating lib H: $<
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_comp_h.py $@ $<


build/src_comps/%.o : src/comps/%.c
	@echo Compiling src C: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 

build/lib_comps/%.o : ../../framework/comps/%.c
	@echo Compiling lib C: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 

build/shared_comps/%.o : ../shared/src/comps/%.c
	@echo Compiling shared C: $<
	@$(MKDIR) -p $(dir $@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) $(GENDEPFLAGS) $< -o $@ 