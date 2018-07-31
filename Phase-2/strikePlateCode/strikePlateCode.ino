#include "Arduino.h"
#include <AES.h>
#define KEYBITS 256
#define HWSERIAL Serial1
#define HWSERIAL2 Serial2
unsigned long rk[RKLENGTH(KEYBITS)]; //for decryption

static int field = 0;
int cipherKey;
int nrounds;
int crc;
int rede = 2;
int message;

static int listening = 0;


void listenToSlave() {

  if (HWSERIAL2.available() > 0) {
    if (field == 0) {
      cipherKey = int(HWSERIAL2.read());
      //Serial.print("slave ID: ");
      Serial.println(cipherKey);
      field++;
      listening = 1;
    }
    else if (field == 1) {
      nrounds = int(HWSERIAL2.read());
      //Serial.print("function ID: ");
      Serial.println(nrounds);
      field++;
      listening = 1;
    }
    else if (field == 2) {
      message = int(HWSERIAL2.read());
      //Serial.print("message: ");
      //Serial.println(message);
      field++;
      listening = 1;
    }
    else if (field == 3) {
      crc = int(HWSERIAL2.read());
      //Serial.print("crc: ");
      //Serial.println(crc);
      field = 0;
      listening = 0;
    }
    else                    //flush the serial port
    {
      int t = HWSERIAL2.read();
      delay(200);
      field = 0;
      listening = 0;
    }
  }
  else                      //flush the serial port
  {
    int t = HWSERIAL2.read();
    delay(200);
    field = 0;
  }
}


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
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
