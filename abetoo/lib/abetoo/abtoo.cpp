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
//#define abtoo_debug
#include "abtoo.h"

AbetooIno::AbetooIno(){}

void AbetooIno::init(String UUID, String channelName, int heartbeat, void (*messageReceived)(String message))
{
  //_client = client;
  _uuid = UUID;
  _channel = "/topic/" + channelName;

  // check heartbeat
  if (heartbeat <= 1)
    _heartbeat = 1 * 60000;
  else if (heartbeat >= 30)
    _heartbeat = 30 * 60000;
  else
    _heartbeat = heartbeat * 60000;
    
  _msgreceivedfunction = messageReceived;
  reconnectionCount = 0;
}

AbetooIno::States AbetooIno::getState()
{
  return state;
}

void AbetooIno::abetooloop()
{
  // We're connected to server => open socket
  if( state == DISCONNECTED ) {

    do
    {
      if (!_client.connect("abetoo.com", 61613)) {
        #ifdef abtoo_debug
          Serial.println("Socket connection failed");
        #endif
        continue;
      }
      #ifdef abtoo_debug
        Serial.println("socket connection success");
      #endif
    }  while (!_client.connected());
    
    _client.setNoDelay(true);
    #ifdef abtoo_debug
      Serial.println("Connecting to remote server");
    #endif
    
    // STOMP connect
    String hb = "";
    hb = hb + _heartbeat + "," + _heartbeat;
    _client.println( "CONNECT" );
    _client.println( "accept-version:1.2" );
    _client.print( "login:" );
    _client.println( "abetoouser" );
    _client.print( "passcode:" );
    _client.println( "abetoopi" );
    _client.print( "heart-beat:" );
    _client.println( hb  );
    _client.println();
    _client.write( endf );
    
    // Waiting for response
    #ifdef abtoo_debug
      Serial.println("Connect sended");
    #endif
    state = WAITING;
  }
  else if( state == CONNECTED ) {
    // Subscribe to TOPIC
    _client.println( "SUBSCRIBE" );
    _client.print( "id:" );
    _client.println( _uuid + (reconnectionCount++) ); // changing id
    _client.print( "destination:" );
    _client.println( _channel );
    _client.println( "receipt:subscribed" );
    _client.println( "client:auto" );
    _client.println();
    _client.write( endf );

    #ifdef abtoo_debug
      Serial.println("Subscribing");
    #endif    
    state = SUBSCRIBING;   
    
  } else if( state == SUBSCRIBED ) {
    // Heartbeat trigger
    currentMillisSendHeartBeat = millis();
    if (currentMillisSendHeartBeat - previousMillisSendHeartBeat >= _heartbeat || currentMillisSendHeartBeat < previousMillisSendHeartBeat) 
    {
      previousMillisSendHeartBeat = currentMillisSendHeartBeat;
      sendHeartBeat();
    }

  } else if( state == SENT ) {
  }

  // Received HeartBeat
  currentMillisReceiveHeartBeat = millis();
  if (currentMillisReceiveHeartBeat - previousMillisReceiveHeartBeat >= 2 * _heartbeat || currentMillisReceiveHeartBeat < previousMillisReceiveHeartBeat) 
  {
    previousMillisReceiveHeartBeat = currentMillisReceiveHeartBeat;
    state = DISCONNECTED;
    _client.flush();
  }
 
  // Treatment of incoming data from broker
  if( _client.available() > 0 ) 
  {
    previousMillisReceiveHeartBeat = currentMillisReceiveHeartBeat;
    
    delay(100);
    String response = _client.readStringUntil( 0x0 );

    if (response == "\n")
    {
      // Heartbeat
      #ifdef abtoo_debug
        Serial.println("Hearbeat received");
      #endif
    }
    else if( response.length() > 0 )
    {
      String frame = getValue( response, 0, "\n" );
      
      if( frame == "CONNECTED" )
      {
        String session = getHeader( response, "session" );
        state = CONNECTED; 
        #ifdef abtoo_debug
          Serial.println("Connected");
        #endif
        
      } 
      else if( frame == "ERROR" ) 
      {
        previousMillisReceiveHeartBeat = currentMillisReceiveHeartBeat;
        state = DISCONNECTED;
        #ifdef abtoo_debug
          Serial.println("Error frame");
        #endif
        _client.flush();
      }
      else if( frame == "MESSAGE" ) 
      {
        // base64 decode
        String body = getValue( response, 1, "\n\n" );
        int bodylength = body.length()+1;
        char bodyarraychar[bodylength];
        char decodedbodychararray[bodylength];
        body.toCharArray(bodyarraychar, bodylength);
        
        int rst = base64_decode(decodedbodychararray,  bodyarraychar, bodylength);
        String str(decodedbodychararray);
        if (_msgreceivedfunction) _msgreceivedfunction(str);

      } 
      else if( frame == "RECEIPT" ) 
      {
        if( state == SUBSCRIBING ) 
        {
          state = SUBSCRIBED;
          #ifdef abtoo_debug
            Serial.println("Subscribed");
          #endif
        } else if( state == SENDING ) 
        {
          #ifdef abtoo_debug
            Serial.println("Message sent");
          #endif
          state = SUBSCRIBED;
          delay(1000);
        } else if( state == DISCONNECTING ) 
        {
          #ifdef abtoo_debug
            Serial.println("Disconnected.");
          #endif
          _client.stop();
          state = STOP;  
        }
      }
    }
    _client.read(); // null byte
  }
  
}

