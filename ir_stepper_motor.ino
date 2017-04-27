#include <IRremote.h>

/* define how long motor should run for */
#define STEPCOUNT  2000

/* define whether debugging will be on */
#define DEBUG 0
#define PRINTCODES 0

/* define pins */
#define STP   2   // alternate to make spin
#define DIR   3   // set direction of spin
#define MS1   4   // sets rotation degree
#define MS2   5   // sets rotation degree
#define EN    6   // enables or dsiables motor
#define RECV  11  // ir receiver pin receives code

/* define ir remote button codes */
#define btn_PWR     284153895
#define btn_A       284162055
#define btn_B       284129415
#define btn_C       284121255
#define btn_UP      284139615
#define btn_DOWN    284098815
#define btn_RIGHT   284131455
#define btn_LEFT    284102895
#define btn_CENTER  284106975

/* stepper motor variables */
int x;

/* ir variables */
IRrecv irrecv(RECV);
decode_results irsignal;

void DebugPrint(String str)
{
  if(DEBUG) {
    Serial.println(str);
  }
}

void ClockWise(int steps)
{
  DebugPrint("ClockWise");
  
  digitalWrite(DIR, LOW);
  for(x = 1; x < steps; x++)
  {
    digitalWrite(STP,HIGH);
    delay(1);
    digitalWrite(STP,LOW);
    delay(1);
  }
}

void CounterClockWise(int steps)
{
  DebugPrint("CounterClockWise");
  
  digitalWrite(DIR, HIGH);
  for(x = 1; x < steps; x++)
  {
    digitalWrite(STP,HIGH);
    delay(1);
    digitalWrite(STP,LOW);
    delay(1);
  }
}

void SetEighthStep()
{
  DebugPrint("SetEighthStep");
  
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
}

void SetQuarterStep()
{
  DebugPrint("SetQuarterStep");
  
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, HIGH);
}

void SetHalfStep()
{
  DebugPrint("StepHalfStep");
  
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, LOW);
}

void SetFullStep()
{
  DebugPrint("StepFullStep");
  
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, LOW);
}

void resetEDPins()
{
  DebugPrint("resetEDPins");
  
  digitalWrite(STP, LOW);
  digitalWrite(DIR, LOW);
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
}

void setup() {
  pinMode(STP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  resetEDPins(); // Set step, direction, microstep and enable pins to default states

  digitalWrite(EN, HIGH); // disables motor

  irrecv.enableIRIn();

  if(DEBUG) {
    Serial.begin(9600); // Open Serial connection for debugging
    Serial.println("ready");
  }
}

void loop() {
  static unsigned long code = 0;
  static unsigned long last = 0;

  if(irrecv.decode(&irsignal)) {
    if(PRINTCODES) {
      Serial.println(irsignal.value);
    }
    
    if (irsignal.value == btn_A)
    {
      SetEighthStep();
    }
    else if(irsignal.value == btn_B)
    {
      SetQuarterStep();
    }
    else if(irsignal.value == btn_C)
    {
      SetHalfStep();
    }
    else if(irsignal.value == btn_PWR)
    {
      SetFullStep();
    }
    else if(irsignal.value == btn_RIGHT)
    {
      ClockWise(100);
      last = btn_RIGHT;
    }
    else if(irsignal.value == btn_LEFT)
    {
      CounterClockWise(100);
      last = btn_LEFT;
    }
    else if(irsignal.value == btn_CENTER && last == btn_RIGHT)
    {
      digitalWrite(EN, LOW);
      ClockWise(STEPCOUNT);
      digitalWrite(EN, HIGH);
    }
    else if(irsignal.value == btn_CENTER && last == btn_LEFT)
    {
      digitalWrite(EN, LOW);
      CounterClockWise(STEPCOUNT);
      digitalWrite(EN, HIGH);
    }
    else if(irsignal.value == btn_UP)
    {
      digitalWrite(EN, LOW); // enables motor
    }
    else if(irsignal.value == btn_DOWN)
    {
      digitalWrite(EN, HIGH); // disables motor
    }
    else
    { /* do nothing */ }

    code = irsignal.value;

    irrecv.resume(); // Receive the next value
  }
  else
  {
    if(code == btn_RIGHT)
      {
        ClockWise(100);
      }
      else
      {
        CounterClockWise(100);
      }
  }
}

