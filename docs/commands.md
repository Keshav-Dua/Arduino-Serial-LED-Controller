# Serial LED Controller - Command Reference

Baud Rate: `9600`
Line Ending: `Newline (\n)`

| Command | Arguments | Example | Description |
| :--- | :--- | :--- | :--- |
| `ON` | `<1-4 \| ALL>` | `ON 2` | Turns on a specific LED or all LEDs. |
| `OFF` | `<1-4 \| ALL>` | `OFF ALL` | Turns off a specific LED or all LEDs. |
| `BLINK` | `<1-4> <ms>` | `BLINK 1 500` | Blinks the specified LED at interval `ms`. |
| `PATTERN` | `CHASE <ms>` | `PATTERN CHASE 200` | Cycles through LEDs 1 to 4 sequentially. |
| `PATTERN` | `ALT <ms>` | `PATTERN ALT 300` | Alternates odd (1,3) and even (2,4) LEDs. |
| `PATTERN` | `OFF` | `PATTERN OFF` | Stops active patterns and turns off LEDs. |
| `STATUS` | None | `STATUS` | Outputs the current state of all LEDs. |
| `HELP` | None | `HELP` | Displays available command syntax. |
