lttng_async_tutorial
====================

This repo contains a demo of the use of lttng ust.
The base, PerlinNoise.cpp  and ppm.cpp, of the demo comes from the async_tutorial from Sol, 
see original readme (section async_tutorial) below.

You could use this program under the terms of GPL v3, for more details see:
http://www.gnu.org/copyleft/gpl.html

Copyright 2014 Jan Van Winkel


Build
=====

mkdir build
cd build
cmake ..
make

Run
===

In the build directory run ./lttng_test, this would run the demo without outputting any traces

To collect traces from the demo use following commands:
lttng create test
lttng enable-event -u -a
lttng start
LD_PRELOAD="./libtp_lttng_test.so:./libtp_ppm.so" ./lttng_test
lttng stop
lttng destroy

After running the demo with the tracing enabled you can import the traces into eclipse 


async_tutorial
==============

C++11 std::async
Here you could find the code for **C++11 async tutorial**, for more informations visit the project webpage:

http://solarianprogrammer.com/2012/10/17/cpp-11-async-tutorial/

You could use this program under the terms of GPL v3, for more details see:

http://www.gnu.org/copyleft/gpl.html

Copyright 2012 Sol from www.solarianprogrammer.com

