int led = 13;

//5 to relay 2
//6 to relay 1

int relay1=6;
int relay2=5;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(relay1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay2, HIGH);
  delay(5000);               // wait for a second
  //digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(relay1, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay2, LOW);
  delay(5000);               // wait for a second
}
