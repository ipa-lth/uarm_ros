uarm ros package
=================

Yet another driver for the UArm.
ROS tools for UArm by UFactory (now named EVOL).

I was not happy with the performance by pyUArm, so I create a mean and lean driver for velocity control only.


Dependency
--------------
- UArmForArduino (arduino library)
  - Essentially UFServo.h, UFServo.cpp which is an (exact ?) clone of Servo.h

Install
===============
* Get the sketch (.ino) onto your Arduino Uno by either:
** Install arduino sdk from Arduino.cc[http://arduino.cc/en/Main/Software].
** Use the online Arduino Create 


Run
================

```bash
    $ rosrun rosserial_python serial_node.py
    $ rostopic list
```

