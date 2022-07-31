#include <IBusBM.h>
#include <Servo.h>


/*
  Move Front Back Left Right using Channel 1 and Channel 2.
  This controls the Hbridge
*/

IBusBM IBus; // IBus object
int PIN_MOTOR_1_A = 13; // IN1
int PIN_MOTOR_1_B = 7; // IN2
int PIN_MOTOR_2_A = 2; // IN3
int PIN_MOTOR_2_B = 4; // IN4
int PIN_MOTOR_1_PWM = 5; //ENA
int PIN_MOTOR_2_PWM = 3; //ENB

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MOTOR_1_A, OUTPUT);
  pinMode(PIN_MOTOR_1_B, OUTPUT);
  pinMode(PIN_MOTOR_2_A, OUTPUT);
  pinMode(PIN_MOTOR_2_B, OUTPUT);
  pinMode(PIN_MOTOR_1_PWM, OUTPUT);
  pinMode(PIN_MOTOR_2_PWM, OUTPUT);
  digitalWrite(PIN_MOTOR_1_A, LOW);
  digitalWrite(PIN_MOTOR_1_B, LOW);
  digitalWrite(PIN_MOTOR_2_A, LOW);
  digitalWrite(PIN_MOTOR_2_B, LOW);
  digitalWrite(PIN_MOTOR_1_PWM, 0);
  digitalWrite(PIN_MOTOR_2_PWM, 0);
  IBus.begin(Serial);    // iBUS connected to Serial0 - change to Serial1 or Serial2 port when required
  Serial.println("Start IBus2PWM");
}

int array_value[10] = {0}; // all elements 0

int transform_raw_to_xy(int value) {
  if(value > 0){
    /* Transform 1000us to 2000us values from flysky RX to -255 to 255 */
    float result = (value - 1500.0) * 255 / 500;
    int result_int = round(result);
    return result_int;
  }
  else {
    return 0;
  }
}

int round_to_255(int value) {
    if (value > 255) {
      return 255;
    }
    else if (value < -255) {
      return -255;
    }
    else {
      return value;
    }
}

int move_motor(int value_x, int value_y) {
  int motor_left = round_to_255(value_x + value_y);
  int motor_right = round_to_255(value_y - value_x);
  change_motor_direction(motor_left, PIN_MOTOR_1_A, PIN_MOTOR_1_B);
  change_motor_speed(abs(motor_left), PIN_MOTOR_1_PWM);
  change_motor_direction(motor_right, PIN_MOTOR_2_A, PIN_MOTOR_2_B);
  change_motor_speed(abs(motor_right), PIN_MOTOR_2_PWM);
}

int change_motor_direction(int direction, int pin_a, int pin_b) {
  if (direction > 0) {
    digitalWrite(pin_a, HIGH);
    digitalWrite(pin_b, LOW);
  }
  else if (direction < 0) {
    digitalWrite(pin_a, LOW);
    digitalWrite(pin_b, HIGH);
  }
  else {
    digitalWrite(pin_a, LOW);
    digitalWrite(pin_b, LOW);
  }
}

int change_motor_speed(int speed, int pin_pwm) {
  Serial.print("Write:");
  Serial.print(pin_pwm);
  Serial.print("=");
  Serial.print(speed);
  analogWrite(pin_pwm, speed);
}

void loop() {
  int bool_changed = 0;
  int value_raw;
  int value;
  for (int channel = 0; channel < 10; channel++) {
    value_raw = IBus.readChannel(channel); // get latest value for servo channel 1
    value = transform_raw_to_xy(value_raw);
    if (value_raw > 0 && array_value[channel] != value) {
      array_value[channel] = value;
      switch(channel) {
        case 0:
          move_motor(array_value[0], array_value[1]);
          break;
        case 1:
          move_motor(array_value[0], array_value[1]);
          break;
        case 2:
          break;
        case 3:
          break;
        case 4:
          break;
        case 5:
          break;
        case 6:
          break;
        case 7:
          break;
        case 8:
          break;
        case 9:
          break;
      }
      bool_changed = 1;
      Serial.print("CH");
      Serial.print(channel);
      Serial.print(":");
      Serial.print(value_raw);
      if (channel == 0) {
        Serial.print("=");
        Serial.print(value);
      }
      if (channel == 1) {
        Serial.print("=");
        Serial.print(value);
      }
      Serial.print(" ");
    }
  }
  if(bool_changed) {
    Serial.println("");
  }
  delay(100);
}

