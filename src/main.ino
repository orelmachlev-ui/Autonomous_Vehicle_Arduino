#include <Servo.h>
Servo myServo;

int triger = 4;
int echo = 9;
int servo = 2;
const int speedA = 5; 
const int pin1A = 6;
const int pin2A = 7;
const int pin1B = 12;
const int pin2B = 8;
const int speedB = 11;
const int led_1 = 10;
const int led_2 = 13;


static float right = 0;
static float left = 0;
static float straight = 0;


void forward() 
{
  digitalWrite(pin1A, LOW);
  digitalWrite(pin2A, HIGH);
  digitalWrite(pin1B, LOW);
  digitalWrite(pin2B, HIGH);
  analogWrite(speedA, 255);
  analogWrite(speedB, 135);
}


void turn_right(int speed) 
{
  digitalWrite(pin1A, LOW);
  digitalWrite(pin2A, HIGH);
  analogWrite(speedA, speed);
  digitalWrite(pin1B, HIGH);
  digitalWrite(pin2B, LOW);
  analogWrite(speedB, speed);
}


void turn_left(int speed) 
{
  digitalWrite(pin1A, HIGH);
  digitalWrite(pin2A, LOW);
  analogWrite(speedA, speed);
  digitalWrite(pin1B, LOW);
  digitalWrite(pin2B, HIGH);
  analogWrite(speedB, speed);
}


void backward_to_left() 
{
  turn_left(255);
  delay(2370);
}


void backward_to_right() 
{
  turn_right(255);
  delay(2370);
}


void stop () 
{
  digitalWrite(pin1A, LOW);
  digitalWrite(pin2A, LOW);
  analogWrite(speedA, 0);
  digitalWrite(pin1B, LOW);
  digitalWrite(pin2B, LOW);
  analogWrite(speedB, 0);
}


float distance_sensor_activation(int triger, int echo)
{
  digitalWrite(triger, LOW);
  delayMicroseconds(2);
  digitalWrite(triger, HIGH);
  delayMicroseconds(10);
  digitalWrite(triger, LOW);
  long duration = pulseIn(echo, HIGH, 6000);
  if (duration == 0)
    return 102;
  float distance = duration * 0.0343 / 2;
    return distance;
}


float getDistanceFiltered(int triger, int echo) 
{
  float sum = 0;
  int count = 0;
  for (int i = 0; i < 7; i++) 
  {
    float d = distance_sensor_activation(triger, echo);  
    if (d < 100 and d > 2)
    {
      sum += d;
      count++;
    }
    delay(10);
  }
  if (count == 0) return 102;
  return sum / count;
}


float straight_distance(int triger, int echo)
{
  myServo.write(87);
  delay(300);
  return getDistanceFiltered(triger, echo);
}


float left_distance(int triger, int echo)
{
  myServo.write(175);
  delay(500);
  return getDistanceFiltered(triger, echo);
}


float right_distance(int triger, int echo)
{
  myServo.write(5);
  delay(500);
  return getDistanceFiltered(triger, echo);
}

void cant_go ()
{
  for (int i=0; i<40; i++)
  {
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, LOW);
    delay(100); 
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, HIGH);
    delay(100);
    digitalWrite(led_2, LOW);
  }
}


void minimum_distance(float straight_dist)
{
  if (straight_dist < 30 and straight_dist > 0)
  {
    stop();
    delay(500);
    left = left_distance(triger, echo);
    delay(500);
    right = right_distance(triger, echo);
    delay(500);
    straight = straight_distance(triger, echo);
    delay(500);
    if (straight > 30)
    {
      forward();
      delay(400);
    }
    else if (right >= left and right > 40)
    {
      turn_right(255);
      delay(1180);
    }
    else if (right < left and left > 40)
    {
      turn_left(255);
      delay(1180);
    }
    else if (right >= left and right < 40 and right > 20)
    {
      backward_to_right();
      stop();
      delay(500);
    }
    else if  (right <  left and left < 40 and left > 20)
    {
      backward_to_left();
      stop();
      delay(500);      
    }
    else 
    {
      stop();
      cant_go();
    }
  }
  else
  {
    forward();
  }
}


void setup() 
{
  myServo.attach(servo);
  myServo.write(87);
  pinMode(triger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(speedA, OUTPUT);
  pinMode(pin1A, OUTPUT);
  pinMode(pin2A, OUTPUT);
  pinMode(speedB, OUTPUT);
  pinMode(pin1B, OUTPUT);
  pinMode(pin2B, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop() 
{
 straight = getDistanceFiltered(triger, echo);
 minimum_distance(straight);
}
