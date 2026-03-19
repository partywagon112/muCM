# μCM

A small ESP32 based flexible controller, intended for Matter to control an RGB light or motor, but very very flexible. Based around ESP32-C6 for flexibility. Built to work in two contexts, in isolated locations where only power may be provided, up to 40V 3A, or as a PD powered dongle, up to 20V, 3A. 

Includes a 3V3 CAN driver for interacting with smaller devices, two pairs of half bridges allow driving servos, LEDs, or outputs more generally. 

![image](https://github.com/partywagon112/MatterControllerESP32/blob/main/docs/image.png)
