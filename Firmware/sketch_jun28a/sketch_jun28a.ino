// Modified by: Ahmed Bahaa El-din bahaaahmed202@gmail.com 

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "--";
const char* password = "--";
const char* n8n_webhook = "https://script.google.com/macros/s/AKfycbwjJJHEr8n0Y9LwNNyBFskLejBUZkf7lk0e2o4reAHiTwJMWNZyEyGH2KLOinnyZQM9/exec";

MAX30105 sensor;

long lastBeat = 0;
float bpm = 0;
float beatAvg = 0;
int spo2 = 98;

// إضافة مصفوفة لتخزين متوسط النبض (عشان الرقم يثبت ويتحسب)
const byte RATE_SIZE = 4;
byte rates[RATE_SIZE];
byte rateSpot = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }

  if (!sensor.begin(Wire, I2C_SPEED_FAST)) { Serial.println("Sensor Error!"); while(1); }
  sensor.setup(0x1F, 4, 2, 400, 411, 4096);
}

void sendData(uint32_t ir, uint32_t red, float heartRate, int oxygen) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(n8n_webhook);
    http.addHeader("Content-Type", "application/json");
    
    String json = "{\"ir_value\": " + String(ir) + 
                  ", \"red_value\": " + String(red) + 
                  ", \"bpm\": " + String(heartRate) + 
                  ", \"spo2\": " + String(oxygen) + "}";
                  
    http.POST(json);
    http.end();
  }
}

void loop() {
  uint32_t ir = sensor.getIR();
  uint32_t red = sensor.getRed();

  if (ir > 50000) { 
    // الجزء ده هو اللي بيحسب النبض صح
    if (checkForBeat(ir) == true) {
      long delta = millis() - lastBeat;
      lastBeat = millis();
      bpm = 60 / (delta / 1000.0);
      
      /*if (bpm < 255 && bpm > 20) {
        rates[rateSpot++] = (byte)bpm;
        rateSpot %= RATE_SIZE;
        beatAvg = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++) beatAvg += rates[x];
        beatAvg /= RATE_SIZE;
      }*/
      // استبدل الجزء ده في كودك:
      if (bpm < 255 && bpm > 20) {
        rates[rateSpot++] = (byte)bpm;
        rateSpot %= RATE_SIZE;
        
        // تعديل: لو المصفوفة لسه مش مليانة، زود المتوسط بسرعة
        int sum = 0;
        int count = 0;
        for (byte x = 0 ; x < RATE_SIZE ; x++) {
            if(rates[x] > 0) {
                sum += rates[x];
                count++;
            }
        }
        beatAvg = sum / count;
      }
    }
    
    if (ir > 0) {
      float ratio = (float)red / (float)ir;
      spo2 = 110 - (ratio * 15);
    }
    
    // إرسال البيانات
    sendData(ir, red, beatAvg, spo2);
    
    Serial.print("BPM: "); Serial.print(beatAvg);
    Serial.print(" | SPO2: "); Serial.println(spo2);
  } else {
    Serial.println("Finger NOT detected");
  }
  // شلنا الـ delay الكبير عشان السنسور يلحق يلقط النبضات
}