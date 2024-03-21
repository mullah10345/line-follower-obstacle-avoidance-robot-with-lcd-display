#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);
#define in1 2
#define in2 4
#define in3 6
#define in4 7
#define enA 3
#define enB 5

#define L_S 10//ir sensor Left
#define R_S 11 //ir sensor Right

#define echo 8   //Echo pin
#define trigger 9 //Trigger pin
#define servo A1

int Set=15;
int distance_L, distance_F, distance_R; 

 int M1_Speed = 80; // speed of motor 1 
 int M2_Speed = 80; // speed of motor 2
 int LeftRotationSpeed = 70;  // Left Rotation Speed
 int RightRotationSpeed = 70; // Right Rotation Speed


 void setup() {
Serial.begin(9600);
   lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(enB,OUTPUT);
  pinMode(A1, INPUT); // initialize Left sensor as an input
  pinMode(A0, INPUT); // initialize Right sensor as an input
  pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
  pinMode(trigger, OUTPUT); // declare ultrasonic sensor
  pinMode(servo, OUTPUT);
  
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
 for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }

 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }

distance_F = Ultrasonic_read();

delay(500);
 // Display initial message on LCD
  lcd.setCursor(0, 0);
  lcd.print("Line Tracker");
}

void loop() {
  distance_F = Ultrasonic_read();
  Serial.print("D F=");
  Serial.println(distance_F);
  int LEFT_SENSOR = digitalRead(A0);
  int RIGHT_SENSOR = digitalRead(A1);
  
if(RIGHT_SENSOR==0 && LEFT_SENSOR==0)
{
    if(distance_F > Set)
    {
      forward();
      lcd.setCursor(0, 1);
    lcd.print("NO Obstacle");
      }
    else
    {  lcd.setCursor(0, 1);
    lcd.print("Obstacle detected");
        Check_side();
  
      
    }
    
}

  else if(RIGHT_SENSOR==0 && LEFT_SENSOR==1) {
    right(); //Move Right
 }

  else if(RIGHT_SENSOR==1 && LEFT_SENSOR==0) {
    left(); //Move Left
}

  else if(RIGHT_SENSOR==1 && LEFT_SENSOR==1) {
    Stop();  //STOP
 }
}


void servoPulse (int pin, int angle){
int pwm = (angle*11) + 500;      // Convert angle to microseconds
 digitalWrite(pin, HIGH);
 delayMicroseconds(pwm);
 digitalWrite(pin, LOW);
 delay(50); // Refresh cycle of servo
}


//**********************Ultrasonic_read****************************
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn (echo, HIGH);
  return time / 29 / 2;
}

void compareDistance(){
    if(distance_L > distance_R){
    left();
  
  delay(400);
  forward();
  delay(700);
  right();
  delay(500);
  forward();
  delay(700);
 left();
 delay(400);
  }
  else{
right();
  delay(400);
  Stop();
  forward();
  delay(600);
  Stop();
  left();
  delay(500);
  Stop();
  forward();
  delay(600);  
  right();
  delay(400);
  }
}

void Check_side(){
    Stop();
    delay(100);
 for (int angle = 70; angle <= 140; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    distance_R = Ultrasonic_read();
    Serial.print("D R=");Serial.println(distance_R);
    delay(100);
  for (int angle = 140; angle >= 0; angle -= 5)  {
   servoPulse(servo, angle);  }
    delay(500);
    distance_L = Ultrasonic_read();
    Serial.print("D L=");Serial.println(distance_L);
    delay(100);
 for (int angle = 0; angle <= 70; angle += 5)  {
   servoPulse(servo, angle);  }
    delay(300);
    compareDistance();
}
void forward()
{
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);
            
                analogWrite(enA, M1_Speed);
                analogWrite(enB, M2_Speed);
}
 
void backward()
{
            digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);
            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);
            
                analogWrite(enA, M1_Speed);
                analogWrite(enB, M2_Speed);
}
 
void right()
{
            digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);
            
                analogWrite(enA, LeftRotationSpeed);
                analogWrite(enB, RightRotationSpeed);
}
 
void left()
{
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);
            
                analogWrite(enA, LeftRotationSpeed);
                analogWrite(enB, RightRotationSpeed);
}
 
void Stop()
{
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
}
