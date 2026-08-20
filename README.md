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
- `lumia-control-center`: multi-panel operations dashboard with a 2 MiB
  diagnostics dataset for Mikasa transfer and SDK integration testing.
- `pocket-notes`: GUI text editor with VFS persistence and clipboard actions.
- `wave-deck`: GUI WAV player with transport controls, seek, and volume.

Each project includes an `app.mikasa` JSON manifest describing its future
Mikasa package metadata and requested permissions.

For a Mikasa repository, keep `mikasa-index.json` at the repository root and
place each package in its own directory. The package artifact and checksum use
the conventional paths `<package>/build/<package>.lxe` and
`<package>/build/<package>.lxe.sha256`. Running `make` builds all three apps and
regenerates their checksum sidecars.

