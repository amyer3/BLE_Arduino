#include <bluefruit.h>

BLEUart bleuart;

// Function prototypes for packetparser.cpp
uint8_t readPacket (BLEUart *ble_uart, uint16_t timeout);
float   parsefloat (uint8_t *buffer);
void    printHex   (const uint8_t * data, const uint32_t numBytes);

int brightness = 0;    // how bright the LED is
int fadeAmount = 10;    // how many points to fade the LED by
// Packet buffer
extern uint8_t packetbuffer[];

#define LED         7
#define SPEAKER     27
#define VIB_MOTOR   16
#define BUTTON      30

void setup(void)
{

  Bluefruit.begin();
  Bluefruit.setName("MS Prototype Munnie");

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
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.ScanResponse.addName();

  pinMode(LED, OUTPUT);
  pinMode(VIB_MOTOR, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  pinMode(BUTTON, INPUT);
  digitalWrite(LED, LOW);
  digitalWrite(VIB_MOTOR, LOW);
  digitalWrite(SPEAKER, LOW);
}


void loop(void)
{
  // Wait for new data to arrive
  uint8_t len = readPacket(&bleuart, 500);
  if (len == 0) return;

  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';

    if (pressed) {
      switch (buttnum) {
        case 1:
          for (int i = 0; i < 7; i++) {
            digitalWrite(LED, HIGH);
            delay(50);
            digitalWrite(LED, LOW);
            delay(50);
          }
          break;

        case 2:
          for (int k = 0; k <= 4; k++) {
            for (int fIN = 0; fIN <= 24; fIN++) {
              fadeIN();
              delay(30);
            }
            for (int fOUT = 0; fOUT <= 24; fOUT++) {
              fadeOUT();
              delay(30);
            }

          }
          digitalWrite(LED, LOW);
          brightness = 0;
          break;

        case 3:
          buzz(9, 15);
          break;

        case 4:
          vibrate(3, 1000);
          break;
      }
    }
  }
  if (digitalRead(BUTTON == HIGH)) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
void fadeIN() {

  int bght = brightness + fadeAmount;
  analogWrite(LED, bght);
  brightness = bght;
}

void fadeOUT() {
  int bght = brightness - fadeAmount;
  analogWrite(LED, bght);
  brightness = bght;
}

void buzz(int times, int pause) {
  for (int i = 0; i < times; i++) {
    analogWrite(SPEAKER, 127);
    delay(pause);
    analogWrite(SPEAKER, 0);
    delay(pause);
    analogWrite(SPEAKER, 255);
    delay(pause);
    analogWrite(SPEAKER, 0);
  }
}

void vibrate(int times, int pause) {
  for (int i = 0; i <= times; i++) {
    analogWrite(VIB_MOTOR, 255);
    delay(pause);
    analogWrite(VIB_MOTOR, 0);
    delay(pause);
  }
}



