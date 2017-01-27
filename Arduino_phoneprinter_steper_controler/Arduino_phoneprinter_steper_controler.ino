#include <SoftwareSerial.h>
#include <Math.h>

#define STEP_PER_MM 456.7 //ilość kroków na 1mm ruchu
#define PRINTER_HIGH 250 //ograniczona wysokość przestrzeni roboczej, w mm
#define def_dir 1 // kierunek ruchu, jeżeli odwrotnie to na -1, okreslone jako kierunek w górę!!!!
#define endstop_logic 0 // logika endstopu, jesli sie nie zatrzymuje zmienic na 1

int x;
int dir;
float distance; // w milimetrach
int dirPin = 4;
int stepPin = 5;
int PULL_UP = 3;    //wartość domyślna, podawana w mm
int LAYER_H = 100;  // wartość domyślna, podawana w um
float d = LAYER_H * 0.001;
float PULL_DOWN = PULL_UP - d;
int kontrolka=1;

SoftwareSerial BT(10, 11); //serial dla BT

void setup()
{
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(12, INPUT); //start button
  pinMode(8, INPUT); //endstop
  pinMode(stepPin, OUTPUT); // Step
  pinMode(dirPin, OUTPUT); // Dir
  digitalWrite(dirPin, LOW);
}

void loop()
{
  
  x = BT.read();
  if (x == 1)
  {
    LAYER_H = BT.read();
    PULL_UP = BT.read();
    d = LAYER_H *0.001;
    PULL_DOWN = PULL_UP - d;
    Serial.println(x);
    Serial.print("Layer h: ");
    Serial.println(LAYER_H);
    Serial.print("Lifting height: ");
    Serial.println(PULL_UP);
    BT.print(LAYER_H);
    delay(100);
    BT.print(PULL_UP);    
  }
  if (x == 5)
  {
    dir = 1;
    distance = PULL_UP;
    steper_move_at_distance();
    dir = -1;
    distance = PULL_DOWN;
    steper_move_at_distance();
    BT.print(5);
    Serial.print("5 ");    
    Serial.println(kontrolka);
    kontrolka=kontrolka+1;
    
  }
  if (x == 3)
  {
    dir = -1;
    distance = PRINTER_HIGH;
    steper_move_at_distance();
    dir = 1;
    distance = 0.1;
    while (digitalRead(8) == 0)
      {steper_move_at_distance();}
    steper_move_at_distance();
  }
  if (x == 7)
  {
    dir = 1;
    distance = 10;
    steper_move_at_distance();
    dir = 1;
  }
  delay(100);
}

void steper_move_at_distance()
{
  if (dir * def_dir > 0)
    digitalWrite(dirPin, HIGH);
  else
    digitalWrite(dirPin, LOW);

  for (x = 0; x < round(STEP_PER_MM * distance); x++) // Loop 200 times
  {
    digitalWrite(5, HIGH); // Output high
    delay(2); // Wait
    digitalWrite(5, LOW); // Output low
    delay(2); // Wait
    if (digitalRead(8) == endstop_logic)
      break;
  }
}
