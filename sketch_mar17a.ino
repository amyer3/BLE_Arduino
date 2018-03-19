#include <bluefruit.h>
#include "pitches.h"

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

#define LED         15
#define SPEAKER     12
#define VIB_MOTOR   30

int melody[]= {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[]={
  4, 8, 8, 4, 4, 4, 4, 4
};
int arrayLen = sizeof(melody);

void setup(void)
{

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
  Bluefruit.Advertising.addService(bleuart);
  Bluefruit.ScanResponse.addName();

  pinMode(LED, OUTPUT);
  pinMode(VIB_MOTOR, OUTPUT);
  pinMode(SPEAKER, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(VIB_MOTOR, LOW);
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
          // makes the light blink fast
          for (int i = 0; i < 8; i++) {
            digitalWrite(LED, HIGH);
            delay(100);
            digitalWrite(LED, LOW);
            delay(100);
          }
          break;

        case 2:
          // make the light blink fade in-out
          for (int k = 0; k >= 4; k + 1) {
            fade();
          }
          break;

        case 3:
          // makes the speaker play noise
          for (int thisNote =0; thisNote < arrayLen; thisNote++){
            int dur = 1000/noteDurations[thisNote];
            tone(SPEAKER, melody[thisNote];
            int pause = dur * 1.30;
            delay(pause);
            noTone(SPEAKER);
          }
          break;

        case 4:
          // makes the vibration motor go
          for (int i = 0; i <= 10; i++) {
            analogWrite(VIB_MOTOR, 255);
            delay(100);
            analogWrite(VIB_MOTOR, 0);
            delay(100);
          }
          break;
      }
    }
  }
}
void fade() {
  analogWrite(LED, brightness);
  brightness = brightness + fadeAmount;
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  delay(30);
}

