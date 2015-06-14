# mod_pedal
Code for an Arduino to interface with foot pedals (or similar) in order to produce modifier key-presses on a *nix system

# Building
mod_pedal is very light on dependencies, requiring only 2 libraries:
`libx11-dev`
`libtst-dev`

Once these dependencies have been installed, simply run `make` in the `pc` directory. This will generate the executable `pedald` 

# Running
Run `pedald` with sudo permissions, and provide it with a device to connect to

For example:
`sudo ./pedald /dev/ttyACM0`
