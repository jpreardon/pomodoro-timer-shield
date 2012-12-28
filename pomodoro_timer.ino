/*
  This is a simple pomodoro timer that can be used to practice the pomodoro technique. For
  a schematic, take a look on my site http://jpreardon.com/ 
  
  For more on the pomodoro technique, check out this site http://www.pomodorotechnique.com
  
  Created 2012-12-27
  By: John P. Reardon <jp@jpreardon.com>
  
*/

// Constants for input/output pins
const int greenLed1 = 7;  // Green LEDs are for the pomodoro count
const int greenLed2 = 8;
const int greenLed3 = 9;
const int greenLed4 = 10;  
const int redLed1 = 2;    // Red LEDs count the time left
const int redLed2 = 3;    // in a pomodoro, or a break
const int redLed3 = 4;
const int redLed4 = 5;
const int redLed5 = 6;
const int amberLed1 = 0;  // Short break indicator
const int amberLed2 = 1;  // Long break indicator
const int button1 = 11;   // Start/Interrupt
const int button2 = 12;   // Reset

// Constants for time values (in minutes)
const int pomodoroTime = 25;
const int shortBreakTime = 5;
const int longBreakTime = 15;

// Fields to hold state
int currentMode = 1;        // Values are 1 = IDLE, 2 = SHORT_BREAK, 3 = LONG_BREAK, 4 = POMODORO
int currentPomodoroCount;
long stateStartTime;        // In milliseconds
int ledState = LOW;        // This will be used if a LED needs to blink
long previousMillis = 0;   // Last time the LED was updated
long blinkInterval = 1000; // In milliseconds

void setup() {
  //Serial.begin(9600);
  
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
  // Check to see if buttons are pressed (code to come)
  checkButtons();
  
  // Increment Everything
  incrementTime();
  
  // Display current state
  displayState();
}

void checkButtons() {
  // See if any button are pressed, if they are, do something
  if(digitalRead(button1) == HIGH) {
    currentMode = 3;
    stateStartTime = millis();
  }
  
}

void incrementTime() {
  
  unsigned long currentMillis = millis();
  
  // Increment the timer, see if the state needs to change
  switch (currentMode) {
    case 1:
      // Idle mode, do nothing
      break;
    case 2:
      // short break code here
      // If break is over, change modes
      if(currentMillis - stateStartTime > (shortBreakTime * 60 * 1000)){
        currentMode = 1;
      }
      break;
    case 3:
      // long break code here
      if(currentMillis - stateStartTime > (longBreakTime * 60 * 1000)){
        currentMode = 1;
      }
      break;
    case 4:
      // pomodoro code here
      if(currentMillis - stateStartTime > (pomodoroTime * 60 * 1000)){
        currentPomodoroCount++;
        currentMode = 1;
      }
      break;
    default:
      // same as IDLE, I think
      currentMode = 1;
  }
 
}

void displayState() {
  
  unsigned long currentMillis = millis();
  unsigned long millisPassed;
  int numLights;
  
  // We always light the proper number of pomodoro number lights
  switch (currentPomodoroCount) {
    case 1:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
      break;
    case 2:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, HIGH);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
      break;
    case 3:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, HIGH);
      digitalWrite(greenLed3, HIGH);
      digitalWrite(greenLed4, LOW);
      break;
    case 4:
      digitalWrite(greenLed1, HIGH);
      digitalWrite(greenLed2, HIGH);
      digitalWrite(greenLed3, HIGH);
      digitalWrite(greenLed4, HIGH);
      break;
    default:
      digitalWrite(greenLed1, LOW);
      digitalWrite(greenLed2, LOW);
      digitalWrite(greenLed3, LOW);
      digitalWrite(greenLed4, LOW);
  }
  
  // Light up leds based on the current state, might need to blinky
  switch (currentMode) {
    case 1:
      // Idle mode, do nothing
      digitalWrite(amberLed1, LOW);
      break;
    case 2:
      // short break code here
      // Figure out how many lights to light
      digitalWrite(amberLed1, HIGH);
      millisPassed = currentMillis - stateStartTime;
      numLights = (shortBreakTime - ((millisPassed / 1000) / 60)) / (longBreakTime / 5);
      switch (numLights) {
        case 1:
          digitalWrite(redLed1, blinkLed());
          digitalWrite(redLed2, LOW);
          digitalWrite(redLed3, LOW);
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);
          break;
        case 2:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, blinkLed());
          digitalWrite(redLed3, LOW);
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);  
          break;     
        case 3:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, HIGH);
          digitalWrite(redLed3, blinkLed());
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);
          break;
        case 4:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, HIGH);
          digitalWrite(redLed3, HIGH);
          digitalWrite(redLed4, blinkLed());
          digitalWrite(redLed5, LOW);
          break;
        case 5:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, HIGH);
          digitalWrite(redLed3, HIGH);
          digitalWrite(redLed4, HIGH);
          digitalWrite(redLed5, blinkLed());
          break;
        default:
          digitalWrite(redLed1, LOW);
          digitalWrite(redLed2, LOW);
          digitalWrite(redLed3, LOW);
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);
          digitalWrite(amberLed1, LOW);
      }
      break;
    case 3:
      // long break code here
      // Figure out how many lights to light
      digitalWrite(amberLed2, HIGH);
      millisPassed = currentMillis - stateStartTime;
      numLights = (longBreakTime - ((millisPassed / 1000) / 60)) / (longBreakTime / 5);
      switch (numLights) {
        case 1:
          digitalWrite(redLed1, blinkLed());
          digitalWrite(redLed2, LOW);
          digitalWrite(redLed3, LOW);
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);
          break;
        case 2:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, blinkLed());
          digitalWrite(redLed3, LOW);
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);  
          break;     
        case 3:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, HIGH);
          digitalWrite(redLed3, blinkLed());
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);
          break;
        case 4:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, HIGH);
          digitalWrite(redLed3, HIGH);
          digitalWrite(redLed4, blinkLed());
          digitalWrite(redLed5, LOW);
          break;
        case 5:
          digitalWrite(redLed1, HIGH);
          digitalWrite(redLed2, HIGH);
          digitalWrite(redLed3, HIGH);
          digitalWrite(redLed4, HIGH);
          digitalWrite(redLed5, blinkLed());
          break;
        default:
          digitalWrite(redLed1, LOW);
          digitalWrite(redLed2, LOW);
          digitalWrite(redLed3, LOW);
          digitalWrite(redLed4, LOW);
          digitalWrite(redLed5, LOW);
      }
      break;
    case 4:
      // pomodoro code here
    default:
      // same as IDLE, I think
      currentMode = 1;
  }
  
}
 
int blinkLed() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis > blinkInterval) {
    
    previousMillis = currentMillis;
    
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  }
  return ledState;
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


