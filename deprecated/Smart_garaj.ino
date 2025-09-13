#include <Stepper.h>
#include "IRremote.h"

IRrecv irrecv(A5); // указываем вывод, к которому подключен приемник
decode_results results;

#define STEPS 300

// pin install ---------------
int buzzer = 7;
int main_led = 4;
int Move_sensor = 3;
int Door_sensor = 2;

Stepper stepper(STEPS, 8, 9, 10, 11);

bool Alert = false; 
bool Alarm = false;
bool Lights = false;
bool Sound = false;
bool GateOpen = false;
int Stepmotor_step = 500;


void setup() {
  Serial.begin(9600); // выставляем скорость COM порта
  irrecv.enableIRIn(); // запускаем прием
  pinMode(tone_pin, OUTPUT); 
  pinMode(main_led, OUTPUT); 
  pinMode(Move_sensor, INPUT); 
  pinMode(Door_sensor, INPUT);
  stepper.setSpeed(30);
}

void loop() {
  read_IR();
  AlertCheck();
  OpenGate();
  digitalWrite(main_led, Lights);
  digitalWrite(buzzer, Sound);
  if (Alarm){
    if((digitalRead(Move_sensor) || digitalRead(Door_sensor)) == false)){ 
      Alert = true; 
    }
  }
}

void read_IR(){
  if (irrecv.decode(&results)){ // если данные пришли
    Serial.println(results.value);
    if (results.value == 16750695){
        Alarm = false;
        Alert = false;
        Lights = false;
        Sound = false;
        noTone(buzzer);
        Serial.println("Alarm OFF");
    }
    else if(results.value == 16738455){
        Alarm = true;
        Lights = true;
        Serial.println("Alarm ON");
    }
    
    if (Alarm == false){
      if(results.value == 16756815){
        Lights = false;
        Serial.println("Lights off");
      }
      else if(results.value == 16748655){
        Lights = true;
        Serial.println("Lights on");
      }
      else if(results.value == 16730805){
        OpenGate(Open);
        Serial.println("Open gate");
      }
      else if(results.value == 16718055){
        OpenGate(Closed);
        Serial.println("Close gate");
      }
    }
      
    irrecv.resume(); // принимаем следующую команду
  }
}


void AlertCheck(){
  if (Alert){
    Serial.println("on");
    Lights = true;
    Sound = true;
  }
  else{
    Serial.println("off");
    Lights = false;
    Sound = false;
  }
}

void OpenGate(String GateStatus){
  if(GateStatus == "Open"){
      stepper.step(Stepmotor_step);
      Serial.println("Gates are open");
  }
  else if (GateStatus == "Closed"){
      stepper.step(-Stepmotor_step);
      Serial.println("Gates are closed");
  }
}
