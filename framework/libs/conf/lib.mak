ifneq (,$(findstring CONF, $(LIBS)))
# lib path
  LIBPATH = $(FRAMEWORK_DIR)/libs/conf
  $(info using LIB: $(LIBPATH))
	
  INCDIRS := $(INCDIRS) $(LIBPATH)/inc
  
  LIBSOURCES := $(LIBSOURCES) $(wildcard $(LIBPATH)/src/*.c)
  GENSOURCES += build/gen/src/conf_templates.c
endif

build/gen/src/conf_templates.c: $(FRAMEWORK_DIR)/tools/create_config.py $(CONFIG_TEMPLATES)
	@echo Generating config
	@$(MKDIR) -p $(dir $@)
	@touch $@
	@$(PYTHON) $(FRAMEWORK_DIR)/tools/create_config.py $@ $(CONFIG_TEMPLATES)