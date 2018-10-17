#include <Arduino.h>
#include <ESP8266WiFi.h> 

#include <ESP8266WebServer.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <page.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

#define SSID "foo"
#define PASSWORD "bar"
#define PWM1 D6
#define PWM2 D7
#define MOTOR_SP 255

// This will represent the motor direction.  If I have things backwards
// it will be easier to reverse the values than rewrite or rewire
#define RB = 1
#define LB = 2

int lState = 0;
int rState = 0;
int lMoveStep = 0;
int rMoveStep = 0;

ESP8266WebServer server(80);
LiquidCrystal_I2C lcd(0x3F,20,4);

void setup() {
    Serial.begin(9600);
    delay(10);
    Serial.println("\n");
    Serial.println("Starting WeMos D1 mini");

    pinMode(PWM1,   OUTPUT);
    pinMode(PWM2,   OUTPUT);

    lcd.init(); 
    lcd.backlight();
    lcd.home();

    connectToNetwork();
}

int connectToNetwork() {
    WiFi.begin(SSID, PASSWORD);
    Serial.print("\nConnecting to network...");

    lcd.print("Connecting...");
    lcd.setCursor(0, 1);

    int i=0;
    while(WiFi.status() != WL_CONNECTED) {
        if(i > 100){
            Serial.println("Connection Failed - Abort");
            lcd.print("Connection Failed");

            WiFi.disconnect();
            return -1;
        }

        delay(1000);
        Serial.print(".");
        i++;
    }

    Serial.print("\nConnected at ");
    Serial.println(WiFi.localIP());

    lcd.home();
    lcd.print("Connected to");
    lcd.setCursor(0,1);
    lcd.print(WiFi.localIP());

    server.on("/", HTTP_GET, getPage);
    server.on("/do", HTTP_GET, performAction);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {

        if(lState == 1){
            Serial.println("Moving Forwards");
            digitalWrite(PWM1, LOW);
            analogWrite(PWM2, MOTOR_SP);
            lState = 0;
        } else if(lState == -1){
            digitalWrite(PWM2, LOW);
            analogWrite(PWM1, MOTOR_SP);
            lState = 0;
        }

    server.handleClient();
}

void getPage() {
    server.send(200, "text/html", PAGE_HTML);
}

void performAction() {
    if(server.hasArg("action")){
           Serial.println(server.arg("action"));

            if(server.arg("action") == "lup" || server.arg("action") == "ahead"){
                lState = 1;
            } else if(server.arg("action") == "ldown" || server.arg("action") == "reverse"){
                lState = -1;
            } else {
                lState = 0;
            }
    }
    getPage();
}

