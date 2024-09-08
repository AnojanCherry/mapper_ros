#define SOFT_RX 4    // Softserial RX port
#define SOFT_TX 5    //Softserial TX port

#define speedPinR 9    //  RIGHT PWM pin connect MODEL-X ENA
#define RightMotorDirPin1  12    //Right Motor direction pin 1 to MODEL-X IN1 
#define RightMotorDirPin2  11    //Right Motor direction pin 2 to MODEL-X IN2
#define speedPinL 6    // Left PWM pin connect MODEL-X ENB
#define LeftMotorDirPin1  7    //Left Motor direction pin 1 to MODEL-X IN3 
#define LeftMotorDirPin2  8   //Left Motor direction pin 1 to MODEL-X IN4 
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
char packetBuffer[5];
int speed_left = 200, speed_right = 200;
int ledPin = 13;
bool leftDirBool=true, rightDirBool=true, stopBool=true, ledBool=true;

void setup() {
  pinMode(ledPin, OUTPUT);
  for(int i=0; i<3; i++){digitalWrite(ledPin,HIGH);delay(250);digitalWrite(ledPin,LOW);delay(250);} 
  digitalWrite(ledPin,HIGH);
  // pinMode(RightDirectPin2, OUTPUT); 
  // pinMode(speedPinL, OUTPUT);  
  // pinMode(LeftDirectPin1, OUTPUT);
  // pinMode(LeftDirectPin2, OUTPUT); 
  // pinMode(speedPinR, OUTPUT); 
  // stop_Stop();//stop move

  /*init HC-SR04*/
  // pinMode(Trig_PIN, OUTPUT); 
  // pinMode(Echo_PIN,INPUT); 

  /*init buzzer*/
  // pinMode(BUZZ_PIN, OUTPUT);
  // digitalWrite(BUZZ_PIN, HIGH);  
  // buzz_OFF(); 

  // digitalWrite(Trig_PIN,LOW);
  /*init servo*/
  
  // head.attach(SERVO_PIN); 
  // head.write(90);
 
  //  delay(2000);
   
  Serial.begin(9600);   // initialize serial for debugging
  // Serial.println("Over here");
 
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

 // Serial.print("Attempting to start AP ");
//  Serial.println(ssid);
  //AP mode
  //status = WiFi.beginAP(ssid, 10, "", 0);

//STA mode
   while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
 

  Serial.println("You're connected to the network");
  Serial.println();



  printWifiStatus();
  
  Udp.begin(localPort);     
  
  Serial.print("Listening on port ");
  Serial.println(localPort);
  // start(); 
 digitalWrite(ledPin,LOW);
}

void loop() {
  // delay(1000);
  int packetSize = Udp.parsePacket();
  if (packetSize) {                               // if you                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 get a client,
     Serial.print("Received packet of size ");
    Serial.println(packetSize);
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
      char c=packetBuffer[0];
      Serial.print("Case ");
      Serial.println(c);
            switch (c)    //serial control instructions
            {
              case 'W':speed_left+=speedIncrement;speed_right+=speedIncrement;updateMotorspeed();break; // Accelerate
              case 'S':speed_left-=speedIncrement;speed_right-=speedIncrement;updateMotorspeed();break; // Decelerate
              // case 'E':Serial.print("Case E");speed_left=0;speed_right=0;break; // Stop
              case 'O':start();break; // Start
              case 'L':stop();break; // Stop
              case 'Q':speed_left+=speedIncrement;updateMotorspeed();break; // Left increment
              case 'A':speed_left-=speedIncrement;updateMotorspeed();break; // Left Decrement
              case 'E':speed_right+=speedIncrement;updateMotorspeed();break; // Right increment
              case 'D':speed_right-=speedIncrement;updateMotorspeed();break; // Right decrement
              // case 'H':Serial.print("Case H");break; // Switch direction of travel
              // if(stopBool){
              // }
              // case 'I':Serial.print("Case I");break;
  
              //  case 'A':Drive_Status=MANUAL_DRIVE; Drive_Num=GO_ADVANCE;  WorkMode="GO_ADVANCE";break;
              //  case 'L':Drive_Status=MANUAL_DRIVE; Drive_Num=GO_LEFT; WorkMode="GO_LEFT";break;
              //  case 'R':Drive_Status=MANUAL_DRIVE; Drive_Num=GO_RIGHT;WorkMode="GO_RIGHT";break;
              //  case 'B':Drive_Status=MANUAL_DRIVE; Drive_Num=GO_BACK;WorkMode="GO_BACK";break;
              //  case 'E':Drive_Status=MANUAL_DRIVE; Drive_Num=STOP_STOP;WorkMode="STOP_STOP";break;
              //  case 'O':Drive_Status=AUTO_DRIVE_UO;Serial.println("go OBSTACLE");WorkMode="OBSTACLE";break;
              //  case 'T':Drive_Status=AUTO_DRIVE_LF;WorkMode="line follow";break;
              //  case 'G':track_speed=track_speed+10;
              //           if(track_speed>200) track_speed=200
              //            ;break;
              //  case 'J':track_speed=track_speed-10;
              //           if(track_speed<80) track_speed=80
              //            ;break;
               default:break;
              } //END OF ACTION SWITCH
  
  }
  //  do_Drive_Tick();
}

