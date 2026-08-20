LUMIA_SDK ?= ../sdk/toolchain
APPS := event-loop-lab frame-lab lumia-dashboard lumia-control-center \
	pocket-notes wave-deck nexus-hub notif-center sdk-abi-test

.PHONY: all checksums clean $(APPS)

all: checksums

$(APPS):
	$(MAKE) -C $@ LUMIA_SDK=$(abspath $(LUMIA_SDK))

checksums: $(APPS)
	@for app in $(APPS); do \
		sha256sum "$$app/build/$$app.lxe" | awk '{print $$1}' > "$$app/build/$$app.lxe.sha256"; \
	done

clean:
	@for app in $(APPS); do $(MAKE) -C $$app clean; done
