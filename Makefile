LUMIA_SDK ?= ../sdk/toolchain
APPS := event-loop-lab frame-lab lumia-dashboard lumia-control-center \
	pocket-notes wave-deck nexus-hub notif-center sdk-abi-test

.PHONY: all checksums metadata verify clean $(APPS)

all: metadata

$(APPS):
	$(MAKE) -C $@ LUMIA_SDK=$(abspath $(LUMIA_SDK))

checksums: $(APPS)
	@for app in $(APPS); do \
		sha256sum "$$app/build/$$app.lxe" | awk '{print $$1}' > "$$app/build/$$app.lxe.sha256"; \
	done

metadata: checksums
	python3 ../scripts/tools/sync_mikasa_packages.py

verify:
	python3 ../scripts/tools/sync_mikasa_packages.py --check

clean:
	@for app in $(APPS); do $(MAKE) -C $$app clean; done
