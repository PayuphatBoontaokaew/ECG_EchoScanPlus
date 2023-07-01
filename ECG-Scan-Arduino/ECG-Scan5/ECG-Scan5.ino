#include <EasyKids3in1.h>
long instance1 = 0, timer;
double hrv = 0, hr = 72, interval = 0;
int value = 0, count = 0;
bool flag = 0;
int a = 0;
int lasta = 0;
int lastb = 0;
int LastTime = 0;
int status_sw = 0;
int status_ecg = 0;
int ecg1, ecg2, ecg3, ecg4, ecg5 ;
float TotalEcg;
#define sw 0
#define shutdown_pin 10
#define threshold 100 // to identify R peak
#define timer_value 10000 // 10 seconds timer to calculate hr

void setup() {
  EasyKids_Setup();
  Serial.begin(9600);
  pinMode(D2, INPUT); // Setup for leads off detection LO +
  pinMode(D1, INPUT); // Setup for leads off detection LO -
  display.fillScreen(TFT_BLACK);
  display.setTextSize(6);
}

void loop() {
  if (digitalRead(0) == 0) {
    status_sw = 1;
  }
  while (status_sw == 1) {
    //    check sensor
    if ((digitalRead(D2) == 1) || (digitalRead(D1) == 1)) {

      Serial.println("leads off!");

      digitalWrite(shutdown_pin, LOW); //standby mode

      instance1 = micros();

      timer = millis();

    }
    // seror ready!
    else {

      digitalWrite(shutdown_pin, HIGH); //normal mode

      value = analog(A0);

      value = map(value, 250, 400, 0, 100); //to flatten the ecg values a bit

      //      calculate hr from sensor

      if ((value > threshold) && (!flag)) {

        count++;

        Serial.println("in");

        flag = 1;

        interval = micros() - instance1; //RR interval

        instance1 = micros();

      }

      else if ((value < threshold)) {

        flag = 0;

      }
      // clear hr count
      if ((millis() - timer) > 10000) {

        hr = count * 6;

        timer = millis();

        count = 0;
        status_ecg++;

      }
      //      calculate hrv from sensor

      hrv = hr / 60 - interval / 1000000;

      //       Serial.print(hr);
      //          Serial.print(",");
      //           Serial.print(hrv);
      //           Serial.print(",");
      //      Serial.println(value);
      //clear Potgraph count
      if (a > 240)
      {
        display.fillScreen(TFT_BLACK);
        a = 0;
        lasta = a;
      }
      //      Potgraph
      int b = 90 - (value / 46);

      display.drawLine(lasta, lastb, a, b,  0xFFFF);
      lastb = b;
      lasta = a;

      //collect values from sensors
      if (status_ecg == 1) {
        ecg1 = hr;
        Serial.print("ecg1");
        Serial.print(",");
        Serial.println(ecg1);
      }

      else if (status_ecg == 2) {
        ecg2 = hr;
        Serial.print("ecg2");
        Serial.print(",");
        Serial.println(ecg2);
      }

      else if (status_ecg == 3) {
        ecg3 = hr;
        Serial.print("ecg3");
        Serial.print(",");
        Serial.println(ecg3);
      }

      else if (status_ecg == 4) {
        ecg4 = hr;
        Serial.print("ecg4");
        Serial.print(",");
        Serial.println(ecg4);
      }

      else if (status_ecg == 5) {
        ecg5 = hr;
        Serial.print("ecg5");
        Serial.print(",");
        Serial.println(ecg5);
      }

      else if (status_ecg == 6) {
        TotalEcg = ecg1 + ecg2 + ecg3 + ecg4 + ecg5 / 5;
      }

      delay(1);
      a++;

    }
  }
}
