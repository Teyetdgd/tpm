LUMIA_SDK ?= ../sdk/toolchain
APPS := lumia-dashboard lumia-control-center pocket-notes wave-deck nexus-hub notif-center

.PHONY: all checksums clean $(APPS)

all: checksums

$(APPS):
	$(MAKE) -C $@ LUMIA_SDK=$(abspath $(LUMIA_SDK))

checksums: $(APPS)
	sha256sum lumia-dashboard/build/lumia-dashboard.lxe | awk '{print $$1}' > lumia-dashboard/build/lumia-dashboard.lxe.sha256
	sha256sum lumia-control-center/build/lumia-control-center.lxe | awk '{print $$1}' > lumia-control-center/build/lumia-control-center.lxe.sha256
	sha256sum pocket-notes/build/pocket-notes.lxe | awk '{print $$1}' > pocket-notes/build/pocket-notes.lxe.sha256
	sha256sum wave-deck/build/wave-deck.lxe | awk '{print $$1}' > wave-deck/build/wave-deck.lxe.sha256
	sha256sum nexus-hub/build/nexus-hub.lxe | awk '{print $$1}' > nexus-hub/build/nexus-hub.lxe.sha256
	sha256sum notif-center/build/notif-center.lxe | awk '{print $$1}' > notif-center/build/notif-center.lxe.sha256

clean:
	@for app in $(APPS); do $(MAKE) -C $$app clean; done

