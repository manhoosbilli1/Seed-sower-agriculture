#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2); // RX, TX
int motorRightA = 4;    // Motor-clockwise+ stop
int motorRightB = 5;   // Motor-anticlockwise+stop
int shower = 10;
bool autoSeedState = false;
bool automaticModeState = false;
#include <Servo.h>
Servo myservo;
int bt = 55;       //BT
/*-----------------------------------------------------------------------------------------------------*/
int pos1 = 10;
int pos2 = 170;
int pos = 90;
#define automaticSeedOn 0 //automatic seeding on 
#define automaticSeedOff 1 //automatic seeding off
#define showerOn 3 //turn on shower
#define showerOff 4 //shower off 
#define motorForward 5 //motor forward
#define motorReverse 6 //motor backward
#define motorStop 7 //motor stop
#define automaticModeOn 8 //automatic mode on
#define automaticModeOff 9 //autoamtic mode off
#define openServo 10 //servo open
#define closeServo 11 //servo close 

unsigned long int curMil;
unsigned long int prevMil;
int servoDelay = 1000;

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  myservo.attach(9);
  pinMode(motorRightA, OUTPUT);
  pinMode(motorRightB, OUTPUT);
  pinMode(shower, OUTPUT);

  digitalWrite(motorRightA, LOW);
  digitalWrite(motorRightB, LOW);
  digitalWrite(shower, HIGH);    //relay high = off
  myservo.write(pos1);
}
void loop()
{
  Serial.println(bt);
  bt = mySerial.read();
  curMil = millis();


  switch (bt)
  {
    case automaticSeedOn:
      autoSeedState = true;
      automaticModeState = false;
      break;

    case automaticSeedOff:
      autoSeedState = false;
      automaticModeState = false;
      break;

    case showerOn:
      digitalWrite(shower, LOW);
      automaticModeState = false;
      break;

    case showerOff:
      digitalWrite(shower, HIGH);
      automaticModeState = false;
      break;

    case openServo:
      myservo.write(pos1);
      automaticModeState = false;
      break;

    case closeServo:
      myservo.write(pos2);
      automaticModeState = false;
      break;


    case motorForward:
      digitalWrite(motorRightA, HIGH);
      digitalWrite(motorRightB, LOW);
      automaticModeState = false;
      break;

    case motorReverse:
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, HIGH);
      automaticModeState = false;
      break;

    case motorStop:
      digitalWrite(motorRightA, HIGH);
      digitalWrite(motorRightB, HIGH);
      automaticModeState = false;
      break;

    case automaticModeOn:
      automaticModeState = true;
      autoSeedState == false;
      bt = 55;
      break;

    case automaticModeOff:
      automaticModeState = false;
      autoSeedState = false;   //reset everything to zero
      digitalWrite(motorRightA, LOW);
      digitalWrite(motorRightB, LOW);
      digitalWrite(shower, HIGH);
      bt = 55;
      break;



  }

  if (automaticModeState == true)
  {
    automaticModeHandler();
  }

  if (autoSeedState == true)
  {
    autoSeed();
  }
}


void autoSeed()
{
  if (curMil - prevMil >= servoDelay)
  {
    if (myservo.read() == pos1)
    {
      myservo.write(pos2);
    }
    else
    {
      myservo.write(pos1);
    }
    prevMil = curMil;
  }
}


void automaticModeHandler()
{

  digitalWrite(motorRightA, HIGH);
  digitalWrite(motorRightB, LOW);
  digitalWrite(shower, LOW);
  autoSeed();
}
