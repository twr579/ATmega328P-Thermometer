#include <Arduino_FreeRTOS.h>

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the sensor is attached to
byte pin[] = { 5, 6, 8, 9, 10, 3, 2 };
int number[10][7] = {
  { 1, 1, 1, 1, 1, 1, 0, }, // 0
  { 0, 1, 1, 0, 0, 0, 0, }, // 1
  { 1, 1, 0, 1, 1, 0, 1, }, // 2
  { 1, 1, 1, 1, 0, 0, 1, }, // 3
  { 0, 1, 1, 0, 0, 1, 1, }, // 4
  { 1, 0, 1, 1, 0, 1, 1, }, // 5
  { 1, 0, 1, 1, 1, 1, 1, }, // 6
  { 1, 1, 1, 0, 0, 0, 0, }, // 7
  { 1, 1, 1, 1, 1, 1, 1, }, // 8
  { 1, 1, 1, 1, 0, 1, 1, }, // 9
};
int D1 = 4;
int D2 = 7;
volatile int tens = 0;
volatile int ones = 0;

void Task_UpdateTens(void *param);
void Task_UpdateOnes(void *param);
void Task_UpdateReadings(void *param);

TaskHandle_t Task_Handle1;
TaskHandle_t Task_Handle2;
TaskHandle_t Task_Handle3;

void setup() {
  for (byte i = 0; i < 8; i++) {
    pinMode(pin[i], OUTPUT);
  }
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);

  xTaskCreate(Task_UpdateTens,"Task1",100,NULL,1,&Task_Handle1);
  xTaskCreate(Task_UpdateOnes,"Task2",100,NULL,1,&Task_Handle2);
  xTaskCreate(Task_UpdateReadings,"Task3",100,NULL,2,&Task_Handle3);
}

void loop() {
}

void Task_UpdateTens(void *param) {
  (void) param;

  while(1) {
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    for (byte i = 0; i < 8; i++) {
      digitalWrite(pin[i], number[tens][i]);
    }
    delay(1);
  }
}

void Task_UpdateOnes(void *param) {
  (void) param;

  while(1) {
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    for (byte i = 0; i < 8; i++) {
      digitalWrite(pin[i], number[ones][i]);
    }
    delay(1);
  }
}

void Task_UpdateReadings(void *param) {
  (void) param;
  TickType_t getTick;
  getTick = xTaskGetTickCount();

  while(1) {
    // read the analog in value:
    int sensorValue = analogRead(analogInPin);
    double outputValue = sensorValue / 1023.0 * 3.3;

    double degreesC = (outputValue - 0.5) * 100.0;
    int degreesF = degreesC * (9.0/5.0) + 32.0;
    tens = degreesF / 10;
    ones = degreesF % 10;

    vTaskDelayUntil(&getTick, 30000/portTICK_PERIOD_MS);
  }
}
