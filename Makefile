LUMIA_SDK ?= ../sdk/toolchain
APPS := lumia-dashboard pocket-notes wave-deck

.PHONY: all checksums clean $(APPS)

all: checksums

$(APPS):
	$(MAKE) -C $@ LUMIA_SDK=$(abspath $(LUMIA_SDK))

checksums: $(APPS)
	sha256sum lumia-dashboard/build/lumia-dashboard.lxe | awk '{print $$1}' > lumia-dashboard/build/lumia-dashboard.lxe.sha256
	sha256sum pocket-notes/build/pocket-notes.lxe | awk '{print $$1}' > pocket-notes/build/pocket-notes.lxe.sha256
	sha256sum wave-deck/build/wave-deck.lxe | awk '{print $$1}' > wave-deck/build/wave-deck.lxe.sha256

clean:
	@for app in $(APPS); do $(MAKE) -C $$app clean; done
