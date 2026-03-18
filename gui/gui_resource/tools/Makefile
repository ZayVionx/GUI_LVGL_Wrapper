PYTHON ?= python
RESOURCE_SCRIPT := resource_gen.py

.PHONY: all font img resource

all: font img

font:
	@"$(PYTHON)" "$(RESOURCE_SCRIPT)" --font

img:
	@"$(PYTHON)" "$(RESOURCE_SCRIPT)" --img

resource:
	@echo "[resource] target completed"
