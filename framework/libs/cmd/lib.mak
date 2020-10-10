ifneq (,$(findstring CMD, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/cmd
  $(info using LIB: $(LIBPATH))

  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
  GENINCS += build/gen/inc/commandslist.h
endif

build/gen/inc/commandslist.h: $(FRAMEWORK_DIR)/tools/create_cmd.py $(SOURCES)
	@echo Generating commands list
	@$(MKDIR) -p $(dir $@)
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_cmd.py $@ $(SOURCES) $(LIBSOURCES) $(SHAREDSOURCES)