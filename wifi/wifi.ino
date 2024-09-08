#include <WiFi.h>
#include <WebServer.h>

// Define motor control pins
#define speedPinR 9    // Right motor PWM pin
#define RightMotorDirPin1  12    // Right Motor direction pin 1
#define RightMotorDirPin2  11    // Right Motor direction pin 2
#define speedPinL 6    // Left motor PWM pin
#define LeftMotorDirPin1  7    // Left Motor direction pin 1
#define LeftMotorDirPin2  8    // Left Motor direction pin 2

// Wi-Fi credentials
const char* ssid = "VM6031920";
const char* password = "cyRvjmnr6dqh";

// Initialize web server on port 80
WebServer server(80);

// Motor speed variables
int speedL = 200;
int speedR = 200;

// Initialize GPIO pins
void init_GPIO() {
  pinMode(RightMotorDirPin1, OUTPUT); 
  pinMode(RightMotorDirPin2, OUTPUT); 
  pinMode(speedPinL, OUTPUT);  
  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT); 
  pinMode(speedPinR, OUTPUT); 
  stop_Stop();
}

// Functions to control the car
void go_Advance() { // Forward
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  analogWrite(speedPinL, speedL);
  analogWrite(speedPinR, speedR);
}

void go_Left() { // Left turn
  analogWrite(speedPinL, speedL - 50); // Decrease left speed
  analogWrite(speedPinR, speedR + 50); // Increase right speed
}

void go_Right() { // Right turn
  analogWrite(speedPinL, speedL + 50); // Increase left speed
  analogWrite(speedPinR, speedR - 50); // Decrease right speed
}

void go_Back() { // Slow down
  analogWrite(speedPinL, speedL - 100);
  analogWrite(speedPinR, speedR - 100);
}

void stop_Stop() { // Stop
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, LOW);
  analogWrite(speedPinL, 0);
  analogWrite(speedPinR, 0);
}

// Handle forward button press
void handleForward() {
  speedL = min(speedL + 50, 255); // Increase speed
  speedR = min(speedR + 50, 255);
  go_Advance();
  server.send(200, "text/html", "<h1>Moving Forward</h1>");
}

// Handle left button press
void handleLeft() {
  go_Left();
  server.send(200, "text/html", "<h1>Turning Left</h1>");
}

// Handle right button press
void handleRight() {
  go_Right();
  server.send(200, "text/html", "<h1>Turning Right</h1>");
}

// Handle back button press
void handleBack() {
  speedL = max(speedL - 50, 0); // Decrease speed
  speedR = max(speedR - 50, 0);
  go_Back();
  server.send(200, "text/html", "<h1>Moving Backward</h1>");
}

// Handle stop button press
void handleStop() {
  stop_Stop();
  server.send(200, "text/html", "<h1>Car Stopped</h1>");
}

// Webpage with buttons
String htmlPage = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Car Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1>Car Control Panel</h1>
  <button onclick="location.href='/forward'">Forward</button><br><br>
  <button onclick="location.href='/left'">Left</button>
  <button onclick="location.href='/right'">Right</button><br><br>
  <button onclick="location.href='/back'">Back</button><br><br>
  <button onclick="location.href='/stop'">Stop</button>
</body>
</html>
)";

// Handle root URL
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void setup() {
  // Initialize GPIO
  init_GPIO();
  
  // Start serial communication
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Define routes for the web server
  server.on("/", handleRoot);        // Home page
  server.on("/forward", handleForward); // Forward
  server.on("/left", handleLeft);      // Left
  server.on("/right", handleRight);    // Right
  server.on("/back", handleBack);      // Back
  server.on("/stop", handleStop);      // Stop

  // Start the web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}
