#define tablesawPIN A0    // table saw current sensor
#define mitersawPIN A1    // mitre saw currenet sensor
#define housevacPIN 3     // handheld vacuum hose 
#define stepPin 4         // send pulses out to the motor controller
#define directionPin 5    // set the direction the motor will turn
#define enablePin 6       // stepper motor enable; Low to enable/ high to release
#define tablesawLEDPIN  12       // green indicator light on the small display 
#define mitersawLEDPIN  10       //             same
#define housevacLEDPIN  11       //             same
#define VACUUMPIN A6      // output to turn on the vacuum

#define ON true
#define OFF false

bool tablesaw = OFF;
bool mitersaw = OFF;
bool housevac = OFF;
bool vacuum = OFF;
int stepstomove = 12000;
void setup()
{
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(tablesawPIN, INPUT);
  pinMode(mitersawPIN, INPUT);
  pinMode(housevacPIN, INPUT_PULLUP);
  pinMode(tablesawLEDPIN, OUTPUT);
  pinMode(mitersawLEDPIN, OUTPUT);
  pinMode(housevacLEDPIN, OUTPUT);
  pinMode(VACUUMPIN, OUTPUT);
   
  Serial.begin(38400);
 
 }
void moveleft()
{
  digitalWrite(enablePin, LOW);       //put motor in control mode
  digitalWrite(directionPin, HIGH);    // set direction to move to the left
  for (int i=0; i<stepstomove; i++)          // move carriage 
           {  
           digitalWrite(stepPin, HIGH);
           delayMicroseconds(100);
           digitalWrite(stepPin, LOW);
           delayMicroseconds(100); 
           }
  digitalWrite(enablePin, HIGH);          
}

void moveright()
{
  digitalWrite(enablePin, LOW);       //put motor in control mode
  digitalWrite(directionPin, LOW);    // set direction to rotate 
  for (int i=0; i<stepstomove; i++)          // move carriage 
           {  
           digitalWrite(stepPin, HIGH);
           delayMicroseconds(100);
           digitalWrite(stepPin, LOW);
           delayMicroseconds(100); 
           }
  digitalWrite(enablePin, HIGH);          
}
int getcurrent(int pin)
   {
   int min = 1000;
   int max = 0;
   for( int i; i<200; i++)
   {
   int currentvalue = analogRead(pin);
   if(currentvalue < min) { min = currentvalue; }
   if(currentvalue > max) {max = currentvalue; }
   }
   
    return max - min;
   }
void loop() 
{ 
  //while(true) {Serial.println(analogRead(mitersawPIN));}
  
  if (getcurrent(tablesawPIN) > 100) {  tablesaw = ON;  }

  if(tablesaw == ON)    //if tablesaw is on, move hose to correct port and turn on vacuum
  {
      moveright();
      digitalWrite(VACUUMPIN, HIGH);
      digitalWrite(tablesawLEDPIN, HIGH);
      while(tablesaw == ON) 
      {
      //while (true) {Serial.println(getcurrent(tablesawPIN));} 
       if(getcurrent(tablesawPIN) < 100) {  tablesaw = OFF;  }
      }
      digitalWrite(tablesawLEDPIN, LOW);
      delay(3000);
      digitalWrite(VACUUMPIN, LOW); 
      moveleft(); 
    }
      
  if(abs(analogRead(mitersawPIN)-512)>20) {  mitersaw = ON;  }

  if(mitersaw == ON) 
  {
      moveright();
      digitalWrite(VACUUMPIN, HIGH);
      while(mitersaw == ON) 
      {
        delay(200);
        if(abs(analogRead(mitersawPIN)-512)>20) { mitersaw = OFF; }
      }
      delay(3000);
      digitalWrite(VACUUMPIN, LOW); 
      moveleft();  
  }
 
  housevac = !digitalRead(housevacPIN);
  
  if (housevac == ON) 
  {
     digitalWrite(VACUUMPIN, HIGH);
     while (housevac == ON) 
     {
         delay(200);
         housevac = !digitalRead(housevacPIN);
     }
     digitalWrite(VACUUMPIN, LOW);
  }
}  
  
  

 
