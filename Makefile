SUBDIRS := $(wildcard */.)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) clean -C $@
	
.PHONY: all $(SUBDIRS)
