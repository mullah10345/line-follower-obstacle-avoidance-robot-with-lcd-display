#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 
#define enA 3//Enable1 L298 Pin enA 
#define in1 4//Motor1  L298 Pin in1 
#define in2 2 //Motor1  L298 Pin in1 
#define in3 6 //Motor2  L298 Pin in1 
#define in4 7 //Motor2  L298 Pin in1 
#define enB 5 //Enable2 L298 Pin enB 

#define L_S 10 //ir sensor Left
#define R_S 11 //ir sensor Right

#define echo 8    //Echo pin
#define trigger 9 //Trigger pin

#define servo A1

int Set=15;
int distance_L, distance_F, distance_R; 

void setup()
{
lcd.init(); 
lcd.init();
lcd.backlight();
Serial.begin(9600); 
pinMode(R_S, INPUT); 
pinMode(L_S, INPUT); 
pinMode(echo, INPUT );
pinMode(trigger, OUTPUT);
pinMode(enA, OUTPUT); 
pinMode(in1, OUTPUT);  
pinMode(in2, OUTPUT);  
pinMode(in3, OUTPUT);   
pinMode(in4, OUTPUT); 
pinMode(enB, OUTPUT); 
pinMode(servo, OUTPUT);
analogWrite(enA, 60);
analogWrite(enB, 60); 
 lcd.setCursor(3,0);
 lcd.print("Line Follower");
 lcd.setCursor(0,1);
 lcd.print("obstacle Avoiding");
 lcd.setCursor(0,2);
 delay(500);
 lcd.clear();
 lcd.print("Robot by");
 lcd.setCursor(2,3);
 lcd.print("Dan");
 for (int angle = 70; angle <= 140; angle += 5){servoPulse(servo, angle);}
 for (int angle = 140; angle >= 0; angle -= 5){servoPulse(servo, angle); }
 for (int angle = 0; angle <= 70; angle += 5) {servoPulse(servo, angle); }
 distance_F = Ultrasonic_read();
 delay(500);
}

void loop()
{  
 distance_F = Ultrasonic_read();Serial.print("D F=");Serial.println(distance_F);
 if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0))
 {
 if(distance_F > Set){forword();lcd.clear();lcd.setCursor(0, 0);lcd.print("Path Clear");}
 else{Check_side();lcd.clear();lcd.setCursor(0, 0);lcd.print("Obstacle Detected");}
 }  
 else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 0)){turnRight();lcd.clear();lcd.setCursor(0, 0);lcd.print("Turning right");}  
 else if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 1)){turnLeft();lcd.clear();lcd.setCursor(0, 0);lcd.print("Turning left");} 
 else if((digitalRead(R_S) == 1)&&(digitalRead(L_S) == 1)){Stop();lcd.clear();lcd.setCursor(0, 0);lcd.print("No track");}    
 delay(10);
}

void servoPulse (int pin, int angle){
 int pwm = (angle*11) + 500;      
 digitalWrite(pin, HIGH);delayMicroseconds(pwm);
 digitalWrite(pin, LOW);delay(50);
}

long Ultrasonic_read(){
  digitalWrite(trigger, LOW);delayMicroseconds(2);
  digitalWrite(trigger, HIGH);delayMicroseconds(10);
  long time = pulseIn (echo, HIGH);return time / 29 / 2;
}

void compareDistance()
{
  if(distance_L > distance_R){
  turnLeft();delay(200);
  forword();delay(700);
  turnRight();delay(500);
  }
  else{
  turnRight();delay(300);
  forword();  delay(700);
  turnLeft(); delay(500);
  }
}

void Check_side()
{
  Stop();delay(100);
  for (int angle = 70; angle <= 140; angle += 5){servoPulse(servo, angle);}
  delay(300);
  distance_R = Ultrasonic_read();Serial.print("D R=");Serial.println(distance_R);
  delay(100);
  for (int angle = 140; angle >= 0; angle -= 5) {servoPulse(servo, angle);}
  delay(500);
  distance_L = Ultrasonic_read();Serial.print("D L=");Serial.println(distance_L);
  delay(100);
  for (int angle = 0; angle <= 70; angle += 5){servoPulse(servo, angle);}
  delay(300);
  compareDistance();
}

void forword(){  //forword
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}

void turnLeft(){ //turn
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, HIGH); //Right Motor backword Pin 
}

void turnRight(){ //turn
digitalWrite(in1, HIGH); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}

void Stop(){ //
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}
