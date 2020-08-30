#include <WiFi.h>
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
String header;

// Auxiliar variables to store the current output state
String output5State = "off";
String output4State = "off";
const int output5 =5;
const int output4 = 4;
char ssid[] = "UV bot";  //Set the AP's SSID
char pass[] = "12345678";     //Set the AP's password
char channel[] = "1";         //Set the AP's channel
int status = WL_IDLE_STATUS;  // the Wifi radio's status
WiFiServer server(80);
void printWifiStatus() {
    // print the SSID of the network you're attached to:
   // Serial.println();
   // Serial.print("SSID: ");
   // Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    //Serial.print("IP Address: ");
   // Serial.println(ip);
}
void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    Serial1.begin(115200);
    pinMode(output5,OUTPUT);
    pinMode(output4,OUTPUT);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        //Serial.println("WiFi shield not present");
        while (true);
    }
    String fv = WiFi.firmwareVersion();
    if (fv != "1.0.0") {
       // Serial.println("Please upgrade the firmware");
    }

    // attempt to start AP:
    while (status != WL_CONNECTED) {
        //Serial.print("Attempting to start AP with SSID: ");
        //Serial.println(ssid);
        status = WiFi.apbegin(ssid, pass, channel);
        delay(10000);
    }

    //AP MODE already started:
   // Serial.println("AP mode already started");
    //Serial.println();
   server.begin();
   printWifiStatus();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

if (client) { // If a new client connects,
 Serial.println("New Client."); // print a message out in the serial port
 String currentLine = ""; // make a String to hold incoming data from the client
 while (client.connected()) { // loop while the client's connected
 if (client.available()) { // if there's bytes to read from the client,
  char c = client.read(); // read a byte, then
  Serial.write(c); // print it out the serial monitor
  header += c;
  if (c == '\n') { // if the byte is a newline character
   // if the current line is blank, you got two newline characters in a row.
   // that's the end of the client HTTP request, so send a response:
   if (currentLine.length() == 0) {
    // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
    // and a content-type so the client knows what's coming, then a blank line:
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    // turns the GPIOs on and off
if (header.indexOf("GET /5/on") >= 0) {
  //Serial.println("GPIO 5 on");
  output5State = "on";
  digitalWrite(output5, HIGH);
  //Serial.print("5");
} else if (header.indexOf("GET /5/off") >= 0) {
 // Serial.println("GPIO 5 off");
  output5State = "off";
  Serial.print("7");
  digitalWrite(output5, LOW);
} else if (header.indexOf("GET /4/on") >= 0) {
  //Serial.println("GPIO 4 on");
  output4State = "on";
  digitalWrite(output4, HIGH);
} else if (header.indexOf("GET /4/off") >= 0) {
  //Serial.println("GPIO 4 off");
  output4State = "off";
  digitalWrite(output4, LOW);
}

 client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
client.println("<h1>UV BoT</h1>");
 client.println("<p>UV Light status " + output5State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output5State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } 
            else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
             client.println();
             break;
   }
     else { // if you got a newline, then clear currentLine
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
   // Serial.println("Client disconnected.");
   // Serial.println("");
  }
}
