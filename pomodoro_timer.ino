/*
  This is a simple pomodoro timer that can be used to practice the pomodoro technique. For
  a schematic, take a look on my site http://jpreardon.com/ 
  
  For more on the pomodoro technique, check out this site http://www.pomodorotechnique.com
  
  Created 2012-12-27
  By: John P. Reardon <jp@jpreardon.com>
  
*/

// Constants for input/output pins
const int greenLed1 = 7;
const int greenLed2 = 8;
const int greenLed3 = 9;
const int greenLed4 = 10;

const int redLed1 = 2;
const int redLed2 = 3;
const int redLed3 = 4;
const int redLed4 = 5;
const int redLed5 = 6;

const int amberLed1 = 0;
const int amberLed2 = 1;

const int button1 = 11;
const int button2 = 12;

void setup() {
  // Set pins to the proper mode
  pinMode(greenLed1, OUTPUT);
  pinMode(greenLed2, OUTPUT);
  pinMode(greenLed3, OUTPUT);
  pinMode(greenLed4, OUTPUT);
  pinMode(redLed1, OUTPUT);
  pinMode(redLed2, OUTPUT);
  pinMode(redLed3, OUTPUT);
  pinMode(redLed4, OUTPUT);
  pinMode(redLed5, OUTPUT);
  pinMode(amberLed1, OUTPUT);
  pinMode(amberLed2, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}

void loop() {
  // Testing 1..2..3, if button 2 is high, light the lights!
  if (digitalRead(button2) == HIGH) {
    turnAllOn();
  }
  // Testing 1..2..3, if button 1 is high, turn out the lights!
  if (digitalRead(button1) == HIGH) { 
     turnAllOff();
  }
 }
 
void turnAllOn() {
 // You guessed it, turn all the lights on at once
 digitalWrite(greenLed1, HIGH);
 digitalWrite(greenLed2, HIGH);
 digitalWrite(greenLed3, HIGH);
 digitalWrite(greenLed4, HIGH);
 digitalWrite(redLed1, HIGH);
 digitalWrite(redLed2, HIGH);
 digitalWrite(redLed3, HIGH);
 digitalWrite(redLed4, HIGH);
 digitalWrite(redLed5, HIGH);
 digitalWrite(amberLed1, HIGH);
 digitalWrite(amberLed2, HIGH);
}

void turnAllOff() {
 // You guessed it, turn all the lights off at once
 digitalWrite(greenLed1, LOW);
 digitalWrite(greenLed2, LOW);
 digitalWrite(greenLed3, LOW);
 digitalWrite(greenLed4, LOW);
 digitalWrite(redLed1, LOW);
 digitalWrite(redLed2, LOW);
 digitalWrite(redLed3, LOW);
 digitalWrite(redLed4, LOW);
 digitalWrite(redLed5, LOW);
 digitalWrite(amberLed1, LOW);
 digitalWrite(amberLed2, LOW);
}
