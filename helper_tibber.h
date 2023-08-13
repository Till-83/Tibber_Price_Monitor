
// A single price valid for one hour
struct tibber_price {
  //First 14 characters are sufficient, other characters are zero
  char starttime[14];
  int price; // price as 0.1 cent
  int level; // Price Level (1-5) as provided by Tibber (Very Cheap, Cheap, Normal, Expensive, Very Expensive)
  boolean isNull; // True, if data not available
};

//Prices for the next 24 hours
struct tibber_prices {
  tibber_price price[24];
  int minimumPrice = 1000;  //Preis als int in zehntel-Cent
  int maximumPrice = 1;  //Preis als int in zehntel-Cent
};

// Prices as global variable
tibber_prices PRICES;

//Print the prices over Serial
void printPrices() {
  for (int i=0;i<=23; i++) {
    if (!PRICES.price[i].isNull) {
      Serial.printf("Beginn: %s Level %i Price: %i \n",PRICES.price[i].starttime, PRICES.price[i].level, PRICES.price[i].price );
    }
  }
  Serial.print("Minimum price: ");
  Serial.println(PRICES.minimumPrice);
  Serial.print("Maximum Price: ");
  Serial.println(PRICES.maximumPrice);
}


////Unused, to memory intensive
// void parseToJsonDoc(String response, DynamicJsonDocument doc) {
//   // Parse the JSON response
//   Serial.print("R1: "); 
//       Serial.println(response);
//   DeserializationError error = deserializeJson(doc, response);
//   if (error) {
//     Serial.print("R2: "); 
//       Serial.println(response);
//     Serial.println(error.c_str());
//     Serial.println("Error parsing JSON response");
//     //return null;
//   }
// }

//Parse the JSON Document and fill the tibber_prices variable
void parseTibberJson(DynamicJsonDocument jsonDoc) {
  //temporay price for a single hour
  tibber_price tmpPrice;
  int size = sizeof(PRICES.price);
  // 0 is today, 1 is tomorrow
  int day = 0;
  int hour = getHoursOfDay();
  int tmpHour;
  // We need the first six letters to parse the price level
  char tmpLevel[6];
  // Reset maximum/minimum price for the next 24 hours - Values will be overwritten with first price
  PRICES.maximumPrice = 1;
  PRICES.minimumPrice = 1000;
  //Loop over 24 hours
  for (int i = 0; i <= 23; i++) {
    //Calculate the hour for the loop
    tmpHour = i + hour;
    if (tmpHour > 23) {
      tmpHour = tmpHour - 24;
      day = 1;
    }
    // Check if we have a price for this hour
    if (jsonDoc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"][(day==0)?"today":"tomorrow"].isNull() || jsonDoc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"][(day==0)?"today":"tomorrow"][tmpHour].isNull() || jsonDoc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"][(day==0)?"today":"tomorrow"][tmpHour]["total"].isNull()) {
      tmpPrice.isNull = true;
      PRICES.price[i] = tmpPrice;
      Serial.print("Wert ist Null: ");
      Serial.println(i);
    } else {
      //Fill the price in the tmp variable
      tmpPrice.price = (int)(1000 * (double)jsonDoc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"][(day==0)?"today":"tomorrow"][tmpHour]["total"]);
      if (PRICES.maximumPrice < tmpPrice.price) {
        PRICES.maximumPrice = tmpPrice.price;
      }
      if (PRICES.minimumPrice > tmpPrice.price) {
        PRICES.minimumPrice = tmpPrice.price;
      }
      strlcpy (tmpPrice.starttime,
               jsonDoc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"][(day==0)?"today":"tomorrow"][tmpHour]["startsAt"],
               sizeof(tmpPrice.starttime));
      strlcpy (tmpLevel,
              jsonDoc["data"]["viewer"]["homes"][0]["currentSubscription"]["priceInfo"][(day==0)?"today":"tomorrow"][tmpHour]["level"],
              sizeof(tmpLevel));
      // Store the level as an int        
      switch (tmpLevel[0]) {
        case 'N': 
          tmpPrice.level = 3;
        break;
        case 'E': 
          tmpPrice.level = 4;
        break;
        case 'C': 
          tmpPrice.level = 2;
        break;
        case 'V': 
          tmpPrice.level = (tmpLevel[5] = 'E') ? 5: 1;
        break;
      }
      tmpPrice.isNull = false;
      //Store the price in the global structure
      PRICES.price[i] = tmpPrice;
    }
  }
  //Print the prices for debugging
  printPrices();
}


//Get the prices via the API
void getTibberInfo() {
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

  // We are happy to ignore the SSL certificate, then use the following line instead:
  client->setInsecure();

  HTTPClient https;
  https.useHTTP10(true);

  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, tibberApi)) {  // HTTPS

    Serial.print("[HTTPS] POST...\n");
    // start connection and send HTTP header
    https.addHeader("Content-Type", "application/json");
    // Personal API key to get the data
    https.addHeader("Authorization", "Bearer " + apiKey);
    //Query for the tibber API
    String payload = "{\"query\": \"{ viewer { homes { currentSubscription{ priceInfo{ current{ total  startsAt level} today { total startsAt level} tomorrow { total startsAt level} }}}} }\" }";

    int httpCode = https.POST(payload);
    if (httpCode == HTTP_CODE_OK) {
      
      DynamicJsonDocument jsonDoc(8000);

      display_freeram();
      //Parse the http request as a stream and convert to JSON doc
      DeserializationError error = deserializeJson(jsonDoc, https.getStream());
      display_freeram();
      if (error) {
        Serial.println(error.c_str());
        Serial.println("Error parsing JSON response");
      }
      display_freeram();
      parseTibberJson(jsonDoc);
    } else {
      Serial.println("something went wrong");
      Serial.println(httpCode);
      Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
  return;
}
