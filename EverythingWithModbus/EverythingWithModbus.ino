#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int relay2=5; //port for the relay switch
int relay1=6;
double tempInCelsius=0; //temperature variable
#define HWSERIAL Serial1
#define HWSERIAL Serial2

static int flick = 0;
byte packet[] = {0,0,0,0};
int slaveID;
int functionID;
int crc;
int rede = 2;
int message;


void setup() {
  Serial.begin(9600);
  pinMode(relay2, OUTPUT);
  pinMode(relay1,OUTPUT);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial.println("Relay getting activated based on the temperature");  
  mlx.begin();  
}

void reportDoorEntry(){
  char incomingByte;
    if (Serial2.available() > 0) {
      incomingByte=Serial2.read();
      if(char(incomingByte) == 'E'){
        writeToMaster(96,20);
        digitalWrite(relay2, HIGH);
        //digitalWrite(relay1, HIGH);
        Serial.print("UART received: ");
        Serial.println(incomingByte);
        delay(5000);
        //digitalWrite(relay1, LOW);
        digitalWrite(relay2, LOW);
      }
      else{
        int t = Serial2.read();       //flush the serial 2 port
        incomingByte = 0;
      }
  }
}

void reportTemperature(){
  tempInCelsius=mlx.readObjectTempC();
  Serial.print("Temperature: ");Serial.print(tempInCelsius); Serial.println("*C");
  Serial.println();
  delay(500);
  if(tempInCelsius>=30.00){
    Serial.println("Temperature: exceeded 27 *C\nTurning off the power\n");
    digitalWrite(relay1, HIGH);
    Serial1.write(static_cast<byte>(tempInCelsius));
    //delay(5000); //Five second delay for the first time detection.

    //code to send to slave Andres the change in temperature
    if(flick == 0){
      writeToMaster(tempInCelsius,10);
      flick = 1;
    }
    
    while(mlx.readObjectTempC()>=30.00){}
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
  reportDoorEntry();
  delay(100);

}
