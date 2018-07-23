#define HWSERIAL Serial1

int relay1=6;
int relay2=5;

void setup() {
  Serial.begin(9600);
  HWSERIAL.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
}
void loop() {
    int incomingByte;
        
  while (HWSERIAL.available() > 0) {
    incomingByte = HWSERIAL.read();
    Serial.print("UART received: ");
    Serial.println(char(incomingByte));
    //digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    Serial.print("UART received: ");
    delay(5000);
    //digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
  }
 // delay(1000);
}
