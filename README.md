This is a fork of @dschmenk 's excellent NTSC video generator [given here](https://github.com/dschmenk/Arduino/tree/master/VideoSPI)

I stripped out the rest of the repo that was unrelated using [this method](https://help.github.com/en/articles/splitting-a-subfolder-out-into-a-new-repository)

The original implementation simply generated a demo screen. My intent is to add either a TWI/I2C or TTL-serial interface to make it into a terminal.

I organized the original file into different functional components.

I also replaced the demo with a simple scrolling character generator.

# TODO

- use `Wire` to listen for characters input on the TWI/I2C port
- add ANSI escape sequences for accessing the low resolution graphics commands in `gfx.cpp`
- add ANSI escape sequences for moving the cursor around
- color?! bold?! underline?!
- extend charROM to include the extended ASCII charset from MSDOS (mainly for text-mode box drawing)
- sound?!
- PS/2 keyboard handling?!
