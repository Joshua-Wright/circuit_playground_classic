#include <Arduino.h>
#include <FastLED.h>
#define NUM_LEDS 10
#define DATA_PIN 17
#include <Adafruit_CircuitPlayground.h>

#define CLICKTHRESHHOLD 120

void colorDance();

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  CircuitPlayground.begin();

  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);   // 2, 4, 8 or 16 G!
  CircuitPlayground.setAccelTap(2, CLICKTHRESHHOLD);
  attachInterrupt(digitalPinToInterrupt(7), colorDance, RISING);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}

void colorDance() {
  for (uint8_t cycle = 0; cycle < 3; cycle++) {
    for (uint8_t baseHue = 0; baseHue < 255; baseHue++) {
      for (size_t i = 0; i < 10; i++) {
        leds[i].setHSV(baseHue + 25*i, 255, 255);
      }
      FastLED.show();
      delay(1);
    }
  }
}

uint8_t hue=0;
void loop() {
  for (size_t i = 0; i < 10; i++) {
    leds[i].setHSV(hue, 255, 255);
  }
  FastLED.show();

  float motionX = CircuitPlayground.motionX();
  float motionY = CircuitPlayground.motionY();
  float motionZ = CircuitPlayground.motionZ();
  float totalMotion = motionX*motionX + motionY*motionY + motionZ*motionZ;
  totalMotion /= 100.0;
  if (totalMotion < 1.0) {
    totalMotion = 0.0;
  } else {
    totalMotion *= 10.0;
  }
  if (totalMotion > 8) {
    totalMotion = 8;
  }
  uint8_t speedHue = totalMotion;
  Serial.print(speedHue);
  Serial.print("\t");
  for (uint8_t i = 0; i < speedHue; i++) {
    Serial.print("#");
  }
  Serial.println();
  
  hue += speedHue;
  delay(10);
}
