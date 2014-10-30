/**
 ******************************************************************************
 * @file    pumpkin-push.cpp
 * @author  BDub
 * @version V1.0.0
 * @date    30-October-2014
 * @brief   Motion Activated Pumpkin Push Notifications
 ******************************************************************************
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
 ******************************************************************************
 */

// Only if you want to code Like a Boss in the web IDE
#pragma SPARK_NO_PREPROCESSOR

// Necessary only if building code locally
#include "application.h"

// This #include statement was automatically added by the Spark IDE.
// To build locally you'll need some files from
// https://github.com/nmattisson/HttpClient
#include "HttpClient/HttpClient.h"

// Create an instance of the HTTPCLIENT to work with called "http"
HttpClient http;

// Uncomment for serial debugging
// #define SERIAL_DEBUG

#define HOSTNAME "technobly.com"
#define PATHNAME "/pumpkin/pushover.php"
uint32_t startTime = 0;
#define DEBOUNCE_TIME 10 // number of 5ms ticks before button is debounced
bool PIR_ACTIVE = false;
uint8_t pir_cnt = 0; // used to count up debounce time
bool s = 0; // output state of D7 led
bool PUMPKIN_ARMED = true;   // enable system by default
int count = 1;  // number of visitors

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    //  { "Content-Type", "application/json" },
    //  { "Accept" , "application/json" },
    { "Accept" , "*/*"},
    { "Referer" , HOSTNAME},
    { "Content-Type", "application/x-www-form-urlencoded" },
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;

// prototypes
bool pirActive();
int pushNotify(const char* title, const char* message);
int setOn(String cmd);
int setOff(String cmd);

void setup() {
    startTime = millis(); // capture the time that our app starts
    pinMode(D0,INPUT);
    pinMode(D7,OUTPUT);
    
#ifdef SERIAL_DEBUG
    Serial.begin(9600); // Make sure serial terminal is closed before powering up Core
    while(!Serial.available()) SPARK_WLAN_Loop(); // Open serial terminal now, and press ENTER
#endif
    
    // POST to https://api.spark.io/v1/devices/DEVICE_ID/on?access_token=12341234
    Spark.function("on", setOn);      // call this function to enable the system
    // POST to https://api.spark.io/v1/devices/DEVICE_ID/off?access_token=12341234
    Spark.function("off", setOff);    // call this function to disable the system
    
} // end setup()

void loop() {
    
  // Check if PIR is active
  if (pirActive() == true) {
    
    // Push notify!
    if (PUMPKIN_ARMED == true) {
        int status = 0;
        int attempts = 0;
        status = pushNotify("Pumpkin", "Trick or Treat");
        // If we don't see a successful response, try again 5 more times.
        while ( status != 200 && attempts++ < 5 ) {
            // If this failed for some reason, try again in 3 seconds, bail after 5 times.
            startTime = millis();
            while (millis() - startTime < 3000UL) {
                digitalWrite(D7,HIGH);
                delay(100);
                digitalWrite(D7,LOW);
                delay(100);
            }
            SPARK_WLAN_Loop();  // process background tasks to stay connected to Spark Cloud
            status = pushNotify("Pumpkin", "Trick or Treat");   // retry
        }
    }
        
    // Do some flashy stuff
    RGB.control(true);
    for (uint8_t x=0; x<10; x++) { // BLINK ORANGE
        RGB.color(255,100,0);   // orange
        delay(200);
        RGB.color(0,0,0);       // off
        delay(200);
    }
    RGB.control(false);
  }
    
  // Make it blinky to indicate everything is running, and not locked up
  if (millis() - startTime > 1000UL) {
    startTime = millis();
    s = !s;
    digitalWrite(D7,s);
  }
  
} // end loop()

/* Read and debounce PIR input
 * Only accumulate an active PIR if pir_state is inactive
 * Technically the PIR sensor is heavily debounced already... 
 * But this is good coding if you wanted to use a different sensor
 */
bool pirActive() {
  
  if(digitalRead(D0) == HIGH && !PIR_ACTIVE) {
    pir_cnt++;
    if(pir_cnt > DEBOUNCE_TIME) {
      PIR_ACTIVE = true; // PIR input is active
      return 1;
    }
  }
  else if(digitalRead(D0) == LOW) {
    if(pir_cnt > 0) {
      pir_cnt--;
    }
    else {
      PIR_ACTIVE = false;
    }
  }
  return 0;
}

int pushNotify(const char* title, const char* message) {
  
    // Build the query string
    char querystring[140] = "title=";
    strcat(querystring,title);
    strcat(querystring,"&message=");
    strcat(querystring,message);
  
#ifdef SERIAL_DEBUG
    Serial.print("Query string: ");
    Serial.println(querystring);
#endif

    // Request path and body can be set at runtime or at setup.
    request.hostname = HOSTNAME;
    request.port = 80;
    request.path = PATHNAME;

    // The library also supports sending a body with your request:
    request.body = querystring;
    
    // POST request
    http.post(request, response, headers);
    
#ifdef SERIAL_DEBUG
    Serial.print("Application>\tResponse status: ");
    Serial.println(response.status);

    Serial.print("Application>\tHTTP Response Body: ");
    Serial.println(response.body);
#endif    

    return response.status; // typically this will be 200 if successful.
}

int setOn(String cmd) {     // enable the system
    PUMPKIN_ARMED = true;
    return 200;
}

int setOff(String cmd) {    // disable the system
    PUMPKIN_ARMED = false;
    return 200;
}
