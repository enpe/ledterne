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

- Add EAGLE schematic and board of the prototype lantern
- Experiment with LEDs
	- Adjust series resistors for balanced LED brightness of the different colors
	- Try diffusing light from the LEDs
- Add some interesting animations
- Add a push button for switching between a number of animations, constant colors etc.
	- Can use pin PD3's external interrupt capabilities instead of polling one of the other free pins
- Make it battery-driven
- Maybe make the PWM smarter for improved energy saving
	- Alternative 1:
		- Avoid generating interrupts for every single possible step in a PWM cycle
		- Compute only the required timeout periods for all 3×5 outputs instead
	- Alternative 2 (easier than 1):
		- Avoid generating interrupts for every single possible step in a PWM cycle
		- Compute only the required timeout periods for all (currently 32) different duty cycles that we actually use
		- This is not as efficient as alternative 1, but it is much simpler to implement since it is not data-dependent (does not change with the desired LED brightness (i.e. with the animations))
- Maybe improve PWM resolution for smoother fading
- Maybe implement the animations using coroutines/generators instead of letting the animation do the diplay update
	- https://en.wikipedia.org/wiki/Generator_%28computer_science%29
	- http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html


### Battery holder and rod
- Dimensions of [mignon/AA batteries][21]:
	- Diameter:  13.5-14.5 mm
    - Length:    49.2-50.5 mm
- General idea:
    1. Rod of the lantern
    	- Thin aluminum tube (outer diameter <= 10mm).
    	- Drill a small hole near the top end of the rod (wire outlet + hook for
    	  lantern).
    	- Close the top end with a small cap (or leave it open).
    	- Take 2-3 corks and drill holes into the centers to fit the rod.
    	- Cut outside part of the corks to fit (tightly) into the tube of 
    	  battery holder.
    	- Slide the corks over the lower end of the rod.
    	- Insert 2 wires from lower end of the rod to the outlet at the top end.
    2. Battery holder
		- Tube with inner diameter of ~15-16 mm.
		- In the first third of the tube, drill a hole for a small switch.
		- Add 2 wires: one from switch to the lower end of the tube the other
		  from one end of the tube to the other.
		- Connect the switch with one wire of the rod, and connect the remaining
		  two wires
		- Connect upper end of the battery holder with the rod.
		- Fill with 3xAA batteries.
		- Add a spring at the lower end and connect it to the wire.
		- Close lower end with a plastic cap (Lamellenstopfen)
- Other DIY project for battery holders (in no particular order):
	- http://makezine.com/2009/08/14/how-to-free-diy-battery-holders/
	- http://makezine.com/2015/08/02/make-a-stylish-flashlight-out-of-kitchen-trash/
	- http://www.instructables.com/id/Bicycle-Battery-Holder-3-x-AA/
	- http://www.instructables.com/id/Easy-custom-battery-holders/
	- http://www.instructables.com/id/Easy-DIY-Battery-Holder/
	- http://www.instructables.com/id/PVC-tube-battery-AA-battery-holder/
	- http://www.pcbheaven.com/projectpages/DIY_Battery_Pack/


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
[21]: https://de.wikipedia.org/wiki/Mignon_%28Batterie%29

