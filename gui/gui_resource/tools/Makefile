PYTHON ?= python
RESOURCE_SCRIPT := resource_gen.py
RESOURCE_FLAGS ?=

.PHONY: all font img resource

all:
	@"$(PYTHON)" "$(RESOURCE_SCRIPT)" $(RESOURCE_FLAGS)

font:
	@"$(PYTHON)" "$(RESOURCE_SCRIPT)" --font $(RESOURCE_FLAGS)

img:
	@"$(PYTHON)" "$(RESOURCE_SCRIPT)" --img $(RESOURCE_FLAGS)

resource:
	@"$(PYTHON)" "$(RESOURCE_SCRIPT)" $(RESOURCE_FLAGS)
