#include "Arduino.h"
#include <AES.h>
#define KEYBITS 256
//#define HWSERIAL Serial1
#define HWSERIAL2 Serial2
unsigned long rk[RKLENGTH(KEYBITS)]; //for decryption

unsigned char ciphertext[16]; //= {76, 21, 174, 42, 40, 26, 100, 56, 90, 119, 129, 134, 18, 165, 165, 100};
unsigned char keyText[16] = {14, 111, 109, 101, 32, 80, 108, 97, 105, 110, 32, 84, 101, 120, 116, 33};
unsigned char plaintext[16];
//static int field = 0;
int slaveID;
int functionID;
int cipherKey;
int nrounds;
//int crc;
int rede = 2;
//int message;

static int listening = 0;   //not sure about this one
unsigned char encryptionKeys[32];

void clearPacket() {
  slaveID = 0;
  functionID = 0;
  cipherKey = 0;
  nrounds = 0;
}

void listenToSlave() {
  static int field = 0;
  if (Serial2.available() > 0) {
    if (field == 0) {
      slaveID = int(Serial2.read());
      Serial.print("Slave ID: ");
      Serial.println(slaveID);
      field++;
      listening = 1;
    }
    else if (field == 1) {
      functionID = int(Serial2.read());
      Serial.print("Function ID: ");
      Serial.println(functionID);
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
  for (int i = 0; i < 32; i++)
  {
    //fetches the keys for decryption
    encryptionKeys[i] = (char)(i + 200);
  }
}
void loop() {
  //Listens to device
  listenToSlave();
  if (listening == 0) {
    //check to see if the packet received is good
    if (slaveID == 2) {
      Serial.println("Slave ID is 2");
      if (functionID == 10) {
        //decryption time boys
        
        //Test encryption for the purposes of checking
        int nrounds = aesSetupEncrypt(rk, encryptionKeys, KEYBITS);
        aesEncrypt(rk, nrounds, keyText, ciphertext); //Works
        for(int i=0;i<16;i++)
          Serial.println(ciphertext[i]);
      }
      Serial.println();
      //Decryption
      aesDecrypt(rk, nrounds, ciphertext, keyText);

      for(int i=0;i<16;i++)
          Serial.println(keyText[i]);
      
      clearPacket();
    }
    delay(100);

    //decryption
  }
}
