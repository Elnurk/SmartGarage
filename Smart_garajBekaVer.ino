
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

bool Trivoga = false; 
bool Alarm = false;
bool Svet = false;
bool Out_cickl = false;
bool Zvuk = false;

int shak = 30000;
int bastapku_shak = 0;
bool shak_TF = true;

bool varota_tf = false;
bool varota_kazir = false;
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

  open_vorota();

  
  digitalWrite(main_led, Svet);

  digitalWrite(tone_pin, Zvuk);


  if (Alarm){
    if((digitalRead(Move_sensor)) || (digitalRead(Door_sensor) == false)){ 
      Trivoga = true; 
    }
  }
  else{
    
  }
}

void read_IR(){
  if ( irrecv.decode( &results )) { // если данные пришли
    Serial.println( results.value);
    if (results.value == 16750695){
        Alarm = false;
        Trivoga = false;
        Svet = false;
        Zvuk = false;
        noTone(tone_pin);
        Serial.println("Allartm OFF");
    }
    else if(results.value == 16738455){
        Alarm = true;
        Svet = false;
        varota_tf = false;
        Serial.println("Allartm ON");
    }
    
    if (Alarm){
      
    }
    else{
      if(results.value == 16756815){
        Svet = false;
        Serial.println("svet osh");
      }
      else if(results.value == 16748655){
          Svet = true;
          Serial.println("svet kosul");
      }
      else if(results.value == 16730805){
        varota_tf = false;
        Serial.println("Vorota_jap");
      }
      else if(results.value == 16718055){
        varota_tf = true;
        Serial.println("Vorota_ash");
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
          Svet = true;
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
          Svet = false;
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

void open_vorota(){
  if(varota_kazir){
    if(varota_tf != varota_kazir){
      Serial.println("asu bastaldu");
      stepper.step(Stepmotor_step);
      Serial.println("asu bitti");
      varota_kazir = varota_tf;
    }
  }
  else{
    if(varota_tf != varota_kazir){
      Serial.println("jaby bastaldu");
      stepper.step(-Stepmotor_step);
      Serial.println("jaby bitti");
      varota_kazir = varota_tf;
    }
  }
}
