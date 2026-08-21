# Event Loop Lab

An interactive Ring 3 test for the LumiaSDK central event queue and
`lumia_wait_event`. The application owns its scheduler context and deliberately
does not register a compositor tick callback.

Build with `make`, publish the directory in a Mikasa repository, then install
and launch `event-loop-lab.lxe`. The screen should update once per second while
the process sleeps between timer events. Mouse, keyboard, focus, move, and
resize activity increments the matching counters.

Press `T` to stop the event timer. With no input, a three-second wait should
expire and increase the timeout counter. Press `T` again to restore the
one-second repeating timer, `R` to reset counters, or `Q` to exit.

