#define HWSERIAL Serial1
#define HWSERIAL2 Serial2

int relay1=6;
int relay2=5;
byte packet[] = {0,0,0,0};
int slaveID;
int functionID;
int crc;
int rede = 2;
int message;

static int listening = 0;


void listenToSlave() {
  static int field = 0;
  if (HWSERIAL.available() > 0) {
    if(field == 0){
      slaveID = int(HWSERIAL.read());
      Serial.print("slave ID: ");
      Serial.println(slaveID);
      field++;
      listening = 1;
    }
    else if (field == 1){
      functionID = int(HWSERIAL.read());    
      Serial.print("function ID: ");
      field++;
      listening = 1;
    }
    else if (field == 2){
      message = int(HWSERIAL.read());
      Serial.print("message: ");
      Serial.println(message);
      field++;
      listening = 1;
    }
    else if(field == 3){
      crc = int(HWSERIAL.read());
      Serial.print("crc: ");
      Serial.println(crc);
      //package = slaveID + functionID + message + crc;   //concatenating all strings
      //package.toCharArray(packet, 5);                   //from string to array of chars
      field = 0;
      listening = 0;
    }
    else  //flush the serial port
    {
      int t = HWSERIAL.read();
      delay(200);
      field = 0;
      listening = 0;
    }
  }
  else  //flush the serial port
    {
      int t = HWSERIAL.read();
      delay(200);
      field = 0;
    }
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  //HWSERIAL.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
}
void loop() {
  //listenToSlave();
    char incomingByte;
    if (Serial2.available() > 0) {
      incomingByte=Serial2.read();
      digitalWrite(relay2, HIGH);
      Serial.print("UART received: ");
      Serial.println(incomingByte);
      delay(5000);
      //digitalWrite(relay1, LOW);
      digitalWrite(relay2, LOW);
  }
 // delay(1000);
}
