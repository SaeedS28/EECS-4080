#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int relay2=5; //port for the relay switch
int relay1=6;
double tempInCelsius=0; //temperature variable
#define HWSERIAL Serial1
#define HWSERIAL Serial2

static int listening = 0;   //not sure about this one
unsigned char ciphertext[16] = {254, 21, 174, 42, 40, 26, 100, 56, 90, 119, 129, 134, 18, 165, 165, 100};
unsigned char keyText[16];
unsigned char plaintext[16];

int reportDoor=0;
static int flick = 0;
byte packet[] = {0,0,0,0};
int slaveID;
int sID;
int functionID;
int fID;
int crc;
int rede = 2;
int message;
int cipherKey;
int nrounds;

void setup() {
  Serial.begin(9600);
  pinMode(relay2, OUTPUT);
  pinMode(relay1,OUTPUT);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial.println("Relay getting activated based on the temperature");  
  mlx.begin();  
}

void listenToSlave() {
  static int field = 0;
  if (Serial2.available() > 0) {
    if (field == 0) {
      sID = int(Serial2.read());
      Serial.print("Slave ID: ");
      Serial.println(slaveID);
      field++;
      listening = 1;
    }
    else if (field == 1) {
      fID = int(Serial2.read());
      Serial.print("Function ID: ");
      Serial.println(fID);
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
      reportDoor=1;   //packet received fully
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

void reportDoorEntry(){
  if (sID == 2) {
      Serial.println("Slave ID is 2");
      if (fID == 10) {
        //decryption time boys
        Serial.println("Reached");      
//        ciphertext[0] = cipherKey-1;
//        aesDecrypt(rk, nrounds, ciphertext, keyText);
//          if(keyText[0]==key){
//            Serial.println("decrypted successfully. even though it fucking sucks");
//          }
      } 
  }
}

void reportTemperature(){
  tempInCelsius=mlx.readObjectTempC();
  Serial.print("Temperature: ");Serial.print(tempInCelsius); Serial.println("*C");
  Serial.println();
  delay(500);
  if(tempInCelsius>=27.00){
    Serial.println("Temperature: exceeded 27 *C\nTurning off the power\n");
    digitalWrite(relay1, HIGH);
    Serial1.write(static_cast<byte>(tempInCelsius));
    //delay(5000); //Five second delay for the first time detection.

    //code to send to slave Andres the change in temperature
    if(flick == 0){
      writeToMaster(tempInCelsius,10);
      flick = 1;
    }
    
    while(mlx.readObjectTempC()>=27.00){}
    digitalWrite(relay1, LOW);
    Serial.println("Power back on");
    delay(500);
  }
  else{
    if(flick == 1){
      writeToMaster(tempInCelsius, 10);
      flick = 0;
    }
  }
}

void writeToMaster(double message, int functionID) {
  packet[0] = 1; //slave id is 1 for Andres
  packet[1] = functionID;  //function code == 10 to indicate temperature reading is sent
  packet[2] = int(message);  //actual temperature reading
  packet[3] = 69;   //crc but serves no purpose atm.
  digitalWrite(rede, HIGH);
  delay(500);
  Serial1.write((uint8_t*)packet, sizeof(packet));
  delay(200);
  digitalWrite(rede, LOW);
  delay(500);
}

void loop() {
  reportTemperature(); 
  delay(100);
  listenToSlave();
  delay(100);
  if(reportDoor==1){
    reportDoorEntry();
    reportDoor=0;
  }
  delay(100);
}
