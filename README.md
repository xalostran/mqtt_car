This is an MQTT based project created for operating a car with a joystick.

In order to send data from joystick to the car, you need to setup an MQTT broker on your local network. You can start by using mosquitto, which is possible to install and run directly in your terminal.

The boards used in this projects are:
- Arduino Uno Rev4 WiFi(Subscriber)
- Raspberry Pi Pico WH(Publisher)

The car is a subscriber of the joystick which is a publisher of data onto your broker.

Important note: 

The two folders in this project (car_subscriber and joystick_pi) should be compiled in two separate VS Code windows with the platformIO extension. If you would like to use a different board, you will need to change your [env] properties in the platform.ini file.