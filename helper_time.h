
//Set the time zone
void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

// initialize the time, time from an NTP Server
void initTime(String timezone){
  struct tm timeinfo;
  Serial.println("Setting up time");
  setTimezone("GMT0");
  configTime(0, 0, ntpServer);    // First connect to NTP server, with 0 TZ offset
  if(!getLocalTime(&timeinfo)){
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
}

//Update the time from the NTP Server
void updateTime(String timezone){
  struct tm timeinfo;
  setTimezone("GMT0");
  configTime(0, 0, ntpServer);    // First connect to NTP server, with 0 TZ offset
  if(!getLocalTime(&timeinfo)){
      Serial.println("  Failed to obtain time");
      return;
  }
  setTimezone(timezone);  
}

//Return the current hour
int getHoursOfDay() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return -1;
  }
  return (int) timeinfo.tm_hour;
}

//Print the time
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.print("Local Time: ");
  char localTime[9];
  strftime(localTime,9, "%H:%M:%S", &timeinfo);
  Serial.println(localTime);
}

//Minutes remaining, not required at the moment 
int minutesToHour()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
      return -1;
    }
  return 59 - (int) timeinfo.tm_min;
}