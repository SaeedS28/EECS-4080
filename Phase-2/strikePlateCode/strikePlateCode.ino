#include "Arduino.h"
#include <AES.h>
#define KEYBITS 256
//#define HWSERIAL Serial1
#define HWSERIAL2 Serial2
unsigned long rk[RKLENGTH(KEYBITS)]; //for decryption

static int field = 0;
int garbage1;
int garbage2;
int cipherKey;
int nrounds;
//int crc;
int rede = 2;
//int message;

static int listening = 0;   //not sure about this one

void listenToSlave() {
  static int field = 0;
  if (Serial2.available() > 0) {
    if (field == 0) {
      garbage1 = int(Serial2.read());
      Serial.print("Garbage 1: ");
      Serial.println(garbage1);
      field++;
      listening = 1;
    }
    else if (field == 1) {
      garbage2= int(Serial2.read());
      Serial.print("Garbage 2: ");
      Serial.println(garbage2);
      field++;
      listening = 1;
    }
    else if (field == 2) {
      cipherKey = int(Serial2.read());
      Serial.print("CipherKey: ");
      Serial.println(cipherKey);
      field++;
      listening = 1;
    }
    else if (field == 3) {
      nrounds = int(Serial2.read());
      Serial.print("Number of rounds: ");
      Serial.println(nrounds);
      field = 0;
      listening = 0;
    }
    else                    //flush the serial port
    {
      int t = Serial2.read();
      delay(200);
      field = 0;
      listening = 0;
    }
  }
  else                      //flush the serial port
  {
    int t = Serial2.read();
    delay(200);
    field = 0;
  }
}


void setup() {
  Serial.begin(9600);
  //Serial1.begin(9600);
  Serial2.begin(9600);
  //HWSERIAL.begin(9600);

}
void loop() {
  //listenToSlave();
  //    int incomingByte;
  //    if (Serial2.available() > 0) {
  //      incomingByte=Serial2.read();
  //      digitalWrite(relay2, HIGH);
  //      Serial.print("UART received: ");
  //      Serial.println(incomingByte);
  //      delay(5000);
  //      //digitalWrite(relay1, LOW);
  //      digitalWrite(relay2, LOW);
  //  }
  listenToSlave();
  delay(100);
}
