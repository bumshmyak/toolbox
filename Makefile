define doinsubdirs
$(foreach d,$(1),$(MAKE) -C $(d) $(2) $@;)
endef

SUBDIRS = basic graph storage string math

.PHONY: all check clean googletest

all check clean:
	$(call doinsubdirs,${SUBDIRS})

googletest:
	$(MAKE) -C googletest/make
