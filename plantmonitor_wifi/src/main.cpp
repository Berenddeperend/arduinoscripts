#include <Arduino.h>


#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
// WiFi parameters to be configured
const char* ssid = "Estis Design 2.4"; // Hoofdlettergevoelig
const char* password = "CooleRooney"; // Hoofdlettergevoelig

const char* http_site = "http://195.240.135.237:4000/api/echo";

bool isPumping = false;
unsigned long myTime;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long currentMillis;
unsigned long interval = 1000;           // interval at which to blink (milliseconds)

String header;
WiFiServer server(80); 


void setup() { 
  pinMode(12, OUTPUT);

  Serial.begin(9600);
  Serial.print("Bezig met verbinden");
  WiFi.begin(ssid, password); // Connect to WiFi
 
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  
  // Verbonden.
  Serial.println("OK!");
  
  // Access Point (SSID).
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  
  // IP adres.
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Signaalsterkte.
  long rssi = WiFi.RSSI();
  Serial.print("Signaal sterkte (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.println("");

  server.begin();
}

void postSensorData() {
  HTTPClient http; //Declare an object of class HTTPClient
  http.begin(http_site); //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST("Maar dit moet toch werken?"); //Send the request
  if (httpCode > 0) { //Check the returning code
    String payload = http.getString(); //Get the request response payload
    Serial.println(payload); //Print the response payload
  }
  http.end(); //Close connection
}

void setupPump() {
  currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    digitalWrite(12, LOW); //stop pumping.
  }
}

void doPump() {
  previousMillis = currentMillis;
  digitalWrite(12, HIGH);
}

void startServer() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // Display the HTML web page

            doPump();
            // isPumping = !isPumping;
            
            // if (isPumping) {
            //   digitalWrite(12, HIGH);
            // } else {
            //   digitalWrite(12, LOW);
            // }

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void loop() {
  myTime = millis();

  if(WiFi.status() == WL_CONNECTED){ //Check WiFi connection status
      // postSensorData();
      startServer();
      setupPump();
    } else {
      Serial.println("Error in WiFi connection");   
    }
    // delay(30000); //Send a request every 30 seconds
}

