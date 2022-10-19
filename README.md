# Da heck is that?
A thief is starting to hammer my balls stealing stuff from the letter boxes of my building, including mine.\
Stealing my credit card being the red line he shoudln't have crossed, and as we say `Don't fuck with a developer`, I've made this simple arduino project consisting of a BLE emiter I put inside the letter box and receiver inside my flat.\
As soon as the receiver screams, I'll run down and god knows what will happen then!\
When the letter box is opened, the receiver knows and screams sounds on a speaker.\
\
It's made to use as low energy as possible so it can run weeks on a Lipo battery.\
There is no actual BLE connection, the emitter simply changes the BLE name to `letterbox-open` or `letterbox-closed` depending on the door state.\
It goes to deepsleep for 2 minutes and wakes up for only 10s just to tell the receivier that board is still alive.\
If the door is opened when in deep sleep, an interrupt wakes the board up so the receiver knows the door has been opened.\
\
I'm "lucky" enough to get my flat just above the letter boxes so the bluetooth connection can be established between the letter box and my home.\
If you don't, this won't help you for this use case but it may for something else, who knows...

# Components
I'll try to add the schematics and stuff later...
- The code is made to run on [Particle Xenon](https://docs.particle.io/xenon/) boards, which are discontinued, but it should run on any other kind of [Particle Boards](https://www.particle.io/devices/) as long as it supports bluetooth.
- For the sound I'm using a [speaker](https://www.adafruit.com/product/1313) hooked on a [DFPlayerMini](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299) that allows to play MP3, the MP3 player being controlled by the Xenon board.
- A [lipo battery](https://www.adafruit.com/product/2011) to power the board. The boards inclueds a lipo charger, just plug on an USB charger.
- A [micro switch](https://www.sparkfun.com/products/13014) that detects the door opening and wakes up the arduino board.
