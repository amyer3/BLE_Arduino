#include <bluefruit.h>

BLEUart bleuart;

// Function prototypes for packetparser.cpp
uint8_t readPacket (BLEUart *ble_uart, uint16_t timeout);
float   parsefloat (uint8_t *buffer);
void    printHex   (const uint8_t * data, const uint32_t numBytes);
int led = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by
// Packet buffer
extern uint8_t packetbuffer[];
#define testLED   15
void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit52 Controller App Example"));
  Serial.println(F("-------------------------------------------"));

  Bluefruit.begin();
  Bluefruit.setName("Bluefruit52");

  // Configure and start the BLE Uart service
  bleuart.begin();

  // Set up the advertising packet
  setupAdv();

  // Start advertising
  Bluefruit.Advertising.start();
}

void setupAdv(void)
{
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(bleuart);

  // There is no room for 'Name' in the Advertising packet
  // Use the optional secondary Scan Response packet for 'Name' instead
  Bluefruit.ScanResponse.addName();

  pinMode(testLED, OUTPUT);
  digitalWrite(testLED, LOW);
}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
  // Wait for new data to arrive
  uint8_t len = readPacket(&bleuart, 500);
  if (len == 0) return;

  // Buttons
  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    Serial.print ("Button "); Serial.print(buttnum);
    if (pressed) {
      switch (buttnum) {
        case 1:
          for (int i = 0; i < 8; i++) {
            digitalWrite(testLED, HIGH);
            delay(300);
            digitalWrite(testLED, LOW);
            delay(300);
          }
          break;
        case 2:
          for (int k = 0; k >= 4; k+1) {
            fade();
          }
          break;
        case 3:
          digitalWrite(LED_BUILTIN, HIGH);
          delay(3000);
          digitalWrite(LED_BUILTIN, LOW);
          break;
        case 4:
          digitalWrite(LED_BUILTIN, HIGH);
          delay(4000);
          digitalWrite(LED_BUILTIN, LOW);
          break;
      }
    }
  }
}
void fade() {
  analogWrite(testLED, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

