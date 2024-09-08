#define SOFT_RX 4    // Softserial RX port
#define SOFT_TX 5    // Softserial TX port

#define speedPinR 9    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  12    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  11    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  7    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  8    //Left Motor direction pin 1 to MODEL-X IN4 
#define speedIncrement 10

#include "WiFiEsp.h"
#include <WiFiEspUdp.h>
WiFiEspUDP Udp;
unsigned int localPort = 8888;  // local port to listen on

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(SOFT_RX, SOFT_TX); // RX, TX
#endif

int status = WL_IDLE_STATUS;     // the Wifi radio's status
char ssid[] = "VM6031920";   // replace *** with your router wifi SSID (name)
char pass[] = "cyRvjmnr6dqh";   // replace *** with your router wifi password
char packetBuffer[255];    // Buffer to hold incoming data
int speed_left = 200, speed_right = 200;
int ledPin = 13;
bool leftDirBool=true, rightDirBool=true, stopBool=true, ledBool=true;

void setup() {
  pinMode(ledPin, OUTPUT);
  for(int i=0; i<3; i++){digitalWrite(ledPin,HIGH);delay(250);digitalWrite(ledPin,LOW);delay(250);} 
  digitalWrite(ledPin,HIGH);
 
  Serial.begin(9600);   // initialize serial for debugging
  Serial1.begin(115200);    // initialize serial for ESP module
  Serial1.print("AT+CIOBAUD=9600\r\n");
  Serial1.write("AT+RST\r\n");

  Serial1.begin(9600);    // initialize serial for ESP module
  
  WiFi.init(&Serial1);    // initialize ESP module

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true); // don't continue
  }

   while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
  
  Udp.begin(localPort);     
  
  Serial.print("Listening on port ");
  Serial.println(localPort);

  digitalWrite(ledPin,LOW);
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    
    int len = Udp.read(packetBuffer, 255); // Read string data
    if (len > 0) {
      packetBuffer[len] = 0; // Null-terminate the string
    }
    
    Serial.print("Received command: ");
    Serial.println(packetBuffer); // Print the received string
    
    processCommand(String(packetBuffer)); // Process the command
  }
}

void processCommand(String command) {
  command.trim(); // Remove any leading/trailing whitespace
  if (command.equalsIgnoreCase("W")) {
    speed_left += speedIncrement;
    speed_right += speedIncrement;
    updateMotorspeed();
  } else if (command.equalsIgnoreCase("S")) {
    speed_left -= speedIncrement;
    speed_right -= speedIncrement;
    updateMotorspeed();
  } else if (command.equalsIgnoreCase("O")) {
    start();
  } else if (command.equalsIgnoreCase("L")) {
    stop();
  } else if (command.equalsIgnoreCase("Q")) {
    speed_left += speedIncrement;
    updateMotorspeed();
  } else if (command.equalsIgnoreCase("A")) {
    speed_left -= speedIncrement;
    updateMotorspeed();
  } else if (command.equalsIgnoreCase("E")) {
    speed_right += speedIncrement;
    updateMotorspeed();
  } else if (command.equalsIgnoreCase("D")) {
    speed_right -= speedIncrement;
    updateMotorspeed();
  } else {
    Serial.println("Unknown command.");
  }
}

void printWifiStatus(){
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.print("To see this page in action, connect to ");
  Serial.print(ssid);
  Serial.print(" and open a browser to http://");
  Serial.println(ip);
  Serial.println();
}

void rightPositive(void){
  Serial.println("Right +");
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  updateMotorspeed();
}

void rightNegative(void){
  Serial.println("Right -");
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  updateMotorspeed();
}

void leftPositive(void){
  Serial.println("Left +");
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  updateMotorspeed();
}

void leftNegative(void){
  Serial.println("Left -");
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  updateMotorspeed();
}

void updateMotorspeed(){
  Serial.println("Update motor speed");
  analogWrite(speedPinL, speed_left); 
  analogWrite(speedPinR, speed_right);   
}

void start(void){
  Serial.println("Start");
  if (leftDirBool) {
    leftPositive();
  } else {
    leftNegative();
  } 
  if (rightDirBool) {
    rightPositive();
  } else {
    rightNegative();
  }
}

void stop(void){
  Serial.println("Stop");
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, LOW);
}