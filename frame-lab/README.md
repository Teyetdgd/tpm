# LumiaSDK Frame Lab

Frame Lab is a temporary Ring 3 application for manually inspecting LumiaSDK
transactional rendering. It animates several independently drawn regions while
publishing each completed canvas through `lumia_gui_begin_frame()` and
`lumia_gui_end_frame()`.

Move, resize, cover, uncover, minimize, maximize, and restore the window. The
application intentionally gives no pass/fail verdict. Inspect whether every
visible update is complete and whether the old window position leaves trails.

