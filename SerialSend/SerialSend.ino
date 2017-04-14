#include <serialsend.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

//#ifdef MEGA
    void loop() {
      // put your main code here, to run repeatedly:
      //SerialSend::transferInt(100);

      Serial.println("ADRIAN");
       package paket;
        paket = SerialSend::receive();
        if(paket.type == 'i') {
          Serial.println(paket.i);
        } else if(paket.type == 'f') {
          Serial.println(paket.f);
        } else if(paket.type == 's') {
          Serial.println(paket.s);
        }
        delay(500);
    }
//#elif UNO
//    void loop() {
//      // put your main code here, to run repeatedly:
//      SerialSend::transferInt(100);
//
//      delay(1000);
//
//      SerialSend::transferFloat(123.8912712);
//
//      delay(1000);
//
//      SerialSend::transferString("Hallo hier ihr!");
//
//      delay(1000);
//    }
//#endif


