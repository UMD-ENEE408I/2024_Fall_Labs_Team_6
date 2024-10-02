#include <Arduino.h>
#include <Adafruit_MCP3008.h>
#include <Encoder.h>

// ADC (line sensor)
Adafruit_MCP3008 adc1;
Adafruit_MCP3008 adc2;

const unsigned int ADC_1_CS = 2;
const unsigned int ADC_2_CS = 17;

int adc1_buf[8];
int adc2_buf[8];

uint8_t lineArray[13];

// Encoders
const unsigned int M1_ENC_A = 39;
const unsigned int M1_ENC_B = 38;
const unsigned int M2_ENC_A = 37;
const unsigned int M2_ENC_B = 36;

// Motors
const unsigned int M1_IN_1 = 13;
const unsigned int M1_IN_2 = 12;
const unsigned int M2_IN_1 = 25;
const unsigned int M2_IN_2 = 14;

const unsigned int M1_IN_1_CHANNEL = 8;
const unsigned int M1_IN_2_CHANNEL = 9;
const unsigned int M2_IN_1_CHANNEL = 10;
const unsigned int M2_IN_2_CHANNEL = 11;

const unsigned int M1_I_SENSE = 35;
const unsigned int M2_I_SENSE = 34;

const unsigned int PWM_MAX = 255;
const int freq = 5000;
const int resolution = 8; // 8-bit resolution -> PWM values go from 0-255

// LED
const int ledChannel = 0;

// PID
const int base_pid = 80; // Base speed for robot
const float mid = 6;

float e = 0;
float d_e = 0;
float total_e = 0;

// Assign values to the following feedback constants:
float Kp = 2.0;
float Kd = 1.0;
float Ki = 0.1;

/*
 *  Line sensor functions
 */
void readADC() {
  for (int i = 0; i < 8; i++) {
    adc1_buf[i] = adc1.readADC(i);
    adc2_buf[i] = adc2.readADC(i);
  }
}

// Converts ADC readings to binary array lineArray[] (Check threshold for your robot) 
void digitalConvert() {
  int threshold = 700;
  for (int i = 0; i < 7; i++) {
    if (adc1_buf[i] > threshold) {
      lineArray[2 * i] = 0;
    } else {
      lineArray[2 * i] = 1;
    }

    if (i < 6) {
      if (adc2_buf[i] > threshold) {
        lineArray[2 * i + 1] = 0;
      } else {
        lineArray[2 * i + 1] = 1;
      }
    }
  }
}

// Calculate robot's position on the line 
float getPosition(/* Arguments */) {
  int position = 6;  // Default position in the middle for 13 sensors
  int sum = 0;
  int weightedSum = 0;

  // Calculate weighted sum based on line sensor positions
  for (int i = 0; i < 13; i++) {
    if (lineArray[i] == 1) {  // Line detected
      weightedSum += i;
      sum++;
    }
  }

  // If no line detected, return middle position
  if (sum != 0) {
    position = weightedSum / sum;
  }

  return position;
}

/*
 *  Movement functions
 */
void M1_forward(int pwm_value) {
  ledcWrite(M1_IN_1_CHANNEL, 0);
  ledcWrite(M1_IN_2_CHANNEL, pwm_value);
}
void M2_forward(int pwm_value) {
  ledcWrite(M2_IN_1_CHANNEL, 0);
  ledcWrite(M2_IN_2_CHANNEL, pwm_value);
}

void M1_backward(int pwm_value) {
  ledcWrite(M1_IN_1_CHANNEL, pwm_value);
  ledcWrite(M1_IN_2_CHANNEL, 0);
}
void M2_backward(int pwm_value) {
  ledcWrite(M2_IN_1_CHANNEL, pwm_value);
  ledcWrite(M2_IN_2_CHANNEL, 0);
}

void M1_stop() {
  ledcWrite(M1_IN_1_CHANNEL, PWM_MAX);
  ledcWrite(M1_IN_2_CHANNEL, PWM_MAX);
}
void M2_stop() {
  ledcWrite(M2_IN_1_CHANNEL, PWM_MAX);
  ledcWrite(M2_IN_2_CHANNEL, PWM_MAX);
}

void turnCorner(/* Arguments */) {
  /* 
   * Use the encoder readings to turn the robot 90 degrees clockwise or 
   * counterclockwise depending on the argument. You can calculate when the 
   * robot has turned 90 degrees using either the IMU or the encoders + wheel measurements
   */
  // Example implementation for a clockwise turn
  int targetTicks = 360; // Adjust based on your robot's configuration
  int currentTicks = 0;
  Encoder enc1(M1_ENC_A, M1_ENC_B);
  Encoder enc2(M2_ENC_A, M2_ENC_B);

  M1_forward(base_pid);
  M2_backward(base_pid);
  
  while (currentTicks < targetTicks) {
    currentTicks = (enc1.read() + enc2.read()) / 2;
  }

  M1_stop();
  M2_stop();
}

/*
 *  setup and loop
 */
void setup() {
  Serial.begin(115200);

  ledcSetup(M1_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M1_IN_2_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_1_CHANNEL, freq, resolution);
  ledcSetup(M2_IN_2_CHANNEL, freq, resolution);

  ledcAttachPin(M1_IN_1, M1_IN_1_CHANNEL);
  ledcAttachPin(M1_IN_2, M1_IN_2_CHANNEL);
  ledcAttachPin(M2_IN_1, M2_IN_1_CHANNEL);
  ledcAttachPin(M2_IN_2, M2_IN_2_CHANNEL);

  adc1.begin(ADC_1_CS);  
  adc2.begin(ADC_2_CS);

  pinMode(M1_I_SENSE, INPUT);
  pinMode(M2_I_SENSE, INPUT);

  M1_stop();
  M2_stop();

  delay(100);
}

void loop() {
  Encoder enc1(M1_ENC_A, M1_ENC_B);
  Encoder enc2(M2_ENC_A, M2_ENC_B);

  while (true) {
    int u;
    int rightWheelPWM;
    int leftWheelPWM;
    float pos;

    readADC();
    digitalConvert();

    pos = getPosition(/* Arguments */);
    
    // Define the PID errors
    e = mid - pos;  // Error
    d_e = e - previous_e;  // Derivative of error
    total_e += e;  // Integral of error

    // Implement PID control
    u = (Kp * e) + (Kd * d_e) + (Ki * total_e);

    // Calculate wheel speeds
    rightWheelPWM = base_pid + u;
    leftWheelPWM = base_pid - u;

    // Ensure PWM values are within bounds
    rightWheelPWM = constrain(rightWheelPWM, 0, PWM_MAX);
    leftWheelPWM = constrain(leftWheelPWM, 0, PWM_MAX);

    M1_forward(rightWheelPWM);
    M2_forward(leftWheelPWM);

    // Check for corners
    if (/* Condition for corner */) {
      turnCorner(/* Arguments */);
    }

    delay(50);  // Small delay to prevent overloading
  }
}
