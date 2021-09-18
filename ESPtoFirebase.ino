#include <ArduinoJson.h>
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Stangkle_House"
#define WIFI_PASSWORD "majumundurkenajuga888?!"

/* 2. Define the API Key */
#define API_KEY "AIzaSyDGQ5BCQcPeTa6aUd5CbbhB7hbDocJjGoc"

/* 3. Define the RTDB URL */
#define DATABASE_URL "tabisa-f72c3-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "aldriansyah@gmail.com"
#define USER_PASSWORD "123123"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  //Or use legacy authenticate method
  //Firebase.begin(DATABASE_URL, "<database secret>");
}

void loop() {
  Serial.flush();
  static uint8_t flag = 0, GantiLarutan, TipePCB;
  static float NilaiPH;
  static String KondisiLarutan;
  static uint32_t datamillis, millisRst, datarst = 1000, timerst = 1500;
  KondisiLarutan = "Baik";

  if (millis() - millisRst > datarst) {
    datamillis = millis();
    if (datamillis - millisRst > timerst) {
      receivedata();
    }
      if (Firebase.ready()) {
        //GantiLarutan
        if (Firebase.getInt(fbdo, "/GantiLarutan")) {
          if (fbdo.dataType() == "int") {
            GantiLarutan = fbdo.intData();
            if (GantiLarutan == 1) {
              String sendLarutan = String(GantiLarutan) + "\n";
              Serial.flush();
              Serial.print(sendLarutan);
              Serial.flush();
              GantiLarutan = 2;
              Serial.printf("GantiLarutan = ... %s\n", Firebase.setInt(fbdo, "/GantiLarutan", GantiLarutan) ? "\n" : fbdo.errorReason().c_str());
            }
          }
        } else {
          Serial.println(fbdo.errorReason());
        }
        //TipePCB
        if (Firebase.getInt(fbdo, "/TipePCB")) {
          if (fbdo.dataType() == "int") {
            TipePCB = fbdo.intData();
            if (TipePCB == 3) {
              String sendLarutan = String(TipePCB) + "\n";
              Serial.flush();
              Serial.print(sendLarutan);
              Serial.flush();
              TipePCB = 2;
              Serial.printf("Tipe PCB = ... %s\n", Firebase.setInt(fbdo, "/TipePCB", TipePCB) ? "\n" : fbdo.errorReason().c_str());
            } else if (TipePCB == 4) {
              String sendLarutan = String(TipePCB) + "\n";
              Serial.flush();
              Serial.print(sendLarutan);
              Serial.flush();
              TipePCB = 2;
              Serial.printf("Tipe PCB = ... %s\n", Firebase.setInt(fbdo, "/TipePCB", TipePCB) ? "\n" : fbdo.errorReason().c_str());
            }
          }
        } else {
          Serial.println(fbdo.errorReason());
        }
        /*nilaiPH
          if (Firebase.getInt(fbdo, "/NilaiPH")) {
          if (fbdo.dataType() == "float") {
            Serial.print("NilaiPH = ");
            Serial.println(fbdo.floatData());
            NilaiPH = fbdo.floatData();
            if (NilaiPH >= 3) {
              //Serial.printf("Kondisi Larutan = ... %s\n", Firebase.setString(fbdo, "/KondisiLarutan", KondisiLarutan) ? "ok" : fbdo.errorReason().c_str());
              flag = 1;
            } else if (NilaiPH == 0) {
              flag = 0;
            }
          }
          } else {
          Serial.println(fbdo.errorReason());
          }*/
      }
      millisRst = millis();
    }
    receivedata();
  }

void receivedata() {
  while (Serial.available() > 0) {
    String inString = Serial.readStringUntil('\n');
    if (inString == "0") {
      Serial.flush();
      Serial.printf("Kondisi = ... %s\n", Firebase.setString(fbdo, "/Kondisi", inString) ? "\n" : fbdo.errorReason().c_str());
      Serial.flush();
      delay(50);
    } else if (inString == "1") {
      Serial.flush();
      Serial.printf("Kondisi = ... %s\n", Firebase.setString(fbdo, "/Kondisi", inString) ? "\n" : fbdo.errorReason().c_str());
      Serial.flush();
      delay(50);
    } else if (inString == "2") {
      Serial.flush();
      Serial.printf("Kondisi = ... %s\n", Firebase.setString(fbdo, "/Kondisi", inString) ? "\n" : fbdo.errorReason().c_str());
      Serial.flush();
      delay(50);
    } else {
      Serial.flush();
      Serial.printf("NilaiPH = ... %s\n", Firebase.setString(fbdo, "/NilaiPH", inString) ? "\n" : fbdo.errorReason().c_str());
      Serial.flush();
      delay(50);
    }
  }
}

/*void jsondata() {
  while (Serial.available()) {
    StaticJsonDocument<300> doc;
    DeserializationError err = deserializeJson(doc, Serial);
    if (err == DeserializationError::Ok)
    {
      NilaiPH = doc["NilaiPH"];
      TipePCB = doc["TipePCB"];



      if (NilaiPH == 3 && flag == 0) {
        count++;
      }

      if (Firebase.ready())
      {
        Serial.printf("Set air NilaiPH... %s\n", Firebase.setFloat(fbdo, "/NilaiPH", NilaiPH) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Set pir counter... %s\n", Firebase.setInt(fbdo, "/counter", count) ? "ok" : fbdo.errorReason().c_str());

        //FirebaseJson json;
        //json.add("tanggal", tanggal).add("waktu", waktu).add("nama", nama).add("nomor", nomor).add("nominal", nominal);
        //Serial.printf("Push json... %s\n", Firebase.pushJSON(fbdo, "/test/push", json) ? "ok" : fbdo.errorReason().c_str());

        //json.set("value", count + 100);
        //Serial.printf("Update json... %s\n\n", Firebase.updateNode(fbdo, String("/test/push/" + fbdo.pushName()), json) ? "ok" : fbdo.errorReason().c_str());
      }
    }
    else
    {
      Serial.println(err.c_str());

      while (Serial.available() > 0)
        Serial.read();
    }
  }

  }
*/
