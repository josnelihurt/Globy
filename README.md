# Globy project

This is an small example to use an nrf51822 and expose some characteristics.

# Prerequisites

 - make
 - docker
 - patience :P

# Build 
-`make build`

# Result files 
At this moment the output file will be placed in  `globy-proj/_build/nrf51822_xxac.hex` where you can transfered to the nrf51822 using a JTAG or STLink V2 with openocd or any other tool.

An example to flash the `hex` file is in the `Makefile` inside the `globy-proj`  folder