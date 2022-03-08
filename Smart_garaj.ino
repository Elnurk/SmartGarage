#include <Stepper.h>
#include "IRremote.h"

IRrecv irrecv(A5); // указываем вывод, к которому подключен приемник
decode_results results;

#define STEPS 300

// pin install ---------------
int tone_pin = 7;
int main_led = 4;
int Move_sensor = 3;
int Door_sensor = 2;

Stepper stepper(STEPS, 8, 9, 10, 11);

bool Alert = false; 
bool Alarm = false;
bool Lights = false;
bool Out_cickl = false;
bool Sound = false;

int shak = 30000;
int bastapku_shak = 0;
bool shak_TF = true;

bool varota_tf = false;
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
  Trivoga_TF();
  OpenGate();
  digitalWrite(main_led, Lights);
  digitalWrite(tone_pin, Zvuk);

  if (Alarm){
    if((digitalRead(Move_sensor) || digitalRead(Door_sensor)) == false)){ 
      Trivoga = true; 
    }
  }
  else{
    
  }
}

void read_IR(){
  if (irrecv.decode(&results)){ // если данные пришли
    Serial.println(results.value);
    if (results.value == 16750695){
        Alarm = false;
        Alert = false;
        Lights = false;
        Zvuk = false;
        noTone(tone_pin);
        Serial.println("Alarm OFF");
    }
    else if(results.value == 16738455){
        Alarm = true;
        Lights = false;
        varota_tf = false;
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
        varota_tf = false;
        Serial.println("Open gate");
      }
      else if(results.value == 16718055){
        varota_tf = true;
        Serial.println("Close gate");
      }
    }
      
    irrecv.resume(); // принимаем следующую команду
  }
}


void Trivoga_TF(){
  if (Trivoga){
    if(shak_TF){
      if (bastapku_shak<shak){
        if(bastapku_shak == 2){
          Serial.println("on");
          Lights = true;
          Zvuk = true;
        }
        bastapku_shak++;
      }
      else{
        bastapku_shak = 0;
        shak_TF = false;
      }
    }
    else{
      if (bastapku_shak<shak){
        if(bastapku_shak == 2){
          Serial.println("off");
          Lights = false;
          Zvuk = false;
        }
        bastapku_shak++;
      }
      else{
        bastapku_shak = 0;
        shak_TF = true;
      }
    }
  }
  else{
  }
}

void OpenGate(){
  if(GateOpen){
    if(varota_tf != GateOpen){
      Serial.println("Gates are opening");
      stepper.step(Stepmotor_step);
      Serial.println("Gates are open");
      GateOpen = varota_tf;
    }
  }
  else{
    if(varota_tf != GateOpen){
      Serial.println("Gates are closing");
      stepper.step(-Stepmotor_step);
      Serial.println("Gates are closed");
      GateOpen = varota_tf;
    }
  }
}
