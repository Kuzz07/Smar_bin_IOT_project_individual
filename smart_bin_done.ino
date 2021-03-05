#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <Servo.h>
//#include <ArduinoJson.h>

#define HOSTIFTTT "maker.ifttt.com"
#define EVENTO "smart_bin2"
#define IFTTTKEY "faQrDPUhAPU3vssbDe9YBBklq--DFq5PyCoa3_77qR"
WiFiClient client;
 const int ProxSensor = A0;
const char* ssid = "G7";
const char* password = "password";
    

#define TRIGGER 5 //d1
#define ECHO    4 //d2

// defines variables
long duration;
int distance;
//bool notification_timer = false;
//WiFiServer server(80);
Servo servo_1;
 
void setup() {
   
  Serial.begin(115200);
  delay(4);
 
//  pinMode(ledPin, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);


  //PINMODE OF IR sensor
  pinMode(ProxSensor, INPUT);

  servo_1.attach(0);
  
//  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
//  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  


  //Send Webook to IFTTT
//  send_webhook(IFTTTEVENT,IFTTTKEY) 
}

bool check = true;
void loop() {
  //Ir sensor
long state = analogRead(ProxSensor);
  if (state > 500)     //Check the sensor output
  {  // set the LED on
      servo_1.write (0);// servo will move to 90 degree angle.
    delay(100);
  }
  else
  {    // set the LED off
    servo_1.write (180);// Servo will move to 45 degree angle.
    delay(100);
  }

  
  
  
  //ultrasonic code
 
  
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  
  Serial.print(distance);
  Serial.println("Centimeter:");
  delay(100);
  
  if(distance < 5 ){
//    digitalWrite(ledPin,HIGH);
    if(check == true){
      check = false;
      if (client.connected())
      {
        client.stop();
        }
      
        client.flush();
      
      if (client.connect(HOSTIFTTT,80)) {
//        https://maker.ifttt.com/use/faQrDPUhAPU3vssbDe9YBBklq--DFq5PyCoa3_77qR
          // build the HTTP request
          String toSend = "GET /trigger/";
          toSend += EVENTO;
          toSend += "/with/key/";
          toSend += IFTTTKEY;
          toSend += "?value1=";
          toSend += "Softwarica";
          toSend += " HTTP/1.1\r\n";
          toSend += "Host: ";
          toSend += HOSTIFTTT;
          toSend += "\r\n";
          toSend += "Connection: close\r\n\r\n";
          client.print(toSend);
          delay(500);
        }
      }
  }  
    else{
//      digitalWrite(ledPin,LOW);
      check = true;
    }
    
  delay(500);
  Serial.println("Client connected");
  Serial.println("");
}
