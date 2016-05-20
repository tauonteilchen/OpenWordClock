# OpenWordClock
Fork for NodeMCU ESP8266 [1]
This fork has some change in Shematic and Code

Changes at the Shematic:
  The NodeMCU
  The LED will be driven with a TLC5947 24Channel [2]
  -  so you need to swap all LED ground to V+ the TLC5947 is open drain
    - my the best way ist swap the LEDs und Power the "Ground Line" of it with V+ (Do it only when you now what you do)

Changes at Code:
 - Setup your Arduino IDE for ESP8266 [3]
 - The OpenWordClock need now an WiFi connection it will get the current time from NTP "0.de.pool.ntp.org" it will syced one time per hour
 - For now Timezone ist hardcoded to Central European Time (Frankfurt, Paris)
 - The TimeChangeRule are CEST & CET so it will show you always the right time

- [1] http://nodemcu.com/index_en.html 
- [2] https://github.com/adafruit/Adafruit_TLC5947
- [3] https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide

*Soory for german english*
