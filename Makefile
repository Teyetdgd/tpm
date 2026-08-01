LUMIA_SDK ?= ../sdk/toolchain
APPS := lumia-dashboard pocket-notes wave-deck

.PHONY: all clean $(APPS)

all: $(APPS)

$(APPS):
	$(MAKE) -C $@ LUMIA_SDK=$(abspath $(LUMIA_SDK))

clean:
	@for app in $(APPS); do $(MAKE) -C $$app clean; done
