This repository contains Arduiono code for testing the subsystem of the OpenAg Food Computer Version 2 that consists of the Arduino Mega 2560, OpenAg Signal Board, Sensors, and Accuators.

The purpose of this code is to provide a tool that allows the following:

1) A general purpose REPL monitor that allows arbitrary commands to be passed to the Arduino over the serial interface. The syntax is Lisp like.  Commands such as (unit_tests) or (get_air_temperature) will be provided so that one can debug sensors and actuators in isolation.

2) Provide a way to "mod hack" the open ag Arduino code to repurpose it for other grow environments.  One will be able to add and subtract sensors and actuators to the Arduiono code and add control and monitor commands for them that will then be available via the serial interface. 

3) Be compatible with the OpenAg code. The code is compatible with OpenAg firmware functionality. After debugging is done one can configure the code to boot up in a manner that is functionaly the same as the OpenAg firmware. Thus the code can be run on an operationl V2 FC instead of the OpenAg firmware.

Status - Not ready for prime time yet.  I hope to realease an alpha version by end of November, 2017.
