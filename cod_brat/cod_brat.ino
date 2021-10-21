#include <Servo.h>

Servo orientation;
Servo distance;
Servo height;
Servo claw;

int i;
String c;

// gripper (4) : 10 deschis    ---   60 inchis
// scripete (3) : 150 sus   ---   70 jos
// balanta (2) : 140 jos   ---   100 aproape
// rotitor (1) : 180 stanga   ---   1 dreapta

void setup() 
{
  Serial.begin(9600); 
  orientation.attach(3);
  distance.attach(5);
  height.attach(6);
  claw.attach(9);
  c="";
  height.write(140);
  distance.write(120);
  claw.write(60);
  orientation.write(90);
}

void loop() 
{
  height.write(120);
  distance.write(80);
  claw.write(60);
  orientation.write(1);
  if (Serial.available() > 0)
  {
    c = Serial.readString();
    if(c.indexOf("Stanga") > -1)
    {
      for(i=1;i<=180;i++)
      {
        orientation.write(i);
        delay(15);
      }
      delay(50);
      for(i=120;i>=90;i--)
      {
        height.write(i);
        delay(15);
      }
      for(i=80;i<=100;i++)
      {
        distance.write(i);
        delay(15);
      }
      delay(1000);
      claw.write(10);
      delay(500);
      for(i=90;i<=120;i++)
      {
        height.write(i);
        delay(15);
      }
      for(i=100;i>=80;i--)
      {
        distance.write(i);
        delay(15);
      }
      delay(500);
      claw.write(60);
      for(i=180;i>=1;i--)
      {
        orientation.write(i);
        delay(15);
      }
      delay(50);
      Serial.println("Done");
    }
    else
    {
      delay(5);
      Serial.println(c);
    }
  }
}
