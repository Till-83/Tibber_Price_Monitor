#include <ArduinoJson.h> 
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecureBearSSL.h>
#include <time.h>

#include "settings.h"
#include "helper_wifi.h"
#include "helper_time.h"
#include "helper_memory.h"
#include "helper_tibber.h"
#include "helper_led.h"

//Global variable, stores the hour when the last data was requested from Tibber - Update will run once the hour changes
int hourLastCheck = -1;

// Counter to avoid using delay() since longer delay times causes issues with the soft WDT on ESP8266
unsigned long timeLastCheck = 0;

// Refresh time of the LEDs and basically everything
unsigned long intervalCheck = 1;



void setup() {
  Serial.begin(115200);
  Serial.println();
  //Setup WiFi
  connectwifi();
  //Get Time from NTP and set the timezone
  initTime(timeZoneBerlin);
  //Prepare the LED Matrix
  matrixInitialize();
}


void loop() {
  //Run the update once every second
  unsigned long timeNow = millis();
  if ( timeLastCheck == 0 || (unsigned long)(timeNow - timeLastCheck) > (intervalCheck * 1000) )
  {   
    timeLastCheck = timeNow;
    
    //Get new data from Tibber every hour
    int hourNow = getHoursOfDay();   
    if ( hourNow > hourLastCheck || ( hourNow == 0 && hourLastCheck == 23 ) ) {
      //Only run the update if WiFi is working
      if ((WiFi.status() == WL_CONNECTED)) {
        // Update data from Tibber
        getTibberInfo();
        
        //Update the LED Matrix
        matrixShowTibber();

        //Update the time from the NTP Server since I don't know how well the internal clock works on the ESP8266
        updateTime(timeZoneBerlin);

        //Update the variable to run this part only once every hour
        hourLastCheck = hourNow;

        //No need to stay connected
        disconnectwifi();
    
      } else { //If WiFi is not working
        Serial.println("No WiFi - Reconnect");
        connectwifi();
      }
    }
    // Update the LED Matrix - necessary for the blinking effect
    matrixShowTibber();
  }
}



