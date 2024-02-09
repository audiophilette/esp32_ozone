# esp32_ozone
Use a DFRobot Ozone sensor and an ESP32 arduino module to monitor ozone levels.


I wrote this to help [nitrogen76](https://github.com/nitrogen76/cheapWeather) to make a very simple arduino thing to monitor ozone concentration for his project.

If you want to use this, you need an influxDB instance (version 2 is how this is configured, but v1 should be easily portable) and set your influxDB details in the arduino_secrets.h file.

The sensor I am using is [this one](https://www.dfrobot.com/product-2005.html) from [DFRobot](https://www.dfrobot.com)

IT's very important that you let it "cook" for 24 hours before using any measurements from it, and the first 15min of data you get from it will be weird and not reliable.

It's also best to keep it protected from wind and direct sunlight, so keeping it in a sensor box is a good idea. 

A good cheap, easy to do sensor box is [this one](https://www.youtube.com/watch?v=qbYXinada-c) i found that a group in the Netherlands uses.  We're testing this one to see how well it works.
