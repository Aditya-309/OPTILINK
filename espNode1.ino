#include <Arduino.h>
#include <BluetoothSerial.h>
#include "driver/ledc.h"

#define IR_LED_PIN   18
#define IR_RX_PIN    32   // Serial1 RX

#define CARRIER_FREQ   38000
#define LEDC_MODE      LEDC_LOW_SPEED_MODE
#define LEDC_TIMER     LEDC_TIMER_0
#define LEDC_RES       LEDC_TIMER_8_BIT
#define LEDC_CHANNEL   LEDC_CHANNEL_0
#define LEDC_DUTY      70
#define IR_BAUD        3000
#define BAUD_DELAY_US  340
#define INTERBYTE_US   2000

BluetoothSerial SerialBT;

static void setupLEDC(uint8_t pin, ledc_channel_t ch){
  ledc_timer_config_t t={LEDC_MODE, LEDC_RES, LEDC_TIMER, CARRIER_FREQ, LEDC_AUTO_CLK};
  ledc_timer_config(&t);
  ledc_channel_config_t c={pin, LEDC_MODE, ch, LEDC_INTR_DISABLE, LEDC_TIMER, 0, 0};
  ledc_channel_config(&c);
}
static inline void carrierOn(){  ledc_set_duty(LEDC_MODE,LEDC_CHANNEL,LEDC_DUTY); ledc_update_duty(LEDC_MODE,LEDC_CHANNEL); }
static inline void carrierOff(){ ledc_set_duty(LEDC_MODE,LEDC_CHANNEL,0);         ledc_update_duty(LEDC_MODE,LEDC_CHANNEL); }

static inline void driveLineLow(){  carrierOn();  }
static inline void driveLineHigh(){ carrierOff(); }
static inline void sendLine(bool low){ if(low) driveLineLow(); else driveLineHigh(); delayMicroseconds(BAUD_DELAY_US); }

static inline void sendByte(char c){
  sendLine(true);                               // START low
  for(int i=0;i<8;i++){ bool bit1=(c>>i)&1; sendLine(!bit1); }
  sendLine(false);                              // STOP high
}
static inline void sendMessage(const String& s){
  sendByte(0x55); delayMicroseconds(INTERBYTE_US);
  sendByte(0x55); delayMicroseconds(INTERBYTE_US);
  for(char c: s){ sendByte(c); delayMicroseconds(INTERBYTE_US); }
  sendByte('\n');
}

void setup(){
  Serial.begin(115200); delay(1200);
  Serial.println("\n=== Corner A boot ===");
  setupLEDC(IR_LED_PIN, LEDC_CHANNEL);

  // UART RX from HW-477 (invert=false)
  Serial1.begin(IR_BAUD, SERIAL_8N1, IR_RX_PIN, -1, false);

  SerialBT.begin("Corner_A");
  Serial.println("Corner A ready. Type to send to Center.");
}

void loop(){
  // TX (Serial/BT)
  if(Serial.available()){ String s=Serial.readStringUntil('\n'); s.trim(); if(s.length()){ sendMessage(s); Serial.println("[Sent IR] "+s); SerialBT.println("[Sent IR] "+s);} }
  if(SerialBT.available()){ String s=SerialBT.readStringUntil('\n'); s.trim(); if(s.length()){ sendMessage(s); Serial.println("[Sent IR] "+s); SerialBT.println("[Echo] "+s);} }

  // RX (UART1)
  static String buf="";
  while(Serial1.available()){
    char c=(char)Serial1.read();
    if(c=='\n'){ if(buf.length()){ Serial.println("[From Center] "+buf); SerialBT.println("[From Center] "+buf); buf=""; } }
    else buf+=c;
  }
}