# Make all libraries
export

CFLAGS := $(patsubst -I%,-I../../%,$(CFLAGS))

SUBDIRS := $(shell ls -F|grep /|grep -v CVS)

.PHONY: subdirs $(SUBDIRS)
.PHONY: clean

subdirs: ${SUBDIRS}

${SUBDIRS}:
	${MAKE} -C $@

clean:
	find . -name *.a -delete
	find . -name *.o -delete
