#include "Arduino.h"
#include <AES.h>

#define ledPin 13 // from teensy 3.5 board
#define KEYBITS 256
#define HWSERIAL Serial1

unsigned char encryptionKeys[32];
unsigned char plaintext[16] = { 83, 111, 109, 101, 32, 80, 108, 97, 105, 110, 32, 84, 101, 120, 116, 33 };
unsigned char ciphertext[16];
unsigned long rk[RKLENGTH(KEYBITS)];

#include <Password.h>
#include <Keypad.h>
Password password = Password( "3992" );
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {14, 15, 16, 17}; //connect to the row pinouts of the teensy 3.5
byte colPins[COLS] = {18, 19, 20, 21}; //connect to the column pinouts of the teensy 3.5

//rs485 variables
int rede = 2;

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void guessPassword() {
  Serial.print("\nGuessing password... ");
  if (password.evaluate())
  {
//    digitalWrite(ledPin, HIGH);
//    delay(500);
//    digitalWrite(ledPin, LOW);
    digitalWrite(rede, HIGH);
    delay(500);
    Serial1.write(69);
    delay(200);
    digitalWrite(rede, LOW);
    delay(500);

    Serial.println("Valid PIN ");
    Serial.println("");//
    password.reset();
  }
  else {
    digitalWrite(ledPin, LOW);
    Serial.println("Invalid PIN ");
    password.reset();
  }
}

void printHex(int num, int precision) {
  char tmp[16];
  char format[128];
  sprintf(format, "0x%%.%dX", precision);
  sprintf(tmp, format, num);
  Serial.print(tmp);
}

void setup() {
  //digitalWrite(ledPin, LOW);   // sets the LED on

  //Serial.println("Encryption Keys:");
  for (int i = 0; i < 32; i++)
  {
    encryptionKeys[i] = (char)(i + 200);
    printHex(encryptionKeys[i], 1); //why do we want to print hex????
  }

  Serial.begin(9600);
  Serial1.begin(9600);
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  keypad.setDebounceTime(250);
  pinMode(rede, OUTPUT);
}

void loop() {
  keypad.getKey(); //are you storing this  in a variable
}

//take care of some special events
void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState())
  {
    case PRESSED:
      Serial.print("\nPressed:");

      //Serial.println(" ");

      plaintext[0] = eKey;

      // the encryption code
      //unsigned long rk[RKLENGTH(KEYBITS)];
      int nrounds = aesSetupEncrypt(rk, encryptionKeys, KEYBITS);
      aesEncrypt(rk, nrounds, plaintext, ciphertext);
      
      
        printHex(ciphertext[0], 1);
        
        Serial.print("\n");
  
        switch (eKey) {
        case '#': guessPassword(); break;
        default:
          password.append(eKey);
      }
  }
}
