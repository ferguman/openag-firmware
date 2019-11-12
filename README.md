This repository contains C++ Arduino code for testing the subsystem of the OpenAg Food Computer Version 2. Within the Food Computer the Arduino subsystem consists of the Arduino Mega 2560, OpenAg Signal Board, Sensors, and Accuators.

The purpose of this code is to provide a tool that allows the following:

1) A general purpose REPL monitor that allows arbitrary commands to be passed to the Arduino over the serial interface. The syntax is Lisp like.  Commands such as (unit_tests) or (get_air_temperature) will be provided so that one can debug sensors and actuators in isolation.

2) Provide a way to "mod hack" the Openag Arduino code to re-purpose it for other grow environments.  One can add and subtract sensors and actuators to the Arduino code and add control and monitor commands which will then be available via the serial interface. 

3) Be compatible with the OpenAg code. The code is compatible with the OpenAg firmware functionality as of January 2018. After debugging is done one can configure the code to boot up in a manner that is functionaly the same as the OpenAg firmware. Thus the code can be run on an operational V2 FC.

Further information can be found at https://github.com/ferguman/openag-firmware/wiki/Creating-a-Firmware-Module
