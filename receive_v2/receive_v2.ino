#define HWSERIAL Serial1
double incomingByte;
int rede = 2;
byte packet[] = {1,2,34,103,54};

void setup() {
 Serial.begin(9600);
 Serial1.begin(9600);
 pinMode(rede, OUTPUT);
 }

void loop() {
   //digitalWrite(rede, LOW);
   
   if (HWSERIAL.available() > 0) {
     incomingByte = HWSERIAL.read();
     Serial.print("UART received: ");
     Serial.println(char(incomingByte)); //only add char when ascii values are recieved
     /*if(char(incomingByte)=='y'){
        digitalWrite(rede, HIGH);
        Serial1.write("i got your letter   digitalWrite(rede, LOW); 
     }*/
   }
   else{
   digitalWrite(rede, HIGH);
   delay(500);
    Serial1.write((uint8_t*)packet, sizeof(packet));
    //Serial1.write(69);
    delay(200);
    digitalWrite(rede, LOW);
    delay(500);
   }
}
