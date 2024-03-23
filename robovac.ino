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
           delayMicroseconds(50);  //(abs((i-6000)/60)+25);
           digitalWrite(stepPin, LOW);
           delayMicroseconds(50);  //(abs((i-6000)/60)+25); 
           }
  digitalWrite(enablePin, HIGH);
  delay(200);         
}

void moveright()
{
  digitalWrite(enablePin, LOW);       //put motor in control mode
  digitalWrite(directionPin, LOW);    // set direction to rotate 
  for (int i=0; i<stepstomove; i++)          // move carriage 
           {  
           digitalWrite(stepPin, HIGH);
           delayMicroseconds(50);
           digitalWrite(stepPin, LOW);
           delayMicroseconds(50); 
           }
  digitalWrite(enablePin, HIGH);
  delay(200);          
}

int getcurrent(int pin)
   {
   int min = 1000;
   int max = 0;
   for( int i; i<100; i++)
   {
   int currentvalue = analogRead(pin);
   if(currentvalue < min) { min = currentvalue; }
   if(currentvalue > max) { max = currentvalue; }
   delay(1);
   //Serial.println(max,min);
   }
   
    return max - min;
   }

void loop() 
{ 
  //while(true) {Serial.println(getcurrent(tablesawPIN));}
  digitalWrite(enablePin, HIGH);
  if (getcurrent(tablesawPIN) > 100)      //if tablesaw is on, move hose to correct port and turn on vacuum
      {
      moveright();
      digitalWrite(VACUUMPIN, HIGH);
      digitalWrite(tablesawLEDPIN, HIGH);
      while(getcurrent(tablesawPIN) > 100) {  delay(200);  }
      digitalWrite(tablesawLEDPIN, LOW);
      delay(2000);
      digitalWrite(VACUUMPIN, LOW); 
      delay(1000);
      moveleft(); 
      }
      
  if(getcurrent(mitersawPIN) > 100)    //if mitersaw is on, move hose to correct port and turn on vacuum
      {
      moveleft();
      digitalWrite(VACUUMPIN, HIGH);
      digitalWrite(mitersawLEDPIN, HIGH);
      while(getcurrent(mitersawPIN) > 100) {  delay(200);  }
      digitalWrite(mitersawLEDPIN, LOW);
      delay(2000);
      digitalWrite(VACUUMPIN, LOW); 
      delay(1000);
      moveright(); 
      }
   
  if (!digitalRead(housevacPIN)) 
     {
     delay(500);
     housevac = OFF;
     if  (!digitalRead(housevacPIN)) 
         {
         digitalWrite(VACUUMPIN, HIGH);
         while (!digitalRead(housevacPIN))   {  delay(200); }  
         digitalWrite(VACUUMPIN, LOW);
         }
     else 
         {
          moveright();
          moveright();
          digitalWrite(VACUUMPIN, HIGH);
          delay(1000);
          while (!digitalRead(housevacPIN))   {  delay(200); }            
          digitalWrite(VACUUMPIN, LOW);
          moveleft(); 
          moveleft();
         }
     }
}  
