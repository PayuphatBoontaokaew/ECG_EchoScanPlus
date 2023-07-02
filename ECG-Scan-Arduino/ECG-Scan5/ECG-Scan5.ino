#include <EasyKids3in1.h>
long instance1 = 0, timer;
double hrv = 0, interval = 0;
int value = 0, count = 0, hr = 72;
bool flag = 0;
int a = 0;
int lasta = 0;
int lastb = 0;
int LastTime = 0;
int status_sw = 0;
int status_ecg = 0;
int ecg1, ecg2, ecg3, ecg4, ecg5 ;
int TotalEcg;
#define sw 0
#define shutdown_pin 10
#define threshold 100 // to identify R peak
#define timer_value 10000 // 10 seconds timer to calculate hr
#define BUZZER 16
#define R 15
#define G 27
#define B 32

void setup() {
  EasyKids_Setup();
  Serial.begin(9600);
  pinMode(D2, INPUT); // Setup for leads off detection LO +
  pinMode(D1, INPUT); // Setup for leads off detection LO -
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  display.fillScreen(TFT_BLACK);
  display.setTextSize(6);
  welcomeSong();
}

void loop() {
  display.setUTF8Font(SarabunBold9EN, SarabunBold9TH , NULL);
  display.setTextColor(0xffff, 0x0);
  display.setTextSize(4);
  display.drawUTF8String(String("ECG"), 76, 20, GFXFF);
  display.setTextSize(2.9);
  display.drawUTF8String(String("EchoScanPlus+"), 34, 90, GFXFF);
  display.setTextSize(1.9);
  display.drawUTF8String(String("Please press the button"), 10, 210, GFXFF);
  analogWrite(B, 255);
  if (digitalRead(0) == 0) {
    display.fillScreen(TFT_BLACK);
    analogWrite(B, 255);
    status_ecg = 0;
    status_sw = 1;
  }

  while (status_sw == 1) {
    if (status_ecg < 6) {
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
          display.fillScreen(TFT_BLACK);
          beep();
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
        // show hr on display
        display.setUTF8Font(SarabunBold9EN, SarabunBold9TH , NULL);
        display.setTextColor(0xffff, 0x0);
        display.setTextSize(4);
        display.setCursor(25, 210);
        display.print(hr);
        display.print(" BPM");
        delay(1);
        a++;
      }
    }

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
    //calculate average from sensor for 5 minute
    else if (status_ecg == 6) {
      display.fillScreen(TFT_BLACK);
      TotalEcg = (ecg1 + ecg2 + ecg3 + ecg4 + ecg5) / 5;
      Serial.print("TotalEcg");
      Serial.print(",");
      Serial.println(TotalEcg);
      // BPM LOW
      if (TotalEcg < 59) {
        display.fillScreen(TFT_RED);
        display.setUTF8Font(SarabunBold9EN, SarabunBold9TH , NULL);
        display.setTextColor(0xffff, 0x0);
        display.setTextSize(5);
        display.setCursor(65, 100);
        display.print("Low");
        display.setTextSize(3.5);
        display.setCursor(10, 210);
        display.print(TotalEcg);
        display.print(" BPM");
        beep();
        analogWrite(R, 255);
        delay(4000);
        analogWrite(R, 0);
        display.fillScreen(TFT_BLACK);
        welcomeSong();
        status_sw = 0;
      }
      // BPM Normal
      else if (TotalEcg < 100 && TotalEcg > 60) {
        display.fillScreen(TFT_GREEN);
        display.setUTF8Font(SarabunBold9EN, SarabunBold9TH , NULL);
        display.setTextColor(0xffff, 0x0);
        display.setTextSize(5);
        display.setCursor(40, 100);
        display.print("Normal");
        display.setTextSize(3.5);
        display.setCursor(10, 210);
        display.print(TotalEcg);
        display.print(" BPM");
        analogWrite(G, 255);
        beep();
        delay(4000);
        analogWrite(G, 0);
        display.fillScreen(TFT_BLACK);
        welcomeSong();
        status_sw = 0;
      }
      // BPM Hight
      else if (TotalEcg > 101) {
        display.fillScreen(TFT_RED);
        display.setUTF8Font(SarabunBold9EN, SarabunBold9TH , NULL);
        display.setTextColor(0xffff, 0x0);
        display.setTextSize(5);
        display.setCursor(40, 100);
        display.print("Hight");
        display.setTextSize(3.5);
        display.setCursor(10, 210);
        display.print(TotalEcg);
        display.print(" BPM");
        beep();
        analogWrite(R, 255);
        delay(4000);
        analogWrite(R, 0);
        display.fillScreen(TFT_BLACK);
        welcomeSong();
        status_sw = 0;
      }
    }
  }
}