// Send HearBeat
void AbetooIno::sendHeartBeat()
{
  _client.println();
  _client.write( endf );
}


// Send Message
int AbetooIno::sendMessage(String destination, String body, String correlationid)
{
    if (state != SUBSCRIBED) return -1;
   
    // base64 encode
    int bodylength = body.length()+1;
    char bodyarraychar[bodylength];
    char encodedbodychararray[bodylength];
    body.toCharArray(bodyarraychar, bodylength);
    int rst = base64_encode(encodedbodychararray, bodyarraychar, bodylength);        
    String str(encodedbodychararray);
    String envio = "SEND\ndestination:/topic/" + destination + "\ncontent-type:text/plain\ncontent-length:" + str.length() + "\ncorrelationid:"+correlationid+"\nfrom:"+_channel+"\n\n"+str;

    _client.print( envio );
    _client.write( endf );
    
    delay(10); 
   
    #ifdef abtoo_debug
      Serial.println("Sending message...");
    #endif
    return 0;
}

int AbetooIno::count( String content, String delimeter )
{
  int count = 0;
  int end;
  int start = 0;
 
  // Count occurances of delimeter
  do {
    end = content.indexOf( delimeter, start );
    start = end + 1;
    count = count + 1;
  } while( end > 0 );
  
  // Return occurance count
  return count;
}

String AbetooIno::getHeader( String content, String header )
{
  int    parts;
  int    start;
  String line;  
  String prefix;
  String result;
  
  // How many lines in response
  parts = count( content, "\n" );
  
  // Start on line after frame line
  // Look for header prefix match
  for( int p = 1; p < parts; p++ )
  {
    // Header line
    // Split into parts
    line = getValue( content, p, "\n" );
    prefix = getValue( line, 0, ":" );
    
    // If prefix matches
    if( prefix == header )
    {
      // Get value for header
      start = line.indexOf( ":" ) + 1;
      result = line.substring( start );
      break;
    }  
  }
  
  // Return result
  return result;
}

String AbetooIno::getValue( String content, int part, String delimeter )
{
  int    end;
  int    start = 0;
  String result;

  // Iterate past unwanted values
  for( int count = 0; count < part; count++ )
  {
    end = content.indexOf( delimeter, start );
    start = end + delimeter.length();
  }
  
  // Get next occurance of delimeter
  // May return -1 if not found
  end = content.indexOf( delimeter, start );
  
  // If no more occurances
  if( end == -1 )
  {
    // Must be last value in content
    // Parse out remainder
    result = content.substring( start );
  } else {
    // Otherwise parse out segment of content
    result = content.substring( start, end );
  }
  
  // Clean off white space
  result.trim();
  
  // Return resulting content
  return result;
}

/*
Copyright (C) 2013 Adam Rudd
Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so,subject to the following conditions:
The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.THE SOFTWARE IS PROVIDED "AS IS", 
WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR 
THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

int AbetooIno::base64_encode(char *output, char *input, int inputLen) {
  int i = 0, j = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];

  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
      a3_to_a4(a4, a3);

      for(i = 0; i < 4; i++) {
        output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }

    a3_to_a4(a4, a3);

    for(j = 0; j < i + 1; j++) {
      output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
    }

    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

int AbetooIno::base64_decode(char * output, char * input, int inputLen) {
  int i = 0, j = 0;
  int decLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];


  while (inputLen--) {
    if(*input == '=') {
      break;
    }

    a4[i++] = *(input++);
    if (i == 4) {
      for (i = 0; i <4; i++) {
        a4[i] = b64_lookup(a4[i]);
      }

      a4_to_a3(a3,a4);

      for (i = 0; i < 3; i++) {
        output[decLen++] = a3[i];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      a4[j] = '\0';
    }

    for (j = 0; j <4; j++) {
      a4[j] = b64_lookup(a4[j]);
    }

    a4_to_a3(a3,a4);

    for (j = 0; j < i - 1; j++) {
      output[decLen++] = a3[j];
    }
  }
  output[decLen] = '\0';
  return decLen;
}

int AbetooIno::base64_enc_len(int plainLen) {
  int n = plainLen;
  return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

int AbetooIno::base64_dec_len(char * input, int inputLen) {
  int i = 0;
  int numEq = 0;
  for(i = inputLen - 1; input[i] == '='; i--) {
    numEq++;
  }

  return ((6 * inputLen) / 8) - numEq;
}

inline void AbetooIno::a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

inline void AbetooIno::a4_to_a3(unsigned char * a3, unsigned char * a4) {
  a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
  a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
  a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

inline unsigned char AbetooIno::b64_lookup(char c) {
  if(c >='A' && c <='Z') return c - 'A';
  if(c >='a' && c <='z') return c - 71;
  if(c >='0' && c <='9') return c + 4;
  if(c == '+') return 62;
  if(c == '/') return 63;
  return -1;
}

