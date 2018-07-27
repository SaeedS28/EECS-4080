#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int relay2=5; //port for the relay switch
double tempInCelsius=0; //temperature variable
#define HWSERIAL Serial1
#define HWSERIAL2 Serial2

void setup() {
  
  Serial.begin(9600);
  pinMode(relay2, OUTPUT);
  Serial1.begin(9600);
 // Serial.println("Relay getting activated based on the temperature");  
  mlx.begin();  
}

void loop() {
  //Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC()); 
  tempInCelsius=mlx.readObjectTempC();
  Serial.print("Temperature: ");Serial.print(tempInCelsius); Serial.println("*C");
  Serial.println();
  delay(500);
  if(tempInCelsius>=26.00){
    Serial.println("Temperature: exceeded 26 *C\nTurning off the power\n");
    digitalWrite(relay2, HIGH);
    Serial1.write(static_cast<byte>(tempInCelsius));
    delay(5000); //Five second delay for the first time detection.
    
    while(mlx.readObjectTempC()>=26.00){}
    digitalWrite(relay2, LOW);
    Serial.println("Power back on");
    delay(500);
  }
  
  //Serial.println();
  delay(500);

}

