
/*
->sign_in to your sender email google account.
->Goto Security -> Enable 2-step verification
->In search bar enter "app_password" and click it.
->create App Name,Eg., ESP_mail -> it shown 16_digit password "copy and paste the[sender_app_password]"
*/

#include <WiFi.h>
#include <ESP_Mail_Client.h>
//Ir_sensor_pin
const int Ir_pin = 15;    //define the IR_sensor Pin connect to the D5 pin

String data = "close";
#define WiFi_SSID "ROBOLIX"     //replace your wifi name
#define WiFi_pwd "asdfghjkl"    //replace your wifi password
#define sender_mail "solutiongenzonix@gmail.com"    //replace your email address
#define sender_app_password "ekvm szfe ckfr lqew" //follow the Gmail_Auth_steps/refer our project1 document.
#define receiver_mail "ahamedazeem67@gmail.com"    //replace the receiver mail address to get notification.

SMTPSession smtp;

void setup() {
    Serial.begin(115200);
    pinMode(Ir_pin, INPUT);
    WiFi.begin(WiFi_SSID, WiFi_pwd);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
}

void loop() {
  int sensor = digitalRead(Ir_pin);
  if(sensor && data == "open"){
    sendMail("⚠️Door is opened... \n⚠️Mysterious Activity placed here...");
    Serial.println("⚠️Door is opened... \n⚠️Mysterious Activity placed here...");
    delay(1000);
    data = "close";
  }
  else if(!sensor && data == "close"){
    sendMail("Door is closed");
    Serial.println("Door is closed");
    data="open";
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
  message.sender.name = "Email Smart Door Alert System";
  message.sender.email = sender_mail;
  message.subject = "Open Door Alert";
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