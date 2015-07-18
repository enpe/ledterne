# LEDterne

## Required Equipment

- Suggested hardware lists:
	1. [mikrocontroller.net][0]
- Atmel AVRISP MKII ([Amazon][1])
- Evaluation board ([ehajo][2], [Pollin][3], [Amazon][4])
- Adapter cable, 10P-to-6P ([mikocontroller.net][7], [Amazon 1][5], [Amazon 2][6])
- Power supply (~9V)
- Bread board ([Amazon][8])
- ATMEGA8-16PU ([Amazon][9])
- Steckbrücken 
- Multimeter
- Resistors
- LEDs (e.g. RGB)
- Software (compiler, etc.) ([mikrocontroller.net][20])

## TODO

- Assemble a prototype lantern for testing illumination
	- Current plan: circular arrangement of 5 RGB LEDs
	- Some sort of hook needs to be attached to the top
- Experiment with LEDs
	- Try diffusing light from the THT LEDs
	- Measure actual forward voltages to adjust series resistors accordingly
	- Try SMD LEDs for much wider beam angle
- Add some interesting animations
- Add a push button for switching between a number of animations, constant colors etc.
	- Can use pin PD3's external interrupt capabilities instead of polling one of the other free pins
- Make it battery-driven
- Maybe make the PWM smarter for improved energy saving
	- Avoid generating interrupts for every single possible step in a PWM cycle
	- Compute only the required timeout periods for all 3×5 outputs instead
- Maybe improve PWM resolution for smoother fading


[0]: http://www.mikrocontroller.net/articles/AVR-Tutorial:_Equipment
[1]: http://www.amazon.de/Atmel-Programmer-ATMEL-AVR-ISP-MKII/dp/B00CASCN2S
[2]: http://www.ehajo.de/ateval-atmel-evaluationboard.html 
[3]: http://www.pollin.de/shop/dt/NTI5OTgxOTk-/Bausaetze_Module/Entwicklerboards/ATMEL_Evaluations_Board_V2_0_1_Fertigmodul.html
[4]: http://www.amazon.de/Atmel-Fertiggerät-Atmel-Evaluations-Board-V2-0/dp/B004BMJZ0O
[5]: http://www.amazon.de/Convert-Standard-Adapter-STK500-AVRISP/dp/B00P783JIU
[6]: http://www.amazon.de/Adapter-Kit-10polig-Atmel-Programmer/dp/B004D2TFYI
[7]: http://www.mikrocontroller.net/articles/AVR_In_System_Programmer#ISP
[8]: http://www.amazon.de/Steckbrett-Breadboard-Experimentierboard-Steckplatine-Kontakte/dp/B009P04XWW
[9]: http://www.amazon.de/ATMEGA8-16PU-Atmel-Microcontroller-MEGA-DIP-28/dp/9827046934
[20]: http://www.mikrocontroller.net/articles/AVR_und_Linux

