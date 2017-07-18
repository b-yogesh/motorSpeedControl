const byte interruptPin = 2;
volatile int count = 0, ecount = 0;
unsigned int currentrpm = 0;
float correctedpwm = 0.0, pwm = 0.0;
unsigned long  prev = 0, prev1 = 0;
long setpoint = 0;
int i = 0, error = 0, c = 0, suberror = 0;
float ratio = 0.0, ratio1 = 0.0;
boolean flag = 0, sflag = 0, lflag = 0, mflag = 0;


void setup() {
  pinMode(interruptPin, INPUT);
  pinMode(9, OUTPUT);
  attachInterrupt(0, calcrpm, RISING);
  Serial.begin(115200);
  analogWrite(9, 255);

}



void loop()
{
  serialInput();

  if (mflag)
  {
    Serial.print(setpoint);
    Serial.print(" ");
    if (correctedpwm > 255.0)
      correctedpwm = 255.0;
    if (correctedpwm == 0.0)
      correctedpwm = 0;
    else if (correctedpwm < 14.0)
      correctedpwm = 14.0;

    Serial.print("correctedpwm ");
    Serial.print(correctedpwm);
    analogWrite(9, correctedpwm);
  }
  currentSpeed();
  checkForError();                     

}

void calcrpm() {
  count++;
  ecount++;
}

void serialInput()
{
  if (Serial.available() > 0)
  {
    setpoint = Serial.parseInt();
    correctedpwm = ((241 * setpoint) / 1005 );
    // correctedpwm = map(setpoint,14,255,0,1084);
    pwm = correctedpwm;
    ratio = 0.0;
    flag = 1;
    mflag = 1;
    c = 0;
  }
}


void currentSpeed()
{
  while (count <= 1);
  currentrpm = (float)(count) / (float)(millis() - prev) * 60000;
  prev = millis();
  // Serial.println("currentrpm   =");
  Serial.print(" ");

  Serial.println(currentrpm);

  count = 0 ;

}
void checkForError()
{
  if (flag)
  {
    if (currentrpm <= (setpoint + 4  ) && currentrpm >= (setpoint - 4))
    {

      lflag = 1;
      flag = 0;
      sflag = 1;
    }
  }
  if (flag)
  {
    while (ecount <= 1);
    ratio = (float)setpoint / (float)currentrpm;
    correctedpwm = (float)(pwm * ratio);
    pwm = correctedpwm;

    ecount = 0;
  }
}


























