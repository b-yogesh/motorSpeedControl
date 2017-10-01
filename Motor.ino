const byte interruptPin = 2;
volatile int count = 0,ecount=0;
unsigned int currentrpm=0,realrpm=0;
float correctedpwm=0.0,pwm=0.0;
unsigned long  prev=0,prev1=0;
long setpoint =0;
int i=0, error = 0,realspd =0;
float ratio =0.0;
boolean flag=0,sflag = 0,lflag = 0,mflag=0;


void setup() {
  pinMode(interruptPin, INPUT);
  pinMode(9,OUTPUT);
  attachInterrupt(0, calcrpm, RISING);
  Serial.begin(115200);
  analogWrite(9,255);
}



void loop() 
{
  serialInput();
  currentSpeed();
  checkForError();                     //for checking if the curr rpm is in +/- 30 rpm of desired
  if(lflag)
    lessenError();
  if(mflag)
  {
    analogWrite(9,correctedpwm);
  }
}

void calcrpm() {
  count++;
  ecount++;
}

void serialInput()
{
  if(Serial.available() > 0)
  {
    setpoint = Serial.parseInt();
    pwm = ((255*setpoint) / 840 );
    analogWrite(9,pwm);
    flag=1;
    mflag=1;
  }
}


void currentSpeed()
{
  while(count<=2);      
  currentrpm =(float)(count)/(float)(millis()-prev)*60000;
  prev = millis();
  Serial.println("currentrpm   =");
  Serial.println(currentrpm);
  count =0 ;
}


void checkForError()
{    
  if(flag)
  { 
    if(currentrpm <= (setpoint + 30) && currentrpm >= (setpoint - 30))
    {    
      Serial.print("correctedpwm");
      Serial.print(correctedpwm);
      Serial.print("locked");
      Serial.print("currentlockedspeed=");
      Serial.println(currentrpm);
      lflag =1;
      flag=0;
      sflag=1;
    }
  }
  if(flag)
  {  
    while(ecount<=2);      
    ratio = (float)setpoint/(float)currentrpm;
    correctedpwm = (float)(pwm * ratio);
    pwm = correctedpwm;
    Serial.print("correctedpwm ");
    Serial.print(correctedpwm);
    ecount=0;
  }
}

void lessenError()
{
  Serial.print("enterd");
  Serial.println(correctedpwm);
  if(currentrpm >= (setpoint - 5) && currentrpm <= (setpoint + 5))
  {
    sflag=0;
    lflag=0;
    mflag=0;
  }
  else
  {
    ;
  }
  error = setpoint - currentrpm;

  if(sflag)
  { 
    if(error > 0)
    {
      correctedpwm++;
      Serial.print("Increment ");
      Serial.println(correctedpwm);

    }
    else
    {
      correctedpwm--;     
      Serial.print("Decrement ");
      Serial.println(correctedpwm);

    }
  }
}
















