#include <TinyGPS++.h>
#include <WiFi.h>
#include <WebServer.h>

HardwareSerial sim800(2);
HardwareSerial gpsSerial(1);

TinyGPSPlus gps;

// ---------------- WIFI ----------------

const char* ssid = "YOUR_WI-FI_NETWORK_NAME";
const char* password = "WI-FI_PASSWORD";
unsigned long alertTime = 0;
WebServer server(80);

String statusMessage = "SAFE";

// ---------------- PINS ----------------

#define SOS_BUTTON 25
#define CALL_BUTTON 27
#define BUZZER 26

// ---------------- VARIABLES ----------------

bool callActive = false;
bool panicMode = false;

bool sosPressed = false;
unsigned long sosPressTime = 0;

bool lastCallState = HIGH;
unsigned long lastCallAction = 0;
const int callCooldown = 2000;

bool ignoreNextRelease = false;

// ---------------- WEB PAGE ----------------

void handleRoot() {

  String page = "<html><head>";

  page += "<meta http-equiv='refresh' content='1'>";

  page += "<title>SOS Device</title></head><body>";

  page += "<h1>SOS DEVICE STATUS</h1>";

  page += "<h2>";
  page += statusMessage;
  page += "</h2>";

  if(gps.location.isValid()){

    page += "<p>Latitude: ";
    page += String(gps.location.lat(),6);
    page += "</p>";

    page += "<p>Longitude: ";
    page += String(gps.location.lng(),6);
    page += "</p>";

    page += "<p><a href='https://maps.google.com/?q=";
    page += String(gps.location.lat(),6);
    page += ",";
    page += String(gps.location.lng(),6);
    page += "'>Open Location</a></p>";
  }

  page += "</body></html>";

  server.send(200, "text/html", page);
}

// ---------------- BUZZER FUNCTIONS ----------------

void sosBeep(){

  for(int i=0;i<3;i++){

    digitalWrite(BUZZER,HIGH);
    delay(200);

    digitalWrite(BUZZER,LOW);
    delay(200);
  }
}

void callStartBeep(){

  digitalWrite(BUZZER,HIGH);
  delay(700);

  digitalWrite(BUZZER,LOW);
}

void callEndBeep(){

  for(int i=0;i<2;i++){

    digitalWrite(BUZZER,HIGH);
    delay(120);

    digitalWrite(BUZZER,LOW);
    delay(120);
  }
}

// ---------------- SETUP ----------------

void setup(){

  Serial.begin(115200);

  sim800.begin(19200, SERIAL_8N1, 16, 17);

  gpsSerial.begin(9600, SERIAL_8N1, 34, -1);

  pinMode(SOS_BUTTON, INPUT_PULLUP);

  pinMode(CALL_BUTTON, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  digitalWrite(BUZZER, LOW);

  delay(5000);

  // ---------------- WIFI CONNECT ----------------

  WiFi.begin(ssid, password);

  Serial.print("Connecting WiFi");

  while(WiFi.status() != WL_CONNECTED){

    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi Connected");

  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();

  // ---------------- GSM TEST ----------------

  Serial.println("TESTING GSM");

  sim800.println("AT");
  delay(1000);

  while(sim800.available()){

    Serial.write(sim800.read());
  }

  sim800.println("AT+CSQ");
  delay(1000);

  while(sim800.available()){

    Serial.write(sim800.read());
  }

  sim800.println("AT+CREG?");
  delay(1000);

  while(sim800.available()){

    Serial.write(sim800.read());
  }
}

// ---------------- LOOP ----------------

void loop(){

  server.handleClient();

  while(gpsSerial.available()){

    gps.encode(gpsSerial.read());
  }

  handleSOSButton();

  handleCallButton();

  while(sim800.available()){

    Serial.write(sim800.read());
  }
  
  if(!panicMode && millis() - alertTime > 5000){

    statusMessage = "SAFE";
  }
}

// ---------------- SOS BUTTON ----------------

void handleSOSButton(){

  bool state = digitalRead(SOS_BUTTON);

  if(state == LOW && !sosPressed){

    sosPressed = true;

    sosPressTime = millis();
  }

  if(state == HIGH && sosPressed){

    unsigned long pressDuration = millis() - sosPressTime;

    if(!panicMode && pressDuration < 3000 && !ignoreNextRelease){

      Serial.println("SHORT SOS");

      statusMessage = "SOS ALERT TRIGGERED";
      alertTime = millis();
      sosBeep();

      String msg = "SOS ALERT\nI NEED HELP\n\n";

      if(gps.location.isValid()){

        msg += "Location:\nhttps://maps.google.com/?q=";

        msg += String(gps.location.lat(),6);

        msg += ",";

        msg += String(gps.location.lng(),6);
      }
      else{

        msg += "GPS not fixed yet.";
      }

      sendSMS(msg);
    }

    ignoreNextRelease = false;

    sosPressed = false;
  }

  if(state == LOW && !panicMode && sosPressed){

    if(millis() - sosPressTime > 4000){

      Serial.println("ENTER PANIC");

      statusMessage = "PANIC MODE ACTIVATED";
      alertTime = millis();

      panicMode = true;

      digitalWrite(BUZZER,HIGH);
      delay(500);

      digitalWrite(BUZZER,LOW);

      String msg = "PANIC ALERT\nUSER IN DANGER\n\n";

      if(gps.location.isValid()){

        msg += "Location:\nhttps://maps.google.com/?q=";

        msg += String(gps.location.lat(),6);

        msg += ",";

        msg += String(gps.location.lng(),6);
      }
      else{

        msg += "GPS not fixed yet.";
      }

      sendSMS(msg);
    }
  }

  static bool panicButtonReleased = false;

  if(panicMode){

    static unsigned long lastBeep = 0;

    static bool buzzerState = false;

    if(millis() - lastBeep > 120){

      buzzerState = !buzzerState;

      digitalWrite(BUZZER, buzzerState);

      lastBeep = millis();
    }

    if(state == HIGH){

      panicButtonReleased = true;
    }

    if(panicButtonReleased && state == LOW){

      Serial.println("EXIT PANIC");
      
      statusMessage = "PANIC MODE EXITED";
      alertTime = millis();

      statusMessage = "SAFE";

      ignoreNextRelease = true;

      panicMode = false;

      panicButtonReleased = false;
    }
  }
}

// ---------------- CALL BUTTON ----------------

void handleCallButton(){

  bool currentCall = digitalRead(CALL_BUTTON);

  if(lastCallState == HIGH && currentCall == LOW){

    if(millis() - lastCallAction > callCooldown){

      delay(50);

      if(!callActive){

        Serial.println("Calling...");

        statusMessage = "EMERGENCY CALL ACTIVE";
        alertTime = millis();

        callStartBeep();

        sim800.println("ATD+919340025212;");

        callActive = true;
      }
      else{

        Serial.println("Hanging...");

        statusMessage = "CALL ENDED";
        alertTime = millis();

        callEndBeep();

        sim800.println("ATH");

        callActive = false;
      }

      lastCallAction = millis();
    }
  }

  lastCallState = currentCall;
}

// ---------------- SMS FUNCTION ----------------

void sendSMS(String msg){

  sim800.println("AT+CMGF=1");

  delay(600);

  sim800.println("AT+CMGS=\"+919340025212\"");

  delay(600);

  sim800.print(msg);

  delay(300);

  sim800.write(26);
}