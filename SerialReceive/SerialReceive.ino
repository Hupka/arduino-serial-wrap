#include <serialwrap.h>

void setup() {
  // put your setup code here, to run once:
  SerialWrap::init(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  SerialWrap::package pac;
  pac = SerialWrap::receive();
  if(pac.type == 'i') {
    Serial.println(pac.i);
  } else if(pac.type == 'f') {
    Serial.println(pac.f);
  } else if(pac.type == 's') {
    Serial.println(pac.s);
  }
  delay(500);
}


