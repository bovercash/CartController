#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Ticker.h>
#include <ESP8266WebServer.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <page.h>
#include <motor.h>

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

#define SSID "xxxxxxx"
#define PASSWORD "xxxxxxx"

#define L1 D0
#define L2 D5
#define R1 D6
#define R2 D7

void connectToNetwork();
void getPage();
void performAction();
void stepMotor();

// Global Variables
ESP8266WebServer server(80);
LiquidCrystal_I2C lcd(0x3F,20,4);

Motor motor(L1, L2, R1, R2, 10, 240);

Ticker timer(stepMotor, 500);

void setup() {
    Serial.begin(9600);
    delay(10);
    Serial.println("\n");
    Serial.println("Starting WeMos D1 mini");

    motor.init();

    lcd.init(); 
    lcd.backlight();
    lcd.home();

    connectToNetwork();
    timer.start();
}

void connectToNetwork() {

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
            return;
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
    timer.update();    
    server.handleClient();
    delay(100);
}

void getPage() {
    server.send(200, "text/html", PAGE_HTML);
}

void performAction() {
    if(server.hasArg("action")){
           Serial.println(server.arg("action"));

            if(server.arg("action") == "ahead"){
                motor.setAction(MotorActions::FORWARD);
            } else if(server.arg("action") == "reverse"){
               motor.setAction(MotorActions::BACKWARD);
            } else if(server.arg("action") == "stop"){
               motor.setAction(MotorActions::STOPPED);
            } else if(server.arg("action") == "lup"){
               motor.setAction(MotorActions::LEFT_FORWARD);
            } else if(server.arg("action") == "ldown"){
               motor.setAction(MotorActions::LEFT_BACKWARD);
            } else if(server.arg("action") == "rup"){
               motor.setAction(MotorActions::RIGHT_FORWARD);
            } else if(server.arg("action") == "rdown"){
               motor.setAction(MotorActions::RIGHT_BACKWARD);
            }
    }
    getPage();
}


void stepMotor() {
    Serial.println("Stepping Motors");
    motor.step();
}

