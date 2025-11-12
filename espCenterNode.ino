#include <Arduino.h>
#include <BluetoothSerial.h>
#include "driver/ledc.h"

#define LED_TO_A      18
#define LED_TO_B      19
#define RX_FROM_A     33    // Serial1 RX
#define RX_FROM_B     27    // Serial2 RX

#define CARRIER_FREQ   38000
#define LEDC_MODE      LEDC_LOW_SPEED_MODE
#define LEDC_TIMER     LEDC_TIMER_0
#define LEDC_RES       LEDC_TIMER_8_BIT
#define LEDC_CH_A      LEDC_CHANNEL_0
#define LEDC_CH_B      LEDC_CHANNEL_1
#define LEDC_DUTY      70
#define IR_BAUD        3000
#define BAUD_DELAY_US  340
#define INTERBYTE_US   2000

BluetoothSerial SerialBT;

// --- LEDC helpers ---
static void setupLEDC(uint8_t pin, ledc_channel_t ch){
  ledc_timer_config_t t={LEDC_MODE, LEDC_RES, LEDC_TIMER, CARRIER_FREQ, LEDC_AUTO_CLK};
  ledc_timer_config(&t);
  ledc_channel_config_t c={pin, LEDC_MODE, ch, LEDC_INTR_DISABLE, LEDC_TIMER, 0, 0};
  ledc_channel_config(&c);
}
static inline void carrierOn(ledc_channel_t ch){ ledc_set_duty(LEDC_MODE,ch,LEDC_DUTY); ledc_update_duty(LEDC_MODE,ch); }
static inline void carrierOff(ledc_channel_t ch){ ledc_set_duty(LEDC_MODE,ch,0);         ledc_update_duty(LEDC_MODE,ch); }

// --- Correct UART-over-IR mapping ---
static inline void driveLineLow(ledc_channel_t ch){ carrierOn(ch); }   // LOW => carrier ON
static inline void driveLineHigh(ledc_channel_t ch){ carrierOff(ch); } // HIGH => carrier OFF
static inline void sendLine(ledc_channel_t ch, bool lineIsLow){ if(lineIsLow) driveLineLow(ch); else driveLineHigh(ch); delayMicroseconds(BAUD_DELAY_US); }

static inline void sendByte(ledc_channel_t ch, char c){
  sendLine(ch, true);                         // START bit LOW
  for(int i=0;i<8;i++){                       // data LSB first
    bool bit1 = ((c>>i)&1);
    sendLine(ch, !bit1);                      // '1' -> HIGH (OFF); '0' -> LOW (ON)
  }
  sendLine(ch, false);                        // STOP bit HIGH
}

static inline void sendMessage(ledc_channel_t ch, const String& s){
  sendByte(ch, 0x55); delayMicroseconds(INTERBYTE_US);   // preamble
  sendByte(ch, 0x55); delayMicroseconds(INTERBYTE_US);
  for(char c: s){ sendByte(ch, c); delayMicroseconds(INTERBYTE_US); }
  sendByte(ch, '\n');
}

static inline String readLine(Stream &S){ if(!S.available()) return ""; String s=S.readStringUntil('\n'); s.trim(); return s; }
static inline void routeAndSend(const String& msg){
  if(!msg.length()) return;
  if(msg.length()>2 && (msg[1]==':'||msg[1]=='>')){
    char d=toupper(msg[0]); String p=msg.substring(2);
    if(d=='A'){ sendMessage(LEDC_CH_A,p); Serial.printf("[Sent->A] %s\n",p.c_str()); SerialBT.printf("[Sent->A] %s\n",p.c_str()); return; }
    if(d=='B'){ sendMessage(LEDC_CH_B,p); Serial.printf("[Sent->B] %s\n",p.c_str()); SerialBT.printf("[Sent->B] %s\n",p.c_str()); return; }
  }
  sendMessage(LEDC_CH_A, msg); sendMessage(LEDC_CH_B, msg);
  Serial.printf("[Broadcast] %s\n", msg.c_str());
  SerialBT.printf("[Broadcast] %s\n", msg.c_str());
}

void setup(){
  Serial.begin(115200); delay(1200);
  Serial.println("\n=== Center boot ===");

  setupLEDC(LED_TO_A, LEDC_CH_A);
  setupLEDC(LED_TO_B, LEDC_CH_B);

  // Hardware UART RX from HW-477 (idle HIGH/start LOW) -> invert=false
  Serial1.begin(IR_BAUD, SERIAL_8N1, RX_FROM_A, -1, false);
  Serial2.begin(IR_BAUD, SERIAL_8N1, RX_FROM_B, -1, false);

  SerialBT.begin("Center_Node");
  Serial.println("Center ready. Use 'A: hi' / 'B: hi' or plain text to broadcast.");
}

void loop(){
  static String bufA, bufB;

  while(Serial1.available()){
    char c=(char)Serial1.read();
    if(c=='\n'){ if(bufA.length()){ Serial.printf("[From A] %s\n",bufA.c_str()); SerialBT.printf("[From A] %s\n",bufA.c_str()); bufA=""; } }
    else bufA+=c;
  }
  while(Serial2.available()){
    char c=(char)Serial2.read();
    if(c=='\n'){ if(bufB.length()){ Serial.printf("[From B] %s\n",bufB.c_str()); SerialBT.printf("[From B] %s\n",bufB.c_str()); bufB=""; } }
    else bufB+=c;
  }

  String s=readLine(Serial);   if(s.length()) routeAndSend(s);
  s=readLine(SerialBT);        if(s.length()) routeAndSend(s);
}