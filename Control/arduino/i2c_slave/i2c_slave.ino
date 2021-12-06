//A4 SDA
//A5 SCL


#include <Wire.h>

#define SLAVE_ADDR 9

#define ANSWERSIZE 10
//5 characters -> 4 * 5 -> 10 bytes

void setup() {
  // I2C communication as Slave
  Wire.begin(SLAVE_ADDR);

  Wire.onReceive(receiveEvent);

  
  Serial.begin(9600);
  Serial.println("I2C Slave");

}


void receiveEvent(){
  //Read while data received
  while(0 < Wire.available()){
    byte x = Wire.read();
    }
  
  Serial.println("Receive event");
 }
void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
 
}
