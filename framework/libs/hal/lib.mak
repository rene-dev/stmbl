ifneq (,$(findstring HAL, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/hal
  $(info using LIB: $(LIBPATH))
  
  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  INCDIRS += build/gen/inc/comps
  INCDIRS += build/gen/inc/shared_comps

  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
  GENSOURCES += build/gen/src/hal_tbl.c
endif

SRC_COMP_OBJECTS = $(addprefix build/,$(addsuffix .o,$(basename $(SRC_COMPS))))
#SHARED_COMP_OBJECTS = $(addprefix build/shared/comps/,$(addsuffix .o,$(basename $(SHARED_COMPS))))
SHARED_COMP_OBJECTS = $(addprefix build/shared/comps/,$(addsuffix .o,$(basename $(notdir $(SHARED_COMPS)))))

build/gen/src/hal_tbl.c: $(FRAMEWORK_DIR)/tools/create_hal_tbl.py $(COMPS)
	@echo Generating HAL table
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_hal_tbl.py $@ $(COMPS)

$(SRC_COMP_OBJECTS): build/src/comps/%.o: build/gen/inc/comps/%_comp.h

$(SHARED_COMP_OBJECTS): build/shared/comps/%.o: build/gen/inc/shared_comps/%_comp.h

build/gen/inc/comps/%_comp.h: src/comps/%.c
	@echo Generating H: $<
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_comp_h.py $@ $<

build/gen/inc/shared_comps/%_comp.h: shared/comps/%.c
	@echo Generating H: $<
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_comp_h.py $@ $<
