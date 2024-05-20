
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <Adafruit_MCP4725.h>
Adafruit_MCP4725 dac;

int remote_key = 6;    //   5/6 on and wait 30s remote key
int start_button = 5;    //   7/8 trigger on start button
int hardwire_enable = 4;   //    13/14 enable disable A-B
int relay4 = 3; //24v
//int relay5=7; // 10v


int pos = 0; 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 1, 102 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 1, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;

void setup() {
 // Open serial communications and wait for port to open:
  dac.begin(0x60);
  dac.setVoltage(0, false);
  digitalWrite(remote_key, HIGH);
  pinMode(remote_key, OUTPUT);
  digitalWrite(start_button, HIGH);
  pinMode(start_button, OUTPUT);
  digitalWrite(hardwire_enable, HIGH);
  pinMode(hardwire_enable, OUTPUT);
  digitalWrite(relay4, HIGH);
  pinMode(relay4, OUTPUT);
//  digitalWrite(relay5, HIGH);
//  pinMode(relay5, OUTPUT);
//   start the Ethernet connection and the server:
  Ethernet.init(8);
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}
void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {  
          readString += c;   
         } 
         if (c == '\n') {          
           Serial.println(readString); 
           //html file 
           client.println("HTTP/1.1 200 OK"); 
           client.println("Content-Type: text/html");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML><BODY><h1>Api Usage:</h1>");
          client.println("<h3>Replace link with (remote_key_on,start_button_on,hardwire_enable_on,Laser_on,relay5_on),(remote_key_off,start_button_off,hardwire_enable_off,Laser_off,relay5_off) in your project</h3>");       
          client.println("<span>http://192.168.1.102/link</span>");
          client.println();
          client.println("</body></html>");
           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("remote_key_off") >0){
               digitalWrite(remote_key, 1);
               digitalWrite(start_button, 1);
               digitalWrite(hardwire_enable, 1);
           }
           if (readString.indexOf("remote_key_on") >0)
           {
               digitalWrite(remote_key, 0); 
               delay(30000);
               digitalWrite(start_button, 0);
               delay(1000);
               digitalWrite(hardwire_enable, 0);
               
           }
           if (readString.indexOf("Laser_on") >0){
               digitalWrite(relay4, 0);
//               digitalWrite(relay5, 0);
           }
           if (readString.indexOf("Laser_off") >0){
               digitalWrite(relay4, 1);
//               digitalWrite(relay5, 1);
               
           }
           
           if (readString.indexOf("start_button_off") >0){
               digitalWrite(start_button, 1);
           }
           if (readString.indexOf("hardwire_enable_off") >0){
               digitalWrite(hardwire_enable, 1);
           }
           if (readString.indexOf("hardwire_enable_on") >0)
           {
               digitalWrite(hardwire_enable, 0); 
           }
           if (readString.indexOf("sp1") >0)
           {
               dac.setVoltage(424, false); 
           }
           if (readString.indexOf("sp2") >0)
           {
               dac.setVoltage(856, false); 
           }
           if (readString.indexOf("sp3") >0)
           {
               dac.setVoltage(1288, false); 
           }
           if (readString.indexOf("sp4") >0)
           {
               dac.setVoltage(1720, false); 
           }
           if (readString.indexOf("sp5") >0)
           {
               dac.setVoltage(2145, false); 
           }
           if (readString.indexOf("sp6") >0)
           {
               dac.setVoltage(2576, false); 
           }
           if (readString.indexOf("sp7") >0)
           {
               dac.setVoltage(3012, false); 
           }
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
