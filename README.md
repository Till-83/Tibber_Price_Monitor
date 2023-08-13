# Tibber_Price_Monitor

Small project to show dynamic hourly energy prices from Tibber on a simple LED matrix.
First column is the current hour, the other colums show the next 7 hours.
High/low are the max/min price for the next 24 hours.
Colours are the price levels estimated by the Tibber API.

##Installation instructions:
- Connect the LED matrix to the board (power, ground, data to PIN D3)
- Modify your Tibber API key, WiFi settings in the settings.h file
- And compile/upload the project with the Arduino IDE

##Hardware required:
- ESP8266 or similar board with WiFi (WEMOS D1 in my case)
- Neopixel / WS2812 compatible LED matrix (8x8)
- Picture frame / box

Hardware cost: Electronics around 10 Euro

##Final result:
![A LED display with 8 columns](https://github.com/Till-83/Tibber_Price_Monitor/blob/main/images/picture.jpeg)
