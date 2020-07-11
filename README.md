# Arduino_tests
Some TEST project using Arduino to build musical instruments.  


# Contents
- 00_helloworld (using 2NanoMusic board)
The basics for using switches, leds, sounds.  

- 01_I2C (using 2NanoMusic board)
Easy samples about using I2C connection.  

- 02_MIDI (using 2NanoMusic board)
Easy examples about using MIDI communication.  

- 03_ISR (using 2NanoMusic board)
Easy examples about using AVR timer-interrupt functions.  

- 04_PWM (using 2NanoMusic board and an additional speaker on D11 pin)
Using AVR timer-interrupt and PWM output in order to play multi-channel sounds.  

- 05_MIDI_ISR (using 2NanoMusic board and an additional speaker on D11 pin)
The example of recieve MIDI, play tone(multi), and send MIDI using AVR interrupt in order to DSP and timing-control.  

- 06_LED_ISR (using 2NanoMusic board)
Using AVR timer-interrupt to control LED and SW matrix, and to control MIDI-send sequencialy.  


# Hardware
Schematics, parts, and PWB designs all are in kicad directory.  
Using two Arduino nano boards, one speaker, some LEDs and 6x6 tactile switches.  

