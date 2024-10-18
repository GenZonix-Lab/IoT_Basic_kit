
/*
->sign_in to your sender email google account.
->Goto Security -> Enable 2-step verification
->In search bar enter "app_password" and click it.
->create App Name,Eg., ESP_mail -> it shown 16_digit password "copy and paste the[sender_app_password]"
*/

#include <WiFi.h>
#include <ESP_Mail_Client.h>
#include "credential.h"
//Ir_sensor_pin
//#define Ir_pin D5    //define the IR_sensor Pin connect to the D5 pin
const int trigPin = 2;
const int echoPin = 15 ;
long duration;
int distance;
String data = "free";
#define WiFi_SSID WiFi_Name     //replace your wifi name
#define WiFi_pwd password    //replace your wifi password
#define sender_mail sender_mail    //replace your email address
#define sender_app_password passcode //follow the Gmail_Auth_steps/refer our project1 document.
#define receiver_mail receiver_mail    //replace the receiver mail address to get notification.
SMTPSession smtp;

void setup() {
    Serial.begin(115200);
    //pinMode(Ir_pin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    WiFi.begin(WiFi_SSID, WiFi_pwd);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
}

void loop() {
  int sensor = value();
  if(sensor < 20 && data == "notfree"){
    int initial_val=0;
    while( initial_val < 60){
      sensor = value();
      delay(1000);
      initial_val++;
      Serial.print(initial_val);
      Serial.print(" : ");
      Serial.println(sensor);
      if(sensor >= 20){
        break;
      }
    }
    if(sensor < 20 && data == "notfree" ){
        sendMail("Parking Slot is occupied");
        delay(1000);
        data = "free";
      }
    }
  else if(sensor >= 20 && data == "free"){
      Serial.println("Parking Slot is free...");
      sendMail("Parking Slot is free...");
      data="notfree";
  }else{
  Serial.print("Distance : ");
  Serial.println(sensor);
  delay(1000);
  }
  Serial.flush();
}

void sendMail(String msg){
  Session_Config config;
  config.server.host_name = "smtp.gmail.com";
  config.server.port = 587;
  config.login.email = sender_mail;
  config.login.password = sender_app_password;

  SMTP_Message message;
  // Set the message content
  message.sender.name = "Parking Alert";
  message.sender.email = sender_mail;
  message.subject = "Parking Alert";
  message.addRecipient("Receiver", receiver_mail);
  message.text.content = msg;

// Set SMTP server settings
  smtp.callback([](SMTP_Status status) {
  Serial.println(status.info());
  });

   if (!smtp.connect(&config))
  {
    MailClient.printf("Connection error, Status Code: %d, Error Code: %d, Reason: %s\n", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error sending email, try again later.");
  } else {
    Serial.println("Email sent successfully!");
  }

}

int value() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}


