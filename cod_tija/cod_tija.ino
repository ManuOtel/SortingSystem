#include <NewPing.h>
#include <Servo.h>

Servo banda;

float invartiteD;
float invartiteS;
String directieC;
String directieA;
float sp,pv,err;
float max_control,min_control;
float kp,out;
float last_time;
const int trigPin = 12; 
const int echoPin = 11;
int distance = 0;

int stage = 0; 

NewPing sonar(trigPin, echoPin, 200);
/* 
Stages available : 
1 - Waiting for a package to arrive
2 - Reading label from package
3 - Transporting package
4 - Dropping package
5 - Returning to initial position
*/

#define MotA 8
#define MotB 9
#define EncA 2
#define EncB 3

void setup() 
{
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(MotA, OUTPUT);
  pinMode(MotB, OUTPUT);
  pinMode(EncA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EncA), encoder, RISING);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  directieC = "Niciuna";
  invartiteD = 0;
  invartiteS = 0;
  sp = 0;
  kp = 250;
  last_time=0;
  max_control = 255;
  min_control = -255;
  stage = 1;
  out = 0;
  analogWrite(8,0);
  analogWrite(9,0);
  banda.attach(13);
  delay(10000);
}

void encoder()
{
  if(digitalRead(EncB) == LOW)
  {
    invartiteS = invartiteS + 1.0/244.8;
    directieC = "Stanga";
  }
  else
  {
    invartiteD = invartiteD + 1.0/244.8;
    directieC = "Dreapta";
  }
} 

void stage1()
{
  // Running the motor until a package arrives
  // When package arrives stops the motor and stage = 2
  distance = sonar.ping_cm();
  //Serial.println(distance);
  if(distance < 20 && distance != 0)
  {
    banda.write(90);
    stage = 2;
  }
  else
  {
    banda.write(140);
  }
}

void stage2()
{
  analogWrite(8,0);
  analogWrite(9,0);
  Serial.println("Read");
  delay(15);
  kp = 250;
  stage = 3;
}

void stage3()
{
  if(Serial.available() > 0)
  {
    sp = Serial.parseInt(); 
    Serial.read();
    sp = sp*7;
    delay(10);
    banda.write(180);
    delay(1000);
    banda.write(90);
  }
  pv = invartiteD-invartiteS;
  if (millis() - last_time >= 10 && sp != 0)
  { 
    err = sp-pv;
    out = kp*err;
    if (out >= max_control) 
    {
      out = max_control;
    }
    if (out <= min_control) 
    {
      out = min_control;
    }
    last_time = millis();
    if(out < -15)
    {
      analogWrite(8,-out);
      analogWrite(9,0);
    }
    if (out > 15)
    {
      analogWrite(8,0);
      analogWrite(9,out);
    }
    if (out > -15 && out < 15)
    {
      analogWrite(8,0);
      analogWrite(9,0);
      stage = 4;
    }
  }
}

void stage4()
{
  if(sp != 0)
  {
    Serial1.println("Stanga");
    sp = 0;
  }
  if (Serial1.available() > 0)
  {
    String c = Serial1.readString();
    if(c.indexOf("Done") > -1)
    {
      stage = 5;
      kp = 350;
    }
  }
}

void stage5()
{
  sp = 0;
  pv = invartiteD-invartiteS;
  if (millis() - last_time >= 10)
  { 
    err = sp-pv;
    out = kp*err;
    if (out >= max_control) 
    {
      out = max_control;
    }
    if (out <= min_control) 
    {
      out = min_control;
    }
    last_time = millis();
    if(out < -15)
    {
      analogWrite(8,-out);
      analogWrite(9,0);
    }
    if (out > 15)
    {
      analogWrite(8,0);
      analogWrite(9,out);
    }
    if (out > -15 && out < 15)
    {
      analogWrite(8,0);
      analogWrite(9,0);
      stage = 1;
    }
  }
}

void loop() 
{
  if(stage == 1)
  {
    out = 0;
    stage1();
  }
  if(stage == 2)
  {
    stage2();
  }
  if(stage == 3)
  {
    stage3();
    if(out == out)
    {
      /*Serial.print("ERR : ");
      Serial.print(err);
      Serial.print("    PV : ");
      Serial.print(pv);
      Serial.print("    SP : ");
      Serial.println(sp);
      Serial.print("    Out : ");
      Serial.println(out);*/
    }
  }
  if(stage == 4 && (out > -15 && out < 15))
  {
    out = 0;
    stage4();
  }
  if(stage == 5)
  {
    stage5();
  }
}
