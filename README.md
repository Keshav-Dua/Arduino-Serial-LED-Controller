# Arduino Serial LED Controller

A non-blocking, state-machine based Arduino application to control up to 4 LEDs using simple Serial commands. Features individual control, blinking intervals, state reporting, and automated light patterns (`CHASE` and `ALT`).

---

## Demo
#soon
[![Arduino LED Controller Demo](https://img.youtube.com/vi/YOUR_YOUTUBE_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_YOUTUBE_VIDEO_ID)

---

## Features

* **Non-Blocking Logic:** Built using `millis()` instead of `delay()`, keeping the Serial interface fast and responsive.
* **Flexible LED Control:** Control individual LEDs or targeting all LEDs at once.
* **Custom Blink Speeds:** Configure distinct blinking intervals for individual LEDs in milliseconds.
* **Automated Patterns:**
  * `CHASE`: Cycles through LEDs 1 to 4 sequentially.
  * `ALT`: Alternates between even (2, 4) and odd (1, 3) LEDs.
* **System Status Reporting:** Live telemetry output showing each LED's state and active pattern.

---

## Hardware Setup

Connect your LEDs to pins **4, 5, 6, and 7** using 220Ω current-limiting resistors connected to Ground.

>  *For full wiring tables, schematic diagrams, and simulator files, see [`docs/hardware_config.md`](docs/hardware_config.md) and [`docs/diagram.json`](docs/diagram.json).*

---

##  Serial Commands

Open your Arduino Serial Monitor at **`9600 Baud`** (set line endings to `Newline (\n)`).

sample inputs-
HELP
ON 1
ON ALL
OFF 1
BLINK 2 200
STATUS
PATTERN CHASE 150
PATTERN ALT 300
PATTERN OFF
OFF ALL

>  *For detailed command descriptions, see [`docs/commands.md`](docs/commands.md).*

---

##  Repository Structure

```text
Arduino-Serial-LED-Control/
├── docs/
│   ├── commands.md       # Command documentation
│   ├── diagram.json      # Wokwi circuit simulation file
│   ├── pinout.md         # Full hardware pin assignments
│   ├── simulation_schematic.png     # Circuit schematic image
|   ├── irl_schematic.jpg #irl image
│   └── wokwi.toml        # Simulation config
├── src/
│   └── LED_Controller.cpp          # Arduino source code
├── .gitignore            # Git ignore rules for build files
├── LICENSE               # Open-source license (MIT)
└── README.md             # Project documentation
