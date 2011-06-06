define doinsubdirs
$(foreach d,$(1),${MAKE} -C ${d} $(2) $@;)
endef

SUBDIRS = basic graph storage string

.PHONY: all check clean

all check clean:
	$(call doinsubdirs,${SUBDIRS})

