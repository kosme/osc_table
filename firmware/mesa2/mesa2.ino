int x;
float actual = 1.0;
float frecuencia=1;
long periodo=2500;
long nuevo;
boolean cambiar=false;

const int Enable = 6;
const int Step = 5;
const int pasos = 200; //Número de pasos del motor

void setup() {
  pinMode(Enable,OUTPUT); // Enable 
  pinMode(Step,OUTPUT); // Step 
  digitalWrite(Enable,LOW); // Set Enable low 
  Serial.begin(9600);
  Serial.println("Indique la frecuencia");
}

void loop() 
{
  if(Serial.available())
  {
    frecuencia=Serial.parseFloat();
    Serial.print("Frecuencia: ");
    Serial.println(frecuencia);
    if((frecuencia > 0)&&(frecuencia<=5))
    {
      digitalWrite(Enable,LOW); //Permitir movimiento
      nuevo = 10000/(4*frecuencia);
      Serial.print("Periodo: ");
      Serial.println(nuevo);
      cambiar=true;
    }
    else
    {
      digitalWrite(Enable,HIGH); //Detener movimiento
      periodo = 5000;
      actual=0.5;
    }
    while(Serial.available())
      Serial.read();
  }
  for(x = 0; x < pasos; x++) // Loop 200 times 
  {
    if(Serial.available())
      break;
    digitalWrite(Step,HIGH); // Output high 
    delayMicroseconds(periodo); // Wait 1/2 a ms 
    digitalWrite(Step,LOW); // Output low 
    delayMicroseconds(periodo); // Wait 1/2 a ms
  } 
  if(cambiar)
  {
    if(nuevo!=periodo)
    {
      periodo=10000/(4*actual);
      Serial.println(periodo);
      Serial.println(actual);
      if(actual>frecuencia)
      {
        actual= actual-0.1;
      }
      else //(frecuencia > actual)
      {
        actual= actual+0.1;
      }
      if(abs(periodo-nuevo)<5)
      {
        periodo= nuevo;
        cambiar=false;
      }
    }
    else
      cambiar=false;
/*    if(abs(nuevo-periodo)>249)
      variar250();
    else if(abs(nuevo-periodo)>99)
      variar100();
    else if(abs(nuevo-periodo)>49)
      variar50();
    else if(abs(nuevo-periodo)>9)
    {
      periodo=nuevo;
      Serial.println(periodo);
      cambiar=false;
    }
/*      variar10();
    else if(abs(nuevo-periodo)>0)
      variar1();
    else
      cambiar=false;*/
  }
}

void variar250()
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
}
