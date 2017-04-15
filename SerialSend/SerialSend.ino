#include <serialwrap.h>

void setup() {
  // put your setup code here, to run once:
  SerialWrap::init(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Integer senden
  SerialWrap::transferInt(30);

  delay(1000);

  SerialWrap::transferInt(-31);

  delay(1000);

  // Integer mit Kommastellen senden --> werden abgeschnitten
  SerialWrap::transferLong(3232323232);

  delay(1000);

  SerialWrap::transferLong(-333333333);

  delay(1000);

  // Float senden
  SerialWrap::transferFloat(34.34);

  delay(1000);

  SerialWrap::transferFloat(-35.35);

  delay(1000);

  // String senden
  SerialWrap::transferString("Wieso denn blos?!");

  delay(1000);

  SerialWrap::transferString(String(-1232121));

  delay(1000);

  SerialWrap::transferString(String(-0.3421, DEC));

  delay(1000);
}


