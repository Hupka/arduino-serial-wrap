#include <serialwrap.h>

void setup() {
  // put your setup code here, to run once:
  SerialWrap::init(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Integer senden
  SerialWrap::transferInt(30);

  delay(500);

  SerialWrap::transferInt(-31);

  delay(500);

  // Integer mit Kommastellen senden --> werden abgeschnitten
  SerialWrap::transferInt(32.32);

  delay(500);

  SerialWrap::transferInt(-33.33);

  delay(500);

  // Float senden
  SerialWrap::transferFloat(34.34);

  delay(500);

  SerialWrap::transferFloat(-35.35);

  delay(500);

  // String senden
  SerialWrap::transferString("Wieso denn blos?!");

  delay(500);

  SerialWrap::transferString(String(-1232121));

  delay(500);

  SerialWrap::transferString(String(-0.3421, DEC));

  delay(500);
}


