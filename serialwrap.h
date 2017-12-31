#pragma once

#include <arduino.h>

namespace SerialWrap
{
    struct package
    {
        char type = ' ';
        float f = 0;
        int i = 0;
        long l = 0;
        String s = "";
    };

    enum board { SWRAP_ESPX, SWRAP_ARDUINO };
 
    byte boardModel = SerialWrap::SWRAP_ARDUINO;

    float ten_shift = 10000.0;

    void init(unsigned int baudrate, SerialWrap::board model = SerialWrap::SWRAP_ARDUINO)
    {
        Serial.begin(baudrate);
        boardModel = model;
    }

    void transferFloat(float f)
    {

        long sendVal = f * ten_shift;

        byte *b = (byte *)&sendVal;
        Serial.print("f:");
        Serial.write(b[0]);
        Serial.write(b[1]);
        Serial.write(b[2]);
        Serial.write(b[3]);
        delay(100);
    }

    void transferInt(int i)
    {

        Serial.print("i:");

        // detect pos/neg
        i < 0 ? Serial.print(1) : Serial.print(0);

        // transfer positive integer value
        i = abs(i);
        byte *b = (byte *)&i;
        switch (boardModel)
        {
        case SerialWrap::SWRAP_ARDUINO:
            for (int i = 0; i < 2; i++)
            {
                Serial.write(0);
            }
            for (int i = 0; i < 2; i++)
            {
                Serial.write(b[i]);
            }
            delay(100);
            break;
        case SerialWrap::SWRAP_ESPX:
            for (int i = 0; i < 4; i++)
            {
                Serial.write(b[i]);
            }
            delay(100);
            break;
        default:
            break;
        }
    }

    void transferLong(long l)
    {
        Serial.print("l:");

        // detect pos/neg
        l < 0 ? Serial.print(1) : Serial.print(0);

        // transfer positive long value
        l = abs(l);
        byte *b = (byte *)&l;
        switch (boardModel)
        {
        case SerialWrap::SWRAP_ARDUINO:
            for (int i = 0; i < 4; i++)
            {
                Serial.write(0);
            }
            for (int i = 0; i < 4; i++)
            {
                Serial.write(b[i]);
            }
            delay(100);
            break;
        case SerialWrap::SWRAP_ESPX:
            for (int i = 0; i < 8; i++)
            {
                Serial.write(b[i]);
            }
            delay(100);
            break;
        default:
            break;
        }
    }

    void transferString(String str)
    {

        if (str.length() > 254)
        {
            Serial.println("String too long");
            return;
        }

        // Length (with one extra character for the null terminator)
        byte str_len = str.length() + 1;

        // Prepare the character array (the buffer)
        char char_arr[str_len];

        // Copy it over
        str.toCharArray(char_arr, str_len);

        Serial.print("s:");

        // transfer string length
        Serial.write(str_len);

        Serial.print(":");

        for (int i = 0; i < str_len; i++)
        {
            Serial.write(char_arr[i]);
        }

        delay(100);
    }

    float receiveFloat()
    {
        Serial.read(); // discard ':'

        byte indata[4] = {0};
        indata[0] = Serial.read();
        indata[1] = Serial.read();
        indata[2] = Serial.read();
        indata[3] = Serial.read();

        long receivedVal;
        memcpy(&receivedVal, &indata, sizeof(receivedVal));
        return receivedVal / ten_shift;
    }

    int receiveInt()
    {
        Serial.read(); // discard ':'

        byte sign = Serial.read();

        byte indata[4] = {0};
        for (int i = 0; i < 4; i++)
        {
            indata[i] = Serial.read();
        }

        int receivedVal = 0;

        switch (boardModel)
        {
        case SerialWrap::SWRAP_ARDUINO:
            receivedVal = *((int *)(&indata[2]));
            break;
        case SerialWrap::SWRAP_ESPX:
            receivedVal = *((int *)(&indata[0]));
            break;
        default:
            receivedVal = 0;
            break;
        }

        if (sign == 1)
        {
            receivedVal = receivedVal * -1;
        }

        return receivedVal;
    }

    long receiveLong()
    {
        Serial.read(); // discard ':'

        byte sign = Serial.read();

        byte indata[8] = {0};
        for (int i = 0; i < 8; i++)
        {
            indata[i] = Serial.read();
        }

        long receivedVal = 0;

        switch (boardModel)
        {
        case SerialWrap::SWRAP_ARDUINO:
            receivedVal = *((long *)(&indata[4]));
            break;
        case SerialWrap::SWRAP_ESPX:
            receivedVal = *((long *)(&indata[0]));
            break;
        default:
            receivedVal = 0;
            break;
        }

        if (sign == 1)
        {
            receivedVal = receivedVal * -1;
        }

        return receivedVal;
    }

    String receiveString()
    {
        Serial.read(); // discard ':'

        // read out string length
        byte str_len = Serial.read();

        // rebuild string
        Serial.read(); // discard ':'
        // 1. Prepare the character array (the buffer)
        char char_arr[str_len];
        Serial.readBytes(char_arr, str_len);
        String str(char_arr);

        return str;
    }

    SerialWrap::package receive()
    {
        SerialWrap::package ret_pack;

        if (Serial.available() > 0)
        {
            char inByte = Serial.read();
            if (inByte == 'f')
            {
                // we expect data with this format f:XXXX
                ret_pack.type = 'f';
                ret_pack.f = receiveFloat();
            }
            else if (inByte == 'i')
            {
                // we expect data with this format i:XXXX
                ret_pack.type = 'i';
                ret_pack.i = receiveInt();
            }
            else if (inByte == 'l')
            {
                // we expect data with this format i:XXXX
                ret_pack.type = 'l';
                ret_pack.l = receiveLong();
            }
            else if (inByte == 's')
            {
                // we expect data with this format s:XXXX
                ret_pack.type = 's';
                ret_pack.s = receiveString();
            }
        }

        return ret_pack;
    }
}