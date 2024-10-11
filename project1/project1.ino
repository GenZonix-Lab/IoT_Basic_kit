/* 
Download the App: 
Configure the App refer the Tutotial Video
Download this code to ESP32 
send the data to th ESP32 to change the colour.
 */
#include "BluetoothSerial.h"
#include "ESP32_WS2812_Lib.h"
#define LEDS_COUNT 1                //Number of LED in the LED pixel
#define LEDS_PIN   2                //Connect ws2812 DI pin to the esp32 D2 pin.
#define CHANNEL    0

ESP32_WS2812 strip = ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);   //name the module as strip and cofigure it.
const int color[5][3]={           //define the color code.
  {255,0,0},      // red
  {0,0,255},      // blue
  {0,255,0},      // green
  {255,255,255},  //white
  {0,0,0}         //off
};  
BluetoothSerial SerialBT;   //name the bluetooth serial communication.

void setup() {
  Serial.begin(115200);   //define baudrate for communication
  strip.begin();          
  strip.setBrightness(30);
  SerialBT.begin("Genzonix"); // Bluetooth device name
  Serial.println("The device started in client mode, make sure your server is running.");
  connectToServer();
}

void loop() {
  if (SerialBT.connected()) {
    if (SerialBT.available()) {
      String commandBT = SerialBT.readStringUntil('.');    // make a command end with '.'
      processCommand(commandBT);    //Led control function call
    }
  }
}

void connectToServer() {          //Bluetooth connection function
  Serial.print("Connecting to server... ");
  while (!SerialBT.connected(5000)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected!");
}

void processCommand(String command) {   //command through change LED colour function.
  if (command == "red") {
    Serial.println("It's red colour");
    strip.setLedColorData(0, color[0][0], color[0][1], color[0][2]);
    strip.show();
  } else if (command == "blue") {
    Serial.println("It's blue colour");
    strip.setLedColorData(0, color[1][0], color[1][1], color[1][2]);
    strip.show();
  } else if (command == "green") {
    Serial.println("It's green colour");
    strip.setLedColorData(0, color[2][0], color[2][1], color[2][2]);
    strip.show();
  } else if (command == "white") {
    Serial.println("It's white colour");
    strip.setLedColorData(0, color[3][0], color[3][1], color[3][2]);
    strip.show();
  } else if (command == "off") {
    Serial.println("LED OFF");
    strip.setLedColorData(0, color[4][0], color[4][1], color[4][2]);
    strip.show();
  } 
}
