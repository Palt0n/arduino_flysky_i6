#include <IBusBM.h>
#include <Servo.h>

// TODO: Handle case when no IBus Signal (RX Switched On state)

IBusBM IBus; // IBus object
Servo list_servo[2];
uint8_t PIN_PWM_1 = 3;
uint8_t PIN_PWM_2 = 5;
uint8_t PIN_PWM_3 = 6;
uint8_t PIN_PWM_4 = 9;
uint8_t PIN_PWM_5 = 10;
uint8_t PIN_PWM_6 = 11;
uint8_t PIN_DIGITAL_1 = 2;
uint8_t PIN_DIGITAL_2 = 4;
uint8_t PIN_DIGITAL_3 = 7;
uint8_t PIN_DIGITAL_4 = 8;
uint8_t PIN_DIGITAL_5 = 12;
uint8_t PIN_DIGITAL_LED = 13;

uint8_t FS_CHANNEL_1 = 0;
uint8_t FS_CHANNEL_2 = 1;
uint8_t FS_CHANNEL_3 = 2;
uint8_t FS_CHANNEL_4 = 3;
uint8_t FS_CHANNEL_5 = 4;
uint8_t FS_CHANNEL_6 = 5;
uint8_t FS_CHANNEL_7 = 6;
uint8_t FS_CHANNEL_8 = 7;
uint8_t FS_CHANNEL_9 = 8;
uint8_t FS_CHANNEL_10 = 9;
uint8_t LIST_FS_CHANNEL[10] = {
  FS_CHANNEL_1,
  FS_CHANNEL_2,
  FS_CHANNEL_3,
  FS_CHANNEL_4,
  FS_CHANNEL_5,
  FS_CHANNEL_6,
  FS_CHANNEL_7,
  FS_CHANNEL_8,
  FS_CHANNEL_9,
  FS_CHANNEL_10
};

uint8_t FS_CHANNEL_PUMP = FS_CHANNEL_2;
uint8_t PIN_DIGITAL_PUMP = PIN_PWM_1;
uint8_t LIST_FS_CHANNEL_RELAY[4] = {
  FS_CHANNEL_7,
  FS_CHANNEL_8,
  FS_CHANNEL_9,
  FS_CHANNEL_10
};
uint8_t LIST_PIN_DIGITAL_RELAY[4] = {
  PIN_DIGITAL_2,
  PIN_DIGITAL_3,
  PIN_DIGITAL_4,
  PIN_DIGITAL_5
};
uint16_t LIST_FS_VALUES[10] = {0}; // all elements 0

void setup() {
  pinMode(PIN_DIGITAL_PUMP, OUTPUT);
  digitalWrite(PIN_DIGITAL_PUMP, LOW);

  for (int i = 0; i < sizeof(LIST_PIN_DIGITAL_RELAY); i++) {
    pinMode(LIST_PIN_DIGITAL_RELAY[i], OUTPUT);
    digitalWrite(LIST_PIN_DIGITAL_RELAY[i], LOW);
  }
  
  IBus.begin(Serial);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required
  Serial.println("Start IBus2PWM");
}

int transform_us_to_255(int value) {
  if(value > 0){
    /* Transform 1000us to 2000us values from flysky RX to -255 to 255 */
    float result = (value - 1000.0) * 255 / 1000;
    int result_int = round(result);
    return result_int;
  }
  else {
    return 0;
  }
}

uint16_t transform_us_to_01(uint16_t value, uint16_t threshold) {
  if(value > 0){
    if (value > threshold) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

uint16_t transform_us_to_012(uint16_t value, uint16_t threshold_high, uint16_t threshold_low) {
  if(value > 0){
    if (value > threshold_high) {
      return 2;
    }
    else if (value < threshold_low)
    {
      return 0;
    }
    else {
      return 1;
    }
  }
  else {
    return 0;
  }
}

void control_pump() {
  uint16_t value = LIST_FS_VALUES[FS_CHANNEL_PUMP];
  value = transform_us_to_01(value, 1200);
  if(value){
    // If signal low, turn off pump
    digitalWrite(PIN_DIGITAL_PUMP, LOW);
    digitalWrite(PIN_DIGITAL_LED, LOW);
  }
  else {
    // If signal high, turn on pump
    digitalWrite(PIN_DIGITAL_PUMP, HIGH);
    digitalWrite(PIN_DIGITAL_LED, HIGH);
  }
}

void control_relays() {
  for (int i = 0; i < sizeof(LIST_FS_CHANNEL_RELAY); i++) {
    uint16_t value = LIST_FS_VALUES[LIST_FS_CHANNEL_RELAY[i]];
    value = transform_us_to_012(value, 1750, 1250);
    if(value){
      digitalWrite(LIST_PIN_DIGITAL_RELAY[i], HIGH);
    }
    else {
      digitalWrite(LIST_PIN_DIGITAL_RELAY[i], LOW);
    }
  }
}

void loop() {
  int bool_changed = 0;
  uint16_t value_raw;
  // Load values into list_value_raw
  for (int i = 0; i < sizeof(LIST_FS_CHANNEL); i++) {
    value_raw = IBus.readChannel(i); // get latest value for servo channel 1
    if (value_raw > 0 && LIST_FS_VALUES[i] != value_raw) {
      bool_changed = 1;
      LIST_FS_VALUES[i] = value_raw;
    }
  }

  // Relay control
  if(bool_changed) {
    bool_changed = 0;
    // Pump control
    control_pump();
    // Relay control
    control_relays();
    //print
    for (int i = 0; i < sizeof(LIST_FS_CHANNEL); i++) {
      Serial.print("CH");
      Serial.print(LIST_FS_CHANNEL[i]);
      Serial.print("=");
      Serial.print(LIST_FS_VALUES[i]);
      Serial.print(" ");
    }
    Serial.println("");
  }
}

