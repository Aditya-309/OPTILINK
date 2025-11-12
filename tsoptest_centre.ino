// hw477_dual_gpio_test.ino
#include <Arduino.h>

#define RX_A 27   // change to any safe GPIO: 16,17,21,22,23,25,26,27,32,33
#define RX_B 33

void setup() {
  Serial.begin(115200);
  pinMode(RX_A, INPUT_PULLUP);  // HW-477 already has pull-up, but this helps on breadboards
  pinMode(RX_B, INPUT_PULLUP);
  Serial.println("HW-477 dual test on GPIO27 & GPIO33. Expect A/B=1 idle, 0 during IR.");
}

void loop() {
  int a = digitalRead(RX_A);
  int b = digitalRead(RX_B);
  Serial.printf("A:%d  B:%d\n", a, b);
  delay(1000);
}
