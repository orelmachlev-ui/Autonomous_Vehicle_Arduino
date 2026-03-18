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
  analogWrite(speedA, 236);
  analogWrite(speedB, 255);
}

void turn_right(int speed) 
{
  digitalWrite(pin1A, LOW);
  digitalWrite(pin2A, HIGH);
  analogWrite(speedA, speed);
  digitalWrite(pin1B, HIGH);
  digitalWrite(pin2B, LOW);
  analogWrite(speedB, speed);
  delay(700);
}

void turn_left(int speed) 
{
  digitalWrite(pin1A, HIGH);
  digitalWrite(pin2A, LOW);
  analogWrite(speedA, speed);
  digitalWrite(pin1B, LOW);
  digitalWrite(pin2B, HIGH);
  analogWrite(speedB, speed);
  delay(720);
}

void backward_to_left() 
{
  turn_left(255);
  delay(647);
}

void backward_to_right() 
{
  turn_right(255);
  delay(640);
}

void stop(int time) 
{
  digitalWrite(pin1A, HIGH);
  digitalWrite(pin2A, LOW);
  analogWrite(speedA, 150);
  digitalWrite(pin1B, HIGH);
  digitalWrite(pin2B, LOW);
  analogWrite(speedB, 150);
  delay(time);
  digitalWrite(pin1A, LOW);
  digitalWrite(pin2A, LOW);
  analogWrite(speedA, 0);
  digitalWrite(pin1B, LOW);
  digitalWrite(pin2B, LOW);
  analogWrite(speedB, 0);
  delay(100);
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


float getDistanceFiltered(int triger, int echo, int times) 
{
  float sum = 0;
  int count = 0;
  for (int i = 0; i < times; i++) 
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
  return getDistanceFiltered(triger, echo, 5);
}


float left_distance(int triger, int echo)
{
  myServo.write(175);
  delay(600);
  return getDistanceFiltered(triger, echo, 5);
}


float right_distance(int triger, int echo)
{
  myServo.write(5);
  delay(600);
  return getDistanceFiltered(triger, echo, 5);
}

void cant_go ()
{
  for (int i=0; i<30; i++)
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


void start()
{
  delay(600); 
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, LOW);
  delay(700); 
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, HIGH);
  delay(700); 
  digitalWrite(led_1, HIGH);
  digitalWrite(led_2, HIGH);
  delay(800); 
  digitalWrite(led_1, LOW);
  digitalWrite(led_2, LOW);
}


void minimum_distance(float straight_dist)
{
  if (straight_dist < 25 and straight_dist > 0)
  {
    stop(170);
    delay(400);
    left = left_distance(triger, echo);
    delay(500);
    right = right_distance(triger, echo);
    delay(500);
    straight = straight_distance(triger, echo);
    delay(500);
    if (straight > 25)
    {
      forward();
      delay(400);
    }
    else if (right >= left and right > 30)
    {
      turn_right(255);
      stop(10);
      delay(200);
    }
    else if (right < left and left > 30)
    {
      turn_left(255);
      stop(10);
      delay(200);
    }
    else if (right >= left and right < 30 and right > 16)
    {
      backward_to_right();
      stop(10);
      delay(200);
    }
    else if (right <  left and left < 30 and left > 16)
    {
      backward_to_left();
      stop(10);
      delay(200);
    }
    else 
    {
      stop(10);
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
  start();
}

void loop() 
{
 straight = getDistanceFiltered(triger, echo, 2);
 minimum_distance(straight);
}
