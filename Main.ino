#include <Stepper.h>
#include "IRremote.h"

IRrecv irrecv(A5);
decode_results results;

#define STEPS 300

int buzzer = 7;
int main_led = 4;
int Move_sensor = 3;
int Door_sensor = 2;
int Open_sensor = 5;

Stepper stepper(STEPS, 8, 9, 10, 11);

bool Alert = false;
bool Alarm = false;
bool Lights = false;
bool Sound = false;
bool GateOpen = false;
int Stepmotor_step = 500;

int GateTarget = 0;
int StepChunk = 10;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(buzzer, OUTPUT);
  pinMode(main_led, OUTPUT);
  pinMode(Move_sensor, INPUT);
  pinMode(Door_sensor, INPUT);
  pinMode(Open_sensor, INPUT);
  stepper.setSpeed(30);
}

void loop() {
  read_IR();
  AlertCheck();
  GateTask();
  digitalWrite(main_led, Lights);
  digitalWrite(buzzer, Sound);
  if (Alarm) {
    if (digitalRead(Move_sensor)) {
      Alert = true;
    }
  }
}

void read_IR() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    if (results.value == 16750695) {
      Alarm = false;
      Alert = false;
      Lights = false;
      Sound = false;
      Serial.println("Alarm OFF");
    }
    else if (results.value == 16738455) {
      Alarm = true;
      Lights = true;
      Serial.println("Alarm ON");
    }
    if (Alarm == false) {
      if (results.value == 16756815) {
        Lights = false;
        Serial.println("Lights off");
      }
      else if (results.value == 16748655) {
        Lights = true;
        Serial.println("Lights on");
      }
      else if (results.value == 16730805) {
        OpenGate("Open");
        Serial.println("Open gate");
      }
      else if (results.value == 16718055) {
        OpenGate("Closed");
        Serial.println("Close gate");
      }
    }
    irrecv.resume();
  }
}

void AlertCheck() {
  if (Alert) {
    Serial.println("on");
    Lights = true;
    Sound = true;
  }
  else {
    Serial.println("off");
    Lights = false;
    Sound = false;
  }
}

void OpenGate(String GateStatus) {
  if (GateStatus == "Open") {
    GateTarget = 1;
  }
  else if (GateStatus == "Closed") {
    GateTarget = -1;
  }
}

void GateTask() {
  if (GateTarget == 1) {
    if (!digitalRead(Open_sensor)) {
      stepper.step(StepChunk);
    }
    else {
      GateTarget = 0;
      GateOpen = true;
      Serial.println("Gates are open");
    }
  }
  else if (GateTarget == -1) {
    if (!digitalRead(Door_sensor)) {
      stepper.step(-StepChunk);
    }
    else {
      GateTarget = 0;
      GateOpen = false;
      Serial.println("Gates are closed");
    }
  }
}
