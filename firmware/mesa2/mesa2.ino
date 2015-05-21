#include <LiquidCrystal.h>

#define ENC_A 14
#define ENC_B 15
#define ENC_PORT PINC
#define boton 2

int x;
float actual = 1.0;
float frecuencia=2;
long periodo;
volatile boolean encendido=false;
int8_t tmpdata;
char contador=0;

const int Enable = 6;
const int Step = 5;
const int pasos = 200; //Número de pasos del motor

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

void setup() 
{
  get_periodo();
  pinMode(Enable,OUTPUT); // Enable 
  pinMode(Step,OUTPUT); // Step 
  digitalWrite(Enable,LOW); // Set Enable low 
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);
  pinMode(boton,INPUT);
  digitalWrite(boton,HIGH);
  lcd.begin(20, 4);
  lcd.print("Control de mesa");
  lcd.setCursor(0, 1);
  lcd.print("Frecuencia:");
  lcd.setCursor(0, 2);
  lcd.print(frecuencia);
  lcd.setCursor(0, 3);
  lcd.print("Apagado");
  attachInterrupt(0,onoff,LOW);
}

void loop()
{  
  if(!encendido)
  {
    read_encoder();
  }
  else
  {
    for(x = 0; x < pasos; x++) // Loop 200 times 
    {
      digitalWrite(Step,HIGH); // Output high 
      delayMicroseconds(periodo); // Wait 1/2 a ms 
      digitalWrite(Step,LOW); // Output low 
      delayMicroseconds(periodo); // Wait 1/2 a ms
      read_encoder();
    }
  }
}

void onoff()
{
  detachInterrupt(0);
  noInterrupts();
  delayMicroseconds(800000); //Debounce
  if(!encendido)
    {
      lcd.setCursor(0, 3);
      lcd.print("Encendido");
    }
    else
    {
      lcd.setCursor(0, 3);
      lcd.print("Apagado  ");
    }
    encendido=!encendido;
    digitalWrite(Enable,!encendido);
    attachInterrupt(0,onoff,LOW);
    interrupts();
}

/*void variar250()
{
  Serial.println("variar250");
  if(nuevo>periodo)
    periodo=periodo+250;
  else if (nuevo<periodo)
    periodo=periodo-250;
  else{}
  Serial.println(periodo);
}

void variar100()
{
  Serial.println("variar100");
  if(nuevo>periodo)
    periodo=periodo+100;
  else if (nuevo<periodo)
    periodo=periodo-100;
  else{}
  Serial.println(periodo);
}

void variar50()
{
  Serial.println("variar50");
  if(nuevo>periodo)
    periodo=periodo+50;
  else if (nuevo<periodo)
    periodo=periodo-50;
  else{}
  Serial.println(periodo);
}*/

void read_encoder()
{
  static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
  static uint8_t old_AB = 0;

  old_AB <<= 2;                   //remember previous state
  old_AB |= ( ENC_PORT & 0x03 );  //add current state
  tmpdata=enc_states[( old_AB & 0x0f )];
  if(tmpdata)
  {
    contador+=tmpdata;
    if((contador%4)==0)
    {
      frecuencia+=(contador*0.0625);
      get_periodo();
      contador=0;
      lcd.setCursor(0, 2);
      lcd.print(frecuencia);
      lcd.print("  ");
    }
  }
}

void get_periodo()
{
  periodo = 2500/frecuencia;
}
