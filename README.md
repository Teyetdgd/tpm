# LumiaSDK Test Applications

This directory contains standalone Ring 3 applications built exclusively with
the public LumiaSDK API. Each child directory is intentionally self-contained
so it can be copied into its own GitHub repository and later published through
Mikasa.

Set `LUMIA_SDK` to the LumiaSDK toolchain directory and build all applications:

```bash
make LUMIA_SDK=/path/to/Lumia-OS/sdk/toolchain
```

The current applications are:

- `lumia-dashboard`: GUI system clock, timers, notifications, clipboard, and events.
- `pocket-notes`: GUI text editor with VFS persistence and clipboard actions.
- `wave-deck`: GUI WAV player with transport controls, seek, and volume.

Each project includes an `app.mikasa` JSON manifest describing its future
Mikasa package metadata and requested permissions.