void printWifiStatus(){
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
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
  analogWrite(speedPinL,speed_left); 
  analogWrite(speedPinR,speed_right);   
  // updateMotorspeed();
}

void start(void){
  Serial.println("Start");
  if(leftDirBool){
    leftPositive();
  } else{
    leftNegative();
  } 
  if(rightDirBool){
    rightPositive();
  }else{
    rightNegative();
  }
  // digitalWrite(RightMotorDirPin1, LOW);
  // digitalWrite(RightMotorDirPin2, LOW);
  // digitalWrite(LeftMotorDirPin1, LOW);
  // digitalWrite(LeftMotorDirPin2, LOW);
  // analogWrite(speedPinL,speed_left);
  // analogWrite(speedPinR,speed_right);
}

void stop(void){
  Serial.println("Stop");
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, LOW);
  // analogWrite(speedPinL,speed_left);
  // analogWrite(speedPinR,speed_right);
}

void acknowledge(){
  digitalWrite(ledPin,HIGH);
  delay(100);
  digitalWrite(ledPin,LOW);
}

// void go_Advance(void)  //Forward
// {
//   digitalWrite(RightMotorDirPin1, HIGH);
//   digitalWrite(RightMotorDirPin2,LOW);
//   digitalWrite(LeftMotorDirPin1,HIGH);
//   digitalWrite(LeftMotorDirPin2,LOW);
//   // analogWrite(speedPinL,speed_left);
//   // analogWrite(speedPinR,speed_right);
// }
// void go_Left()  //Turn left
// {
//   digitalWrite(RightMotorDirPin1, HIGH);
//   digitalWrite(RightMotorDirPin2,LOW);
//   digitalWrite(LeftMotorDirPin1,LOW);
//   digitalWrite(LeftMotorDirPin2,HIGH);
//   // analogWrite(speedPinL,200);
//   // analogWrite(speedPinR,200);
//   // delay(t);
// }
// void go_Right()  //Turn right
// {
//   digitalWrite(RightMotorDirPin1, LOW);
//   digitalWrite(RightMotorDirPin2,HIGH);
//   digitalWrite(LeftMotorDirPin1,HIGH);
//   digitalWrite(LeftMotorDirPin2,LOW);
//   // analogWrite(speedPinL,200);
//   // analogWrite(speedPinR,200);
//   // delay(t);
// }
// void go_Back()  //Reverse
// {
//   digitalWrite(RightMotorDirPin1, LOW);
//   digitalWrite(RightMotorDirPin2,HIGH);
//   digitalWrite(LeftMotorDirPin1,LOW);
//   digitalWrite(LeftMotorDirPin2,HIGH);
//   // analogWrite(speedPinL,200);
//   // analogWrite(speedPinR,200);
//   // delay(t);
// }
// void stop_Stop()    //Stop
// {
//   digitalWrite(RightMotorDirPin1, LOW);
//   digitalWrite(RightMotorDirPin2,LOW);
//   digitalWrite(LeftMotorDirPin1,LOW);
//   digitalWrite(LeftMotorDirPin2,LOW);
// }