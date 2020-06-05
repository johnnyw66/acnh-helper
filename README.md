## Animal Crossing New Horizons Joystick Helper

Runs common laborious ACNH chores, allowing you to put your feet up and have a cup of tea.

* Make a Wish Chore :- Infinite loop attempting to make a wish. Use this on nights when Celeste makes a visit.
 (Don't forget to scavenge the beach to pick up your stars the next morning!)

* Buy full (4 Rows) stock of fruit items (40x10) Chore :- Go to Shop Cabinet armed with 160K of Bells to fill up on Fruit.

* Buy 3 Rows of fruit items (30x10) Chore

* Buy 2 Rows of fruit items (20x10) Chore

* Buy 1 Rows of fruit items (10x10) Chore

* Build 40 Items :- Set this off by standing next to a DIY Workbench and selecting the item you want to build.
                    (leave the hand cursor on the 'Craft' button)


[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/udo8mv5oarg/0.jpg)](https://www.youtube.com/watch?v=udo8mv5oarg)


I've modified the original "Bertrand Fan's" Zelda Snowball code to allow multiple macros along with macro loops and calls.

Loops and Calls can be nested up to 12 levels.


#### Hardware mods/additions

I've made changes to the original code to allow selection of multiple macros.  
This was achieved by adding in a 4-way or 8-way dip-switch to the C port of the teensy.
Since I only needed around 8 macros, I soldered only the first three pins of the switch
to C1, C2 and C3. Since I intended to set these teensy pins to input mode (DDRC = 0)
with internal pull-up resistors (PORTC = 255;), I only need to ground the other
end of the dip-switch.

Note:

As the software stands - missing out the dip-switch mod will result in the 'Buy 4 Rows Fruit'
macro being selected. (see hardware.c if you want to change this)



#### 2nd LED indicator (Optional)
I've added a Red LED onto pin D5 of the teensy (through a 500 Ohm current limiting resistor).
Simply solder the anode end of the LED to D5. One end of the resistor goes on to the cathode end
of the LED. Finally, the other end of the resistor goes to the GND pin of the teensy.
This LED will Strobe continuously when the macro has finished (i.e reached 'ENDMACRO')


#### How to use (Example: Stock up on 400 lots Fruit to sell off Island)

First, make sure your teensy is NOT powered/plugged into the USB port.

Also set the first free switches (1 to 3) on your dip-switch is set to (off, on, off).

This will select the 'Buy 400 fruit' macro.

Make sure your pocket is empty  of items other than the 160K cash.

Walk up to the Shop Cabinet with 160K bells of cash and plug in your Teensy Controller.  The Teensy should sync with the console.
Don't worry about the speech bubble - this was done to allow the macros to work in both docked and handheld mode.
If you are using a Pro-controller you will need to unlink this buy pressing in the reset button on the face of the
Joystick controller. Wait for the JoyStick select menu before plugging in the Teensy.

Since the dip-switch is only read once, on powering up - you need to power down the teensy before selecting a new macro.


This repository has been tested using a Teensy 2.0++.

#### Compiling and Flashing onto the Teensy 2.0++
Make Sure you have 'git' installed on your system

First Clone this directory

git clone https://github.com/johnnyw66/acnh-helper


cd to the newly created directory 'acnh-helper' and clone the USB LUFA support libraries with the command

git clone https://github.com/abcminiuser/lufa.git


(alternatively you can run the bash script clonelufa.sh)

Thanks to 'Bertrand Fan' for the following text.

"Go to the Teensy website and download/install the [Teensy Loader application](https://www.pjrc.com/teensy/loader.html). For Linux, follow their instructions for installing the [GCC Compiler and Tools](https://www.pjrc.com/teensy/gcc.html). For Windows, you will need the [latest AVR toolchain](http://www.atmel.com/tools/atmelavrtoolchainforwindows.aspx) from the Atmel site. See [this issue](https://github.com/LightningStalker/Splatmeme-Printer/issues/10) and [this thread](http://gbatemp.net/threads/how-to-use-shinyquagsires-splatoon-2-post-printer.479497/) on GBAtemp for more information. (Note for Mac users - the AVR MacPack is now called AVR CrossPack. If that does not work, you can try installing `avr-gcc` with `brew`.)

Open a terminal window in the `acnh-helper` directory, type `make`, and hit enter to compile. If all goes well, the printout in the terminal will let you know it finished the build! Follow the directions on flashing `acnh-helper.hex` onto your Teensy, which can be found page where you downloaded the Teensy Loader application."


You can modify the board and macro code (macro.c) to implement and expand your own macros.

Use the script 'runtest.sh' to test on your native machine (see the test main function in macro.c). You'll need native gcc compiler, installed
to run this test script.

if you want to test different macros with this script - then modify the -D_DEFAULTMACRO_FRUITBUY10 define option to one of the following
other macro definitions.

-D_DEFAULTMACRO_WISH    -- Used for wishing on starry nights when Celeste is around.
-D_DEFAULTMACRO_BUILD   -- Build 40 items
-D_DEFAULTMACRO_FRUITBUY400  -- Stock up on Fruit to Sell for a Big Profit on a friend's island.
-D_DEFAULTMACRO_FRUITBUY300
-D_DEFAULTMACRO_FRUITBUY200
-D_DEFAULTMACRO_FRUITBUY100
-D_DEFAULTMACRO_FRUITBUY10
-D_DEFAULTMACRO_DEMO --Useless demo - move around pocket and swipe net/axe a few times.

Be aware that some macros (such as -D_DEFAULTMACRO_WISH) may loop indefinitely!

### Writing your own macros

Hopefully the source in macro.c is pretty easy to follow and you can start implementing your own macros.
You define your macros in by inserting the first element of your key list in 'static command *macros[]' array
Each command starts with an underscore '_' and at some point (unless you're looping indefinitely) this will
end with an '_ENDMACRO' instruction.

SUBROUTINES are define in the 'rstcommands' array. Each subroutine is called with the _RST(indexnum) command, using
its array index as the call parameter. Make sure you end your subroutine with a '_RETURN' instruction.

LOOPS are defined with the _REPEAT() and _ENDRPT instructions. The parameter used in the REPEAT instruction is the loop count.

_REPEAT(-1) will loop forever. Loops are terminated with a paired '_ENDRPT' instruction.

Nested Loops and Calls are allowed - up to 12 levels.


If you're using an 8 way DIL dip-switch you can define up to 256 macro definitions - but make sure all pins C0 to C7 are soldered
to the dip-switch and the opposite ends of the dip-switch are grounded to the GND pin on the teensy. You will also need to make
sure that there is a break in the middle of the DIL.


The internal pull-up resistor mode is used for the C PORT pins - so there's no need to use external resistors.

(Note: One external 500 ohm resistor is used on D Port for an optional LED indicator)


#### Thanks

Thanks to 'Bertrand Fan' for his snowball post including his code and documentation.

Below is a quote from his project and I wish to extend my thanks to him and to those people he mentioned below.

"A full writeup is of his project is [here](https://medium.com/@bertrandom/automating-zelda-3b37127e24c8).

Thanks to Shiny Quagsire for his [Splatoon post printer](https://github.com/shinyquagsire23/Switch-Fightstick) and progmem for his [original discovery](https://github.com/progmem/Switch-Fightstick).

Thanks to [exsilium](https://github.com/bertrandom/snowball-thrower/pull/1) for improving the command structure, optimizing the waiting times, and handling the failure scenarios. It can now run indefinitely!"

Please do let me know if I have omitted to thank anyone!


Johnny Wilson, Sussex, UK. June 2020
