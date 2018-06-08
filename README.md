# OLED display VU meter — AVR/Arduino project

This project is to create digital version of a nice gadget that adds a little magic 
to any audio equipment: a [VU meter](https://en.wikipedia.org/wiki/VU_meter). Main design
goals are:
  - 2-channel operation,
  - as close to real VU-meter specification as possible,
  - high FPS, 60+ -- no visible jumping of needle, as is common in other designs.

Project uses AVR ATMega88/168/328 (and ATMega48 if possible), so should be compatibile with
Arduino Uno boards. Small (and cheap) monochrome OLED modules with SSD1306 or SH1106 driver are 
employed as displays. Unlike other AVR VU meters that you can find on the web this one uses
quite complex analog circuitry (it can be much simpler if you are not as interested as me in
up-to-specs VU meter emulation).

Here's a video showing test of VU meter prototype:

[![Background and needle animation test video](https://img.youtube.com/vi/HKTR07n0YTo/maxresdefault.jpg)](https://youtu.be/e8a-bNMEwk4)

## Building The Project

To build hex file you'll need:
  - avr-gcc,
  - GNU make,
  - Python 2.7 or 3.
For windows you can use this distribution of gcc and make: http://blog.zakkemble.co.uk/avr-gcc-builds/

Before compilation please modify [src/config.h](src/config.h) file to set proper OLED controller type and [Makefile](Makefile) to set uC type and oscillator frequency (F_CPU).

To create hex file type:
```
$ make all
```
This will create `build/main.hex` that you can use to flash your device.

__Note: This is Work In Progress design. To make it work for you you'll most probably need to modify it in one way or another.__

## Hardware Design

Full documentation will be included in the future, as the project progresses.

For now here's very high-level block diagram of the device:

![Block diagram of AVR/OLED VU meter](images/block_diagram.png)

Second analog channel and OLED display were omitted for the sake of brevity.

Analog part of the circuit, that performs simulation of VU meter function, presents itself as follows:

![Schematic diagram of single analog channel](images/analog_simulation.png)

It consists of three distinct functional parts: virtual ground supply, voltage follower providing 2.5V for other op amps, precision rectifier and ballistics simulation.

Precision rectifier was used to prevent loosing low-level signals and distortion inevitable in traditional four-diode full wave rectifier. In actual VU meters it was only about 0.4V because copper-oxide rectifiers were used (and non-linearity was accounted for in scale), but with silicon diodes this would be at least 1.2V (forward voltage of two diodes), or >0.6V if Shottky or germanium diodes were used. At the same time any loss can be prevented by using precision rectifier consisting of two op amps and two diodes of any type (their forward voltage doesn't matter that much). Limiting factor in the design here will be slew rate of op amp output.

Ballistics simulation consists of three op amp circuits: adder, which sums all forces and outputs acceleration, and two integrators, first outputting velocity, and second the angle of the needle. Velocity and angle are fed back to account for viscous dampening and spring tension, and angle is measured by DAC in microcontroller.

There are several unresolved issues with this circuit (e.g. lower-than-one gain in ballistics first stage, or high capacitance load in the last), please do not copy it as is!

And below you can find a diagram explaining connection between ATMega and OLED displays:

![Schematic showing details of connection between ATMega and OLED displays](images/oled_connection.png)

The key issue here is that display controller is supplied with 3.3V from regulator included on its board. This means that SCL and SDA pins logic levels are relative to 3.3V and providing higher voltage there would damage displays. And at the same time VCC pin has to be provided with at least 4.3V, since lower voltage will prevent the regulator from functioning properly.

One solution, presented here, is to power ATMega from 5V supply and use logic-level converters to drive OLED inputs. It's common for such converters (available on SparkFun and similar sites) to use design described in this application note from NXP: [Level shifting techniques in I<sup>2</sup>C-bus design](https://www.nxp.com/docs/en/application-note/AN10441.pdf). 3.3V supply is provided by local 3.3V regulator (LD1117).

Other equally valid solution would be to power ATMega from 3.3V, thus forgoing level-shifting. 5V supply would still be needed for OLED's supply pins.
