/**The MIT License (MIT)

Copyright (c) 2018 by ThingPulse Ltd., https://thingpulse.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "FastLED.h"
#define NUM_LEDS 86
CRGB leds[NUM_LEDS];
#define PIN D7 
#include <ESP8266WiFi.h>
#include <JsonListener.h>
#include <time.h>
#include "OpenWeatherMapCurrent.h"
double timerOne = 0;
double timeNow = 0;

// initiate the client
OpenWeatherMapCurrent client;
 OpenWeatherMapCurrentData data;
// See https://docs.thingpulse.com/how-tos/openweathermap-key/
String OPEN_WEATHER_MAP_APP_ID = "";
/*
Go to https://openweathermap.org/find?q= and search for a location. Go through the
result set and select the entry closest to the actual location you want to display 
data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
at the end is what you assign to the constant below.
 */
String OPEN_WEATHER_MAP_LOCATION_ID = "5849297";
/*
Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
*/
String OPEN_WEATHER_MAP_LANGUAGE = "en";
boolean IS_METRIC = true;

/**
 * WiFi Settings
 */
const char* ESP_HOST_NAME = "esp-" + ESP.getFlashChipId();
const char* WIFI_SSID     = "";
const char* WIFI_PASSWORD = "";

// initiate the WifiClient
WiFiClient wifiClient;



/**
 * Helping funtions
 */
void connectWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());
  Serial.println();
}


/**
 * SETUP
 */
void setup() {
  Serial.begin(115200);
  delay(500);
  connectWifi();
FastLED.addLeds<WS2811, PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  Serial.println();
  Serial.println("\n\nNext Loop-Step: " + String(millis()) + ":");

  OpenWeatherMapCurrentData data;
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);

  Serial.println("------------------------------------");

  // "lon": 8.54, float lon;
  Serial.printf("lon: %f\n", data.lon);
  // "lat": 47.37 float lat;
  Serial.printf("lat: %f\n", data.lat);
  // "id": 521, weatherId weatherId;
  Serial.printf("weatherId: %d\n", data.weatherId);
  // "main": "Rain", String main;
  Serial.printf("main: %s\n", data.main.c_str());
  // "description": "shower rain", String description;
  Serial.printf("description: %s\n", data.description.c_str());
  // "icon": "09d" String icon; String iconMeteoCon;
  Serial.printf("icon: %s\n", data.icon.c_str());
  Serial.printf("iconMeteoCon: %s\n", data.iconMeteoCon.c_str());
  // "temp": 290.56, float temp;
  Serial.printf("temp: %f\n", data.temp);
  // "pressure": 1013, uint16_t pressure;
  Serial.printf("pressure: %d\n", data.pressure);
  // "humidity": 87, uint8_t humidity;
  Serial.printf("humidity: %d\n", data.humidity);
  // "temp_min": 289.15, float tempMin;
  Serial.printf("tempMin: %f\n", data.tempMin);
  // "temp_max": 292.15 float tempMax;
  Serial.printf("tempMax: %f\n", data.tempMax);
  // "wind": {"speed": 1.5}, float windSpeed;
  Serial.printf("windSpeed: %f\n", data.windSpeed);
  // "wind": {"deg": 1.5}, float windDeg;
  Serial.printf("windDeg: %f\n", data.windDeg);
  // "clouds": {"all": 90}, uint8_t clouds;
  Serial.printf("clouds: %d\n", data.clouds);
  // "dt": 1527015000, uint64_t observationTime;
  time_t time = data.observationTime;
  Serial.printf("observationTime: %d, full date: %s", data.observationTime, ctime(&time));
  // "country": "CH", String country;
  Serial.printf("country: %s\n", data.country.c_str());
  // "sunrise": 1526960448, uint32_t sunrise;
  time = data.sunrise;
  Serial.printf("sunrise: %d, full date: %s", data.sunrise, ctime(&time));
  // "sunset": 1527015901 uint32_t sunset;
  time = data.sunset;
  Serial.printf("sunset: %d, full date: %s", data.sunset, ctime(&time));

  // "name": "Zurich", String cityName;
  Serial.printf("cityName: %s\n", data.cityName.c_str());
  Serial.println();
  Serial.println("---------------------------------------------------/\n");

}


/**
 * LOOP
 */
void loop() {
if(millis() - timeNow > 600000) {
  if(WiFi.status() != WL_CONNECTED){
  connectWifi();
  }
 
  client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
  client.setMetric(IS_METRIC);
  client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);

  Serial.print(data.weatherId);
  Serial.println("weather Id");
  if((data.weatherId > 500) && (data.weatherId < 610)) {
    
 
   timerOne = millis();
    while(millis() - timerOne < 60000){
      int q=random(20,60);
      int b = random( 64,150);
      int p = random(5,10);
      int r = random(100, 2000);
      meteorRain(0x1C,0x69,0x24,p, b, true, q);
      delay(r);
    }
    setAll(0,0,0);
  }
  timeNow = millis();
}
}
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  setAll(0,0,0);
  setAll(30,0,0);
  for(int i = NUM_LEDS; i > 0; i--) {
    
    
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
        fadeToBlack(j, meteorTrailDecay );        
      }
    }
    
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        setPixel(i-j, red, green, blue);
      } 
    }
   
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
    // NeoPixel
    uint32_t oldColor;
    uint8_t r, g, b;
    int value;
    
    oldColor = strip.getPixelColor(ledNo);
    r = (oldColor & 0x00ff0000UL) >> 16;
    g = (oldColor & 0x0000ff00UL) >> 8;
    b = (oldColor & 0x000000ffUL);

    r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
    g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
    b=(b<=10)? 0 : (int) b-(b*fadeValue/256);
    
    strip.setPixelColor(ledNo, r,g,b);
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[ledNo].fadeToBlackBy( fadeValue );
 #endif  
}
 
// ---> here we define the effect function <---
// *** REPLACE TO HERE ***

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}


