/*
    Copyright (C) 2017 Chocrón.
    Abetoo communication framework. ver 1.1
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef Abtoo_h
#define Abtoo_h
#include <ESP8266WiFi.h>

class AbetooIno
{
  public:
    enum States {DISCONNECTED, CONNECTING, CONNECTED, WAITING, SUBSCRIBING, SUBSCRIBED, SENDING, SENT, DISCONNECTING, STOP};
    AbetooIno();
    void init(String UUID, String channelName, int heartbeat, void (*messageReceived)(String message));
    void abetooloop();
    int sendMessage(String destination, String body, String correlationid);
    States getState();
    
  private:
    long y = 0;
    int vez = 0;
    const byte endf = 0x0;
    typedef void (*msgRcv)(String);
    msgRcv _msgreceivedfunction;
    States state = DISCONNECTED;
    const char* b64_alphabet = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

    String lastlog;
    String _uuid;
    String _channel;
    int _heartbeat;
    int reconnectionCount;
    int messageid;
    WiFiClient _client;
    unsigned long currentMillisSendHeartBeat = millis();
    unsigned long previousMillisSendHeartBeat = millis();

    unsigned long currentMillisReceiveHeartBeat = millis();
    unsigned long previousMillisReceiveHeartBeat = millis();

    void sendHeartBeat();
    int count( String content, String delimeter );
    String getHeader( String content, String header );
    String getValue( String content, int part, String delimeter );
    inline void a3_to_a4(unsigned char * a4, unsigned char * a3);
    inline void a4_to_a3(unsigned char * a3, unsigned char * a4);
    inline unsigned char b64_lookup(char c);
    int base64_encode(char *output, char *input, int inputLen);
    int base64_decode(char * output, char * input, int inputLen);
    int base64_enc_len(int plainLen);
    int base64_dec_len(char * input, int inputLen);
};

 #endif
