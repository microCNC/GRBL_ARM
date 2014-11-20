GRBL_ARM
========

GRBL Ported to the Arm Processor

WARNING THIS IS A WORK IN PROGRESS AND CURRENTLY DOES NOT COMPILE

We have gone for the Texas Instruments Tiva C Launchpad TM4C123G ( http://www.ti.com/tool/ek-tm4c123gxl ) because of its price and its capabilties.

It is an ARM Cortex M4F based microcontroller.

Features Include :

```
80Mhz CPU
256KB Flash
32KB RAM
2KB EEPROM
Floating Point Unit
24 Timers
43 GPIO Pins
12bit ADC
```

Arm GCC eabi to compile
LM4Flash to flash to the board ( https://github.com/utzig/lm4tools/tree/master/lm4flash )

***
TODO List.

```
There is a bug that locks up the program after it has complete all the gcode sent to the board.
Spindle PWM
Limit Switches

```
