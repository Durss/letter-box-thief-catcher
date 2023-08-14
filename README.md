<div align="center"><img src="https://github.com/Durss/mailbox-thief-catcher/blob/main/boxes/receiver_render.png" width="500"></div>

# Da heck is that?
A thief is starting to hammer my balls stealing stuff from the mailboxes of my building, including mine.\
Stealing my credit card being the red line he shoudln't have crossed, and as we say `Don't fuck with a developer`, I've made this simple arduino project consisting of a BLE emiter I put inside the mailbox and a receiver inside my flat.\
When the mailbox is opened, the receiver knows and screams sounds on a speaker.\
As soon as the receiver screams, I'll run down and god knows what will happen then!\
\
It's made to use as low energy as possible so it can run for weeks on a Lipo battery.\
There is no actual BLE connection, the emitter simply changes the bluetooth connexion name to `letterbox-open` or `letterbox-closed` depending on the door state.\
It goes to deep sleep for 2 minutes and wakes up for only 10s just to tell the receivier that battery is still alive.\
If the door is opened when in deep sleep, an interrupt wakes the board up so the receiver knows the door has been opened as fast as possible.\
\
I'm lucky enough to get my flat just above the mailboxes so the bluetooth connection can be established between the mailbox and my home.\
If you don't, this won't help you for this use case but it may for something else, who knows...

# Component parts
- 1 [Particle Xenon](https://docs.particle.io/xenon/) board. These are discontinued, but it should run on any other kind of [Particle Boards](https://www.particle.io/devices/) as long as it supports bluetooth and can most probably run on ESP32 boards with probably some tweaks.
- 1 [speaker](https://www.adafruit.com/product/1313).
- 1 [DFPlayer mini](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299) that plays MP3s on the speaker.
- 1 [lipo battery](https://www.adafruit.com/product/2011) to power the board. The boards includes a lipo charger, just plug it to a USB wall adapter.
- 1 [micro switch](https://www.sparkfun.com/products/13014) that detects the door opening and wakes up the arduino board.
- 1 [Piranha RGB LED](https://www.adafruit.com/product/1451) (or any RGB led but it might not fit in the [given box](#boxes)
- 1 micro SD card to store MP3s
- 3 resistors (330Ω)
- [Optional] 1 or 2 [bluetooth antennas](https://www.adafruit.com/product/2308) to expand bluetooth range *(the one linked is for wifi but it should work also for BLE)*
- [Optional] 1 [protoboard](https://www.adafruit.com/product/4786) to solder components on it ([see schematics](#schematics))
- [Optional] [headers](https://www.adafruit.com/product/4160) if you don't want to solder the arduino board and the DFPlayer mini on the protoboard.

# Build
Build and upload emitter and receiver codes on their Xenon board.\
For that, you may want to use [VSCode](https://code.visualstudio.com) with [Particle Workbench](https://www.particle.io/workbench/).\
Also install [Particle CLI](https://docs.particle.io/getting-started/developer-tools/cli/) that installs necessary drivers.\

# Flash particle board
Open "Emitter" or "Receiver" folders in VScode. Not the root folder.\
Plug the board via USB, press `Ctrl+Shift+P` and select `Particle: Flash application (local)`. This will flash the board. Use `Particle: Serial monitor to get logs`.

# Boxes
If you have access to a laser cutter, you'll find SVG files to cut boxes for the emitter and the receiver under [boxes](https://github.com/Durss/mailbox-thief-catcher/tree/main/boxes) folder.

## Emiter:
<div align="center"><img src="https://github.com/Durss/mailbox-thief-catcher/blob/main/boxes/emitter_render.png" width="500"></div>

## Receiver:
<div align="center"><img src="https://github.com/Durss/mailbox-thief-catcher/blob/main/boxes/receiver_render.png" width="500"></div>

# Schematics
## Emitter
<img src="https://github.com/Durss/mailbox-thief-catcher/blob/main/schematics/receiver.png">

## Receiver
<img src="https://github.com/Durss/mailbox-thief-catcher/blob/main/schematics/emitter.png">
