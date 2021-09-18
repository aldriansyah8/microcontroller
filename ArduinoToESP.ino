//TODO
/* Ganti Parameter PH automatis
   Seting waktu pengocokan
   Seting waktu pengisian/pengosongan air
*/

#define button1 9
#define button2 8
#include <Servo.h>
Servo servo1, servo2;

const int relay1 = 3;
const int relay2 = 10;
const byte ledPin = 4;
const byte ledPin2 = 11;
unsigned long startMillis;
unsigned long phMillis = 0;//some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 1000;
const unsigned long phmillis = 1200;
int pos = 0;
int buttonState = 0;
long previousMillis = 0;
long interval = 15;
long interval2 = 30;
long shake = 10000;
long empty = 10000;
int ph_value = 2;
float Voltage;
float calibration_value = 21.90 - 0.75;
int avgval;
int buffer_arr[10], temp;
float ph_act;
String kondisi = "";
int kondisival = 3;

void singleLayer() {

  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    for (interval = 180; interval >= 0; interval -= 1) {
      for (pos = 50; pos <= 100; pos += 1 ) {
        servo1.write(pos);
        delay(3);
        servo2.write (180 - pos);
        delay(3);
        digitalWrite(ledPin, HIGH);
      }
      for (pos = 100; pos >= 50; pos -= 1 ) {
        servo1.write(pos);
        delay(3);
        servo2.write(180 - pos);
        delay(3);
        digitalWrite(ledPin, LOW);
      }

    }
    Serial.print("Etching Single Layer Sedang Berjalan");
    kondisival = 1;
    String kondisi = String(kondisival) + "\n";
    Serial1.print(kondisi);
    delay(10);
  }
}

void doubleLayer() {
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    for (interval = 180; interval >= 0; interval -= 1) {
      for (pos = 50; pos <= 100; pos += 1 ) {
        servo1.write(pos);
        delay(3);
        servo2.write (175 - pos);
        delay(3);
        digitalWrite(ledPin2, HIGH);
      }
      for (pos = 100; pos >= 50; pos -= 1 ) {
        servo1.write(pos);
        delay(3);
        servo2.write(175 - pos);
        delay(3);
        digitalWrite(ledPin2, LOW);
      }

    }
    kondisival = 2;
    String kondisi = String(kondisival) + "\n";
    Serial1.print(kondisi);

    delay(10);
  }
}

void phmeter() {
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A2);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++)
    avgval += buffer_arr[i];
  float volt = (float)avgval * 5.0 / 1024 / 6;
  ph_act = -5.70 * volt + calibration_value;

  String ph = "pH Val: ";
  String phval = ph + ph_act;
  String phvalue = String(ph_act) + "\n";
  if (millis() - startMillis >= period) {
    startMillis = millis();
    Serial1.print(phvalue);

    Serial.println(phval);
    Serial.println("");
    delay(10);
  }
  if (ph_act >= 10) { //WAJIB BANGET KUDU GANTI -B
    //valve
    for (empty = 10000; empty >= 0; empty -= 1) {
      digitalWrite(relay2, LOW);
      delay(10);
    }
    digitalWrite(relay2, HIGH);
    delay(10);
    //pompa
    for (shake = 10000; shake >= 0; shake -= 1) {
      digitalWrite(relay1, LOW);
      delay(10);
    }
    kondisival = 0;
    String kondisi = String(kondisival) + "\n";
    Serial1.print(kondisi);
    delay(10);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(ledPin, OUTPUT);          // define LED as an output
  digitalWrite(ledPin, LOW);
  pinMode(ledPin2, OUTPUT);          // define LED as an output
  digitalWrite(ledPin2, LOW);
  pinMode(ph_value, INPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  servo1.attach(6);
  servo2.attach(7);
  startMillis = 0;
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    digitalWrite(ledPin, !digitalRead(ledPin));  //if so, change the state of the LED.  Uses a neat trick to change the state
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }
  if (buttonState == 0) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }

  if (digitalRead(button2) == HIGH) {
    buttonState = 1;
    digitalWrite(ledPin2, HIGH);
    doubleLayer();
  } else if (digitalRead(button1) == HIGH) {
    buttonState = 1;
    digitalWrite(ledPin, HIGH);
    singleLayer();
  } else {
    buttonState = 0;
    interval = 15;
    servo1.write(90);
    delay(10);
    servo2.write(90);
  }
  phmeter();

  while (Serial1.available() > 0) {
    String inString = Serial1.readStringUntil('\n');

    //nerima dari esp

    if (inString == "1") {
      //run this code

      //valve
      for (empty = 1000; empty >= 0; empty -= 1) {
      digitalWrite(relay2, LOW);
      delay(10);
      digitalWrite(ledPin, HIGH);
      delay(10);
      digitalWrite(ledPin2, HIGH);
      delay(10);
      }
      digitalWrite(relay2, HIGH);
      //pompa
      delay(10);
      for (shake = 500; shake >= 0; shake -= 1) {
      digitalWrite(relay1, LOW);
      delay(10);
      digitalWrite(ledPin, HIGH);
      delay(10);
      digitalWrite(ledPin2, HIGH);
      delay(10);
      }
      digitalWrite(ledPin, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(relay1, HIGH);
      kondisival = 0;
      String kondisi = String(kondisival) + "\n";
      Serial1.print(kondisi);
      delay(10);
      Serial.println(inString);
      Serial.println("larutan diganti");

    } else if (inString == "3") {
      buttonState = 1;
      singleLayer();
    } else if (inString == "4") {
      buttonState = 1;
      doubleLayer();
    } else {
      Serial.print("Arduino received: ");
      Serial.println(inString);
      buttonState = 0;
      interval = 15;
      servo1.write(90);
      delay(10);
      servo2.write(90);
    }
  }
  buttonState = 0;
  interval = 15;
  digitalWrite(relay1, HIGH);
  delay(10);
}
