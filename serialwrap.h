#pragma once

#include <Arduino.h>

namespace SerialWrap {

    struct package {
        char type = ' ';
        float f = 0;
        int i = 0;
        long l = 0;
        String s = "";
    };

    float ten_shift = 10000.0;

    void init (unsigned int baudrate) {
        Serial.begin(baudrate);
    }

    void transferFloat(float f) {
        
        long sendVal = f * ten_shift;

        byte * b = (byte *) &sendVal;
        Serial.print("f:");
        Serial.write(b[0]);
        Serial.write(b[1]);
        Serial.write(b[2]);
        Serial.write(b[3]);
        delay(100);
    }

    void transferInt(int i) {
        byte * b = (byte *) &i;
        Serial.print("i:");
        Serial.write(b[0]);
        Serial.write(b[1]);
        Serial.write(b[2]);
        Serial.write(b[3]);
        delay(100);
    }

    void transferLong(long l) {
        byte * b = (byte *) &l;
        Serial.print("l:");
        Serial.write(b[0]);
        Serial.write(b[1]);
        Serial.write(b[2]);
        Serial.write(b[3]);
        delay(100);
    }

    void transferString(String str) {
        // Length (with one extra character for the null terminator)
        int str_len = str.length() + 1;

        // Prepare the character array (the buffer) 
        char char_arr[str_len];

        // Copy it over 
        str.toCharArray(char_arr, str_len);

        Serial.print("s:");

        // transfer string length
        byte * b = (byte *) &str_len;
        Serial.write(b[0]);
        Serial.write(b[1]);
        Serial.write(b[2]);
        Serial.write(b[3]);

        Serial.print(":");

        for (int i=0; i<str_len; i++) {
            Serial.write(char_arr[i]);
        }

        delay(100);
    }

    SerialWrap::package receive() {
        SerialWrap::package ret_pack;
        
        if(Serial.available() > 0) {
            char inByte = Serial.read();
            if(inByte == 'f') {
                // we expect data with this format f:XXXX
                ret_pack.type = 'f';
                
                Serial.read(); // discard ':'

                byte indata[4] = { 0 };
                indata[0] = Serial.read();
                indata[1] = Serial.read();
                indata[2] = Serial.read();
                indata[3] = Serial.read();

                long g;
                memcpy(&g, &indata, sizeof(g));
                ret_pack.f =  g / ten_shift;
            } else if(inByte == 'i') {
                // we expect data with this format i:XXXX
                ret_pack.type = 'i';
                
                Serial.read(); // discard ':'

                byte indata[4] = { 0 };
                indata[0] = Serial.read();
                indata[1] = Serial.read();
                indata[2] = Serial.read();
                indata[3] = Serial.read();

                int g;
                memcpy(&g, &indata, sizeof(g));
                ret_pack.i = g;
            } else if(inByte == 'l') {
                // we expect data with this format i:XXXX
                ret_pack.type = 'l';
                
                Serial.read(); // discard ':'

                byte indata[4] = { 0 };
                indata[0] = Serial.read();
                indata[1] = Serial.read();
                indata[2] = Serial.read();
                indata[3] = Serial.read();

                long g;
                memcpy(&g, &indata, sizeof(g));
                ret_pack.l = g;
            } else if(inByte == 's') {
                // we expect data with this format s:XXXX
                ret_pack.type = 's';
                
                Serial.read(); // discard ':'

                // read out string length
                byte indata[4] = { 0 };
                indata[0] = Serial.read();
                indata[1] = Serial.read();
                indata[2] = Serial.read();
                indata[3] = Serial.read();

                int str_len;
                memcpy(&str_len, &indata, sizeof(str_len));

                // rebuild string
                    Serial.read(); // discard ':'
                // 1. Prepare the character array (the buffer) 
                char char_arr[str_len];
                Serial.readBytes(char_arr, str_len);
                String str(char_arr);

                ret_pack.s = str;
            } 
        }

        return ret_pack;
    }
}