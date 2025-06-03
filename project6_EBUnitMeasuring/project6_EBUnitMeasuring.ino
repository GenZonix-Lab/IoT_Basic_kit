#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/* Blynk Credentials */
#define BLYNK_TEMPLATE_ID "TMPL3JIKpfgbe"
#define BLYNK_TEMPLATE_NAME "EB unit analyzer"
#define BLYNK_AUTH_TOKEN "1udRa5Yygmbp1LGMwNxbCVwOHxcR3gkU"


#define impulse 3200  // change the impulse in EB meter
#define LDR_PIN 15    // sensor Data pin connects to GPIO 15 pin.

char ssid[] = "WiFi-Name";  //change the WiFi name
char pass[] = "WiFi-Password";  //change the WiFi password

int pulse = 0;
float unit;

WiFiServer wifiServer(23);  // Telnet server port number 
WiFiClient client;
BlynkTimer timer;

void sendSensor() {
  if (!digitalRead(LDR_PIN)) {
    if (++pulse >= impulse/10) {
      pulse = 0;
      unit++;
    }
    delay(300);
    if (client) {
      client.printf("Pulse: %d    Unit: %.1f\n", pulse, unit / 10);
    }
    else if (!client || !client.connected()) {
    client = wifiServer.available();  // Wait for a new connection
  }
  }
}

void checkClientInput() {
  if (client && client.connected() && client.available()) {
    float data = client.parseFloat();
    if(data > 100.00){
      unit = data;
      client.print("Unit initialized to: ");
      client.println(unit);
    }
  } else if (!client || !client.connected()) {
    client = wifiServer.available();  // Wait for a new connection
  }
}

void runningBlynk() {
  Blynk.virtualWrite(V0, pulse);
  Blynk.virtualWrite(V1, unit / 10);
}
void checkWiFiReconnect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
  }
}

void setup() {
  pinMode(LDR_PIN, INPUT);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(100);
  wifiServer.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  while(!unit){
    checkClientInput();
    delay(100);
  }
  timer.setInterval(5000L,runningBlynk);
  timer.setInterval(5000L, checkWiFiReconnect);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  } else {
    Blynk.disconnect();  // Prevent run() from hanging
  }
  sendSensor();
  timer.run();
}
